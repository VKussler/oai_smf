/*
 * Copyright (c) 2017 Sprint
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "async_shell_cmd.hpp"
#include "common_defs.h"
#include "itti.hpp"
#include "logger.hpp"
#include "options.hpp"
#include "pid_file.hpp"
#include "smf_app.hpp"
#include "smf_config.hpp"
#include "smf-api-server.h"
#include "pistache/endpoint.h"
#include "pistache/http.h"
#include "pistache/router.h"
#include "smf-http2-server.h"
#include "sbi_helper.hpp"
#include "http_client.hpp"
#include "config_monitor.hpp"

#include <iostream>
#include <thread>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>  // srand
#include <unistd.h>  // get_pid(), pause()
#include <chrono>

using namespace smf;
using namespace util;
using namespace std;
using namespace oai::smf_server::api;
using namespace oai::config::smf;

itti_mw* itti_inst                    = nullptr;
async_shell_cmd* async_shell_cmd_inst = nullptr;
smf_app* smf_app_inst                 = nullptr;
std::unique_ptr<smf_config> smf_cfg;
SMFApiServer* smf_api_server_1                           = nullptr;
smf_http2_server* smf_api_server_2                       = nullptr;
std::shared_ptr<oai::http::http_client> http_client_inst = nullptr;

void send_heartbeat_to_tasks(const uint32_t sequence);

//------------------------------------------------------------------------------
void send_heartbeat_to_tasks(const uint32_t sequence) {
  itti_msg_ping* itti_msg = new itti_msg_ping(TASK_SMF_APP, TASK_ALL, sequence);
  std::shared_ptr<itti_msg_ping> i = std::shared_ptr<itti_msg_ping>(itti_msg);
  int ret                          = itti_inst->send_broadcast_msg(i);
  if (RETURNok != ret) {
    Logger::smf_app().error(
        "Could not send ITTI message %s to task TASK_ALL", i->get_msg_name());
  }
}

//------------------------------------------------------------------------------
void my_app_signal_handler(int s) {
  auto shutdown_start = std::chrono::system_clock::now();
  // Setting log level arbitrarly to debug to show the whole
  // shutdown procedure in the logs even in case of off-logging
  Logger::set_level(spdlog::level::debug);
  Logger::system().info("Caught signal %d", s);
  // we have to trigger ITTI message before terminate
  smf_app_inst->trigger_nf_deregistration();
  itti_inst->send_terminate_msg(TASK_SMF_APP);
  itti_inst->wait_tasks_end();

  if (smf_api_server_1) {
    Logger::system().debug("Stopping HTTP/1 server.");
    smf_api_server_1->shutdown();
  }
  if (smf_api_server_2) {
    Logger::system().debug("Stopping HTTP/2 server.");
    smf_api_server_2->stop();
  }

  Logger::system().debug("Freeing Allocated memory...");
  if (async_shell_cmd_inst) {
    delete async_shell_cmd_inst;
    async_shell_cmd_inst = nullptr;
    Logger::system().debug("Async Shell CMD memory done.");
  }
  if (smf_api_server_1) {
    delete smf_api_server_1;
    smf_api_server_1 = nullptr;
  }
  if (smf_api_server_2) {
    delete smf_api_server_2;
    smf_api_server_2 = nullptr;
  }
  Logger::system().debug("SMF API Server memory done.");
  if (smf_app_inst) {
    delete smf_app_inst;
    smf_app_inst = nullptr;
    Logger::system().debug("SMF APP memory done.");
  }

  // itti_inst is used in a lot of code without any nullPtr check
  // it has to be deallocated last
  if (itti_inst) {
    delete itti_inst;
    itti_inst = nullptr;
    Logger::system().debug("ITTI memory done.");
  }
  Logger::system().info("Freeing Allocated memory done.");
  auto elapsed = std::chrono::system_clock::now() - shutdown_start;
  auto ms_diff = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);
  Logger::system().info("Bye. Shutdown Procedure took %d ms", ms_diff.count());
  exit(0);
}
//------------------------------------------------------------------------------
int main(int argc, char** argv) {
  srand(time(NULL));

  // Command line options
  if (!Options::parse(argc, argv)) {
    std::cout << "Options::parse() failed" << std::endl;
    return 1;
  }

  // Logger
  Logger::init("smf", Options::getlogStdout(), Options::getlogRotFilelog());
  Logger::smf_app().startup("Options parsed");

  std::signal(SIGTERM, my_app_signal_handler);
  std::signal(SIGINT, my_app_signal_handler);

  // Config
  smf_cfg = std::make_unique<smf_config>(
      Options::getlibconfigConfig(), Options::getlogStdout(),
      Options::getlogRotFilelog());

  if (!smf_cfg->init()) {
    smf_cfg->display();
    Logger::system().error("Reading the configuration failed. Exiting.");
    return 1;
  }
  smf_cfg->display();
  Logger::set_level(smf_cfg->log_level);

  // Inter-task Interface
  itti_inst = new itti_mw();
  itti_inst->start(smf_cfg->itti.itti_timer_sched_params);

  // HTTP Client
  http_client_inst = oai::http::http_client::create_instance(
      Logger::smf_sbi(), smf_cfg->get_http_request_timeout(),
      smf_cfg->sbi.if_name, smf_cfg->http_version);

  // system command
  async_shell_cmd_inst =
      new async_shell_cmd(smf_cfg->itti.async_cmd_sched_params);

  // SMF application layer
  smf_app_inst = new smf_app(Options::getlibconfigConfig());

  // Start the configuration file monitor thread
  if (!oai::config::start_config_monitor_thread(Options::getlibconfigConfig(), smf_cfg.get())) {
      Logger::system().error("Failed to start configuration monitor thread. Configuration changes will not be automatically loaded.");
      // Decide if this is a fatal error or just a warning
      // return 1; // Example: exit if monitoring fails
  }

  // PID file
  // Currently hard-coded value. TODO: add as config option.
  string pid_file_name =
      oai::utils::get_exe_absolute_path("/var/run", smf_cfg->instance);
  if (!oai::utils::is_pid_file_lock_success(pid_file_name.c_str())) {
    Logger::smf_app().error("Lock PID file %s failed\n", pid_file_name.c_str());
    exit(-EDEADLK);
  }

  if (smf_cfg->get_http_version() == 1) {
    // SMF Pistache API server (HTTP1)
    Pistache::Address addr(
        std::string(inet_ntoa(*((struct in_addr*) &smf_cfg->sbi.addr4))),
        Pistache::Port(smf_cfg->sbi.port));
    smf_api_server_1 = new SMFApiServer(addr, smf_app_inst);
    smf_api_server_1->init(2);
    // smf_api_server_1->start();
    std::thread smf_http1_manager(&SMFApiServer::start, smf_api_server_1);
    // Register to NRF and discover appropriate UPFs

    // Quick fix: without sleep, http server is not ready for nr registration
    std::this_thread::sleep_for(1000ms);

    smf_app_inst->start_nf_registration_discovery();
    smf_http1_manager.join();
  } else if (smf_cfg->get_http_version() == 2) {
    // SMF NGHTTP API server (HTTP2)
    smf_api_server_2 = new smf_http2_server(
        oai::utils::conv::toString(smf_cfg->sbi.addr4), smf_cfg->sbi_http2_port,
        smf_app_inst);
    // smf_api_server_2->start();
    std::thread smf_http2_manager(&smf_http2_server::start, smf_api_server_2);
    // Register to NRF and discover appropriate UPFs

    // Quick fix: without sleep, http server is not ready for nr registration
    std::this_thread::sleep_for(1000ms);

    smf_app_inst->start_nf_registration_discovery();
    smf_http2_manager.join();
  }

  FILE* fp             = NULL;
  std::string filename = fmt::format("/tmp/smf_{}.status", getpid());
  fp                   = fopen(filename.c_str(), "w+");
  fprintf(fp, "STARTED\n");
  fflush(fp);
  fclose(fp);

  pause();
  return 0;
}

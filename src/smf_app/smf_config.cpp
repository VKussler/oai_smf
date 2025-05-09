/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this
 * file except in compliance with the License. You may obtain a copy of the
 * License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*! \file smf_config.cpp
 \brief
 \author  Lionel GAUTHIER, Tien-Thinh NGUYEN, Stefan Spettel
 \company Eurecom, phine.tech
 \date 2023
 \email: lionel.gauthier@eurecom.fr, tien-thinh.nguyen@eurecom.fr,
 stefan.spettel@phine.tech
 */

#include "smf_config.hpp"

#include <iostream>

#include <arpa/inet.h>
#include <regex>

#include "common_defs.h"
#include "if.hpp"
#include "logger.hpp"
#include "fqdn.hpp"
#include "smf_config_types.hpp"

using namespace std;
using namespace smf;
using namespace oai::config::smf;
using namespace oai::config;
using namespace oai::model::nrf;
using namespace oai::utils;

smf_config::smf_config(
    const std::string& configPath, bool logStdout, bool logRotFile)
    : config(configPath, oai::config::SMF_CONFIG_NAME, logStdout, logRotFile),
      n4(),
      sbi(),
      itti() {
  m_used_config_values = {LOG_LEVEL_CONFIG_NAME, REGISTER_NF_CONFIG_NAME,
                          NF_LIST_CONFIG_NAME,   SMF_CONFIG_NAME,
                          DNNS_CONFIG_NAME,      NF_CONFIG_HTTP_NAME};

  m_used_sbi_values = {
      SMF_CONFIG_NAME, PCF_CONFIG_NAME, NRF_CONFIG_NAME, AMF_CONFIG_NAME,
      UDM_CONFIG_NAME};

  // Define default values in YAML
  auto smf = std::make_shared<smf_config_type>(
      "smf", "oai-smf", sbi_interface("sbi", "oai-smf", 80, "v1", "eth0"),
      local_interface("n4", "oai-smf", 8805, "eth0"));
  add_nf("smf", smf);

  auto amf = std::make_shared<nf>(
      "amf", "oai-amf", sbi_interface("sbi", "oai-amf", 80, "v1", ""));
  add_nf("amf", amf);

  auto udm = std::make_shared<nf>(
      "udm", "oai-udm", sbi_interface("sbi", "oai-udm", 80, "v1", ""));
  add_nf("udm", udm);

  auto pcf = std::make_shared<nf>(
      "pcf", "oai-pcf", sbi_interface("sbi", "oai-pcf", 80, "v1", ""));
  add_nf("pcf", pcf);

  auto nrf = std::make_shared<nf>(
      "nrf", "oai-nrf", sbi_interface("sbi", "oai-nrf", 80, "v1", ""));
  add_nf("nrf", nrf);

  // DNN default values
  dnn_config dnn("default", "IPV4", "12.1.1.0/24", "");
  m_dnns.push_back(dnn);

  // Local subscription default values
  subscription_info_config info(
      "default", 1, DEFAULT_QOS, DEFAULT_S_AMBR, DEFAULT_SNSSAI);
  smf->get_subscription_info().push_back(info);

  // SMF profile default values
  oai::model::nrf::SmfInfo smf_info;
  oai::model::nrf::SnssaiSmfInfoItem info_item;
  oai::model::nrf::DnnSmfInfoItem dnn_item;
  dnn_item.setDnn(DEFAULT_DNN);
  info_item.setSNssai(DEFAULT_SNSSAI);
  info_item.setDnnSmfInfoList(std::vector{dnn_item});
  smf_info.setSNssaiSmfInfoList(std::vector{info_item});
  smf->m_smf_info = smf_info;
}

//------------------------------------------------------------------------------
int smf_config::get_pfcp_node_id(pfcp::node_id_t& node_id) {
  oai::config::local_interface _n4 = smf()->get_n4();
  // if we give a FQDN here, the IP address will be empty

  std::regex re_ipv4(IPV4_ADDRESS_VALIDATOR_REGEX);
  std::regex re_ipv6(IPV6_ADDRESS_VALIDATOR_REGEX);
  std::regex re_host(HOSTNAME_VALIDATOR_REGEX);
  if (std::regex_match(local().get_host(), re_ipv4)) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV4_ADDRESS;
    node_id.u1.ipv4_address = _n4.get_addr4();
    return RETURNok;
  } else if (std::regex_match(local().get_host(), re_host)) {
    node_id.node_id_type = pfcp::NODE_ID_TYPE_FQDN;
    node_id.fqdn         = local().get_host();
    return RETURNok;
  } else if (std::regex_match(local().get_host(), re_ipv6)) {
    node_id.node_id_type    = pfcp::NODE_ID_TYPE_IPV6_ADDRESS;
    node_id.u1.ipv6_address = _n4.get_addr6();
    return RETURNok;
  }
  return RETURNerror;
}

int smf_config::get_pfcp_fseid(pfcp::fseid_t& fseid) {
  oai::config::local_interface _n4 = smf()->get_n4();
  if (_n4.get_addr4().s_addr) {
    fseid.v4           = 1;
    fseid.ipv4_address = _n4.get_addr4();
    return RETURNok;
  }
  if (!IN6_IS_ADDR_UNSPECIFIED(&_n4.get_addr6())) {
    fseid.v6           = 1;
    fseid.ipv6_address = _n4.get_addr6();
    return RETURNok;
  }
  return RETURNerror;
}

//------------------------------------------------------------------------------
bool smf_config::is_dotted_dnn_handled(
    const std::string& dnn, const pdu_session_type_t& pdn_session_type) {
  std::lock_guard<std::mutex> lock(dnn_mutex); // Lock for reading dnns map
  Logger::smf_app().debug("Requested DNN: %s", dnn.c_str());

  for (const auto& it : dnns) {
    Logger::smf_app().debug(
        "DNN label: %s, dnn: %s", it.second.dnn_label.c_str(),
        it.second.dnn.c_str());
    if (dnn == it.second.dnn) {
      Logger::smf_app().debug("DNN matched!");
      Logger::smf_app().debug(
          "PDU Session Type %d, PDN Type %d", pdn_session_type.pdu_session_type,
          it.second.pdu_session_type.pdu_session_type);
      if (pdn_session_type.pdu_session_type ==
          it.second.pdu_session_type.pdu_session_type) {
        return true;
      }
    }
  }

  return false;
}

//------------------------------------------------------------------------------
std::string smf_config::get_default_dnn() {
  std::lock_guard<std::mutex> lock(dnn_mutex); // Lock for reading dnns map
  for (const auto& it : dnns) {
    Logger::smf_app().debug("Default DNN: %s", it.second.dnn.c_str());
    return it.second.dnn;
  }
  return "default";  // default DNN
}

void smf_config::to_smf_config() {
  log_level    = spdlog::level::from_str(config::log_level());
  auto smf_cfg = smf();

  use_local_subscription_info =
      smf_cfg->get_smf_support_features().use_local_subscription_info();
  use_local_pcc_rules =
      smf_cfg->get_smf_support_features().use_local_pcc_rules();

  // TODO discover PCF is not implemented
  discover_pcf = false;

  local_interface _n4 = smf()->get_n4();

  n4.port    = _n4.get_port();
  n4.addr4   = _n4.get_addr4();
  n4.addr6   = _n4.get_addr6();
  n4.if_name = _n4.get_if_name();
  n4.mtu     = _n4.get_mtu();

  // TODO these values are not configurable anymore
  sbi.thread_rd_sched_params.sched_priority   = 90;
  itti.itti_timer_sched_params.sched_priority = 85;
  itti.n4_sched_params.sched_priority         = 84;
  itti.smf_app_sched_params.sched_priority    = 84;
  itti.async_cmd_sched_params.sched_priority  = 84;
  n4.thread_rd_sched_params.sched_priority    = 90;

  force_push_pco = true;

  sbi.port        = smf_cfg->get_sbi().get_port();
  sbi.addr4       = smf_cfg->get_sbi().get_addr4();
  sbi.addr6       = smf_cfg->get_sbi().get_addr6();
  sbi.if_name     = smf_cfg->get_sbi().get_if_name();
  sbi_http2_port  = smf_cfg->get_sbi().get_port();
  sbi_api_version = smf_cfg->get_sbi().get_api_version();
  http_version    = get_http_version();

  // DNNs
  {
    std::lock_guard<std::mutex> lock(dnn_mutex); // Lock before accessing dnns map
    dnns.clear(); // Clear the map before repopulating
    for (const auto& cfg_dnn : get_dnns()) {
      dnn_t dnn;
      dnn.pdu_session_type  = cfg_dnn.get_pdu_session_type();
      dnn.dnn               = cfg_dnn.get_dnn();
      dnn.ue_pool_range_low = cfg_dnn.get_ipv4_pool_start();
      // we need to add one IP as it is reserved for the GW
      dnn.ue_pool_range_low.s_addr += be32toh(1);
      dnn.ue_pool_range_high = cfg_dnn.get_ipv4_pool_end();

      logger::logger_registry::get_logger(LOGGER_NAME)
          .debug(
              "DNN %s: -- First UE IPv4: %s -- Last UE IPv4: %s", dnn.dnn,
              conv::toString(dnn.ue_pool_range_low),
              conv::toString(dnn.ue_pool_range_high));

      dnn.paa_pool6_prefix     = cfg_dnn.get_ipv6_prefix();
      dnn.paa_pool6_prefix_len = cfg_dnn.get_ipv6_prefix_length();
      dnns[dnn.dnn]            = dnn;
    }
  } // Mutex is automatically unlocked here
}

bool smf_config::init() {
  bool success = config::init();
  // we update DNS settings per DNN if user did not set it
  for (auto& dnn : m_dnns) {
    if (!dnn.get_ue_dns().is_set()) {
      dnn.set_ue_dns(smf()->get_ue_dns());
    }
  }

  // if there is an error here, we print the configuration before throwing
  try {
    to_smf_config();
  } catch (std::exception&) {
    display();
    throw;
  }

  return success;
}

std::map<std::string, DnnChange> smf_config::reload_config() {
    Logger::smf_app().debug("Entering reload_config().");
    Logger::smf_app().info("Attempting to reload configuration from %s", get_config_path().c_str());
    std::map<std::string, DnnChange> dnn_changes;
    std::map<std::string, dnn_t> old_dnns;

    // --- Create a copy of the current DNN map (needs mutex protection) ---
    {
        std::lock_guard<std::mutex> lock(dnn_mutex);
        old_dnns = dnns; // Make a copy
    }

    // --- Re-run the base class initialization (parses file) ---
    if (!config::init()) {
        Logger::smf_app().error("Failed to re-initialize base configuration during reload.");
        return {}; // Return empty map on failure
    }

    // --- Re-populate derived class members (updates internal dnns map) ---
    try {
      // to_smf_config already locks dnn_mutex internally for writing
      to_smf_config();
    } catch (const std::exception& e) {
      Logger::smf_app().error("Exception during configuration mapping in reload: %s", e.what());
      // Config might be inconsistent, don't report changes. Reload failed effectively.
      // TODO: Consider reverting to old_dnns state?
      return {}; // Return empty map on failure
    }

    // --- Compare old and new DNN maps (needs mutex protection for reading new map) ---
    {
        std::lock_guard<std::mutex> lock(dnn_mutex);
        // Check for removed or modified DNNs
        for (const auto& [dnn_name, old_dnn_data] : old_dnns) {
            auto it = dnns.find(dnn_name);
            if (it == dnns.end()) {
                // DNN was removed
                dnn_changes[dnn_name] = DnnChange::REMOVED;
                Logger::smf_app().warn("Detected REMOVED DNN: %s", dnn_name.c_str());
            } else {
                // DNN exists, check if modified (simple check: compare UE pool ranges)
                // TODO: Define a proper comparison operator or function for dnn_t if more detailed checks are needed.
                if (it->second.ue_pool_range_low.s_addr != old_dnn_data.ue_pool_range_low.s_addr ||
                    it->second.ue_pool_range_high.s_addr != old_dnn_data.ue_pool_range_high.s_addr ||
                    memcmp(&it->second.paa_pool6_prefix, &old_dnn_data.paa_pool6_prefix, sizeof(in6_addr)) != 0
                    /* || add other relevant field comparisons here */
                    ) {
                    dnn_changes[dnn_name] = DnnChange::MODIFIED;
                     Logger::smf_app().warn("Detected MODIFIED DNN: %s", dnn_name.c_str());
                     // Add DEBUG log showing specific changes (e.g., IP pools)
                     Logger::smf_app().debug("  - Old IPv4 Pool: %s - %s", conv::toString(old_dnn_data.ue_pool_range_low).c_str(), conv::toString(old_dnn_data.ue_pool_range_high).c_str());
                     Logger::smf_app().debug("  - New IPv4 Pool: %s - %s", conv::toString(it->second.ue_pool_range_low).c_str(), conv::toString(it->second.ue_pool_range_high).c_str());
                     // TODO: Add logging for IPv6 pool changes if relevant
                }
            }
        }

        // Check for added DNNs
        for (const auto& [dnn_name, new_dnn_data] : dnns) {
            if (old_dnns.find(dnn_name) == old_dnns.end()) {
                dnn_changes[dnn_name] = DnnChange::ADDED;
                 Logger::smf_app().warn("Detected ADDED DNN: %s", dnn_name.c_str());
            }
        }
    }

    Logger::smf_app().info("Successfully reloaded configuration. %ld DNN changes detected needing action.", dnn_changes.size());
    Logger::smf_app().debug("Exiting reload_config().");

    // Actual application of log level etc. happens in the caller (handle_config_change)
    // Return the map of detected DNN changes for the caller to handle propagation
    return dnn_changes;
}

std::shared_ptr<smf_config_type> smf_config::smf() const {
  return std::dynamic_pointer_cast<smf_config_type>(get_local());
}

void smf_config::update_used_nfs() {
  config::update_used_nfs();
  if (config::register_nrf()) {
    if (!smf()->get_smf_support_features().use_local_pcc_rules()) {
      logger::logger_registry::get_logger(LOGGER_NAME)
          .warn("PCF NRF discovery not supported. Using the provided values");
      get_nf(PCF_CONFIG_NAME)->set_config();
    }
    // When NRF discovery is used, we disable the local UPF INFO
    for (auto& upf : smf()->m_upfs) {
      upf.enable_upf_info(false);
    }
  }
  if (!smf()->get_smf_support_features().use_local_subscription_info()) {
    // here we remove the local subscription info
    smf()->get_subscription_info().clear();
    logger::logger_registry::get_logger(LOGGER_NAME)
        .warn("UDM NRF discovery not supported. Using the provided values");
    get_nf(UDM_CONFIG_NAME)->set_config();
    // TODO check if DNN from configuration is still used
  } else {
    get_nf(UDM_CONFIG_NAME)->unset_config();
  }
  if (smf()->get_smf_support_features().use_local_pcc_rules()) {
    get_nf(PCF_CONFIG_NAME)->unset_config();
  }
}

//------------------------------------------------------------------------------
void smf_config::to_json(nlohmann::json& json_data) {
  auto smf_cfg = smf();
  json_data    = smf()->to_json();
  config::to_json(json_data);
}

//------------------------------------------------------------------------------
bool smf_config::from_json(nlohmann::json& json_data) {
  auto smf_cfg = smf();
  try {
    // TODO: before enabling this, should check which configuration parameters
    // can be updated
    // smf()->from_json(json_data);
    // config::from_json(json_data);
    Logger::smf_app().warn("This feature is not ready to be enabled yet!");
  } catch (nlohmann::detail::exception& e) {
    Logger::smf_app().error(
        "Exception when getting SMF configuration from JSON %s", e.what());
    return false;
  } catch (std::exception& e) {
    Logger::smf_app().error(
        "Exception when getting SMF configuration from JSON %s", e.what());
    return false;
  }
  return false;  // TODO
}

//------------------------------------------------------------------------------
const ue_dns& smf_config::get_dns_from_dnn(const string& dnn) {
  for (const auto& dnn_cfg : get_dnns()) {
    if (dnn_cfg.get_dnn() == dnn) {
      return dnn_cfg.get_ue_dns();
    }
  }
  return smf()->get_ue_dns();
}

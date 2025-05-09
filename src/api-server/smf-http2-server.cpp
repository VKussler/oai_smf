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

/*! \file smf_http2-server.cpp
 \brief
 \author  Tien-Thinh NGUYEN
 \company Eurecom
 \date 2020
 \email: tien-thinh.nguyen@eurecom.fr
 */

#include "smf-http2-server.h"

#include <boost/algorithm/string.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <nlohmann/json.hpp>
#include <string>

#include "3gpp_29.500.h"
#include "3gpp_29.502.h"
#include "3gpp_conversions.hpp"
#include "http_client.hpp"
#include "itti_msg_n11.hpp"
#include "logger.hpp"
#include "mime_parser.hpp"
#include "smf.h"
#include "smf_config.hpp"
#include "smf_msg.hpp"

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;
using namespace oai::model::smf;
using namespace oai::model::common;

extern std::unique_ptr<oai::config::smf::smf_config> smf_cfg;

using namespace oai::common::sbi;

//------------------------------------------------------------------------------
void smf_http2_server::start() {
  boost::system::error_code ec;

  Logger::smf_api_server().info("HTTP2 server being started");
  // Create SM Context Request
  server.handle(
      NSMF_PDU_SESSION_BASE + smf_cfg->sbi_api_version +
          NSMF_PDU_SESSION_SM_CONTEXT_CREATE_URL,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          if (len > 0) {
            std::string msg((char*) data, len);
            Logger::smf_api_server().debug("");
            Logger::smf_api_server().info(
                "Received a SM context create request from AMF.");
            Logger::smf_api_server().debug(
                "Message content \n %s", msg.c_str());
            // check HTTP method manually
            if (request.method().compare("POST") != 0) {
              // error
              Logger::smf_api_server().debug(
                  "This method (%s) is not supported",
                  request.method().c_str());
              response.write_head(http_status_code::METHOD_NOT_ALLOWED);
              response.end();
              return;
            }

            SmContextMessage smContextMessage       = {};
            SmContextCreateData smContextCreateData = {};

            // simple parser
            mime_parser sp = {};
            if (!sp.parse(msg)) {
              // send reply!!!
              response.write_head(http_status_code::BAD_REQUEST);
              response.end();
              return;
            }

            std::vector<mime_part> parts = {};
            sp.get_mime_parts(parts);
            uint8_t size = parts.size();
            Logger::smf_api_server().debug("Number of MIME parts %d", size);
            // at least 2 parts for Json data and N1 (+ N2)
            if (size < 2) {
              // send reply!!!
              response.write_head(http_status_code::BAD_REQUEST);
              response.end();
              return;
            }

            // step 2. process the request
            try {
              nlohmann::json::parse(parts[0].body.c_str())
                  .get_to(smContextCreateData);
              smContextMessage.setJsonData(smContextCreateData);
              if (parts[1].content_type.compare("application/vnd.3gpp.5gnas") ==
                  0) {
                smContextMessage.setBinaryDataN1SmMessage(parts[1].body);
              } else if (
                  parts[1].content_type.compare("application/vnd.3gpp.ngap") ==
                  0) {
                smContextMessage.setBinaryDataN2SmInformation(parts[1].body);
              }
              // process the request
              this->create_sm_contexts_handler(smContextMessage, response);
            } catch (nlohmann::detail::exception& e) {
              Logger::smf_api_server().warn(
                  "Can not parse the json data (error: %s)!", e.what());
              response.write_head(http_status_code::BAD_REQUEST);
              response.end();
              return;
            } catch (std::exception& e) {
              Logger::smf_api_server().warn("Error: %s!", e.what());
              response.write_head(http_status_code::INTERNAL_SERVER_ERROR);
              response.end();
              return;
            }
          }
        });
      });

  // Update SM Context Request
  server.handle(
      NSMF_PDU_SESSION_BASE + smf_cfg->sbi_api_version +
          NSMF_PDU_SESSION_SM_CONTEXT_UPDATE_URL,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          if (len > 0) {
            std::string msg((char*) data, len);
            Logger::smf_api_server().debug("");
            Logger::smf_api_server().info(
                "Received a SM context update request from AMF.");
            Logger::smf_api_server().debug(
                "Message content \n %s", msg.c_str());

            // Get the smf reference context and method
            std::vector<std::string> split_result;
            boost::split(
                split_result, request.uri().path, boost::is_any_of("/"));
            if (split_result.size() != 6) {
              Logger::smf_api_server().warn("Requested URL is not implemented");
              response.write_head(http_status_code::NOT_IMPLEMENTED);
              response.end();
              return;
            }

            std::string smf_ref = split_result[split_result.size() - 2];
            std::string method  = split_result[split_result.size() - 1];
            Logger::smf_api_server().info(
                "smf_ref %s, method %s",
                split_result[split_result.size() - 2].c_str(),
                split_result[split_result.size() - 1].c_str());

            if (method.compare("modify") == 0) {  // Update SM Context Request
              Logger::smf_api_server().info(
                  "Handle Update SM Context Request from AMF");

              SmContextUpdateMessage smContextUpdateMessage = {};
              SmContextUpdateData smContextUpdateData       = {};

              // simple parser
              mime_parser sp = {};
              if (!sp.parse(msg)) {
                // send reply!!!
                response.write_head(http_status_code::BAD_REQUEST);
                response.end();
                return;
              }

              std::vector<mime_part> parts = {};
              sp.get_mime_parts(parts);
              uint8_t size = parts.size();
              Logger::smf_api_server().debug("Number of MIME parts %d", size);

              try {
                if (size > 0) {
                  nlohmann::json::parse(parts[0].body.c_str())
                      .get_to(smContextUpdateData);
                } else {
                  nlohmann::json::parse(msg.c_str())
                      .get_to(smContextUpdateData);
                }
                smContextUpdateMessage.setJsonData(smContextUpdateData);

                for (int i = 1; i < size; i++) {
                  if (parts[i].content_type.compare(
                          "application/vnd.3gpp.5gnas") == 0) {
                    smContextUpdateMessage.setBinaryDataN1SmMessage(
                        parts[i].body);
                    Logger::smf_api_server().debug("N1 SM message is set");
                  } else if (
                      parts[i].content_type.compare(
                          "application/vnd.3gpp.ngap") == 0) {
                    smContextUpdateMessage.setBinaryDataN2SmInformation(
                        parts[i].body);
                    Logger::smf_api_server().debug("N2 SM information is set");
                  }
                }
                this->update_sm_context_handler(
                    smf_ref, smContextUpdateMessage, response);

              } catch (nlohmann::detail::exception& e) {
                Logger::smf_api_server().warn(
                    "Can not parse the json data (error: %s)!", e.what());
                response.write_head(http_status_code::BAD_REQUEST);
                response.end();
                return;
              } catch (std::exception& e) {
                Logger::smf_api_server().warn("Error: %s!", e.what());
                response.write_head(http_status_code::INTERNAL_SERVER_ERROR);
                response.end();
                return;
              }

            } else if (
                method.compare("release") == 0) {  // smContextReleaseMessage
              Logger::smf_api_server().info(
                  "Handle Release SM Context Request from AMF");

              SmContextReleaseMessage smContextReleaseMessage = {};

              // simple parser
              mime_parser sp = {};
              if (!sp.parse(msg)) {
                // send reply!!!
                response.write_head(http_status_code::BAD_REQUEST);
                response.end();
                return;
              }

              std::vector<mime_part> parts = {};
              sp.get_mime_parts(parts);
              uint8_t size = parts.size();
              Logger::smf_api_server().debug("Number of MIME parts %d", size);

              // Getting the body param
              SmContextReleaseData smContextReleaseData = {};
              try {
                if (size > 0) {
                  nlohmann::json::parse(parts[0].body.c_str())
                      .get_to(smContextReleaseData);
                } else {
                  nlohmann::json::parse(msg.c_str())
                      .get_to(smContextReleaseData);
                }

                smContextReleaseMessage.setJsonData(smContextReleaseData);

                for (int i = 1; i < size; i++) {
                  if (parts[i].content_type.compare(
                          "application/vnd.3gpp.ngap") == 0) {
                    smContextReleaseMessage.setBinaryDataN2SmInformation(
                        parts[i].body);
                    Logger::smf_api_server().debug("N2 SM information is set");
                  }
                }

                this->release_sm_context_handler(
                    smf_ref, smContextReleaseMessage, response);

              } catch (nlohmann::detail::exception& e) {
                Logger::smf_api_server().warn(
                    "Can not parse the json data (error: %s)!", e.what());
                response.write_head(http_status_code::BAD_REQUEST);
                response.end();
                return;
              } catch (std::exception& e) {
                Logger::smf_api_server().warn("Error: %s!", e.what());
                response.write_head(http_status_code::INTERNAL_SERVER_ERROR);
                response.end();
                return;
              }

            } else if (
                method.compare("retrieve") == 0) {  // smContextRetrieveData
              // TODO: retrieve_sm_context_handler

            } else {  // Unknown method
              Logger::smf_api_server().warn("Unknown method");
              response.write_head(http_status_code::METHOD_NOT_ALLOWED);
              response.end();
              return;
            }
          }
        });
      });

  // NFStatusNotify
  server.handle(
      NNRF_NF_STATUS_NOTIFY_BASE + smf_cfg->sbi_api_version +
          NNRF_NF_STATUS_SUBSCRIBE_URL,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          std::string msg((char*) data, len);
          try {
            if (request.method().compare("POST") == 0 && len > 0) {
              NotificationData notificationData = {};
              nlohmann::json::parse(msg.c_str()).get_to(notificationData);
              this->nf_status_notify_handler(notificationData, response);
            }
          } catch (nlohmann::detail::exception& e) {
            Logger::smf_sbi().warn(
                "Can not parse the json data (error: %s)!", e.what());
            response.write_head(http_status_code::BAD_REQUEST);
            response.end();
            return;
          }
        });
      });

  // SMF Configuration
  server.handle(
      NSMF_CUSTOMIZED_API_BASE + smf_cfg->sbi_api_version +
          NSMF_CUSTOMIZED_API_CONFIGURATION_URL,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          try {
            if (request.method().compare("GET") == 0) {
              this->get_configuration_handler(response);
            }
            if (request.method().compare("PUT") == 0 && len > 0) {
              std::string msg((char*) data, len);
              auto configuration_info = nlohmann::json::parse(msg.c_str());
              this->update_configuration_handler(configuration_info, response);
            }
          } catch (nlohmann::detail::exception& e) {
            Logger::smf_sbi().warn(
                "Can not parse the JSON data (error: %s)!", e.what());
            response.write_head(http_status_code::BAD_REQUEST);
            response.end();
            return;
          }
        });
      });
  // Event Exposure
  server.handle(
      NSMF_EVENT_EXPOSURE_API_BASE + smf_cfg->sbi_api_version +
          NSMF_EVENT_EXPOSURE_SUBSCRIBE_URL,
      [&](const request& request, const response& response) {
        request.on_data([&](const uint8_t* data, std::size_t len) {
          try {
            if (request.method().compare("GET") == 0) {
              // TODO: Get subscriptions
            }
            if (request.method().compare("PUT") == 0 && len > 0) {
              std::string msg((char*) data, len);
              // TODO: Update subscription
            }
            if (request.method().compare("POST") == 0 && len > 0) {
              std::string msg((char*) data, len);
              NsmfEventExposure subsctiptionData = {};
              auto subscription_create_data =
                  nlohmann::json::parse(msg.c_str()).get_to(subsctiptionData);
              this->create_event_subscription_handler(
                  subscription_create_data, response);
            }
          } catch (nlohmann::detail::exception& e) {
            Logger::smf_sbi().warn(
                "Can not parse the JSON data (error: %s)!", e.what());
            response.write_head(http_status_code::BAD_REQUEST);
            response.end();
            return;
          }
        });
      });

  running_server = true;
  if (server.listen_and_serve(ec, m_address, std::to_string(m_port))) {
    Logger::smf_api_server().error("HTTP2 server error: %s", ec.message());
  }
  running_server = false;
  Logger::smf_api_server().info("HTTP2 server fully stopped");
  return;
}

//------------------------------------------------------------------------------
void smf_http2_server::create_sm_contexts_handler(
    const SmContextMessage& smContextMessage, const response& response) {
  Logger::smf_api_server().info(
      "Handle PDU Session Create SM Context Request.");

  SmContextCreateData smContextCreateData = smContextMessage.getJsonData();
  std::string n1_sm_msg = smContextMessage.getBinaryDataN1SmMessage();
  Logger::smf_api_server().debug("N1 SM message: %s", n1_sm_msg.c_str());

  // Create a pdu_session_create_sm_context_request message and store
  // the necessary information
  Logger::smf_api_server().debug(
      "Create a pdu_session_create_sm_context_request message and store the "
      "necessary information");
  smf::pdu_session_create_sm_context_request sm_context_req_msg = {};

  // Convert from SmContextMessage to pdu_session_create_sm_context_request
  xgpp_conv::sm_context_create_from_openapi(
      smContextMessage, sm_context_req_msg);

  // Set api root to be used as location header in HTTP response
  sm_context_req_msg.set_api_root(
      // m_address + ":" + std::to_string(m_port) +
      NSMF_PDU_SESSION_BASE + smf_cfg->sbi_api_version +
      NSMF_PDU_SESSION_SM_CONTEXT_CREATE_URL);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f;
  f = p->get_future();

  // Generate ID for this promise (to be used in SMF-APP)
  uint32_t promise_id = m_smf_app->generate_promise_id();
  Logger::smf_api_server().debug("Promise ID generated %d", promise_id);
  m_smf_app->add_promise(promise_id, p);

  // Handle the pdu_session_create_sm_context_request message in smf_app
  std::shared_ptr<itti_n11_create_sm_context_request> itti_msg =
      std::make_shared<itti_n11_create_sm_context_request>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id);
  itti_msg->req          = sm_context_req_msg;
  itti_msg->http_version = 2;
  m_smf_app->handle_pdu_session_create_sm_context_request(itti_msg);

  boost::future_status status;
  // Wait for the result from APP and send reply to AMF
  status = f.wait_for(boost::chrono::milliseconds(FUTURE_STATUS_TIMEOUT_MS));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());

    // Wait for the result from APP and send reply to AMF
    nlohmann::json sm_context_response = f.get();
    Logger::smf_api_server().debug("Got result for promise ID %d", promise_id);

    nlohmann::json json_data = {};
    std::string json_format  = {};
    bool n1_sm_msg_is_set    = false;
    bool n2_sm_info_is_set   = false;
    uint16_t http_code       = http_status_code::REQUEST_TIMEOUT;

    if (sm_context_response.find("http_code") != sm_context_response.end()) {
      http_code = sm_context_response["http_code"].get<int>();
    }

    if (sm_context_response.find("json_format") != sm_context_response.end()) {
      json_format = sm_context_response["json_format"].get<std::string>();
    }
    if (sm_context_response.find("json_data") != sm_context_response.end()) {
      json_data = sm_context_response["json_data"];
    }

    if (sm_context_response.find("n1_sm_message") !=
        sm_context_response.end()) {
      // json_data = sm_context_response["n1_sm_message"].get<std::string>();
      n1_sm_msg_is_set = true;
    }

    if (sm_context_response.find("n2_sm_information") !=
        sm_context_response.end()) {
      n2_sm_info_is_set = true;
    }

    std::string body = {};

    // Add header
    header_map h;

    if (n1_sm_msg_is_set and n2_sm_info_is_set) {
      mime_parser::create_multipart_related_content(
          body, json_data.dump(), oai::http::CURL_MIME_BOUNDARY,
          sm_context_response["n1_sm_message"].get<std::string>(),
          sm_context_response["n2_sm_information"].get<std::string>(),
          json_format);
      h.emplace(
          "content-type", header_value{
                              "multipart/related; boundary=" +
                              std::string(oai::http::CURL_MIME_BOUNDARY)});
    } else if (n1_sm_msg_is_set) {
      mime_parser::create_multipart_related_content(
          body, json_data.dump(), oai::http::CURL_MIME_BOUNDARY,
          sm_context_response["n1_sm_message"].get<std::string>(),
          multipart_related_content_part_e::NAS, json_format);
      h.emplace(
          "content-type", header_value{
                              "multipart/related; boundary=" +
                              std::string(oai::http::CURL_MIME_BOUNDARY)});
    } else if (n2_sm_info_is_set) {
      mime_parser::create_multipart_related_content(
          body, json_data.dump(), oai::http::CURL_MIME_BOUNDARY,
          sm_context_response["n2_sm_information"].get<std::string>(),
          multipart_related_content_part_e::NGAP, json_format);
      h.emplace(
          "content-type", header_value{
                              "multipart/related; boundary=" +
                              std::string(oai::http::CURL_MIME_BOUNDARY)});
    } else {
      h.emplace("content-type", header_value{json_format});
      body = json_data.dump().c_str();
    }

    // Location header
    if (sm_context_response.find("smf_context_uri") !=
        sm_context_response.end()) {
      Logger::smf_api_server().debug(
          "Add location header %s",
          sm_context_response["smf_context_uri"].get<std::string>().c_str());
      h.emplace(
          "location", header_value{sm_context_response["smf_context_uri"]
                                       .get<std::string>()
                                       .c_str()});
    }

    // content-type header
    //  h.emplace("content-type", header_value{json_format});
    //  response.write_head(http_code, h);

    // response.end(json_data.dump().c_str());

    response.write_head(http_code, h);
    response.end(body);

  } else {
    uint16_t http_code = http_status_code::REQUEST_TIMEOUT;
    response.write_head(http_code);
    response.end();
  }
}

//------------------------------------------------------------------------------
void smf_http2_server::update_sm_context_handler(
    const std::string& smf_ref,
    const SmContextUpdateMessage& smContextUpdateMessage,
    const response& response) {
  Logger::smf_api_server().info(
      "Handle PDU Session Update SM Context Request.");

  // Get the SmContextUpdateData from this message and process in smf_app
  Logger::smf_api_server().info(
      "Received a PDUSession_UpdateSMContext Request from AMF.");

  smf::pdu_session_update_sm_context_request sm_context_req_msg = {};

  // Convert from SmContextUpdateMessage to
  // pdu_session_update_sm_context_request
  xgpp_conv::sm_context_update_from_openapi(
      smContextUpdateMessage, sm_context_req_msg);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f;
  f = p->get_future();

  // Generate ID for this promise (to be used in SMF-APP)
  uint32_t promise_id = m_smf_app->generate_promise_id();
  Logger::smf_api_server().debug("Promise ID generated %d", promise_id);
  m_smf_app->add_promise(promise_id, p);

  // Handle the itti_n11_update_sm_context_request message in smf_app
  std::shared_ptr<itti_n11_update_sm_context_request> itti_msg =
      std::make_shared<itti_n11_update_sm_context_request>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id, smf_ref);
  itti_msg->req          = sm_context_req_msg;
  itti_msg->http_version = 2;
  m_smf_app->handle_pdu_session_update_sm_context_request(itti_msg);

  boost::future_status status;
  // Wait for the result from APP and send reply to AMF
  status = f.wait_for(boost::chrono::milliseconds(FUTURE_STATUS_TIMEOUT_MS));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());

    // Wait for the result from APP and send reply to AMF
    nlohmann::json sm_context_response = f.get();
    Logger::smf_api_server().debug("Got result for promise ID %d", promise_id);

    nlohmann::json json_data = {};
    std::string body         = {};
    header_map h             = {};
    std::string json_format  = {};
    bool n1_sm_msg_is_set    = false;
    bool n2_sm_info_is_set   = false;
    uint16_t http_code       = http_status_code::REQUEST_TIMEOUT;

    if (sm_context_response.find("http_code") != sm_context_response.end()) {
      http_code = sm_context_response["http_code"].get<int>();
    }

    if (sm_context_response.find("json_format") != sm_context_response.end()) {
      json_format = sm_context_response["json_format"].get<std::string>();
    }
    if (sm_context_response.find("json_data") != sm_context_response.end()) {
      json_data = sm_context_response["json_data"];
    }

    if (sm_context_response.find("n1_sm_message") !=
        sm_context_response.end()) {
      // json_data = sm_context_response["n1_sm_message"].get<std::string>();
      n1_sm_msg_is_set = true;
    }

    if (sm_context_response.find("n2_sm_information") !=
        sm_context_response.end()) {
      n2_sm_info_is_set = true;
    }

    Logger::smf_api_server().debug("Json data %s", json_data.dump().c_str());

    if (n1_sm_msg_is_set and n2_sm_info_is_set) {
      mime_parser::create_multipart_related_content(
          body, json_data.dump(), oai::http::CURL_MIME_BOUNDARY,
          sm_context_response["n1_sm_message"].get<std::string>(),
          sm_context_response["n2_sm_information"].get<std::string>(),
          json_format);
      h.emplace(
          "content-type", header_value{
                              "multipart/related; boundary=" +
                              std::string(oai::http::CURL_MIME_BOUNDARY)});
    } else if (n1_sm_msg_is_set) {
      mime_parser::create_multipart_related_content(
          body, json_data.dump(), oai::http::CURL_MIME_BOUNDARY,
          sm_context_response["n1_sm_message"].get<std::string>(),
          multipart_related_content_part_e::NAS, json_format);
      h.emplace(
          "content-type", header_value{
                              "multipart/related; boundary=" +
                              std::string(oai::http::CURL_MIME_BOUNDARY)});
    } else if (n2_sm_info_is_set) {
      mime_parser::create_multipart_related_content(
          body, json_data.dump(), oai::http::CURL_MIME_BOUNDARY,
          sm_context_response["n2_sm_information"].get<std::string>(),
          multipart_related_content_part_e::NGAP, json_format);
      h.emplace(
          "content-type", header_value{
                              "multipart/related; boundary=" +
                              std::string(oai::http::CURL_MIME_BOUNDARY)});
    } else {
      h.emplace("content-type", header_value{json_format});
      body = json_data.dump().c_str();
    }

    response.write_head(http_code, h);
    response.end(body);
  } else {
    uint16_t http_code = http_status_code::REQUEST_TIMEOUT;
    response.write_head(http_code);
    response.end();
  }
}

//------------------------------------------------------------------------------
void smf_http2_server::release_sm_context_handler(
    const std::string& smf_ref,
    const SmContextReleaseMessage& smContextReleaseMessage,
    const response& response) {
  Logger::smf_api_server().info(
      "Handle PDU Session Release SM Context Request.");

  smf::pdu_session_release_sm_context_request sm_context_req_msg = {};
  // Convert from SmContextReleaseMessage to
  // pdu_session_release_sm_context_request
  xgpp_conv::sm_context_release_from_openapi(
      smContextReleaseMessage, sm_context_req_msg);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f;
  f = p->get_future();

  // Generate ID for this promise (to be used in SMF-APP)
  uint32_t promise_id = m_smf_app->generate_promise_id();
  Logger::smf_api_server().debug("Promise ID generated %d", promise_id);
  m_smf_app->add_promise(promise_id, p);

  // handle Nsmf_PDUSession_UpdateSMContext Request
  Logger::smf_api_server().info(
      "Received a PDUSession_ReleaseSMContext Request: PDU Session Release "
      "request from AMF.");
  std::shared_ptr<itti_n11_release_sm_context_request> itti_msg =
      std::make_shared<itti_n11_release_sm_context_request>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id, smf_ref);
  itti_msg->req          = sm_context_req_msg;
  itti_msg->scid         = smf_ref;
  itti_msg->http_version = 2;
  m_smf_app->handle_pdu_session_release_sm_context_request(itti_msg);

  boost::future_status status;
  // wait for timeout or ready
  status = f.wait_for(boost::chrono::milliseconds(FUTURE_STATUS_TIMEOUT_MS));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());

    // wait for the result from APP and send reply to AMF
    nlohmann::json sm_context_response = f.get();
    Logger::smf_api_server().debug("Got result for promise ID %d", promise_id);

    uint16_t http_code = http_status_code::REQUEST_TIMEOUT;
    if (sm_context_response.find("http_code") != sm_context_response.end()) {
      http_code = sm_context_response["http_code"].get<int>();
    }

    response.write_head(http_code);
    response.end();

  } else {
    uint16_t http_code = http_status_code::REQUEST_TIMEOUT;
    response.write_head(http_code);
    response.end();
  }
}

//------------------------------------------------------------------------------
void smf_http2_server::nf_status_notify_handler(
    const NotificationData& notificationData, const response& response) {
  Logger::smf_api_server().info(
      "NFStatusNotifyApiImpl, received a NF status notification...");

  nlohmann::json json_data = {};
  std::string content_type = "application/problem+json";

  // Handle the message in smf_app
  auto itti_msg =
      std::make_shared<itti_sbi_notification_data>(TASK_SMF_SBI, TASK_SMF_APP);
  itti_msg->notification_msg = notificationData;
  itti_msg->http_version     = 2;

  ProblemDetails problem_details = {};
  uint8_t http_code              = 0;
  header_map h;

  if (m_smf_app->handle_nf_status_notification(
          itti_msg, problem_details, http_code)) {
    http_code = 204;
    response.write_head(http_code, h);
    response.end();
  } else {
    to_json(json_data, problem_details);
    h.emplace("content-type", header_value{content_type});
    response.end(json_data.dump().c_str());
  }
}

//------------------------------------------------------------------------------
void smf_http2_server::get_configuration_handler(const response& response) {
  Logger::smf_api_server().debug("Get SMF Configuration, handling...");

  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_smf_app->generate_promise_id();
  Logger::smf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_smf_app->add_promise(promise_id, p);

  // Handle the SMFConfiguration in smf_app
  std::shared_ptr<itti_sbi_smf_configuration> itti_msg =
      std::make_shared<itti_sbi_smf_configuration>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id);

  itti_msg->http_version = 2;
  itti_msg->promise_id   = promise_id;

  // TODO:
  m_smf_app->handle_sbi_get_configuration(itti_msg);

  boost::future_status status;
  // Wait for the result from APP and send reply to AMF
  status = f.wait_for(boost::chrono::milliseconds(FUTURE_STATUS_TIMEOUT_MS));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());

    // result includes json content and http response code
    nlohmann::json result = f.get();
    Logger::smf_api_server().debug("Got result for promise ID %d", promise_id);

    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == 200) {
      if (result.find("content") != result.end()) {
        json_data = result["content"];
      }
      h.emplace("content-type", header_value{"application/json"});
      response.end(json_data.dump().c_str());
    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }
      h.emplace("content-type", header_value{"application/problem+json"});
      response.end(json_data.dump().c_str());
    }
  } else {
    uint16_t http_code = http_status_code::REQUEST_TIMEOUT;
    response.write_head(http_code);
    response.end();
  }
}

//------------------------------------------------------------------------------
void smf_http2_server::update_configuration_handler(
    nlohmann::json& configuration_info, const response& response) {
  Logger::smf_api_server().debug("Update SMF Configuration, handling...");

  header_map h;

  // Generate a promise and associate this promise to the ITTI message
  uint32_t promise_id = m_smf_app->generate_promise_id();
  Logger::smf_n1().debug("Promise ID generated %d", promise_id);

  boost::shared_ptr<boost::promise<nlohmann::json>> p =
      boost::make_shared<boost::promise<nlohmann::json>>();
  boost::shared_future<nlohmann::json> f = p->get_future();
  m_smf_app->add_promise(promise_id, p);

  // Handle the SMFConfiguration in smf_app
  std::shared_ptr<itti_sbi_update_smf_configuration> itti_msg =
      std::make_shared<itti_sbi_update_smf_configuration>(
          TASK_SMF_SBI, TASK_SMF_APP, promise_id);

  itti_msg->http_version  = 2;
  itti_msg->promise_id    = promise_id;
  itti_msg->configuration = configuration_info;

  // TODO:
  m_smf_app->handle_sbi_update_configuration(itti_msg);

  boost::future_status status;
  // wait for timeout or ready
  status = f.wait_for(boost::chrono::milliseconds(FUTURE_STATUS_TIMEOUT_MS));
  if (status == boost::future_status::ready) {
    assert(f.is_ready());
    assert(f.has_value());
    assert(!f.has_exception());
    // Wait for the result from APP
    // result includes json content and http response code
    nlohmann::json result = f.get();
    Logger::smf_api_server().debug("Got result for promise ID %d", promise_id);

    // process data
    uint32_t http_response_code = 0;
    nlohmann::json json_data    = {};

    if (result.find("httpResponseCode") != result.end()) {
      http_response_code = result["httpResponseCode"].get<int>();
    }

    if (http_response_code == 200) {
      if (result.find("content") != result.end()) {
        json_data = result["content"];
      }
      h.emplace("content-type", header_value{"application/json"});
      response.end(json_data.dump().c_str());
    } else {
      // Problem details
      if (result.find("ProblemDetails") != result.end()) {
        json_data = result["ProblemDetails"];
      }
      h.emplace("content-type", header_value{"application/problem+json"});
      response.end(json_data.dump().c_str());
    }
  } else {
    uint16_t http_code = http_status_code::REQUEST_TIMEOUT;
    response.write_head(http_code);
    response.end();
  }
}
//------------------------------------------------------------------------------
void smf_http2_server::create_event_subscription_handler(
    const NsmfEventExposure& nsmfEventExposure, const response& response) {
  Logger::smf_api_server().info("Received SmfCreateEventSubscription Request");

  header_map h;
  smf::event_exposure_msg event_exposure = {};

  // Convert from NsmfEventExposure to event_exposure_msg
  xgpp_conv::smf_event_exposure_notification_from_openapi(
      nsmfEventExposure, event_exposure);

  // Handle the message in smf_app
  std::shared_ptr<itti_sbi_event_exposure_request> itti_msg =
      std::make_shared<itti_sbi_event_exposure_request>(
          TASK_SMF_SBI, TASK_SMF_APP);
  itti_msg->event_exposure = event_exposure;
  itti_msg->http_version   = 2;

  evsub_id_t sub_id = m_smf_app->handle_event_exposure_subscription(itti_msg);

  // Send response
  nlohmann::json json_data = {};
  to_json(json_data, nsmfEventExposure);

  if (sub_id != -1) {
    json_data["subId"] = std::to_string(sub_id);
    h.emplace(
        "Location",
        header_value{
            m_address + NSMF_EVENT_EXPOSURE_API_BASE +
            smf_cfg->sbi_api_version + NSMF_EVENT_EXPOSURE_SUBSCRIBE_URL +
            std::to_string(sub_id)});
  }

  h.emplace("content-type", header_value{"application/json"});
  response.write_head(http_status_code::CREATED, h);
  response.end(json_data.dump().c_str());
}

//------------------------------------------------------------------------------
void smf_http2_server::stop() {
  Logger::smf_api_server().debug("HTTP2 server to be stopped");
  server.stop();
  while (running_server) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  Logger::smf_api_server().debug("HTTP2 server should be fully stopped");
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

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

#ifndef _SECURITY_MODE_COMMAND_H_
#define _SECURITY_MODE_COMMAND_H_

#include "NasIeHeader.hpp"

namespace oai::nas {

class SecurityModeCommand : public Nas5gmmMessage {
 public:
  SecurityModeCommand();
  ~SecurityModeCommand();

  void SetHeader(uint8_t security_header_type);

  int Encode(uint8_t* buf, int len) override;
  int Decode(uint8_t* buf, int len) override;

  uint32_t GetLength() const override;

  void SetNasSecurityAlgorithms(uint8_t ciphering, uint8_t integrity);
  // TODO: Get

  void SetNgKsi(uint8_t tsc, uint8_t key_set_id);
  // TODO: Get

  void SetUeSecurityCapability(uint8_t ea, uint8_t ia);
  // TODO: Get
  void SetUeSecurityCapability(
      uint8_t ea, uint8_t ia, uint8_t eea, uint8_t eia);
  // TODO: Get
  void SetUeSecurityCapability(
      const UeSecurityCapability& ue_security_capability);

  void SetImeisvRequest(uint8_t value);
  // TODO: Get

  void SetEpsNasSecurityAlgorithms(uint8_t ciphering, uint8_t integrity);
  // TODO: Get

  void SetAdditional5gSecurityInformation(bool rinmr, bool hdp);
  // TODO: Get

  void SetEapMessage(bstring eap);
  // TODO: Get

  void SetAbba(uint8_t length, uint8_t* value);
  // TODO: Get

  void SetS1UeSecurityCapability(uint8_t eea, uint8_t eia);
  // TODO: Get

 private:
  NasMmPlainHeader ie_header_;                                 // Mandatory
  NasSecurityAlgorithms ie_selected_nas_security_algorithms_;  // Mandatory
  NasKeySetIdentifier ie_ng_ksi_;  // Mandatory (1/2 octet)
  // Spare half octet
  UeSecurityCapability ie_ue_security_capability_;  // Mandatory

  std::optional<ImeisvRequest> ie_imeisv_request_;  // Optional
  std::optional<EpsNasSecurityAlgorithms>
      ie_eps_nas_security_algorithms_;  // Optional
  std::optional<Additional5gSecurityInformation>
      ie_additional_5g_security_information_;  // Optional
  std::optional<EapMessage> ie_eap_message_;   // Optional
  std::optional<Abba> ie_abba_;                // Optional
  std::optional<S1UeSecurityCapability>
      ie_s1_ue_security_capability_;  // Optional
};

}  // namespace oai::nas

#endif

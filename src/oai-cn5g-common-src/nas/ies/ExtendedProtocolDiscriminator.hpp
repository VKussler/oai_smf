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

#ifndef EXTENDED_PROTOCOL_DISCRIMINATOR_H_
#define EXTENDED_PROTOCOL_DISCRIMINATOR_H_

#include "NasIe.hpp"

constexpr uint8_t kExtendedProtocolDiscriminatorLength = 1;
constexpr auto kExtendedProtocolDiscriminatorIeName =
    "Extended Protocol Discriminator";

namespace oai::nas {

class ExtendedProtocolDiscriminator : public NasIe {
 public:
  ExtendedProtocolDiscriminator(){};  // TODO: = delete;
  ExtendedProtocolDiscriminator(uint8_t epd);
  virtual ~ExtendedProtocolDiscriminator();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = true) override;

  static std::string GetIeName() {
    return kExtendedProtocolDiscriminatorIeName;
  }

  uint32_t GetIeLength() const override;
  bool Validate(int len) const override;

  void Set(uint8_t epd);
  void Get(uint8_t& epd) const;
  uint8_t Get() const;

 private:
  uint8_t epd_;
};

}  // namespace oai::nas

#endif

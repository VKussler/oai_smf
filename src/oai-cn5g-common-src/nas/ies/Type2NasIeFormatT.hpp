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

#ifndef _TYPE2_NAS_IE_FORMAT_T_H_
#define _TYPE2_NAS_IE_FORMAT_T_H_

#include "NasIe.hpp"

constexpr uint8_t kType2NasIeFormatTLength = 1;
namespace oai::nas {

class Type2NasIeFormatT : public NasIe {
 public:
  Type2NasIeFormatT();
  Type2NasIeFormatT(uint8_t iei);
  virtual ~Type2NasIeFormatT();

  int Encode(uint8_t* buf, int len) const override;
  int Decode(const uint8_t* const buf, int len, bool is_iei = true) override;

  uint32_t GetIeLength() const override;

  bool Validate(int len) const override;

 protected:
  uint8_t iei_;  // 1 byte
};

}  // namespace oai::nas

#endif

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

#ifndef _NAS_IE_H_
#define _NAS_IE_H_

#include <stdint.h>

#include <optional>
#include <vector>

#include "3gpp_24.501.hpp"
#include "IeConst.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

extern "C" {
#include "TLVDecoder.h"
#include "TLVEncoder.h"
#include "bstrlib.h"
}

namespace oai::nas {

class NasIe {
 public:
  NasIe();
  virtual ~NasIe();

  virtual int Encode(uint8_t* buf, int len) const = 0;
  virtual int Decode(
      const uint8_t* const buf, int len, bool is_option = false) = 0;

  virtual uint32_t GetIeLength() const = 0;

  virtual bool Validate(int len) const = 0;

 protected:
};

}  // namespace oai::nas

#endif

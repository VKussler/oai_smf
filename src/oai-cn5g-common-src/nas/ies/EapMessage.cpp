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

#include "EapMessage.hpp"

#include "3gpp_24.501.hpp"
#include "common_defs.h"
#include "logger_base.hpp"

using namespace oai::nas;

//------------------------------------------------------------------------------
EapMessage::EapMessage() : Type6NasIe(), eap_() {
  SetLengthIndicator(kEapMessageContentMinimumLength);
}

//------------------------------------------------------------------------------
EapMessage::EapMessage(uint8_t iei) : Type6NasIe(iei), eap_() {
  SetLengthIndicator(kEapMessageContentMinimumLength);
}

//------------------------------------------------------------------------------
EapMessage::EapMessage(uint8_t iei, const bstring& eap) : Type6NasIe(iei) {
  eap_ = bstrcpy(eap);
  SetLengthIndicator(
      (blength(eap_) > kEapMessageContentMinimumLength) ?
          blength(eap_) :
          kEapMessageContentMinimumLength);
}

//------------------------------------------------------------------------------
EapMessage::~EapMessage() {}

//------------------------------------------------------------------------------
void EapMessage::SetValue(const bstring& eap) {
  eap_ = bstrcpy(eap);
  SetLengthIndicator(
      (blength(eap_) > kEapMessageContentMinimumLength) ?
          blength(eap_) :
          kEapMessageContentMinimumLength);
}

//------------------------------------------------------------------------------
void EapMessage::GetValue(bstring& eap) const {
  eap = bstrcpy(eap_);
}

//------------------------------------------------------------------------------
int EapMessage::Encode(uint8_t* buf, int len) const {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoding %s", GetIeName().c_str());

  int encoded_size = 0;
  // Validate the buffer's length and Encode IEI/Length (later)
  int len_pos = 0;
  int encoded_header_size =
      Type6NasIe::Encode(buf + encoded_size, len, len_pos);
  if (encoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  encoded_size += encoded_header_size;

  // Value
  int size = encode_bstring(eap_, (buf + encoded_size), len - encoded_size);
  encoded_size += size;

  // Encode length
  int encoded_len_ie = 0;
  ENCODE_U16(buf + len_pos, encoded_size - GetHeaderLength(), encoded_len_ie);

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Encoded %s, len (%d)", GetIeName().c_str(), encoded_size);
  return encoded_size;
}

//------------------------------------------------------------------------------
int EapMessage::Decode(const uint8_t* const buf, int len, bool is_iei) {
  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoding %s", GetIeName().c_str());
  int decoded_size = 0;

  // IEI and Length
  uint16_t ie_len         = 0;
  int decoded_header_size = Type6NasIe::Decode(buf + decoded_size, len, is_iei);
  if (decoded_header_size == KEncodeDecodeError) return KEncodeDecodeError;
  decoded_size += decoded_header_size;
  ie_len = GetLengthIndicator();

  if (len < GetIeLength()) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .error("Len is less than %d", ie_len);
    return KEncodeDecodeError;
  }

  // Value
  decode_bstring(&eap_, ie_len, (buf + decoded_size), len - decoded_size);
  decoded_size += ie_len;
  for (int i = 0; i < ie_len; i++) {
    oai::logger::logger_registry::get_logger(LOGGER_COMMON)
        .debug(
            "Decoded NasMessageContainer value 0x%x", (uint8_t) eap_->data[i]);
  }

  oai::logger::logger_registry::get_logger(LOGGER_COMMON)
      .debug("Decoded %s (len %d)", GetIeName().c_str(), decoded_size);
  return decoded_size;
}

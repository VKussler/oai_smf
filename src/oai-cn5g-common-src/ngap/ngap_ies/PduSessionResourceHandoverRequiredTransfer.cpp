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

#include "PduSessionResourceHandoverRequiredTransfer.hpp"

#include "logger.hpp"
#include "ngap_utils.hpp"

namespace oai::ngap {

//------------------------------------------------------------------------------
PduSessionResourceHandoverRequiredTransfer::
    PduSessionResourceHandoverRequiredTransfer() {
  m_HandoverRquiredTransferIe = (Ngap_HandoverRequiredTransfer_t*) calloc(
      1, sizeof(Ngap_HandoverRequiredTransfer_t));
  m_DirectForwardingPathAvailability = std::nullopt;
}

//------------------------------------------------------------------------------
PduSessionResourceHandoverRequiredTransfer::
    ~PduSessionResourceHandoverRequiredTransfer() {}

//------------------------------------------------------------------------------
void PduSessionResourceHandoverRequiredTransfer::
    setDirectForwardingPathAvailability(
        const Ngap_DirectForwardingPathAvailability_t&
            directForwardingPathAvailability) {
  m_DirectForwardingPathAvailability =
      std::make_optional<Ngap_DirectForwardingPathAvailability_t>(
          directForwardingPathAvailability);
  m_HandoverRquiredTransferIe->directForwardingPathAvailability =
      (Ngap_DirectForwardingPathAvailability_t*) calloc(
          1, sizeof(Ngap_DirectForwardingPathAvailability_t));
  *m_HandoverRquiredTransferIe->directForwardingPathAvailability =
      directForwardingPathAvailability;
}

//------------------------------------------------------------------------------
int PduSessionResourceHandoverRequiredTransfer::encode(
    uint8_t* buf, int buf_size) {
  ngap_utils::print_asn_msg(
      &asn_DEF_Ngap_HandoverRequiredTransfer, m_HandoverRquiredTransferIe);
  asn_enc_rval_t er = aper_encode_to_buffer(
      &asn_DEF_Ngap_HandoverRequiredTransfer, NULL, m_HandoverRquiredTransferIe,
      buf, buf_size);
  Logger::ngap().debug("er.encoded %d", er.encoded);
  return er.encoded;
}

//------------------------------------------------------------------------------
bool PduSessionResourceHandoverRequiredTransfer::decode(
    uint8_t* buf, int buf_size) {
  asn_dec_rval_t rc = asn_decode(
      NULL, ATS_ALIGNED_CANONICAL_PER, &asn_DEF_Ngap_HandoverRequiredTransfer,
      (void**) &m_HandoverRquiredTransferIe, buf, buf_size);
  if (rc.code == RC_OK) {
    Logger::ngap().debug("Decoded successfully");
  } else if (rc.code == RC_WMORE) {
    Logger::ngap().debug("More data expected, call again");
    return false;
  } else {
    Logger::ngap().debug("Failure to decode data");
    return false;
  }
  Logger::ngap().debug("rc.consumed to decode %d", rc.consumed);

  // asn_fprint(stderr, &asn_DEF_Ngap_PDUSessionResourceSetupResponseTransfer,
  // pduSessionResourceSetupResponseTransferIEs);
  if (m_HandoverRquiredTransferIe->directForwardingPathAvailability) {
    Ngap_DirectForwardingPathAvailability_t* directForwardingPathAvailability =
        new Ngap_DirectForwardingPathAvailability_t;
    directForwardingPathAvailability =
        m_HandoverRquiredTransferIe->directForwardingPathAvailability;
  }

  return true;
}

//------------------------------------------------------------------------------
bool PduSessionResourceHandoverRequiredTransfer::
    getDirectForwardingPathAvailability(
        long& directForwardingPathAvailability) const {
  if (m_DirectForwardingPathAvailability.has_value()) {
    directForwardingPathAvailability =
        (long) m_DirectForwardingPathAvailability.value();
    return true;
  }
  return false;
}

}  // namespace oai::ngap

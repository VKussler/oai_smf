/**
 * Nsmf_PDUSession
 * SMF PDU Session Service. © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "ReleaseData.h"

namespace oai::model::smf {

using namespace oai::model::common;

ReleaseData::ReleaseData() {
  m_CauseIsSet                   = false;
  m_NgApCauseIsSet               = false;
  m__5gMmCauseValue              = 0;
  m__5gMmCauseValueIsSet         = false;
  m_UeLocationIsSet              = false;
  m_UeTimeZone                   = "";
  m_UeTimeZoneIsSet              = false;
  m_AddUeLocationIsSet           = false;
  m_SecondaryRatUsageReportIsSet = false;
  m_SecondaryRatUsageInfoIsSet   = false;
}

ReleaseData::~ReleaseData() {}

void ReleaseData::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const ReleaseData& o) {
  j = nlohmann::json();
  if (o.causeIsSet()) j["cause"] = o.m_Cause;
  if (o.ngApCauseIsSet()) j["ngApCause"] = o.m_NgApCause;
  if (o._5gMmCauseValueIsSet()) j["5gMmCauseValue"] = o.m__5gMmCauseValue;
  if (o.ueLocationIsSet()) j["ueLocation"] = o.m_UeLocation;
  if (o.ueTimeZoneIsSet()) j["ueTimeZone"] = o.m_UeTimeZone;
  if (o.addUeLocationIsSet()) j["addUeLocation"] = o.m_AddUeLocation;
  if (o.secondaryRatUsageReportIsSet())
    j["secondaryRatUsageReport"] = o.m_SecondaryRatUsageReport;
  if (o.secondaryRatUsageInfoIsSet())
    j["secondaryRatUsageInfo"] = o.m_SecondaryRatUsageInfo;
}

void from_json(const nlohmann::json& j, ReleaseData& o) {
  if (j.find("cause") != j.end()) {
    j.at("cause").get_to(o.m_Cause);
    o.m_CauseIsSet = true;
  }
  if (j.find("ngApCause") != j.end()) {
    j.at("ngApCause").get_to(o.m_NgApCause);
    o.m_NgApCauseIsSet = true;
  }
  if (j.find("5gMmCauseValue") != j.end()) {
    j.at("5gMmCauseValue").get_to(o.m__5gMmCauseValue);
    o.m__5gMmCauseValueIsSet = true;
  }
  if (j.find("ueLocation") != j.end()) {
    j.at("ueLocation").get_to(o.m_UeLocation);
    o.m_UeLocationIsSet = true;
  }
  if (j.find("ueTimeZone") != j.end()) {
    j.at("ueTimeZone").get_to(o.m_UeTimeZone);
    o.m_UeTimeZoneIsSet = true;
  }
  if (j.find("addUeLocation") != j.end()) {
    j.at("addUeLocation").get_to(o.m_AddUeLocation);
    o.m_AddUeLocationIsSet = true;
  }
  if (j.find("secondaryRatUsageReport") != j.end()) {
    j.at("secondaryRatUsageReport").get_to(o.m_SecondaryRatUsageReport);
    o.m_SecondaryRatUsageReportIsSet = true;
  }
  if (j.find("secondaryRatUsageInfo") != j.end()) {
    j.at("secondaryRatUsageInfo").get_to(o.m_SecondaryRatUsageInfo);
    o.m_SecondaryRatUsageInfoIsSet = true;
  }
}

Cause ReleaseData::getCause() const {
  return m_Cause;
}
void ReleaseData::setCause(Cause const& value) {
  m_Cause      = value;
  m_CauseIsSet = true;
}
bool ReleaseData::causeIsSet() const {
  return m_CauseIsSet;
}
void ReleaseData::unsetCause() {
  m_CauseIsSet = false;
}
NgApCause ReleaseData::getNgApCause() const {
  return m_NgApCause;
}
void ReleaseData::setNgApCause(NgApCause const& value) {
  m_NgApCause      = value;
  m_NgApCauseIsSet = true;
}
bool ReleaseData::ngApCauseIsSet() const {
  return m_NgApCauseIsSet;
}
void ReleaseData::unsetNgApCause() {
  m_NgApCauseIsSet = false;
}
int32_t ReleaseData::get5gMmCauseValue() const {
  return m__5gMmCauseValue;
}
void ReleaseData::set5gMmCauseValue(int32_t const value) {
  m__5gMmCauseValue      = value;
  m__5gMmCauseValueIsSet = true;
}
bool ReleaseData::_5gMmCauseValueIsSet() const {
  return m__5gMmCauseValueIsSet;
}
void ReleaseData::unset_5gMmCauseValue() {
  m__5gMmCauseValueIsSet = false;
}
UserLocation ReleaseData::getUeLocation() const {
  return m_UeLocation;
}
void ReleaseData::setUeLocation(UserLocation const& value) {
  m_UeLocation      = value;
  m_UeLocationIsSet = true;
}
bool ReleaseData::ueLocationIsSet() const {
  return m_UeLocationIsSet;
}
void ReleaseData::unsetUeLocation() {
  m_UeLocationIsSet = false;
}
std::string ReleaseData::getUeTimeZone() const {
  return m_UeTimeZone;
}
void ReleaseData::setUeTimeZone(std::string const& value) {
  m_UeTimeZone      = value;
  m_UeTimeZoneIsSet = true;
}
bool ReleaseData::ueTimeZoneIsSet() const {
  return m_UeTimeZoneIsSet;
}
void ReleaseData::unsetUeTimeZone() {
  m_UeTimeZoneIsSet = false;
}
UserLocation ReleaseData::getAddUeLocation() const {
  return m_AddUeLocation;
}
void ReleaseData::setAddUeLocation(UserLocation const& value) {
  m_AddUeLocation      = value;
  m_AddUeLocationIsSet = true;
}
bool ReleaseData::addUeLocationIsSet() const {
  return m_AddUeLocationIsSet;
}
void ReleaseData::unsetAddUeLocation() {
  m_AddUeLocationIsSet = false;
}
std::vector<SecondaryRatUsageReport>&
ReleaseData::getSecondaryRatUsageReport() {
  return m_SecondaryRatUsageReport;
}
bool ReleaseData::secondaryRatUsageReportIsSet() const {
  return m_SecondaryRatUsageReportIsSet;
}
void ReleaseData::unsetSecondaryRatUsageReport() {
  m_SecondaryRatUsageReportIsSet = false;
}
std::vector<SecondaryRatUsageInfo>& ReleaseData::getSecondaryRatUsageInfo() {
  return m_SecondaryRatUsageInfo;
}
bool ReleaseData::secondaryRatUsageInfoIsSet() const {
  return m_SecondaryRatUsageInfoIsSet;
}
void ReleaseData::unsetSecondaryRatUsageInfo() {
  m_SecondaryRatUsageInfoIsSet = false;
}

}  // namespace oai::model::smf

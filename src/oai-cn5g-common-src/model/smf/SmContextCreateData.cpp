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

#include <iostream>
#include "SmContextCreateData.h"

namespace oai::model::smf {
using namespace oai::model::common;

SmContextCreateData::SmContextCreateData() {
  m_Supi                         = "";
  m_SupiIsSet                    = false;
  m_UnauthenticatedSupi          = false;
  m_UnauthenticatedSupiIsSet     = false;
  m_Pei                          = "";
  m_PeiIsSet                     = false;
  m_Gpsi                         = "";
  m_GpsiIsSet                    = false;
  m_PduSessionId                 = 0;
  m_PduSessionIdIsSet            = false;
  m_Dnn                          = "";
  m_DnnIsSet                     = false;
  m_SNssaiIsSet                  = false;
  m_HplmnSnssaiIsSet             = false;
  m_ServingNfId                  = "";
  m_GuamiIsSet                   = false;
  m_ServiceNameIsSet             = false;
  m_RequestTypeIsSet             = false;
  m_N1SmMsgIsSet                 = false;
  m_SecondAnTypeIsSet            = false;
  m_RatTypeIsSet                 = false;
  m_PresenceInLadnIsSet          = false;
  m_UeLocationIsSet              = false;
  m_UeTimeZone                   = "";
  m_UeTimeZoneIsSet              = false;
  m_AddUeLocationIsSet           = false;
  m_SmContextStatusUri           = "";
  m_HSmfUri                      = "";
  m_HSmfUriIsSet                 = false;
  m_AdditionalHsmfUriIsSet       = false;
  m_OldPduSessionId              = 0;
  m_OldPduSessionIdIsSet         = false;
  m_PduSessionsActivateListIsSet = false;
  m_UeEpsPdnConnection           = "";
  m_UeEpsPdnConnectionIsSet      = false;
  m_HoStateIsSet                 = false;
  m_PcfId                        = "";
  m_PcfIdIsSet                   = false;
  m_NrfUri                       = "";
  m_NrfUriIsSet                  = false;
  m_SupportedFeatures            = "";
  m_SupportedFeaturesIsSet       = false;
  m_SelModeIsSet                 = false;
  m_BackupAmfInfoIsSet           = false;
  m_TraceDataIsSet               = false;
  m_UdmGroupId                   = "";
  m_UdmGroupIdIsSet              = false;
  m_RoutingIndicator             = "";
  m_RoutingIndicatorIsSet        = false;
  m_EpsInterworkingIndIsSet      = false;
  m_IndirectForwardingFlag       = false;
  m_IndirectForwardingFlagIsSet  = false;
  m_TargetIdIsSet                = false;
  m_EpsBearerCtxStatus           = "";
  m_EpsBearerCtxStatusIsSet      = false;
  m_CpCiotEnabled                = false;
  m_CpCiotEnabledIsSet           = false;
  m_InvokeNef                    = false;
  m_InvokeNefIsSet               = false;
  m_MaPduIndication              = false;
  m_MaPduIndicationIsSet         = false;
  m_N2SmInfoIsSet                = false;
  m_SmContextRef                 = "";
  m_SmContextRefIsSet            = false;
}

SmContextCreateData::~SmContextCreateData() {}

void SmContextCreateData::validate() {
  // TODO: implement validation
}

void to_json(nlohmann::json& j, const SmContextCreateData& o) {
  j = nlohmann::json();
  if (o.supiIsSet()) j["supi"] = o.m_Supi;
  if (o.unauthenticatedSupiIsSet())
    j["unauthenticatedSupi"] = o.m_UnauthenticatedSupi;
  if (o.peiIsSet()) j["pei"] = o.m_Pei;
  if (o.gpsiIsSet()) j["gpsi"] = o.m_Gpsi;
  if (o.pduSessionIdIsSet()) j["pduSessionId"] = o.m_PduSessionId;
  if (o.dnnIsSet()) j["dnn"] = o.m_Dnn;
  if (o.sNssaiIsSet()) j["sNssai"] = o.m_SNssai;
  if (o.hplmnSnssaiIsSet()) j["hplmnSnssai"] = o.m_HplmnSnssai;
  j["servingNfId"] = o.m_ServingNfId;
  if (o.guamiIsSet()) j["guami"] = o.m_Guami;
  if (o.serviceNameIsSet()) j["serviceName"] = o.m_ServiceName;
  j["servingNetwork"] = o.m_ServingNetwork;
  if (o.requestTypeIsSet()) j["requestType"] = o.m_RequestType;
  if (o.n1SmMsgIsSet()) j["n1SmMsg"] = o.m_N1SmMsg;
  j["anType"] = o.m_AnType;
  if (o.secondAnTypeIsSet()) j["secondAnType"] = o.m_SecondAnType;
  if (o.ratTypeIsSet()) j["ratType"] = o.m_RatType;
  if (o.presenceInLadnIsSet()) j["presenceInLadn"] = o.m_PresenceInLadn;
  if (o.ueLocationIsSet()) j["ueLocation"] = o.m_UeLocation;
  if (o.ueTimeZoneIsSet()) j["ueTimeZone"] = o.m_UeTimeZone;
  if (o.addUeLocationIsSet()) j["addUeLocation"] = o.m_AddUeLocation;
  j["smContextStatusUri"] = o.m_SmContextStatusUri;
  if (o.hSmfUriIsSet()) j["hSmfUri"] = o.m_HSmfUri;
  if (o.additionalHsmfUriIsSet())
    j["additionalHsmfUri"] = o.m_AdditionalHsmfUri;
  if (o.oldPduSessionIdIsSet()) j["oldPduSessionId"] = o.m_OldPduSessionId;
  if (o.pduSessionsActivateListIsSet())
    j["pduSessionsActivateList"] = o.m_PduSessionsActivateList;
  if (o.ueEpsPdnConnectionIsSet())
    j["ueEpsPdnConnection"] = o.m_UeEpsPdnConnection;
  if (o.hoStateIsSet()) j["hoState"] = o.m_HoState;
  if (o.pcfIdIsSet()) j["pcfId"] = o.m_PcfId;
  if (o.nrfUriIsSet()) j["nrfUri"] = o.m_NrfUri;
  if (o.supportedFeaturesIsSet())
    j["supportedFeatures"] = o.m_SupportedFeatures;
  if (o.selModeIsSet()) j["selMode"] = o.m_SelMode;
  if (o.backupAmfInfoIsSet()) j["backupAmfInfo"] = o.m_BackupAmfInfo;
  if (o.traceDataIsSet()) j["traceData"] = o.m_TraceData;
  if (o.udmGroupIdIsSet()) j["udmGroupId"] = o.m_UdmGroupId;
  if (o.routingIndicatorIsSet()) j["routingIndicator"] = o.m_RoutingIndicator;
  if (o.epsInterworkingIndIsSet())
    j["epsInterworkingInd"] = o.m_EpsInterworkingInd;
  if (o.indirectForwardingFlagIsSet())
    j["indirectForwardingFlag"] = o.m_IndirectForwardingFlag;
  if (o.targetIdIsSet()) j["targetId"] = o.m_TargetId;
  if (o.epsBearerCtxStatusIsSet())
    j["epsBearerCtxStatus"] = o.m_EpsBearerCtxStatus;
  if (o.cpCiotEnabledIsSet()) j["cpCiotEnabled"] = o.m_CpCiotEnabled;
  if (o.invokeNefIsSet()) j["invokeNef"] = o.m_InvokeNef;
  if (o.maPduIndicationIsSet()) j["maPduIndication"] = o.m_MaPduIndication;
  if (o.n2SmInfoIsSet()) j["n2SmInfo"] = o.m_N2SmInfo;
  if (o.smContextRefIsSet()) j["smContextRef"] = o.m_SmContextRef;
}

void from_json(const nlohmann::json& j, SmContextCreateData& o) {
  if (j.find("supi") != j.end()) {
    j.at("supi").get_to(o.m_Supi);
    o.m_SupiIsSet = true;
  }
  if (j.find("unauthenticatedSupi") != j.end()) {
    j.at("unauthenticatedSupi").get_to(o.m_UnauthenticatedSupi);
    o.m_UnauthenticatedSupiIsSet = true;
  }
  if (j.find("pei") != j.end()) {
    j.at("pei").get_to(o.m_Pei);
    o.m_PeiIsSet = true;
  }
  if (j.find("gpsi") != j.end()) {
    j.at("gpsi").get_to(o.m_Gpsi);
    o.m_GpsiIsSet = true;
  }
  if (j.find("pduSessionId") != j.end()) {
    j.at("pduSessionId").get_to(o.m_PduSessionId);
    o.m_PduSessionIdIsSet = true;
  }
  if (j.find("dnn") != j.end()) {
    j.at("dnn").get_to(o.m_Dnn);
    o.m_DnnIsSet = true;
  }
  if (j.find("sNssai") != j.end()) {
    j.at("sNssai").get_to(o.m_SNssai);
    o.m_SNssaiIsSet = true;
  }
  if (j.find("hplmnSnssai") != j.end()) {
    j.at("hplmnSnssai").get_to(o.m_HplmnSnssai);
    o.m_HplmnSnssaiIsSet = true;
  }
  j.at("servingNfId").get_to(o.m_ServingNfId);

  if (j.find("guami") != j.end()) {
    j.at("guami").get_to(o.m_Guami);
    o.m_GuamiIsSet = true;
  }
  if (j.find("serviceName") != j.end()) {
    j.at("serviceName").get_to(o.m_ServiceName);
    o.m_ServiceNameIsSet = true;
  }
  j.at("servingNetwork").get_to(o.m_ServingNetwork);
  if (j.find("requestType") != j.end()) {
    j.at("requestType").get_to(o.m_RequestType);
    o.m_RequestTypeIsSet = true;
  }
  if (j.find("n1SmMsg") != j.end()) {
    j.at("n1SmMsg").get_to(o.m_N1SmMsg);
    o.m_N1SmMsgIsSet = true;
  }
  j.at("anType").get_to(o.m_AnType);
  if (j.find("secondAnType") != j.end()) {
    j.at("secondAnType").get_to(o.m_SecondAnType);
    o.m_SecondAnTypeIsSet = true;
  }
  if (j.find("ratType") != j.end()) {
    j.at("ratType").get_to(o.m_RatType);
    o.m_RatTypeIsSet = true;
  }
  if (j.find("presenceInLadn") != j.end()) {
    j.at("presenceInLadn").get_to(o.m_PresenceInLadn);
    o.m_PresenceInLadnIsSet = true;
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
  j.at("smContextStatusUri").get_to(o.m_SmContextStatusUri);
  if (j.find("hSmfUri") != j.end()) {
    j.at("hSmfUri").get_to(o.m_HSmfUri);
    o.m_HSmfUriIsSet = true;
  }
  if (j.find("additionalHsmfUri") != j.end()) {
    j.at("additionalHsmfUri").get_to(o.m_AdditionalHsmfUri);
    o.m_AdditionalHsmfUriIsSet = true;
  }
  if (j.find("oldPduSessionId") != j.end()) {
    j.at("oldPduSessionId").get_to(o.m_OldPduSessionId);
    o.m_OldPduSessionIdIsSet = true;
  }
  if (j.find("pduSessionsActivateList") != j.end()) {
    j.at("pduSessionsActivateList").get_to(o.m_PduSessionsActivateList);
    o.m_PduSessionsActivateListIsSet = true;
  }
  if (j.find("ueEpsPdnConnection") != j.end()) {
    j.at("ueEpsPdnConnection").get_to(o.m_UeEpsPdnConnection);
    o.m_UeEpsPdnConnectionIsSet = true;
  }
  if (j.find("hoState") != j.end()) {
    j.at("hoState").get_to(o.m_HoState);
    o.m_HoStateIsSet = true;
  }
  if (j.find("pcfId") != j.end()) {
    j.at("pcfId").get_to(o.m_PcfId);
    o.m_PcfIdIsSet = true;
  }
  if (j.find("nrfUri") != j.end()) {
    j.at("nrfUri").get_to(o.m_NrfUri);
    o.m_NrfUriIsSet = true;
  }
  if (j.find("supportedFeatures") != j.end()) {
    j.at("supportedFeatures").get_to(o.m_SupportedFeatures);
    o.m_SupportedFeaturesIsSet = true;
  }
  if (j.find("selMode") != j.end()) {
    j.at("selMode").get_to(o.m_SelMode);
    o.m_SelModeIsSet = true;
  }
  if (j.find("backupAmfInfo") != j.end()) {
    j.at("backupAmfInfo").get_to(o.m_BackupAmfInfo);
    o.m_BackupAmfInfoIsSet = true;
  }
  if (j.find("traceData") != j.end()) {
    j.at("traceData").get_to(o.m_TraceData);
    o.m_TraceDataIsSet = true;
  }
  if (j.find("udmGroupId") != j.end()) {
    j.at("udmGroupId").get_to(o.m_UdmGroupId);
    o.m_UdmGroupIdIsSet = true;
  }
  if (j.find("routingIndicator") != j.end()) {
    j.at("routingIndicator").get_to(o.m_RoutingIndicator);
    o.m_RoutingIndicatorIsSet = true;
  }
  if (j.find("epsInterworkingInd") != j.end()) {
    j.at("epsInterworkingInd").get_to(o.m_EpsInterworkingInd);
    o.m_EpsInterworkingIndIsSet = true;
  }
  if (j.find("indirectForwardingFlag") != j.end()) {
    j.at("indirectForwardingFlag").get_to(o.m_IndirectForwardingFlag);
    o.m_IndirectForwardingFlagIsSet = true;
  }
  if (j.find("targetId") != j.end()) {
    j.at("targetId").get_to(o.m_TargetId);
    o.m_TargetIdIsSet = true;
  }
  if (j.find("epsBearerCtxStatus") != j.end()) {
    j.at("epsBearerCtxStatus").get_to(o.m_EpsBearerCtxStatus);
    o.m_EpsBearerCtxStatusIsSet = true;
  }
  if (j.find("cpCiotEnabled") != j.end()) {
    j.at("cpCiotEnabled").get_to(o.m_CpCiotEnabled);
    o.m_CpCiotEnabledIsSet = true;
  }
  if (j.find("invokeNef") != j.end()) {
    j.at("invokeNef").get_to(o.m_InvokeNef);
    o.m_InvokeNefIsSet = true;
  }
  if (j.find("maPduIndication") != j.end()) {
    j.at("maPduIndication").get_to(o.m_MaPduIndication);
    o.m_MaPduIndicationIsSet = true;
  }
  if (j.find("n2SmInfo") != j.end()) {
    j.at("n2SmInfo").get_to(o.m_N2SmInfo);
    o.m_N2SmInfoIsSet = true;
  }
  if (j.find("smContextRef") != j.end()) {
    j.at("smContextRef").get_to(o.m_SmContextRef);
    o.m_SmContextRefIsSet = true;
  }
}

std::string SmContextCreateData::getSupi() const {
  return m_Supi;
}
void SmContextCreateData::setSupi(std::string const& value) {
  m_Supi      = value;
  m_SupiIsSet = true;
}
bool SmContextCreateData::supiIsSet() const {
  return m_SupiIsSet;
}
void SmContextCreateData::unsetSupi() {
  m_SupiIsSet = false;
}
bool SmContextCreateData::isUnauthenticatedSupi() const {
  return m_UnauthenticatedSupi;
}
void SmContextCreateData::setUnauthenticatedSupi(bool const value) {
  m_UnauthenticatedSupi      = value;
  m_UnauthenticatedSupiIsSet = true;
}
bool SmContextCreateData::unauthenticatedSupiIsSet() const {
  return m_UnauthenticatedSupiIsSet;
}
void SmContextCreateData::unsetUnauthenticatedSupi() {
  m_UnauthenticatedSupiIsSet = false;
}
std::string SmContextCreateData::getPei() const {
  return m_Pei;
}
void SmContextCreateData::setPei(std::string const& value) {
  m_Pei      = value;
  m_PeiIsSet = true;
}
bool SmContextCreateData::peiIsSet() const {
  return m_PeiIsSet;
}
void SmContextCreateData::unsetPei() {
  m_PeiIsSet = false;
}
std::string SmContextCreateData::getGpsi() const {
  return m_Gpsi;
}
void SmContextCreateData::setGpsi(std::string const& value) {
  m_Gpsi      = value;
  m_GpsiIsSet = true;
}
bool SmContextCreateData::gpsiIsSet() const {
  return m_GpsiIsSet;
}
void SmContextCreateData::unsetGpsi() {
  m_GpsiIsSet = false;
}
uint8_t SmContextCreateData::getPduSessionId() const {
  return m_PduSessionId;
}
void SmContextCreateData::setPduSessionId(int32_t const value) {
  m_PduSessionId      = value;
  m_PduSessionIdIsSet = true;
}
bool SmContextCreateData::pduSessionIdIsSet() const {
  return m_PduSessionIdIsSet;
}
void SmContextCreateData::unsetPduSessionId() {
  m_PduSessionIdIsSet = false;
}
std::string SmContextCreateData::getDnn() const {
  return m_Dnn;
}
void SmContextCreateData::setDnn(std::string const& value) {
  m_Dnn      = value;
  m_DnnIsSet = true;
}
bool SmContextCreateData::dnnIsSet() const {
  return m_DnnIsSet;
}
void SmContextCreateData::unsetDnn() {
  m_DnnIsSet = false;
}
oai::model::common::Snssai SmContextCreateData::getSNssai() const {
  return m_SNssai;
}
void SmContextCreateData::setSNssai(oai::model::common::Snssai const& value) {
  m_SNssai      = value;
  m_SNssaiIsSet = true;
}
bool SmContextCreateData::sNssaiIsSet() const {
  return m_SNssaiIsSet;
}
void SmContextCreateData::unsetSNssai() {
  m_SNssaiIsSet = false;
}
oai::model::common::Snssai SmContextCreateData::getHplmnSnssai() const {
  return m_HplmnSnssai;
}
void SmContextCreateData::setHplmnSnssai(
    oai::model::common::Snssai const& value) {
  m_HplmnSnssai      = value;
  m_HplmnSnssaiIsSet = true;
}
bool SmContextCreateData::hplmnSnssaiIsSet() const {
  return m_HplmnSnssaiIsSet;
}
void SmContextCreateData::unsetHplmnSnssai() {
  m_HplmnSnssaiIsSet = false;
}
std::string SmContextCreateData::getServingNfId() const {
  return m_ServingNfId;
}
void SmContextCreateData::setServingNfId(std::string const& value) {
  m_ServingNfId = value;
}
Guami SmContextCreateData::getGuami() const {
  return m_Guami;
}
void SmContextCreateData::setGuami(Guami const& value) {
  m_Guami      = value;
  m_GuamiIsSet = true;
}
bool SmContextCreateData::guamiIsSet() const {
  return m_GuamiIsSet;
}
void SmContextCreateData::unsetGuami() {
  m_GuamiIsSet = false;
}
std::string SmContextCreateData::getServiceName() const {
  return m_ServiceName;
}
void SmContextCreateData::setServiceName(std::string const& value) {
  m_ServiceName      = value;
  m_ServiceNameIsSet = true;
}
bool SmContextCreateData::serviceNameIsSet() const {
  return m_ServiceNameIsSet;
}
void SmContextCreateData::unsetServiceName() {
  m_ServiceNameIsSet = false;
}
oai::model::common::PlmnId SmContextCreateData::getServingNetwork() const {
  return m_ServingNetwork;
}
void SmContextCreateData::setServingNetwork(
    oai::model::common::PlmnId const& value) {
  m_ServingNetwork = value;
}
std::string SmContextCreateData::getRequestType() const {
  return m_RequestType;
}
void SmContextCreateData::setRequestType(std::string const& value) {
  m_RequestType      = value;
  m_RequestTypeIsSet = true;
}
bool SmContextCreateData::requestTypeIsSet() const {
  return m_RequestTypeIsSet;
}
void SmContextCreateData::unsetRequestType() {
  m_RequestTypeIsSet = false;
}
RefToBinaryData SmContextCreateData::getN1SmMsg() const {
  return m_N1SmMsg;
}
void SmContextCreateData::setN1SmMsg(RefToBinaryData const& value) {
  m_N1SmMsg      = value;
  m_N1SmMsgIsSet = true;
}
bool SmContextCreateData::n1SmMsgIsSet() const {
  return m_N1SmMsgIsSet;
}
void SmContextCreateData::unsetN1SmMsg() {
  m_N1SmMsgIsSet = false;
}
std::string SmContextCreateData::getAnType() const {
  return m_AnType;
}
void SmContextCreateData::setAnType(std::string const& value) {
  m_AnType = value;
}
std::string SmContextCreateData::getSecondAnType() const {
  return m_SecondAnType;
}
void SmContextCreateData::setSecondAnType(std::string const& value) {
  m_SecondAnType      = value;
  m_SecondAnTypeIsSet = true;
}
bool SmContextCreateData::secondAnTypeIsSet() const {
  return m_SecondAnTypeIsSet;
}
void SmContextCreateData::unsetSecondAnType() {
  m_SecondAnTypeIsSet = false;
}
std::string SmContextCreateData::getRatType() const {
  return m_RatType;
}
void SmContextCreateData::setRatType(std::string const& value) {
  m_RatType      = value;
  m_RatTypeIsSet = true;
}
bool SmContextCreateData::ratTypeIsSet() const {
  return m_RatTypeIsSet;
}
void SmContextCreateData::unsetRatType() {
  m_RatTypeIsSet = false;
}
std::string SmContextCreateData::getPresenceInLadn() const {
  return m_PresenceInLadn;
}
void SmContextCreateData::setPresenceInLadn(std::string const& value) {
  m_PresenceInLadn      = value;
  m_PresenceInLadnIsSet = true;
}
bool SmContextCreateData::presenceInLadnIsSet() const {
  return m_PresenceInLadnIsSet;
}
void SmContextCreateData::unsetPresenceInLadn() {
  m_PresenceInLadnIsSet = false;
}
UserLocation SmContextCreateData::getUeLocation() const {
  return m_UeLocation;
}
void SmContextCreateData::setUeLocation(UserLocation const& value) {
  m_UeLocation      = value;
  m_UeLocationIsSet = true;
}
bool SmContextCreateData::ueLocationIsSet() const {
  return m_UeLocationIsSet;
}
void SmContextCreateData::unsetUeLocation() {
  m_UeLocationIsSet = false;
}
std::string SmContextCreateData::getUeTimeZone() const {
  return m_UeTimeZone;
}
void SmContextCreateData::setUeTimeZone(std::string const& value) {
  m_UeTimeZone      = value;
  m_UeTimeZoneIsSet = true;
}
bool SmContextCreateData::ueTimeZoneIsSet() const {
  return m_UeTimeZoneIsSet;
}
void SmContextCreateData::unsetUeTimeZone() {
  m_UeTimeZoneIsSet = false;
}
UserLocation SmContextCreateData::getAddUeLocation() const {
  return m_AddUeLocation;
}
void SmContextCreateData::setAddUeLocation(UserLocation const& value) {
  m_AddUeLocation      = value;
  m_AddUeLocationIsSet = true;
}
bool SmContextCreateData::addUeLocationIsSet() const {
  return m_AddUeLocationIsSet;
}
void SmContextCreateData::unsetAddUeLocation() {
  m_AddUeLocationIsSet = false;
}
std::string SmContextCreateData::getSmContextStatusUri() const {
  return m_SmContextStatusUri;
}
void SmContextCreateData::setSmContextStatusUri(std::string const& value) {
  m_SmContextStatusUri = value;
}
std::string SmContextCreateData::getHSmfUri() const {
  return m_HSmfUri;
}
void SmContextCreateData::setHSmfUri(std::string const& value) {
  m_HSmfUri      = value;
  m_HSmfUriIsSet = true;
}
bool SmContextCreateData::hSmfUriIsSet() const {
  return m_HSmfUriIsSet;
}
void SmContextCreateData::unsetHSmfUri() {
  m_HSmfUriIsSet = false;
}
std::vector<std::string>& SmContextCreateData::getAdditionalHsmfUri() {
  return m_AdditionalHsmfUri;
}
bool SmContextCreateData::additionalHsmfUriIsSet() const {
  return m_AdditionalHsmfUriIsSet;
}
void SmContextCreateData::unsetAdditionalHsmfUri() {
  m_AdditionalHsmfUriIsSet = false;
}
int32_t SmContextCreateData::getOldPduSessionId() const {
  return m_OldPduSessionId;
}
void SmContextCreateData::setOldPduSessionId(int32_t const value) {
  m_OldPduSessionId      = value;
  m_OldPduSessionIdIsSet = true;
}
bool SmContextCreateData::oldPduSessionIdIsSet() const {
  return m_OldPduSessionIdIsSet;
}
void SmContextCreateData::unsetOldPduSessionId() {
  m_OldPduSessionIdIsSet = false;
}
std::vector<int32_t>& SmContextCreateData::getPduSessionsActivateList() {
  return m_PduSessionsActivateList;
}
bool SmContextCreateData::pduSessionsActivateListIsSet() const {
  return m_PduSessionsActivateListIsSet;
}
void SmContextCreateData::unsetPduSessionsActivateList() {
  m_PduSessionsActivateListIsSet = false;
}
std::string SmContextCreateData::getUeEpsPdnConnection() const {
  return m_UeEpsPdnConnection;
}
void SmContextCreateData::setUeEpsPdnConnection(std::string const& value) {
  m_UeEpsPdnConnection      = value;
  m_UeEpsPdnConnectionIsSet = true;
}
bool SmContextCreateData::ueEpsPdnConnectionIsSet() const {
  return m_UeEpsPdnConnectionIsSet;
}
void SmContextCreateData::unsetUeEpsPdnConnection() {
  m_UeEpsPdnConnectionIsSet = false;
}
std::string SmContextCreateData::getHoState() const {
  return m_HoState;
}
void SmContextCreateData::setHoState(std::string const& value) {
  m_HoState      = value;
  m_HoStateIsSet = true;
}
bool SmContextCreateData::hoStateIsSet() const {
  return m_HoStateIsSet;
}
void SmContextCreateData::unsetHoState() {
  m_HoStateIsSet = false;
}
std::string SmContextCreateData::getPcfId() const {
  return m_PcfId;
}
void SmContextCreateData::setPcfId(std::string const& value) {
  m_PcfId      = value;
  m_PcfIdIsSet = true;
}
bool SmContextCreateData::pcfIdIsSet() const {
  return m_PcfIdIsSet;
}
void SmContextCreateData::unsetPcfId() {
  m_PcfIdIsSet = false;
}
std::string SmContextCreateData::getNrfUri() const {
  return m_NrfUri;
}
void SmContextCreateData::setNrfUri(std::string const& value) {
  m_NrfUri      = value;
  m_NrfUriIsSet = true;
}
bool SmContextCreateData::nrfUriIsSet() const {
  return m_NrfUriIsSet;
}
void SmContextCreateData::unsetNrfUri() {
  m_NrfUriIsSet = false;
}
std::string SmContextCreateData::getSupportedFeatures() const {
  return m_SupportedFeatures;
}
void SmContextCreateData::setSupportedFeatures(std::string const& value) {
  m_SupportedFeatures      = value;
  m_SupportedFeaturesIsSet = true;
}
bool SmContextCreateData::supportedFeaturesIsSet() const {
  return m_SupportedFeaturesIsSet;
}
void SmContextCreateData::unsetSupportedFeatures() {
  m_SupportedFeaturesIsSet = false;
}
std::string SmContextCreateData::getSelMode() const {
  return m_SelMode;
}
void SmContextCreateData::setSelMode(std::string const& value) {
  m_SelMode      = value;
  m_SelModeIsSet = true;
}
bool SmContextCreateData::selModeIsSet() const {
  return m_SelModeIsSet;
}
void SmContextCreateData::unsetSelMode() {
  m_SelModeIsSet = false;
}
std::vector<BackupAmfInfo>& SmContextCreateData::getBackupAmfInfo() {
  return m_BackupAmfInfo;
}
bool SmContextCreateData::backupAmfInfoIsSet() const {
  return m_BackupAmfInfoIsSet;
}
void SmContextCreateData::unsetBackupAmfInfo() {
  m_BackupAmfInfoIsSet = false;
}
TraceData SmContextCreateData::getTraceData() const {
  return m_TraceData;
}
void SmContextCreateData::setTraceData(TraceData const& value) {
  m_TraceData      = value;
  m_TraceDataIsSet = true;
}
bool SmContextCreateData::traceDataIsSet() const {
  return m_TraceDataIsSet;
}
void SmContextCreateData::unsetTraceData() {
  m_TraceDataIsSet = false;
}
std::string SmContextCreateData::getUdmGroupId() const {
  return m_UdmGroupId;
}
void SmContextCreateData::setUdmGroupId(std::string const& value) {
  m_UdmGroupId      = value;
  m_UdmGroupIdIsSet = true;
}
bool SmContextCreateData::udmGroupIdIsSet() const {
  return m_UdmGroupIdIsSet;
}
void SmContextCreateData::unsetUdmGroupId() {
  m_UdmGroupIdIsSet = false;
}
std::string SmContextCreateData::getRoutingIndicator() const {
  return m_RoutingIndicator;
}
void SmContextCreateData::setRoutingIndicator(std::string const& value) {
  m_RoutingIndicator      = value;
  m_RoutingIndicatorIsSet = true;
}
bool SmContextCreateData::routingIndicatorIsSet() const {
  return m_RoutingIndicatorIsSet;
}
void SmContextCreateData::unsetRoutingIndicator() {
  m_RoutingIndicatorIsSet = false;
}
EpsInterworkingIndication SmContextCreateData::getEpsInterworkingInd() const {
  return m_EpsInterworkingInd;
}
void SmContextCreateData::setEpsInterworkingInd(
    EpsInterworkingIndication const& value) {
  m_EpsInterworkingInd      = value;
  m_EpsInterworkingIndIsSet = true;
}
bool SmContextCreateData::epsInterworkingIndIsSet() const {
  return m_EpsInterworkingIndIsSet;
}
void SmContextCreateData::unsetEpsInterworkingInd() {
  m_EpsInterworkingIndIsSet = false;
}
bool SmContextCreateData::isIndirectForwardingFlag() const {
  return m_IndirectForwardingFlag;
}
void SmContextCreateData::setIndirectForwardingFlag(bool const value) {
  m_IndirectForwardingFlag      = value;
  m_IndirectForwardingFlagIsSet = true;
}
bool SmContextCreateData::indirectForwardingFlagIsSet() const {
  return m_IndirectForwardingFlagIsSet;
}
void SmContextCreateData::unsetIndirectForwardingFlag() {
  m_IndirectForwardingFlagIsSet = false;
}
NgRanTargetId SmContextCreateData::getTargetId() const {
  return m_TargetId;
}
void SmContextCreateData::setTargetId(NgRanTargetId const& value) {
  m_TargetId      = value;
  m_TargetIdIsSet = true;
}
bool SmContextCreateData::targetIdIsSet() const {
  return m_TargetIdIsSet;
}
void SmContextCreateData::unsetTargetId() {
  m_TargetIdIsSet = false;
}
std::string SmContextCreateData::getEpsBearerCtxStatus() const {
  return m_EpsBearerCtxStatus;
}
void SmContextCreateData::setEpsBearerCtxStatus(std::string const& value) {
  m_EpsBearerCtxStatus      = value;
  m_EpsBearerCtxStatusIsSet = true;
}
bool SmContextCreateData::epsBearerCtxStatusIsSet() const {
  return m_EpsBearerCtxStatusIsSet;
}
void SmContextCreateData::unsetEpsBearerCtxStatus() {
  m_EpsBearerCtxStatusIsSet = false;
}
bool SmContextCreateData::isCpCiotEnabled() const {
  return m_CpCiotEnabled;
}
void SmContextCreateData::setCpCiotEnabled(bool const value) {
  m_CpCiotEnabled      = value;
  m_CpCiotEnabledIsSet = true;
}
bool SmContextCreateData::cpCiotEnabledIsSet() const {
  return m_CpCiotEnabledIsSet;
}
void SmContextCreateData::unsetCpCiotEnabled() {
  m_CpCiotEnabledIsSet = false;
}
bool SmContextCreateData::isInvokeNef() const {
  return m_InvokeNef;
}
void SmContextCreateData::setInvokeNef(bool const value) {
  m_InvokeNef      = value;
  m_InvokeNefIsSet = true;
}
bool SmContextCreateData::invokeNefIsSet() const {
  return m_InvokeNefIsSet;
}
void SmContextCreateData::unsetInvokeNef() {
  m_InvokeNefIsSet = false;
}
bool SmContextCreateData::isMaPduIndication() const {
  return m_MaPduIndication;
}
void SmContextCreateData::setMaPduIndication(bool const value) {
  m_MaPduIndication      = value;
  m_MaPduIndicationIsSet = true;
}
bool SmContextCreateData::maPduIndicationIsSet() const {
  return m_MaPduIndicationIsSet;
}
void SmContextCreateData::unsetMaPduIndication() {
  m_MaPduIndicationIsSet = false;
}
RefToBinaryData SmContextCreateData::getN2SmInfo() const {
  return m_N2SmInfo;
}
void SmContextCreateData::setN2SmInfo(RefToBinaryData const& value) {
  m_N2SmInfo      = value;
  m_N2SmInfoIsSet = true;
}
bool SmContextCreateData::n2SmInfoIsSet() const {
  return m_N2SmInfoIsSet;
}
void SmContextCreateData::unsetN2SmInfo() {
  m_N2SmInfoIsSet = false;
}
std::string SmContextCreateData::getSmContextRef() const {
  return m_SmContextRef;
}
void SmContextCreateData::setSmContextRef(std::string const& value) {
  m_SmContextRef      = value;
  m_SmContextRefIsSet = true;
}
bool SmContextCreateData::smContextRefIsSet() const {
  return m_SmContextRefIsSet;
}
void SmContextCreateData::unsetSmContextRef() {
  m_SmContextRefIsSet = false;
}

}  // namespace oai::model::smf

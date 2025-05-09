/**
 * Namf_Communication
 * AMF Communication Service © 2019, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.0.alpha-1
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * UeContextTransferRspData.h
 *
 *
 */

#ifndef UeContextTransferRspData_H_
#define UeContextTransferRspData_H_

#include "N2InfoContent.h"
#include "UeContext.h"
#include <string>
#include <nlohmann/json.hpp>

namespace oai::model::amf {

/// <summary>
///
/// </summary>
class UeContextTransferRspData {
 public:
  UeContextTransferRspData();
  virtual ~UeContextTransferRspData();

  void validate();

  /////////////////////////////////////////////
  /// UeContextTransferRspData members

  /// <summary>
  ///
  /// </summary>
  UeContext getUeContext() const;
  void setUeContext(UeContext const& value);
  /// <summary>
  ///
  /// </summary>
  N2InfoContent getUeRadioCapability() const;
  void setUeRadioCapability(N2InfoContent const& value);
  bool ueRadioCapabilityIsSet() const;
  void unsetUeRadioCapability();
  /// <summary>
  ///
  /// </summary>
  N2InfoContent getUeNbiotRadioCapability() const;
  void setUeNbiotRadioCapability(N2InfoContent const& value);
  bool ueNbiotRadioCapabilityIsSet() const;
  void unsetUeNbiotRadioCapability();
  /// <summary>
  ///
  /// </summary>
  std::string getSupportedFeatures() const;
  void setSupportedFeatures(std::string const& value);
  bool supportedFeaturesIsSet() const;
  void unsetSupportedFeatures();

  friend void to_json(nlohmann::json& j, const UeContextTransferRspData& o);
  friend void from_json(const nlohmann::json& j, UeContextTransferRspData& o);

 protected:
  UeContext m_UeContext;

  N2InfoContent m_UeRadioCapability;
  bool m_UeRadioCapabilityIsSet;
  N2InfoContent m_UeNbiotRadioCapability;
  bool m_UeNbiotRadioCapabilityIsSet;
  std::string m_SupportedFeatures;
  bool m_SupportedFeaturesIsSet;
};

}  // namespace oai::model::amf

#endif /* UeContextTransferRspData_H_ */

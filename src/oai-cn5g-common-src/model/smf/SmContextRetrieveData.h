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
/*
 * SmContextRetrieveData.h
 *
 *
 */

#ifndef SmContextRetrieveData_H_
#define SmContextRetrieveData_H_

#include "MmeCapabilities.h"
#include "SmContextType.h"
#include <nlohmann/json.hpp>

namespace oai::model::smf {

/// <summary>
///
/// </summary>
class SmContextRetrieveData {
 public:
  SmContextRetrieveData();
  virtual ~SmContextRetrieveData();

  void validate();

  /////////////////////////////////////////////
  /// SmContextRetrieveData members

  /// <summary>
  ///
  /// </summary>
  MmeCapabilities getTargetMmeCap() const;
  void setTargetMmeCap(MmeCapabilities const& value);
  bool targetMmeCapIsSet() const;
  void unsetTargetMmeCap();
  /// <summary>
  ///
  /// </summary>
  SmContextType getSmContextType() const;
  void setSmContextType(SmContextType const& value);
  bool smContextTypeIsSet() const;
  void unsetSmContextType();

  friend void to_json(nlohmann::json& j, const SmContextRetrieveData& o);
  friend void from_json(const nlohmann::json& j, SmContextRetrieveData& o);

 protected:
  MmeCapabilities m_TargetMmeCap;
  bool m_TargetMmeCapIsSet;
  SmContextType m_SmContextType;
  bool m_SmContextTypeIsSet;
};

}  // namespace oai::model::smf

#endif /* SmContextRetrieveData_H_ */

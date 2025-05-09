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
 * NssaiMapping.h
 *
 *
 */

#ifndef NssaiMapping_H_
#define NssaiMapping_H_

#include "Snssai.h"
#include <nlohmann/json.hpp>

namespace oai::model::amf {

/// <summary>
///
/// </summary>
class NssaiMapping {
 public:
  NssaiMapping();
  virtual ~NssaiMapping();

  void validate();

  /////////////////////////////////////////////
  /// NssaiMapping members

  /// <summary>
  ///
  /// </summary>
  oai::model::common::Snssai getMappedSnssai() const;
  void setMappedSnssai(oai::model::common::Snssai const& value);
  /// <summary>
  ///
  /// </summary>
  oai::model::common::Snssai getHSnssai() const;
  void setHSnssai(oai::model::common::Snssai const& value);

  friend void to_json(nlohmann::json& j, const NssaiMapping& o);
  friend void from_json(const nlohmann::json& j, NssaiMapping& o);

 protected:
  oai::model::common::Snssai m_MappedSnssai;

  oai::model::common::Snssai m_HSnssai;
};

}  // namespace oai::model::amf

#endif /* NssaiMapping_H_ */

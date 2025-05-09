/**
 * Namf_Communication
 * AMF Communication Service © 2022, 3GPP Organizational Partners (ARIB, ATIS,
 * CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.8
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */
/*
 * AreaOfValidity.h
 *
 *
 */

#ifndef AreaOfValidity_H_
#define AreaOfValidity_H_

#include "Tai.h"
#include <vector>
#include <nlohmann/json.hpp>

namespace oai::model::amf {

/// <summary>
///
/// </summary>
class AreaOfValidity {
 public:
  AreaOfValidity();
  virtual ~AreaOfValidity() = default;

  /// <summary>
  /// Validate the current data in the model. Throws a ValidationException on
  /// failure.
  /// </summary>
  void validate() const;

  /// <summary>
  /// Validate the current data in the model. Returns false on error and writes
  /// an error message into the given stringstream.
  /// </summary>
  bool validate(std::stringstream& msg) const;

  /// <summary>
  /// Helper overload for validate. Used when one model stores another model and
  /// calls it's validate. Not meant to be called outside that case.
  /// </summary>
  bool validate(std::stringstream& msg, const std::string& pathPrefix) const;

  bool operator==(const AreaOfValidity& rhs) const;
  bool operator!=(const AreaOfValidity& rhs) const;

  /////////////////////////////////////////////
  /// AreaOfValidity members

  /// <summary>
  ///
  /// </summary>
  std::vector<oai::model::common::Tai> getTaiList() const;
  void setTaiList(std::vector<oai::model::common::Tai> const& value);

  friend void to_json(nlohmann::json& j, const AreaOfValidity& o);
  friend void from_json(const nlohmann::json& j, AreaOfValidity& o);

 protected:
  std::vector<oai::model::common::Tai> m_TaiList;
};

}  // namespace oai::model::amf

#endif /* AreaOfValidity_H_ */

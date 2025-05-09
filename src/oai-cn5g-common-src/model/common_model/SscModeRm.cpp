/**
 * Common Data Types
 * Common Data Types for Service Based Interfaces. © 2022, 3GPP Organizational
 * Partners (ARIB, ATIS, CCSA, ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.2.7
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "SscModeRm.h"
#include "Helpers.h"

#include <sstream>

namespace oai::model::common {

SscModeRm::SscModeRm() {}

void SscModeRm::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::model::common::helpers::ValidationException(msg.str());
  }
}

bool SscModeRm::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool SscModeRm::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success                  = true;
  const std::string _pathPrefix = pathPrefix.empty() ? "SscModeRm" : pathPrefix;

  return success;
}

bool SscModeRm::operator==(const SscModeRm& rhs) const {
  return (*this == rhs);
}

bool SscModeRm::operator!=(const SscModeRm& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const SscModeRm& o) {
  j = nlohmann::json();
}

void from_json(const nlohmann::json& j, SscModeRm& o) {}

}  // namespace oai::model::common

/**
 * LMF Location
 * LMF Location Service. © 2022, 3GPP Organizational Partners (ARIB, ATIS, CCSA,
 * ETSI, TSDSI, TTA, TTC). All rights reserved.
 *
 * The version of the OpenAPI document: 1.1.5
 *
 *
 * NOTE: This class is auto generated by OpenAPI Generator
 * (https://openapi-generator.tech). https://openapi-generator.tech Do not edit
 * the class manually.
 */

#include "EllipsoidArc_allOf.h"
#include "Helpers.h"

#include <sstream>

namespace oai::model::lmf {

EllipsoidArc_allOf::EllipsoidArc_allOf() {
  m_InnerRadius       = 0;
  m_UncertaintyRadius = 0.0f;
  m_OffsetAngle       = 0;
  m_IncludedAngle     = 0;
  m_Confidence        = 0;
}

void EllipsoidArc_allOf::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::model::common::helpers::ValidationException(msg.str());
  }
}

bool EllipsoidArc_allOf::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool EllipsoidArc_allOf::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "EllipsoidArc_allOf" : pathPrefix;

  /* InnerRadius */ {
    const int32_t& value               = m_InnerRadius;
    const std::string currentValuePath = _pathPrefix + ".innerRadius";

    if (value < 0) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
    }
    if (value > 327675) {
      success = false;
      msg << currentValuePath << ": must be less than or equal to 327675;";
    }
  }

  /* UncertaintyRadius */ {
    const float& value                 = m_UncertaintyRadius;
    const std::string currentValuePath = _pathPrefix + ".uncertaintyRadius";

    if (value < static_cast<float>(0)) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
    }
  }

  /* OffsetAngle */ {
    const int32_t& value               = m_OffsetAngle;
    const std::string currentValuePath = _pathPrefix + ".offsetAngle";

    if (value < 0) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
    }
    if (value > 360) {
      success = false;
      msg << currentValuePath << ": must be less than or equal to 360;";
    }
  }

  /* IncludedAngle */ {
    const int32_t& value               = m_IncludedAngle;
    const std::string currentValuePath = _pathPrefix + ".includedAngle";

    if (value < 0) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
    }
    if (value > 360) {
      success = false;
      msg << currentValuePath << ": must be less than or equal to 360;";
    }
  }

  /* Confidence */ {
    const int32_t& value               = m_Confidence;
    const std::string currentValuePath = _pathPrefix + ".confidence";

    if (value < 0) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
    }
    if (value > 100) {
      success = false;
      msg << currentValuePath << ": must be less than or equal to 100;";
    }
  }

  return success;
}

bool EllipsoidArc_allOf::operator==(const EllipsoidArc_allOf& rhs) const {
  return

      (getPoint() == rhs.getPoint()) &&

      (getInnerRadius() == rhs.getInnerRadius()) &&

      (getUncertaintyRadius() == rhs.getUncertaintyRadius()) &&

      (getOffsetAngle() == rhs.getOffsetAngle()) &&

      (getIncludedAngle() == rhs.getIncludedAngle()) &&

      (getConfidence() == rhs.getConfidence())

          ;
}

bool EllipsoidArc_allOf::operator!=(const EllipsoidArc_allOf& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const EllipsoidArc_allOf& o) {
  j                      = nlohmann::json();
  j["point"]             = o.m_Point;
  j["innerRadius"]       = o.m_InnerRadius;
  j["uncertaintyRadius"] = o.m_UncertaintyRadius;
  j["offsetAngle"]       = o.m_OffsetAngle;
  j["includedAngle"]     = o.m_IncludedAngle;
  j["confidence"]        = o.m_Confidence;
}

void from_json(const nlohmann::json& j, EllipsoidArc_allOf& o) {
  j.at("point").get_to(o.m_Point);
  j.at("innerRadius").get_to(o.m_InnerRadius);
  j.at("uncertaintyRadius").get_to(o.m_UncertaintyRadius);
  j.at("offsetAngle").get_to(o.m_OffsetAngle);
  j.at("includedAngle").get_to(o.m_IncludedAngle);
  j.at("confidence").get_to(o.m_Confidence);
}

oai::model::lmf::GeographicalCoordinates EllipsoidArc_allOf::getPoint() const {
  return m_Point;
}
void EllipsoidArc_allOf::setPoint(
    oai::model::lmf::GeographicalCoordinates const& value) {
  m_Point = value;
}
int32_t EllipsoidArc_allOf::getInnerRadius() const {
  return m_InnerRadius;
}
void EllipsoidArc_allOf::setInnerRadius(int32_t const value) {
  m_InnerRadius = value;
}
float EllipsoidArc_allOf::getUncertaintyRadius() const {
  return m_UncertaintyRadius;
}
void EllipsoidArc_allOf::setUncertaintyRadius(float const value) {
  m_UncertaintyRadius = value;
}
int32_t EllipsoidArc_allOf::getOffsetAngle() const {
  return m_OffsetAngle;
}
void EllipsoidArc_allOf::setOffsetAngle(int32_t const value) {
  m_OffsetAngle = value;
}
int32_t EllipsoidArc_allOf::getIncludedAngle() const {
  return m_IncludedAngle;
}
void EllipsoidArc_allOf::setIncludedAngle(int32_t const value) {
  m_IncludedAngle = value;
}
int32_t EllipsoidArc_allOf::getConfidence() const {
  return m_Confidence;
}
void EllipsoidArc_allOf::setConfidence(int32_t const value) {
  m_Confidence = value;
}

}  // namespace oai::model::lmf

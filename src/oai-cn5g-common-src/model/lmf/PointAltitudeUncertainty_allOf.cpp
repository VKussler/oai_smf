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

#include "PointAltitudeUncertainty_allOf.h"
#include "Helpers.h"

#include <sstream>

namespace oai::model::lmf {

PointAltitudeUncertainty_allOf::PointAltitudeUncertainty_allOf() {
  m_Altitude            = 0.0;
  m_UncertaintyAltitude = 0.0f;
  m_Confidence          = 0;
}

void PointAltitudeUncertainty_allOf::validate() const {
  std::stringstream msg;
  if (!validate(msg)) {
    throw oai::model::common::helpers::ValidationException(msg.str());
  }
}

bool PointAltitudeUncertainty_allOf::validate(std::stringstream& msg) const {
  return validate(msg, "");
}

bool PointAltitudeUncertainty_allOf::validate(
    std::stringstream& msg, const std::string& pathPrefix) const {
  bool success = true;
  const std::string _pathPrefix =
      pathPrefix.empty() ? "PointAltitudeUncertainty_allOf" : pathPrefix;

  /* Altitude */ {
    const double& value                = m_Altitude;
    const std::string currentValuePath = _pathPrefix + ".altitude";

    if (value < -32767) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to -32767;";
    }
    if (value > 32767) {
      success = false;
      msg << currentValuePath << ": must be less than or equal to 32767;";
    }
  }

  /* UncertaintyAltitude */ {
    const float& value                 = m_UncertaintyAltitude;
    const std::string currentValuePath = _pathPrefix + ".uncertaintyAltitude";

    if (value < static_cast<float>(0)) {
      success = false;
      msg << currentValuePath << ": must be greater than or equal to 0;";
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

bool PointAltitudeUncertainty_allOf::operator==(
    const PointAltitudeUncertainty_allOf& rhs) const {
  return

      (getPoint() == rhs.getPoint()) &&

      (getAltitude() == rhs.getAltitude()) &&

      (getUncertaintyEllipse() == rhs.getUncertaintyEllipse()) &&

      (getUncertaintyAltitude() == rhs.getUncertaintyAltitude()) &&

      (getConfidence() == rhs.getConfidence())

          ;
}

bool PointAltitudeUncertainty_allOf::operator!=(
    const PointAltitudeUncertainty_allOf& rhs) const {
  return !(*this == rhs);
}

void to_json(nlohmann::json& j, const PointAltitudeUncertainty_allOf& o) {
  j                        = nlohmann::json();
  j["point"]               = o.m_Point;
  j["altitude"]            = o.m_Altitude;
  j["uncertaintyEllipse"]  = o.m_UncertaintyEllipse;
  j["uncertaintyAltitude"] = o.m_UncertaintyAltitude;
  j["confidence"]          = o.m_Confidence;
}

void from_json(const nlohmann::json& j, PointAltitudeUncertainty_allOf& o) {
  j.at("point").get_to(o.m_Point);
  j.at("altitude").get_to(o.m_Altitude);
  j.at("uncertaintyEllipse").get_to(o.m_UncertaintyEllipse);
  j.at("uncertaintyAltitude").get_to(o.m_UncertaintyAltitude);
  j.at("confidence").get_to(o.m_Confidence);
}

oai::model::lmf::GeographicalCoordinates
PointAltitudeUncertainty_allOf::getPoint() const {
  return m_Point;
}
void PointAltitudeUncertainty_allOf::setPoint(
    oai::model::lmf::GeographicalCoordinates const& value) {
  m_Point = value;
}
double PointAltitudeUncertainty_allOf::getAltitude() const {
  return m_Altitude;
}
void PointAltitudeUncertainty_allOf::setAltitude(double const value) {
  m_Altitude = value;
}
oai::model::lmf::UncertaintyEllipse
PointAltitudeUncertainty_allOf::getUncertaintyEllipse() const {
  return m_UncertaintyEllipse;
}
void PointAltitudeUncertainty_allOf::setUncertaintyEllipse(
    oai::model::lmf::UncertaintyEllipse const& value) {
  m_UncertaintyEllipse = value;
}
float PointAltitudeUncertainty_allOf::getUncertaintyAltitude() const {
  return m_UncertaintyAltitude;
}
void PointAltitudeUncertainty_allOf::setUncertaintyAltitude(float const value) {
  m_UncertaintyAltitude = value;
}
int32_t PointAltitudeUncertainty_allOf::getConfidence() const {
  return m_Confidence;
}
void PointAltitudeUncertainty_allOf::setConfidence(int32_t const value) {
  m_Confidence = value;
}

}  // namespace oai::model::lmf

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
/*
 * EventReportingStatus.h
 *
 *
 */

#ifndef EventReportingStatus_H_
#define EventReportingStatus_H_

#include <nlohmann/json.hpp>

namespace oai::model::lmf {

/// <summary>
///
/// </summary>
class EventReportingStatus {
 public:
  EventReportingStatus();
  virtual ~EventReportingStatus() = default;

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

  bool operator==(const EventReportingStatus& rhs) const;
  bool operator!=(const EventReportingStatus& rhs) const;

  /////////////////////////////////////////////
  /// EventReportingStatus members

  /// <summary>
  ///
  /// </summary>
  int32_t getEventReportCounter() const;
  void setEventReportCounter(int32_t const value);
  bool eventReportCounterIsSet() const;
  void unsetEventReportCounter();
  /// <summary>
  ///
  /// </summary>
  int32_t getEventReportDuration() const;
  void setEventReportDuration(int32_t const value);
  bool eventReportDurationIsSet() const;
  void unsetEventReportDuration();

  friend void to_json(nlohmann::json& j, const EventReportingStatus& o);
  friend void from_json(const nlohmann::json& j, EventReportingStatus& o);

 protected:
  int32_t m_EventReportCounter;
  bool m_EventReportCounterIsSet;
  int32_t m_EventReportDuration;
  bool m_EventReportDurationIsSet;
};

}  // namespace oai::model::lmf

#endif /* EventReportingStatus_H_ */

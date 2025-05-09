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
 * AreaEventInfo.h
 *
 *
 */

#ifndef AreaEventInfo_H_
#define AreaEventInfo_H_

#include "ReportingArea.h"
#include "OccurrenceInfo.h"
#include <vector>
#include <nlohmann/json.hpp>

namespace oai::model::lmf {

/// <summary>
///
/// </summary>
class AreaEventInfo {
 public:
  AreaEventInfo();
  virtual ~AreaEventInfo() = default;

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

  bool operator==(const AreaEventInfo& rhs) const;
  bool operator!=(const AreaEventInfo& rhs) const;

  /////////////////////////////////////////////
  /// AreaEventInfo members

  /// <summary>
  ///
  /// </summary>
  std::vector<oai::model::lmf::ReportingArea> getAreaDefinition() const;
  void setAreaDefinition(
      std::vector<oai::model::lmf::ReportingArea> const& value);
  /// <summary>
  ///
  /// </summary>
  oai::model::lmf::OccurrenceInfo getOccurrenceInfo() const;
  void setOccurrenceInfo(oai::model::lmf::OccurrenceInfo const& value);
  bool occurrenceInfoIsSet() const;
  void unsetOccurrenceInfo();
  /// <summary>
  ///
  /// </summary>
  int32_t getMinimumInterval() const;
  void setMinimumInterval(int32_t const value);
  bool minimumIntervalIsSet() const;
  void unsetMinimumInterval();
  /// <summary>
  ///
  /// </summary>
  int32_t getMaximumInterval() const;
  void setMaximumInterval(int32_t const value);
  bool maximumIntervalIsSet() const;
  void unsetMaximumInterval();
  /// <summary>
  ///
  /// </summary>
  int32_t getSamplingInterval() const;
  void setSamplingInterval(int32_t const value);
  bool samplingIntervalIsSet() const;
  void unsetSamplingInterval();
  /// <summary>
  ///
  /// </summary>
  int32_t getReportingDuration() const;
  void setReportingDuration(int32_t const value);
  bool reportingDurationIsSet() const;
  void unsetReportingDuration();
  /// <summary>
  ///
  /// </summary>
  bool isReportingLocationReq() const;
  void setReportingLocationReq(bool const value);
  bool reportingLocationReqIsSet() const;
  void unsetReportingLocationReq();

  friend void to_json(nlohmann::json& j, const AreaEventInfo& o);
  friend void from_json(const nlohmann::json& j, AreaEventInfo& o);

 protected:
  std::vector<oai::model::lmf::ReportingArea> m_AreaDefinition;

  oai::model::lmf::OccurrenceInfo m_OccurrenceInfo;
  bool m_OccurrenceInfoIsSet;
  int32_t m_MinimumInterval;
  bool m_MinimumIntervalIsSet;
  int32_t m_MaximumInterval;
  bool m_MaximumIntervalIsSet;
  int32_t m_SamplingInterval;
  bool m_SamplingIntervalIsSet;
  int32_t m_ReportingDuration;
  bool m_ReportingDurationIsSet;
  bool m_ReportingLocationReq;
  bool m_ReportingLocationReqIsSet;
};

}  // namespace oai::model::lmf

#endif /* AreaEventInfo_H_ */

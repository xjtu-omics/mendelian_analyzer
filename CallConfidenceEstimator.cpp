/*
 * QualityEstimator.cpp
 *
 *  Created on: Jan 5, 2017
 *      Author: admin123
 */

#include "CallConfidenceEstimator.h"

#include <fstream>

#include "Utilities.h"


CallConfidenceEstimator::CallConfidenceEstimator() {
  // Step 1: read in coverage qualities
  std::ifstream coverage_quality_file("coverage_qualities.txt");
  coverage_quality_file >> max_support_ >> support_per_bin_;
  for (int i = 0; i <= max_support_ / support_per_bin_; i++ ) {
    int index;
    double confidence;

    coverage_quality_file >> index >> confidence;
    coverage_confidences_.push_back(confidence);
  }
  coverage_quality_file.close();

  // Step 2: read in VAF-related qualities
  std::ifstream vaf_quality_file("vaf_qualities.txt");

  vaf_quality_file >> num_vaf_bins_;
  for (int i = 0; i <= num_vaf_bins_; i++ ) {
    int index;
    double confidence;

    vaf_quality_file >> index >> confidence;
    vaf_confidences_.push_back(confidence);
  }
  vaf_quality_file.close();

  // Step 3: read in event-related qualities
  std::ifstream event_quality_file("event_qualities.txt");
  do {
    std::string event_name;
    double confidence;
    event_quality_file >> event_name >> confidence;
    if (event_quality_file.eof()) {
      break;
    }
    std::string event_name_minus_colon =
        Utilities::GetUntilFirstOccurrenceOf(event_name,':');
    EventType event_type = GetEventTypeFromString(event_name_minus_colon);
    eventtype_confidences_[event_type] = confidence;
  } while (true);
  event_quality_file.close();
}

CallConfidenceEstimator::~CallConfidenceEstimator() {
  // TODO Auto-generated destructor stub
}

/** What is the confidence in a certain prediction of a genotype? **/
double CallConfidenceEstimator::GetConfidence(const Event& event, const Support& support) {

  double min_confidence = 1.0;
  double event_type_confidence = eventtype_confidences_[event.GetType()];
  min_confidence = std::min(min_confidence, event_type_confidence);

  int coverage_bin_index = support.GetTotalSupport() / support_per_bin_;
  double coverage_confidence = coverage_confidences_[coverage_bin_index];
  min_confidence = std::min(min_confidence, coverage_confidence);

  int vaf_bin_index = support.GetVaf() * num_vaf_bins_;
  double vaf_confidence = vaf_confidences_[vaf_bin_index];
  //std::cout << "[va]" <<support.GetVaf() << "[vn]" << num_vaf_bins_ << "[vb]" << vaf_bin_index << "[vc]" << vaf_confidence << "[]";
  min_confidence = std::min(min_confidence, vaf_confidence);

  return min_confidence;

}


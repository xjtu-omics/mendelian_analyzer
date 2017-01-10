/*
 * CallConfidenceEstimator.h
 * 
 * Estimates the confidence we have in that a certain GenotypingResult
 * (the results of assessing a certain event in a particular sample) is called
 * correctly.
 *
 *  Created on: Jan 5, 2017
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef CALLCONFIDENCEESTIMATOR_H_
#define CALLCONFIDENCEESTIMATOR_H_

#include <map>
#include <vector>

#include "Event.h"
#include "Support.h"

class CallConfidenceEstimator {
public:
  CallConfidenceEstimator();
  virtual ~CallConfidenceEstimator();

  double GetConfidence(const Event& event, const Support& support);

private:

  int max_support_;
  int support_per_bin_;
  int num_vaf_bins_;

  std::vector<double> coverage_confidences_;
  std::map<EventType, double> eventtype_confidences_;
  std::vector<double> vaf_confidences_;
};

#endif /* CALLCONFIDENCEESTIMATOR_H_ */

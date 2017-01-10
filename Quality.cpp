/*
 * Quality.cpp
 *
 * Contains an element that assesses the quality associated with a certain
 * descriptor: how many individuals/samples possessing a certain value /
 * value range of the descriptor have been called correctly or incorrectly.
 *
 *  Created on: Dec 27, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#include "Quality.h"

/** Constructor **/
Quality::Quality() {
  correct_calls_ = 0;
  incorrect_calls_ = 0;
}

Quality::~Quality() {
  // TODO Auto-generated destructor stub
}

/** Adds an occurrence/'event': whether something went right (success == true)
    or wrong (success == false) **/
void Quality::AddOccurrence(bool success) {
  if (success) {
    ++correct_calls_;
  }
  else {
    ++incorrect_calls_;
  }
}

/** Returns the fraction of correctness (0.0 - 1.0). If no data is available,
 * assume 100% correctness. **/
double Quality::GetCorrectnessFraction() const {
  if (GetTotalCalls() == 0) {
    return 1.0;
  }
  else {
    return correct_calls_ / static_cast<double>(GetTotalCalls());
  }
}

/** returns the total number of calls. **/
int Quality::GetTotalCalls() const {
  return correct_calls_ + incorrect_calls_;
}

/** overloaded outputstream operator for Quality objects. **/
std::ostream& operator<<(std::ostream& os, const Quality& quality) {

  os << quality.correct_calls_ << "\t" << quality.incorrect_calls_ <<
      " (";
  int total_calls = quality.GetTotalCalls();
  if (total_calls  == 0) {
    os << "undefined";
  }
  else {
    os << 100 * quality.correct_calls_ / static_cast<double>(total_calls);
  }
  os << "%)";
  return os;
}


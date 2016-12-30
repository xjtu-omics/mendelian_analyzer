/*
 * Quality.cpp
 *
 *  Created on: Dec 27, 2016
 *      Author: admin123
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

/** Increases the number of correct calls. **/
void Quality::IncreaseCorrect() {
  ++correct_calls_;
}

/** Increases the count of incorrect calls. **/
void Quality::IncreaseIncorrect() {
  ++incorrect_calls_;
}

/** overloaded outputstream operator for Quality objects. **/
std::ostream& operator<<(std::ostream& os, const Quality& quality) {

  os << quality.correct_calls_ << "\t" << quality.incorrect_calls_;
  return os;
}


/*
 * Quality.h
 *
 * Contains an element that assesses the quality associated with a certain
 * descriptor: how many individuals/samples possessing a certain value /
 * value range of the descriptor have been called correctly or incorrectly.
 *
 *  Created on: Dec 27, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef QUALITY_H_
#define QUALITY_H_

#include <iostream>

class Quality {
friend std::ostream& operator<<(std::ostream& os, const Quality& quality);

public:
  Quality();
  virtual ~Quality();

  void AddOccurrence(bool success);
  double GetCorrectnessFraction() const;


private:
  int GetTotalCalls() const;

  int correct_calls_;
  int incorrect_calls_;

};

#endif /* QUALITY_H_ */

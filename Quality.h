/*
 * Quality.h
 *
 *  Created on: Dec 27, 2016
 *      Author: admin123
 */

#ifndef QUALITY_H_
#define QUALITY_H_

#include <iostream>

class Quality {
friend std::ostream& operator<<(std::ostream& os, const Quality& quality);

public:
  Quality();
  virtual ~Quality();

  void IncreaseCorrect();
  void IncreaseIncorrect();

private:
  int correct_calls_;
  int incorrect_calls_;

};

#endif /* QUALITY_H_ */

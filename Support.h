/*
 * Support.h
 *
 * Contains counts of the reads supporting different alleles for a particular
 * variant for a particular sample.
 *
 *  Created on: Dec 23, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <string>
#include <vector>

class Support {
public:
  Support(const std::string& raw_genotype);
  virtual ~Support();

  int GetTotalSupport();
  double GetVaf();

private:
  std::vector<int> supports_;
};

#endif /* SUPPORT_H_ */

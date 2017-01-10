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
friend std::ostream& operator<<(std::ostream& os, Support support);


public:
  Support(const std::string& raw_genotype);
  Support();
  virtual ~Support();

  int GetTotalSupport() const;
  double GetVaf() const;
  bool IndicatesUnknownGenotype() const;
  static Support NoSupport();

private:
  std::vector<int> supports_;
};

#endif /* SUPPORT_H_ */

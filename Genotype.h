/*
 * Genotype.h
 *
 * Contains the basic information of a genotype.
 *
 *  Created on: Dec 23, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef GENOTYPE_H_
#define GENOTYPE_H_

#include <iostream>
#include <string>
#include <vector>

/** 'Genotype' stores a genotype as a list of numbers. Usually 2, can be one
* (Y chromosome genotypes, for example). Can be 0, 1, 0/0, 0/1, 0/2, 1/2, etc. **/
class Genotype {
  friend std::ostream& operator<<(std::ostream& os, const Genotype& genotype);

public:
  Genotype(const std::string& genotypeAsString);
  Genotype();
  ~Genotype();

  std::string AsString() const;
  int GetAllele(int alleleIndex) const;
  bool IsHaploid() const;
  bool HasAllele(int allele) const;
  int GetNumberOfAlleles() const;
  bool IsUnknown() const;
  bool IsHomRef() const;
  void SetToUnknown();

private:

  void AbortWhenUnknown() const;

  std::vector<int> m_alleles;
  bool m_isUnknown;
};

#endif /* GENOTYPE_H_ */

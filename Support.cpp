/*
 * Support.cpp
 *
 * Contains counts of the reads supporting different alleles for a particular
 * variant for a particular sample.
 *
 *  Created on: Dec 23, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#include "Support.h"

#include <cstdlib>
#include <string>

#include "Utilities.h"

std::ostream& operator<<(std::ostream& os, Support support) {
  for (size_t index = 0; index < support.supports_.size(); ++index) {
    os << support.supports_[index];
    if (index < support.supports_.size() - 1) {
      os << ",";
    }
  }
  return os;
}

/** Constructor. Takes the 'miscellaneous info' part of the genotype string.
 * So if the full VCF genotyping string is "0/1:15,25", then the constructor
 * takes "15,25" as input. **/
Support::Support(const std::string& support_string) {
  std::vector<std::string> supports_as_strings = Utilities::Split(support_string,',');
  for (size_t index = 0; index < supports_as_strings.size(); ++index) {
    supports_.push_back(atoi(supports_as_strings[index].c_str()));
  }
}

/** To allow Support to be a regular data member of another class. **/
Support::Support() {
}

/** returns the Variant Allele Frequency. **/
double Support::GetVaf() const {
  Utilities::Require(supports_.size() == 2,
      "Support::GetVaf error: there need to be two alleles to recover the"
      "variant allele frequency.");
  /*Utilities::Require(GetTotalSupport() > 0,
      "Support::GetVaf error: the total support must be greater than zero");*/
  int total_support = GetTotalSupport();
  if (total_support <= 0) {
    return -1;
  }

  int alt_coverage = total_support - supports_[0];
  return alt_coverage / static_cast<double>(total_support);
}


/** Returns the total support for all alleles at a certain position. **/
int Support::GetTotalSupport() const {
  int total_support = 0;
  for (size_t index = 0; index < supports_.size(); ++index) {
    total_support += supports_[index];
  }
  return total_support;
}

/** In some cases, the support provided by Pindel indicates that we actually
 * do not know the genotype, despite an official genotype call. **/
bool Support::IndicatesUnknownGenotype() const {
  if (supports_.size() == 0) {
    return true;
  }
  else if (GetTotalSupport() == 0) {
    return true;
  }
  else if (supports_[0] < 0 ) {
    // pindel uses refcount of -1 to indicate a problem
    return true;
  }
  else {
    return false;
  }
}

Support::~Support() {
  // TODO Auto-generated destructor stub
}

Support Support::NoSupport() {
  Support dummy_support("");
  return dummy_support;
}


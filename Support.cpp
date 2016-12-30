/*
 * Support.cpp
 *
 * Contains counts of the reads supporting different alleles for a particular
 * variant for a particular sample.
 *
 *  Created on: Dec 23, 2016
 *      Author: admin123
 */

#include "Support.h"

#include <cstdlib>
#include <string>

#include "Utilities.h"

/** Constructor **/
Support::Support(const std::string& raw_genotype) {
  const char SEPARATOR = ':';
  size_t separator_pos = raw_genotype.find_first_of(SEPARATOR);
  Utilities::Require(separator_pos != std::string::npos,
      "Support constructor error: this genotype string does not seem to"\
      "provide information on the number of supporting reads.");
  std::string support_string = raw_genotype.substr(separator_pos + 1);
  std::vector<std::string> supports_as_strings = Utilities::Split(support_string,',');
  for (size_t index = 0; index < supports_as_strings.size(); ++index) {
    supports_.push_back(atoi(supports_as_strings[index].c_str()));
  }

}

/** returns the Variant Allele Frequency. **/
double Support::GetVaf() {
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
int Support::GetTotalSupport() {
  int total_support = 0;
  for (size_t index = 0; index < supports_.size(); ++index) {
    total_support += supports_[index];
  }
  return total_support;
}

Support::~Support() {
  // TODO Auto-generated destructor stub
}


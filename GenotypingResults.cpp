/*
 * GenotypingData.cpp
 *
 * Contains data gathered during the caller's genotyping process, usually
 * displayed in a VCF file in the item that lies in the intersection of the
 * row representing the event and the column representing the sample.
 *
 *  Created on: Jan 3, 2017
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#include "GenotypingResults.h"

#include <iostream>

#include "Utilities.h"

std::ostream& operator<<(std::ostream& os,
    GenotypingResults genotyping_results) {

  os << genotyping_results.GetGenotype();

  Support support = genotyping_results.GetSupport();
  //if (!support.IndicatesUnknownGenotype()) {
    os << ":";
    os << support;
  //}
  return os;
}

/** Constructor. Note; while pointers may be more elegant for genotype and
 * support, they gave quite a headache **/
GenotypingResults::GenotypingResults(const std::string& genotyping_data_as_string,
    bool is_pindel) {


  size_t colon_position = genotyping_data_as_string.find_first_of(':');
  std::string basic_genotype_string =
      genotyping_data_as_string.substr(0, colon_position);
  Genotype new_genotype(basic_genotype_string);
  genotype_ = new_genotype;
  if (is_pindel) {
    std::string extra_genotyping_info =
        genotyping_data_as_string.substr(colon_position + 1);
    Support new_support(extra_genotyping_info);
    support_ = new_support;
    if (support_.IndicatesUnknownGenotype()) {
      genotype_.SetToUnknown();
    }
  }
  else {
    support_ = Support("");
  }
}

/** Destructor **/
GenotypingResults::~GenotypingResults() {
}

/** returns the genotype (aborts when genotype pointer is NULL) **/
Genotype& GenotypingResults::GetGenotype() {
  return genotype_;
}

/** Returns the support of the genotype (if any) **/
Support& GenotypingResults::GetSupport() {
    return support_;
}

/** returns the genotype (aborts when genotype pointer is NULL) **/
const Genotype& GenotypingResults::GetGenotype() const {
  return genotype_;
}

/** Returns the support of the genotype (if any) **/
const Support& GenotypingResults::GetSupport() const {
    return support_;
}



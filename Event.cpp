/*
 * Event.cpp
 *
 * Contains data about an event (a putative difference in genotype with the
 * reference genome) in one or more of the samples described by this VCF file.
 *
 *  Created on: Dec 19, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#include "Event.h"

#include "Utilities.h"

/** Event constructor **/
Event::Event(const std::string& reference, const std::string& alt) {
  reference_ = reference;
  alternatives_ = Utilities::Split(alt,',');
}

Event::~Event() {
  // TODO Auto-generated destructor stub
}

/** Get the size of the alternative allele. Note that this is a rather primitive
 * function, some VCF formats will have a <DEL> with some indication of length
 * in the INFO field; this function should ultimately handle those too.  **/
int Event::AltSize() const {
  Utilities::Require(HasSingleAltAllele(),
      "Event::AltSize() error: there are multiple alt alleles");
  return static_cast<int>(alternatives_[0].size());
}

/** Does this event only have a single alternative allele (as opposed to
    multiple? **/
bool Event::HasSingleAltAllele() const {
  return alternatives_.size() == 1;
}

/** Returns the size of the reference allele. **/
int Event::RefSize() const {
  return static_cast<int>(reference_.size());
}



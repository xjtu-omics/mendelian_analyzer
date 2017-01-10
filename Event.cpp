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

/** What kind of type is this event? **/
EventType Event::GetType() const {
  if (!HasSingleAltAllele()) {
    return kUnknown;
  }
  if (RefSize() == 1 && AltSize() == 1) {
    return kSnp;
  }
  else if (RefSize() == 1 && AltSize() > 1) {
    return kIns;
  }
  else if (RefSize() > 1 && AltSize() == 1) {
    return kDel;
  }
  else {
    return kRpl;
  }
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

/** Transforms the name of an event to the type of event. **/
EventType GetEventTypeFromString(const std::string& input) {
  std::map<std::string, EventType> event_name_map;
  event_name_map["snp"] = kSnp;
  event_name_map["ins"] = kIns;
  event_name_map["del"] = kDel;
  event_name_map["rpl"] = kRpl;
  event_name_map["unknown"] = kUnknown;

  std::string normalized_input = Utilities::StringToLowerCase(input);
  Utilities::Require(Utilities::MapContainsKey(event_name_map,
      normalized_input), "GetEventTypeFromString error: SV type unknown.");
  return event_name_map[normalized_input];
}




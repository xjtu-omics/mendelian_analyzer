/*
 * GlobalSettings.cpp
 *
 * Creates an object that contains the global settings, for example which
 * variants should be analyzed.
 *
 *  Created on: Dec 20, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#include "Settings.h"

#include <algorithm>
#include <climits>
#include <iostream>
#include <string>

#include "Utilities.h"

/** Constructor; sets initial values for variables. **/
Settings::Settings() {
  variant_type_ = UNDEFINED;
  is_pindel_ = false;
  min_size_ = 0;
  max_size_ = INT_MAX;
  min_confidence_ = 0;
  string_to_variant_type_map_["all"] = ALL;
  string_to_variant_type_map_["snp"] = SNP;
  string_to_variant_type_map_["sv"] = SV;
  string_to_variant_type_map_["del"] = DEL;
  string_to_variant_type_map_["ins"] = INS;
  string_to_variant_type_map_["rpl"] = RPL;
}

/** Completes the settings, if necessary. **/
void Settings::Complete() {
  if (variant_type_ == UNDEFINED) {
    std::cout << "Variant type undefined, setting it to 'ALL'\n";
    variant_type_ = ALL;
  }
}

/** Adds an argument to global settings; aborts if an error happens. **/
void Settings::Add(const char* command_line_argument) {
  std::string argument = command_line_argument;
  std::transform(argument.begin(), argument.end(), argument.begin(), ::tolower);
  if (argument == "pindel") {
    Utilities::Require(is_pindel_ == false,
        "GlobalSettings::add error: cannot set pindel twice!");
    is_pindel_ = true;
  }
  else if (Utilities::MapContainsKey(string_to_variant_type_map_,argument)) {
    Utilities::Require(variant_type_ == UNDEFINED,
              "GlobalSettings::add error: conflicting SV requests!");

    variant_type_ = string_to_variant_type_map_[argument];
  } else if (Utilities::StringStartsWith(argument,"size")) {
    std::string comparator = argument.substr(4,2);
    //std::cout << "Comparator: " << comparator << "\n";
    Utilities::Require(comparator == "gt" || comparator == "lt",
        "GlobalSettings::add error: size argument is malformed.");
    size_t startOfNumber = 6;
    std::string numberCandidate = argument.substr(startOfNumber);
    Utilities::Require(Utilities::isPositiveInteger(numberCandidate),
        "GlobalSettings::add error: size argument is not a number.");
    int targetSize = atoi(numberCandidate.c_str());
    if (comparator == "lt") {
      Utilities::Require(max_size_ == INT_MAX,
                "GlobalSettings::add error: Cannot change maximum size twice!");
      max_size_ = targetSize - 1;
    }
    else { // size> or size>=
      Utilities::Require(min_size_ == 0,
          "GlobalSettings::add error: Cannot change minimum size twice!");
      min_size_ = targetSize + 1;
    }
  } else if (Utilities::StringStartsWith(argument,"c")) {
    std::string numberCandidate = argument.substr(1);
    min_confidence_ = atof(numberCandidate.c_str());
    std::cout << "Min confidence = " << min_confidence_ << std::endl;
  } else {
    Utilities::Require(false,
            "GlobalSettings::add error: argument is invalid");
  }
}

/** get the variant type **/
VariantType Settings::GetVariantType() const {
  return variant_type_;
}

/** Get the minimal confidence one must have to make a call **/
double Settings::GetMinConfidence() const {
  return min_confidence_;
}

/** Get the minimal size of an event **/
int Settings::GetMinEventSize() const {
  return min_size_;
}


/** Get the maximal size of an event. **/
int Settings::GetMaxEventSize() const {
  return max_size_;
}


/** Is this a pindel input file? **/
bool Settings::isPindel() const {
  return is_pindel_;
}


Settings::~Settings() {
  // TODO Auto-generated destructor stub
}

/** is this event of a type that should be investigated? **/
bool Settings::isTypeCorrect(int ref_size, int alt_size) const{
  if (variant_type_ == ALL) {
    return true;
  }
  else if (ref_size == 1 ) {

    if (alt_size == 1) {
      // ref size == 1, alt_size == 1. SNP
      return (variant_type_ == SNP);
    }
    else {
      // ref size == 1, alt size is greater than 1. Insertion!
      return ((variant_type_ == SV) || (variant_type_ == INS));

    }
  }
  else {
    // ref size !=1. Deletion or RPL
    if (alt_size == 1) {
      // deletion
      return ((variant_type_ == SV) || (variant_type_ == DEL));
    }
    else {
      // replacement
      return ((variant_type_ == SV) || (variant_type_ == RPL));
    }
  }
}

/** is the size of the event within acceptable bounds? **/
bool Settings::isSizeCorrect(int size) const {
  return Utilities::isInclusiveBetween(size, min_size_, max_size_);
}


/** Should this variant be analyzed? **/
bool Settings::ShouldAnalyzeVariant(const Event& event) const {

  if (!event.HasSingleAltAllele()) {
    // events with multiple alt alleles are hard to genotype;
    // skip them from statistics
    return false;
  }
  int ref_size = event.RefSize();
  int alt_size = event.AltSize();

  int size_to_check = std::max(ref_size, alt_size);
  if (size_to_check > 1) {
    // no SNP
    --size_to_check;
  }

  return (isTypeCorrect(ref_size,alt_size) && isSizeCorrect(size_to_check));
}



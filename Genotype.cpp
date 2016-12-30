/*
 * Genotype.cpp
 *
 * Contains the basic information of a genotype.
 *
 *  Created on: Dec 23, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#include "Genotype.h"

#include <cstdlib>
#include <sstream>

#include "Utilities.h"

/** formats the genotype neatly for output purposes. **/
std::ostream& operator<<(std::ostream& os, const Genotype& genotype) {
  if (genotype.isUnknown()) {
    os << "unknown";
  }
  else {
    os << genotype.getAllele(0);
    for (int genotypeIndex = 1; genotypeIndex < genotype.getNumberOfAlleles();
        ++genotypeIndex) {

      os << "/" << genotype.getAllele(genotypeIndex);
    }
  }
  return os;
}

/** returns the genotype as a string. **/
std::string Genotype::AsString() const {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}


/** Aborts the program when the user tries to get 'forbidden' info from an
    unknown genotype. **/
void Genotype::AbortWhenUnknown() const {
  if (isUnknown()) {
    Utilities::Require(false, "Genotype::getAllele error: genotype is unknown!\n");
  }
}
/** Is this genotype homozygous reference? **/
bool Genotype::isHomRef() const {
  AbortWhenUnknown();
  for (size_t allele_index = 0; allele_index < m_alleles.size();
      ++allele_index) {
    if (m_alleles[allele_index] != 0) {
      return false;
    }
  }
  return true;
}

/** is this genotype (certainly) haploid? (unknown does not count). **/
bool Genotype::isHaploid() const {
  return (m_alleles.size() == 1);
}

/** sets the genotype to 'unknown' **/
void Genotype::setToUnknown() {
  m_isUnknown = true;
}

/** Returns the number of alleles. **/
int Genotype::getNumberOfAlleles() const {
  AbortWhenUnknown();
  return m_alleles.size();
}

/** Returns the allele at position 'alleleIndex' (so 0 or 1 for
 * a diploid genome) **/
int Genotype::getAllele(int alleleIndex) const {
  AbortWhenUnknown();
  return m_alleles[alleleIndex];
}

/** Genotype constructor. **/
Genotype::Genotype(const std::string& genotypeAsString, bool isPindel) {
  //std::cout << "GTAS" << genotypeAsString << std::endl;

  std::string basicGenotypeString =
      Utilities::GetUntilFirstOccurrenceOf(genotypeAsString, ':');
  if (isPindel) {
    size_t colonPos = genotypeAsString.find_first_of(':');
    std::string afterColon = genotypeAsString.substr(colonPos + 1);
    //std::cout << "AC" << afterColon << "\n";
    //pause();
    if (Utilities::StringStartsWith(afterColon,"-1") || Utilities::StringStartsWith(afterColon, "0,0")) {
      //std::cout << "ALARM";
      setToUnknown();
      return;
    }
  }
  //std::cout << "BGTAS" << genotypeAsString << std::endl;
  std::vector<std::string> alleles = Utilities::Split(basicGenotypeString, '/');
  for (std::vector<std::string>::iterator iter = alleles.begin();
    iter != alleles.end(); iter++ ) {

    std::string currentGenotypeAsString = *iter;
    if (currentGenotypeAsString == ".") {
      setToUnknown();
      return;
    }
    if (Utilities::isPositiveInteger(currentGenotypeAsString)) {
      m_alleles.push_back(atoi(currentGenotypeAsString.c_str()));
    }
    else {
      Utilities::Require(false, "Genotype constructor error: genotype " +
          genotypeAsString +" is unknown!");
    }
  }
  // if you get here, everything is normal
  m_isUnknown = false;
}

/** Does this genotype have a certain allele? **/
bool Genotype::hasAllele(int allele) const {
  if (isUnknown()) {
    Utilities::Require(false, "Genotype::hasAllele error: genotype is unknown!\n");
  }
  int numberOfAlleles = m_alleles.size();
  for (int alleleIndex = 0; alleleIndex < numberOfAlleles; alleleIndex++) {
    if (m_alleles[alleleIndex] == allele) {
      return true;
    }
  }
  return false;
}

/** is the genotype known? **/
bool Genotype::isUnknown() const {
  return m_isUnknown;
}

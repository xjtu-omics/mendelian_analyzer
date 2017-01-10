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
  if (genotype.IsUnknown()) {
    os << ".";
  }
  else {
    os << genotype.GetAllele(0);
    for (int genotypeIndex = 1; genotypeIndex < genotype.GetNumberOfAlleles();
        ++genotypeIndex) {
      os << "/" << genotype.GetAllele(genotypeIndex);
    }
  }
  return os;
}

Genotype::Genotype() {
  m_isUnknown = true;
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
  if (IsUnknown()) {
    Utilities::Require(false, "Genotype::AbortWhenUnknown error: genotype is unknown!\n");
  }
}
/** Is this genotype homozygous reference? **/
bool Genotype::IsHomRef() const {
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
bool Genotype::IsHaploid() const {
  return (m_alleles.size() == 1);
}

/** sets the genotype to 'unknown' **/
void Genotype::SetToUnknown() {
  m_isUnknown = true;
}

/** Returns the number of alleles. **/
int Genotype::GetNumberOfAlleles() const {
  if (m_isUnknown) {
    return 0;
  }
  else {
    return m_alleles.size();
  }
}

/** Returns the allele at position 'alleleIndex' (so 0 or 1 for
 * a diploid genome) **/
int Genotype::GetAllele(int alleleIndex) const {
  AbortWhenUnknown();
  return m_alleles[alleleIndex];
}

/** Genotype constructor. Takes the genotype as string (so for example "0/1")**/
Genotype::Genotype(const std::string& genotype_as_string) {
  //std::cout << "GTstart" << genotype_as_string;
  std::vector<std::string> alleles = Utilities::Split(genotype_as_string, '/');
  for (std::vector<std::string>::iterator iter = alleles.begin();
    iter != alleles.end(); iter++ ) {

    std::string currentGenotypeAsString = *iter;
    if (currentGenotypeAsString == ".") {
      SetToUnknown();
      return;
    }
    if (Utilities::isPositiveInteger(currentGenotypeAsString)) {
      m_alleles.push_back(atoi(currentGenotypeAsString.c_str()));
    }
    else {
      Utilities::Require(false, "Genotype constructor error: genotype " +
          genotype_as_string +" is unknown!");
    }
  }
  // if you get here, everything is normal
  m_isUnknown = false;
  //std::cout << "GTend";
}

/** Does this genotype have a certain allele? **/
bool Genotype::HasAllele(int allele) const {
  if (IsUnknown()) {
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
bool Genotype::IsUnknown() const {
  return m_isUnknown;
}

Genotype::~Genotype() {
  m_alleles.clear();
}

/*
 * BioUtils.cpp
 *
 *  Created on: Dec 27, 2016
 *      Author: admin123
 */

#include "BioUtils.h"

#include <sstream>

#include "Utilities.h"

namespace BioUtils {

const int GENOTYPES_01 = 1;
const int GENOTYPES_12 = 12;

/** What is the 'genotypic depth pattern of a duo. Usually, for a pair of
    parents, the pattern would be 22 (both members have two chromosomes of each
    type), though the pattermn will be 12 for the X chromosome and 01 for the Y
    chromosome. **/
static int getGenotypicDepthPattern(const Genotype& firstParentGenotype,
    const Genotype& secondParentGenotype) {
  int firstDepth = firstParentGenotype.isUnknown() ?
      0 : firstParentGenotype.getNumberOfAlleles();
  int secondDepth = secondParentGenotype.isUnknown() ?
        0 : secondParentGenotype.getNumberOfAlleles();
  if (firstDepth > secondDepth) {
    std::swap(firstDepth, secondDepth);
  }
  return (10 * firstDepth + secondDepth);
}

/** Can you assess Mendelian correctness? In most cases, you need the genotypes
 * of both parents and the child. However, in the case of father-mother-son, the
 * mother is allowed to have an unknown/undefined genotype on the Y chromosome.
 * Still, father's and son's haplotype should match unless affecterd by a
 * de novo mutation. **/
bool canAssessMendelianCorrectness(const Genotype& firstParentGenotype,
    const Genotype& secondParentGenotype, const Genotype& childGenotype) {
  if (childGenotype.isUnknown()) {
    return false;
  }
  if (childGenotype.isHaploid()) { // son X or Y
    int genotypicDepthPattern =
        getGenotypicDepthPattern(firstParentGenotype, secondParentGenotype);
    return ((genotypicDepthPattern == GENOTYPES_01) || // Y chromosome son
            (genotypicDepthPattern == GENOTYPES_12)); // X chromosome son
  }
  if (firstParentGenotype.isUnknown() || secondParentGenotype.isUnknown()) {
    return false;
  }
  // otherwise, we can check for Mendelian correctness
  return true;
}

/** Can this combination of parents and children be correct,
    Mendelian-speaking... **/
bool canBeMendelianCorrect(const Genotype& firstParent,
    const Genotype& secondParent, const Genotype& child) {

  int childsNumberOfAlleles = child.getNumberOfAlleles();
  if (childsNumberOfAlleles == 2) {
    // normal diploid inheritance
    if (
        (firstParent.hasAllele(child.getAllele(0)) &&
         secondParent.hasAllele(child.getAllele(1)) )
        ||
         (firstParent.hasAllele(child.getAllele(1)) &&
          secondParent.hasAllele(child.getAllele(0)) )
        ) {
      return true;
    }
  }
  else if (childsNumberOfAlleles == 1) {
    // haploid inheritance! Like male child on X and Y chromosome
    return (firstParent.hasAllele(child.getAllele(0))||
        secondParent.hasAllele(child.getAllele(0)));
  }
  else {
    Utilities::Require(false, "canBeMendelianCorrect error: cannot handle children "
        "with more than 2 or less than 1 allele");
  }
  return false;
}

/** Returns basic data about this variant; currently the sequences of the ref
 * and alt alleles. */
void GetDataAboutVariant(std::stringstream& ss, std::string& refSequence,
    std::string& altSequence) {

  std::string dummy;
  for (int i = 0; i < 3; i++ ) {
    ss >> dummy;
  }
  ss >> refSequence;
  ss >> altSequence;

  for (int i = 0; i < 4; i++ ) {
    ss >> dummy;
  }
}

}; // namespace BioUtils

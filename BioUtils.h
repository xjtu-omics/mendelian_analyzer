/*
 * BioUtils.h
 *
 * Utility functions that really don't belong to the core functionality of any
 * class, but that are biologically useful.
 *
 *  Created on: Dec 27, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef BIOUTILS_H_
#define BIOUTILS_H_

#include "Genotype.h"

namespace BioUtils {
  bool canAssessMendelianCorrectness(const Genotype& firstParentGenotype,
    const Genotype& secondParentGenotype, const Genotype& childGenotype);
  bool canBeMendelianCorrect(const Genotype& firstParent,
      const Genotype& secondParent, const Genotype& child);
  void GetDataAboutVariant(std::stringstream& ss, std::string& refSequence,
      std::string& altSequence);

}; // namespace bioutils

#endif /* BIOUTILS_H_ */

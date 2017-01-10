/*
 * GenotypingData.h
 *
 * Contains data gathered during the caller's genotyping process, usually
 * displayed in a VCF file in the item that lies in the intersection of the
 * row representing the event and the column representing the sample.
 *
 *  Created on: Jan 3, 2017
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef GENOTYPINGRESULTS_H_
#define GENOTYPINGRESULTS_H_



#include "Genotype.h"
#include "Support.h"

class GenotypingResults {
  friend std::ostream& operator<<(std::ostream& os,
      GenotypingResults genotyping_results);

public:
  GenotypingResults(const std::string& genotyping_data_as_string, bool is_pindel);
  virtual ~GenotypingResults();

  Genotype& GetGenotype();
  Support& GetSupport();
  const Genotype& GetGenotype() const;
  const Support& GetSupport() const;

private:
  Genotype genotype_;
  Support support_;
};

#endif /* GENOTYPINGRESULTS_H_ */

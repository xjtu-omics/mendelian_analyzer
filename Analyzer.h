/*
 * Analyzer.h
 *
 *  Analyzer is a class that stores the running analysis data, in order to
 *  reduce the number of parameters that have to be passed by the various
 *  functions/methods. Though I guess that an analysis object could also have
 *  been okay.
 *
 *  Created on: Dec 27, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef ANALYZER_H_
#define ANALYZER_H_

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Genotype.h"
#include "GlobalSettings.h"


class Analyzer {
public:
  Analyzer(const std::string& fileName, const GlobalSettings* const settings);
  void analyze();
  virtual ~Analyzer();

private:
  bool AnalyzeSamples(std::stringstream& ss, const std::string& line,
      bool* all_genotyped);
  bool ClassifyTrio(const std::vector<Genotype>& genotypes,
      const std::string& line, bool* is_genotypable);
  void ShowMendelianErrorMap() const;
  void ShowResults() const;
  void UpdateMendelianErrorMap(const Genotype& firstParentGenotype,
      const Genotype& secondParentGenotype, const Genotype& childGenotype);

  int m_unknown;
  int m_mendelianCorrect;
  int m_mendelianError;
  std::ifstream m_inputFile;
  int all_genotyped_;
  int all_homref_;
  int event_count_;
  int trio_count_[4] = {0,0,0,0};
  const GlobalSettings* settings_;
  std::map<std::string, int> me_map_;
};

#endif /* ANALYZER_H_ */

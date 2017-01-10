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

#include "CallConfidenceEstimator.h"
#include "Event.h"
#include "Genotype.h"
#include "GenotypingResults.h"
#include "Quality.h"
#include "Settings.h"


class Analyzer {
public:
  Analyzer(const std::string& fileName, const Settings* const settings,  const::std::string& name_of_output_file);
  void analyze();
  virtual ~Analyzer();

private:
  bool AnalyzeSamples(std::stringstream& ss, const Event& event,
      const std::string& line, bool* all_genotyped, std::stringstream& buffer_ss);
  bool ClassifyTrio(const std::vector<GenotypingResults>& genotyping_results,
      const Event& event, const std::string& line, bool* is_genotypable);
  void ScoreAsCorrect(const std::vector<GenotypingResults>& genotyping_results,
      const Event& event, bool is_correct, const std::string& line);

  void ShowMendelianErrorMap() const;
  void ShowResults() const;
  void UpdateMendelianErrorMap(const Genotype& firstParentGenotype,
      const Genotype& secondParentGenotype, const Genotype& childGenotype);
  void OutputBins() const;


  static const int NUM_VAF_BINS = 40;
  Quality vaf_qualities_[NUM_VAF_BINS];

  static const int COVERAGE_RESOLUTION = 5;
  static const int MAX_SUPPORT = 200;
  Quality coverage_qualities_[1 + MAX_SUPPORT / COVERAGE_RESOLUTION];

  std::map<EventType, Quality> eventtype_qualities_;

  CallConfidenceEstimator call_confidence_estimator_;
  std::ofstream output_file_;
  int unknown_count_;
  int m_mendelianCorrect;
  int m_mendelianError;
  std::ifstream m_inputFile;
  int all_genotyped_;
  int all_homref_;
  int event_count_;
  int trio_count_[4] = {0,0,0,0};
  int unknown_calls_;
  int homref_calls_;
  int variant_calls_;

  const Settings* settings_;
  std::map<std::string, int> me_map_;
};

#endif /* ANALYZER_H_ */

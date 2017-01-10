/*
 * Analyzer.cpp
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

#include "Analyzer.h"

#include <cstdlib>
#include <sstream>

#include "BioUtils.h"
#include "Event.h"
#include "GenotypingResults.h"
#include "Utilities.h"

/** Analyzer constructor. **/
Analyzer::Analyzer( const std::string& nameOfInputFile,
    const Settings* settings, const::std::string& name_of_output_file): settings_(settings) {
  m_inputFile.open( nameOfInputFile.c_str() );
  output_file_.open(name_of_output_file.c_str());
  if (!m_inputFile) { // input file does not exist
    std::cout << "Input file does not exist.\n";
        exit(-1);
  }
  unknown_calls_ = 0;
  homref_calls_ = 0;
  variant_calls_ = 0;

  unknown_count_ = 0;
  m_mendelianCorrect = 0;
  m_mendelianError = 0;
  all_homref_ = 0;
  event_count_ = 0;
  all_genotyped_  = 0;
  Quality default_quality;
  eventtype_qualities_[kSnp] = default_quality;
  eventtype_qualities_[kUnknown] = default_quality;
  eventtype_qualities_[kIns] = default_quality;
  eventtype_qualities_[kDel] = default_quality;
  eventtype_qualities_[kRpl] = default_quality;

}

/** Destructor **/
Analyzer::~Analyzer() {
  m_inputFile.close();
}

/** Updates the Mendelian error map. **/
void Analyzer::UpdateMendelianErrorMap(const Genotype& firstParentGenotype,
    const Genotype& secondParentGenotype, const Genotype& childGenotype) {

  // first sort parents' genotypes
  std::string first_parent_as_string = firstParentGenotype.AsString();
  std::string second_parent_as_string = secondParentGenotype.AsString();
  std::string parents_string =
      (first_parent_as_string <= second_parent_as_string) ?
          (first_parent_as_string + "," + second_parent_as_string) :
          (second_parent_as_string + "," + first_parent_as_string);
  // Now construct total identifier string
  std::string id_string = parents_string + " => " + childGenotype.AsString();
  if (Utilities::MapContainsKey(me_map_, id_string)) {
    me_map_[id_string]++;
  }
  else {
    me_map_[id_string] = 1;
  }


}

/** Scores a vector of genotypes as correct or incorrect **/
void Analyzer::ScoreAsCorrect(
    const std::vector<GenotypingResults>& genotyping_results,
    const Event& event,
    bool is_correct,
    const std::string& line) {
  for (size_t index = 0; index < genotyping_results.size(); index++) {

    // first: score VAFs
    double vaf = genotyping_results[index].GetSupport().GetVaf();
    if (vaf >= 0.0) {
      int vaf_bin_index = vaf * NUM_VAF_BINS;
      vaf_qualities_[vaf_bin_index].AddOccurrence(is_correct);
    }

    // then: score coverage
    int total_support =
        genotyping_results[index].GetSupport().GetTotalSupport();
    static const int COVERAGE_RESOLUTION = 5;
    static const int MAX_SUPPORT = 200;
    if (total_support > MAX_SUPPORT) {
      total_support = MAX_SUPPORT;
    }
    int support_bin_index = total_support / COVERAGE_RESOLUTION;
    coverage_qualities_[support_bin_index].AddOccurrence(is_correct);


    // finally: score event
    EventType eventtype = event.GetType();
    if (!Utilities::MapContainsKey(eventtype_qualities_, eventtype)) {
      Quality new_quality;
      eventtype_qualities_[eventtype] = new_quality;
    }
    eventtype_qualities_[eventtype].AddOccurrence(is_correct);
  }
}

/** Scores a vector of genotypes as incorrect **/


/** classifies/counts the trio as having unknown, correct or incorrect Mendelian
 * inheritance. **/
bool Analyzer::ClassifyTrio(const std::vector<GenotypingResults>& genotyping_results,
     const Event& event, const std::string& line, bool* is_genotypable) {
  const Genotype& firstParentGenotype = genotyping_results[0].GetGenotype();
  const Genotype& secondParentGenotype = genotyping_results[1].GetGenotype();
  const Genotype& childGenotype = genotyping_results[2].GetGenotype();
  //std::cout << firstParentGenotype << " " << secondParentGenotype << " " << childGenotype << "\n";
  //Utilities::Pause();
  *is_genotypable = BioUtils::canAssessMendelianCorrectness(firstParentGenotype, secondParentGenotype,
      childGenotype);
  //std::cout << *is_genotypable << "]IG\n";
  //Utilities::Pause();
  if (*is_genotypable) {
    if (firstParentGenotype.IsHomRef() && secondParentGenotype.IsHomRef() &&
        childGenotype.IsHomRef()) {
      all_homref_++;
    }
    else if (BioUtils::canBeMendelianCorrect(firstParentGenotype, secondParentGenotype, childGenotype)) {
      m_mendelianCorrect++;
      ScoreAsCorrect(genotyping_results, event, true, line);
      return true;
    } else {
      m_mendelianError++;
      ScoreAsCorrect(genotyping_results, event, false, line);
      UpdateMendelianErrorMap(firstParentGenotype, secondParentGenotype, childGenotype);
      //SystematicDisplay(line);
      //pause();

        //        std::cout << line << std::endl;
        //        char ch;
        //        std::cin >> ch;
      return true;
    }
  } else { // if cannot assess Mendelian correctness
    unknown_count_++;
    //std::cout << "UK";
    //Utilities::Pause();
  }
  return false;
}

/**  analyze the samples associated with a particular structural variant.
 * Returns true if any of the samples has been called with sufficient confidence
 * as containing an alt-allele. **/
bool Analyzer::AnalyzeSamples(std::stringstream& ss, const Event& event, const std::string& line,
    bool* all_genotyped, std::stringstream& buffer_ss) {

  double MIN_CONFIDENCE = settings_->GetMinConfidence();

  std::vector<GenotypingResults> genotyping_results_of_trio;
  bool success = false;

  int number_of_genotyped_trios = 0;
  while (true) {
     std::string genotypeWithReadDepths;
     ss >> genotypeWithReadDepths;
     if (ss.fail()) {
       break;
     }
//std::cout <<"X1";
     GenotypingResults genotyping_results(genotypeWithReadDepths, settings_->isPindel());
     double confidence = call_confidence_estimator_.GetConfidence(event, genotyping_results.GetSupport());
     //std::cout << line << "[]" << confidence << "\n";
     //Utilities::Pause();
     if (confidence < MIN_CONFIDENCE || genotyping_results.GetGenotype().IsUnknown()) {
       genotyping_results.GetGenotype().SetToUnknown();
       unknown_calls_++;
     } else {
       //std::cout << "GT:" << genotyping_results.GetGenotype() << "\n";
       //Utilities::Pause();

       if (genotyping_results.GetGenotype().IsHomRef()) {
         homref_calls_++;
       }
       else {
         variant_calls_++;
         success = true;
       }
     }

     buffer_ss << "\t" << genotyping_results;
     genotyping_results_of_trio.push_back(genotyping_results);
     bool this_genotyped = false;
     if (genotyping_results_of_trio.size() == 3) {
       ClassifyTrio(genotyping_results_of_trio, event, line, &this_genotyped);
       *all_genotyped = *all_genotyped && this_genotyped;
       genotyping_results_of_trio.clear();
     }
   }
   if (genotyping_results_of_trio.size() != 0 ) {
     std::cout << "This does not seem to be trio data!\n";
     exit( -1 );
   }
   //std::cout <<"X2";
   ++trio_count_[number_of_genotyped_trios];
   return success;
}

/** Shows the counts of different Mendelian errors. **/
void Analyzer::ShowMendelianErrorMap() const {
  for (std::map<std::string, int>::const_iterator it = me_map_.begin();
      it != me_map_.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
  }
}

/** Output the bins: what is the qualuity per bin? (currently, bins defined
 * to contain VAF)
 */
void Analyzer::OutputBins() const {
  std::map<EventType, Quality> eventtype_qualities;

  std::cout << "VAF qualities\n";
  std::ofstream vaf_file("vaf_qualities_1.txt");
  vaf_file << NUM_VAF_BINS << "\n";
  for (int index = 0; index <= NUM_VAF_BINS; index++) {
      std::cout << index / static_cast<double>(NUM_VAF_BINS) << "-" <<
          (index + 1) / static_cast<double>(NUM_VAF_BINS) << ": " <<
          vaf_qualities_[index] << "\n";
      vaf_file << index << "\t" <<
          vaf_qualities_[index].GetCorrectnessFraction() << "\n";
  }
  vaf_file.close();
  std::cout << "\n";

  std::cout << "Coverage qualities\n";
  std::ofstream coverage_file("coverage_qualities_1.txt");
  coverage_file << MAX_SUPPORT << "\t" <<  COVERAGE_RESOLUTION << "\n";
  for (int index = 0; index <= MAX_SUPPORT / COVERAGE_RESOLUTION; index++) {
    std::cout << index * COVERAGE_RESOLUTION << "-" <<
      (index + 1) *  COVERAGE_RESOLUTION << ": " <<
      coverage_qualities_[index] << "\n";
    coverage_file << index << "\t" <<
        coverage_qualities_[index].GetCorrectnessFraction() << "\n";
  }
  coverage_file.close();
  std::cout << "\n";

  std::ofstream event_file("event_qualities_1.txt");
  std::cout << "Event-type qualities";
  std::cout << "SNP: " << eventtype_qualities_.find(kSnp)->second << "\n";
  std::cout << "INS: " << eventtype_qualities_.find(kIns)->second  << "\n";
  std::cout << "DEL: " << eventtype_qualities_.find(kDel)->second  << "\n";
  std::cout << "RPL: " << eventtype_qualities_.find(kRpl)->second  << "\n";
  std::cout << "UNKNOWN: " << eventtype_qualities_.find(kUnknown)->second << "\n";

  event_file << "SNP: " << eventtype_qualities_.find(kSnp)->second.GetCorrectnessFraction() << "\n";
  event_file << "INS: " << eventtype_qualities_.find(kIns)->second.GetCorrectnessFraction()  << "\n";
  event_file << "DEL: " << eventtype_qualities_.find(kDel)->second.GetCorrectnessFraction()  << "\n";
  event_file << "RPL: " << eventtype_qualities_.find(kRpl)->second.GetCorrectnessFraction()  << "\n";
  event_file << "UNKNOWN: " << eventtype_qualities_.find(kUnknown)->second.GetCorrectnessFraction() << "\n";
  event_file.close();
}

/** show the counting results to the user. **/
void Analyzer::ShowResults() const {
  std::cout << std::endl;
  std::cout << "Number of events: " << event_count_ << std::endl;
  std::cout << "Number of events with 0, 1, 2 and 3 trios genotyped: " <<
      trio_count_[0] << " " << trio_count_[1] << " " <<
      trio_count_[2] << " " << trio_count_[3] << std::endl;
  std::cout << "Unknown calls: " << unknown_calls_ << "\n";
  std::cout << "Homref calls: " << homref_calls_ << "\n";
  std::cout << "Variant calls: " << variant_calls_ << "\n";
  std::cout << "Number of events with all samples genotyped: " << all_genotyped_ << std::endl;
  std::cout << "Unknown: " << unknown_count_ << std::endl;
  std::cout << "Pure homref trios: " << all_homref_ <<std::endl;
  std::cout << "Mendelian correct: " << m_mendelianCorrect << std::endl;
  std::cout << "Mendelian error: " << m_mendelianError << std::endl;
  std::cout << "Mendelian error rate: "
      << 100.0 * m_mendelianError / (m_mendelianError + m_mendelianCorrect) << "%\n";
  std::cout << "Mendelian error rate including all-homref trios: "
        << 100.0 * m_mendelianError / (m_mendelianError + m_mendelianCorrect + all_homref_) << "%\n";
  ShowMendelianErrorMap();
  OutputBins();
}



/** Reads the input file, analyzes the trios found therein for Mendelian
 * violations. **/
void Analyzer::analyze() {

  // for every line do
  while (!m_inputFile.eof()) {
    std::string line;
    std::stringstream buffer_ss;
    getline(m_inputFile, line );
    //std::cout << line << std::endl;

    const char START_OF_COMMENT_CHAR = '#';
    // skip lines beginning with '#'
    if (line[0] == START_OF_COMMENT_CHAR ) {
      output_file_ << line << "\n";
      continue;
    }

    std::stringstream ss, ss2;
    ss << line;
    // ss2 is for getting general event info to the output file;
    ss2 << line;
    for (int i = 0; i < 9; i++) {
      std::string item;
      ss2 >> item;
      if (i != 0 ) {
        buffer_ss << "\t";
      }
      buffer_ss << item;
    }
    //std::cout << buffer_ss.str() << "]B";
    //Utilities::Pause();
    std::string refString, altString;
    BioUtils::GetDataAboutVariant(ss, refString, altString);
    Event event(refString, altString);
    bool should_analyze_variant = settings_->ShouldAnalyzeVariant(event);
    if (should_analyze_variant) {
      bool all_genotyped;
      bool at_least_one_sample_analyzable = AnalyzeSamples(ss, event, line, &all_genotyped, buffer_ss);
      if (all_genotyped) {
        ++all_genotyped_;
      }
      if (at_least_one_sample_analyzable) {
        ++event_count_;
        output_file_ << buffer_ss.str() << "\n";
      }
    }
  } // loop over lines
  ShowResults();
}


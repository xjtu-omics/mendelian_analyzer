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
#include "Genotype.h"
#include "Utilities.h"

/** Analyzer constructor. **/
Analyzer::Analyzer( const std::string& nameOfInputFile,
    const GlobalSettings* settings): settings_(settings) {
  m_inputFile.open( nameOfInputFile.c_str() );
  if (!m_inputFile) { // input file does not exist
    std::cout << "Input file does not exist.\n";
        exit(-1);
  }
  m_unknown = 0;
  m_mendelianCorrect = 0;
  m_mendelianError = 0;
  all_homref_ = 0;
  event_count_ = 0;
  all_genotyped_  = 0;
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

/** classifies/counts the trio as having unknown, correct or incorrect Mendelian
 * inheritance. **/
bool Analyzer::ClassifyTrio(const std::vector<Genotype>& genotypes,
     const std::string& line, bool* is_genotypable) {
  Genotype firstParentGenotype = genotypes[0];
  Genotype secondParentGenotype = genotypes[1];
  Genotype childGenotype = genotypes[2];
  *is_genotypable = BioUtils::canAssessMendelianCorrectness(firstParentGenotype, secondParentGenotype,
      childGenotype);
  if (*is_genotypable) {
    if (firstParentGenotype.isHomRef() && secondParentGenotype.isHomRef() &&
        childGenotype.isHomRef()) {
      all_homref_++;
    }
    else if (BioUtils::canBeMendelianCorrect(firstParentGenotype, secondParentGenotype, childGenotype)) {
      m_mendelianCorrect++;
      return true;
    } else {
      m_mendelianError++;
      UpdateMendelianErrorMap(firstParentGenotype, secondParentGenotype, childGenotype);
      //SystematicDisplay(line);
      //pause();

        //        std::cout << line << std::endl;
        //        char ch;
        //        std::cin >> ch;
      return true;
    }
  } else { // if cannot assess Mendelian correctness
    m_unknown++;
  }
  return false;
}

/**  analyze the samples associated with a particular structural variant.
 * Returns true if any of the samples has been analyzed successfully in a
 * Mendelian manner*/
bool Analyzer::AnalyzeSamples(std::stringstream& ss, const std::string& line,
    bool* all_genotyped) {
  std::vector<Genotype> genotypes;
  bool success;

  int number_of_genotyped_trios = 0;
  while (true) {
     std::string genotypeWithReadDepths;
     ss >> genotypeWithReadDepths;
     if (ss.fail()) {
       break;
     }

     Genotype genotype(genotypeWithReadDepths, settings_->isPindel());
     genotypes.push_back(genotype);
     bool this_genotyped = false;
     if (genotypes.size() == 3) {
       if (ClassifyTrio(genotypes, line, &this_genotyped)) {
         success = true;

       }
       *all_genotyped = *all_genotyped && this_genotyped;
       genotypes.clear();
     }
   }
   if ( genotypes.size() != 0 ) {
     std::cout << "This does not seem to be trio data!\n";
     exit( -1 );
   }
   ++trio_count_[number_of_genotyped_trios];
   return success;
}

/** Shows the counts of different Mendelian errors. **/
void Analyzer::ShowMendelianErrorMap() const {
  // first get total count of errors.
  int error_total = 0;
  for (std::map<std::string, int>::const_iterator it = me_map_.begin();
        it != me_map_.end(); ++it) {
    error_total += it->second;
  }
  for (std::map<std::string, int>::const_iterator it = me_map_.begin();
      it != me_map_.end(); ++it) {
    std::cout << it->first << ": " << it->second << " ("
        << 100 * it->second / static_cast<double>(error_total) << "%)" << std::endl;
  }
}

/** Output the bins: what is the qualuity per bin? (currently, bins defined
 * to contain VAF)
 */
void Analyzer::OutputBins() const {

}

/** show the counting results to the user. **/
void Analyzer::ShowResults() const {
  std::cout << std::endl;
  std::cout << "Number of events: " << event_count_ << std::endl;
  std::cout << "Number of events with 0, 1, 2 and 3 trios genotyped: " <<
      trio_count_[0] << " " << trio_count_[1] << " " <<
      trio_count_[2] << " " << trio_count_[3] << std::endl;
  std::cout << "Number of events with all samples genotyped: " << all_genotyped_ << std::endl;
  std::cout << "Unknown: " << m_unknown << std::endl;
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
    getline(m_inputFile, line );
    //std::cout << line << std::endl;

    const char START_OF_COMMENT_CHAR = '#';
    // skip lines beginning with '#'
    if (line[0] == START_OF_COMMENT_CHAR ) {
      continue;
    }

    std::stringstream ss;
    ss << line;
    std::string refString, altString;
    BioUtils::GetDataAboutVariant(ss,refString, altString);
    Event event(refString, altString);
    bool should_analyze_variant = settings_->ShouldAnalyzeVariant(event);
    if (should_analyze_variant) {
      bool all_genotyped;
      bool at_least_one_sample_analyzable = AnalyzeSamples(ss, line, &all_genotyped);
      if (all_genotyped) {
        ++all_genotyped_;
      }
      if (at_least_one_sample_analyzable) {
        ++event_count_;
      }
    }
  } // loop over lines
  ShowResults();
}


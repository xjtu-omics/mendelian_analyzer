#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "Analyzer.h"
#include "Event.h"
#include "Genotype.h"
#include "GlobalSettings.h"
#include "Support.h"
#include "Utilities.h"




GlobalSettings g_global_settings;

enum VcfType {UNKNOWN, PINDEL, GATK};




/* pauses (waits for user to press a key. **/
void pause() {
  char ch;
  std::cin.get(ch);
}


/** Displays the 'role' of a sample (assuming trio data) based on its position
    in the sequence. **/
std::string Role(int count) {
  int cyclicCount = count % 3;
  switch (cyclicCount) {
  case 0: return "first parent";
  case 1: return "second parent";
  case 2: return "child";
  default: Utilities::Require(false,"Role:error. Wrong value provided.");
  }
  return "error";
}

/** 'SystematicDisplay' displays/parses the line as it would be genotyped. **/
void SystematicDisplay(const std::string& line) {
  std::stringstream ss;
  ss << line;
  std::cout << "Variant data:";
  for (int i = 0; i < 9; i++ ) {
    std::string str;
    ss >> str;
    std::cout << " " << str;
  }
  std::cout << "\n";

  int sample_count = 0;
  do {
    std::string str;
    ss >> str;
    if (ss.fail()) {
      break;
    }
    Genotype genotype(str, false);
    Support support(str);
    std::cout << str << " (" << Role(sample_count) << "):"<< genotype << ", VAF: " << support.GetVaf() << "\n";
    ++sample_count;
    if (sample_count == 3) {
      std::cout << "\n";
      sample_count = 0;
    }

  } while (true);
}



/** Transforms a Vcf-type indicating string into the correct VcfType enum **/
VcfType ParseVcfType(const std::string& vcf_type_as_string ) {
  if (vcf_type_as_string == "p") {
    return PINDEL;
  }
  else if (vcf_type_as_string == "g") {
    return GATK;
  }
  else {
    Utilities::Require(false, "ParseVcfType error: input string is unknown.");
  }
  return UNKNOWN;
}

int main(int argc, char** argv) {

  std::cout << "Analyzing the genotypes from the given trio-based VCF\n";
  if (argc < 2) {
    std::cout << "Invalid number of arguments. At least one argument " <<
        "is needed, the name of the input file. Other possible arguments " <<
        "are 'pindel' (to correctly process pindel VCFs with -1,X and 0,0 " <<
        "coverage), 'all', 'snp', 'sv', 'del', 'ins' to indicate the " <<
        "type, and 'sizegtX' (for example 'sizegt50') or sizeltY to " <<
        "indicate the size of the event (note: rpls are not filtered " <<
        "on size yet).\n";

    return -1;
  } else {
    std::string nameOfInputFile = argv[1];
    std::vector<std::string> arguments;

    for (int argument_index = 2; argument_index < argc; ++argument_index) {
      g_global_settings.Add(argv[argument_index]);
    }
    g_global_settings.Complete();

    std::cout << "input " << nameOfInputFile;
    Analyzer analyzer(nameOfInputFile, &g_global_settings);
    analyzer.analyze();
  }

  return 0;
}

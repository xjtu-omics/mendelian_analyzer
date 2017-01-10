/*
 * GlobalSettings.h
 *
 * Creates an object that contains the global settings, for example which
 * variants should be analyzed.
 *
 *  Created on: Dec 20, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <map>
#include <string>

#include "Event.h"

enum VariantType {UNDEFINED, ALL, SNP, SV, DEL, INS, RPL};

class Settings {
public:
  Settings();
  virtual ~Settings();

  void Add(const char* argument);
  void Complete();
  bool isPindel() const;
  int GetMaxEventSize() const;
  double GetMinConfidence() const;

  int GetMinEventSize() const;
  VariantType GetVariantType() const;
  bool ShouldAnalyzeVariant(const Event& event) const;
private:

  bool isTypeCorrect(int ref_size, int alt_size) const;
  bool isSizeCorrect(int size) const;


  VariantType variant_type_;
  bool is_pindel_;
  double min_confidence_;
  int min_size_;
  int max_size_;
  std::map<std::string, VariantType> string_to_variant_type_map_;
};

#endif /* SETTINGS_H_ */

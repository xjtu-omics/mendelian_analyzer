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

#ifndef GLOBALSETTINGS_H_
#define GLOBALSETTINGS_H_

#include <map>
#include <string>

#include "Event.h"

enum VariantType {UNDEFINED, ALL, SNP, SV, DEL, INS, RPL};

class GlobalSettings {
public:
  GlobalSettings();
  virtual ~GlobalSettings();

  void Add(const char* argument);
  void Complete();
  bool isPindel() const;
  int GetMaxEventSize() const;
  int GetMinEventSize() const;
  VariantType GetVariantType() const;
  bool ShouldAnalyzeVariant(const Event& event) const;
private:
  VariantType variant_type_;
  bool is_pindel_;
  int min_size_;
  int max_size_;
  std::map<std::string, VariantType> string_to_variant_type_map_;
};

#endif /* GLOBALSETTINGS_H_ */

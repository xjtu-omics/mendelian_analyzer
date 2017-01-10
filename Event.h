/*
 * Event.h
 *
 * Contains data about an event (a putative difference in genotype with the
 * reference genome) in one or more of the samples described by this VCF file.
 *
 *  Created on: Dec 19, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <string>
#include <vector>

enum EventType { kUnknown, kSnp, kIns, kDel, kRpl };

EventType GetEventTypeFromString(const std::string& input);

class Event {
public:
  Event(const std::string& reference, const std::string& alt);
  virtual ~Event();

  bool HasSingleAltAllele() const;
  int RefSize() const;
  int AltSize() const;
  EventType GetType() const;

private:
  std::string reference_;
  std::vector<std::string> alternatives_;
};

#endif /* EVENT_H_ */

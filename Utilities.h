/*
 * Utilities.h
 *
 * Contains generic functions that would have been great if C++ had them, but
 * as C++ does not (seem to) possess them, they are defined here.
 *
 *  Created on: Dec 19, 2016
 *      Author: Eric-Wubbo Lameijer, Xi'an Jiaotong University,
 *              eric_wubbo@hotmail.com
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace Utilities {

std::string GetUntilFirstOccurrenceOf(const std::string& str, char ch);
template<class T> bool isBetween(T value, T min_value, T max_value);
bool isPositiveInteger(const std::string& str);
template <class K,class V> bool MapContainsKey(std::map<K,V> the_map, K key);
void Pause();
void Require(bool requirementMet, std::string errorMessage);
std::vector<std::string> Split(const std::string& str, char separator);
bool StringStartsWith(const std::string& stringToBeAssessed,
    const std::string& putativeStart);
std::string StringToLowerCase(const std::string& input);


template <class K,class V> bool MapContainsKey(std::map<K,V> the_map,K key) {
  //std::cout << "Key: " << key << "\n";
  return the_map.find(key) != the_map.end();
}

/** Is this value larger than min_value and smaller than max_value? **/
template<class T> bool isInclusiveBetween(T value, T min_value, T max_value) {
  return ((value >= min_value) && (value <= max_value));
}


}; // namespace Utilities

#endif /* UTILITIES_H_ */

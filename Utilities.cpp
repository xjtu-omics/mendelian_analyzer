/*
 * Utilities.cpp
 *
 *  Created on: Dec 19, 2016
 *      Author: admin123
 */

#include "Utilities.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace Utilities {

/** Gets the part of string 'str' that occurs before character 'ch'. If there
 * is no character 'ch' in the string, returns the entire string. **/
std::string GetUntilFirstOccurrenceOf(const std::string& str,
    char ch) {
  std::string outputString = str;
  size_t charPos = str.find_first_of(ch);
  if (charPos != std::string::npos) {
    // character found
    outputString = str.substr(0, charPos);
  }
  return outputString;
}

/** Utility function that halts/crashes the program, helps to catch bugs early.
**/
void Require(bool requirementMet, std::string errorMessage) {
  if (!requirementMet) {
    std::cerr << errorMessage << std::endl;
    exit(-1);
  }
}

/** Splits a string pre character 'ch' into a vector of strings. So 4/5/3 split
 * on '/' would yield ["4","5","3"]. **/
std::vector<std::string> Split(const std::string& str, char separator) {

  std::vector<std::string> output;
  size_t startSearchPos = 0;
  while (true) {
    size_t separatorPos = str.find_first_of(separator, startSearchPos);
    output.push_back(str.substr(startSearchPos, separatorPos));
    if (separatorPos == std::string::npos) {
      break; // we're done
    }
    else {
      startSearchPos = separatorPos + 1;
    }
  };
  return output;
}

/** Does this string represent a positive integer? atoi may not work properly,
    as it turns 3.14 into 3. **/
bool isPositiveInteger(const std::string& str) {

  if (str == "") {
    return false;
  }
  if (str == "0") {
    return true;
  }
  char firstCharacter = str[0];
  if (firstCharacter < '1' || firstCharacter > '9') {
    return false; // also eliminates 0123 or 0.342-strings
  }

  for (size_t string_position = 1; string_position < str.size(); ++string_position) {
    if (!isdigit(str[string_position])) {
      return false;
    }
  }
  return true;
}

/* Returns whether a string starts with a certain other string, so if
   'stringToBeAssessed' is 'albert' and 'putativeStart' is 'al', this function
   returns true. */
bool StringStartsWith(const std::string& stringToBeAssessed,
    const std::string& putativeStart) {
  int subStringLength = putativeStart.length();
  std::string startOfAssessableString =
      stringToBeAssessed.substr(0,subStringLength);
  return (startOfAssessableString.compare(putativeStart) == 0 );
}



} //namespace Utilities

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_TEXT_NESTEDSTRINGTOKENIZER_H
#define BPP_TEXT_NESTEDSTRINGTOKENIZER_H


// From the STL:
#include <deque>
#include <string>

#include "StringTokenizer.h"
#include "../Exceptions.h"

namespace bpp
{
/**
 * @brief An improved tokenizer for strings.
 *
 * Splits a string according to a given (set of) delimiter(s).
 * Delimiters in certains blocks ({}, [], etc) are ignored.
 */
class NestedStringTokenizer :
  public StringTokenizer
{
public:
  /**
   * @brief Build a new StringTokenizer from a string.
   *
   * @param s          The string to parse.
   * @param open       Opening block.
   * @param end        Ending block.
   * @param delimiters Chars that must be considered as delimiters.
   * @param solid      If true, delimiters is considered as a single bloc delimiter.
   */
  NestedStringTokenizer(const std::string& s, const std::string& open, const std::string& end, const std::string& delimiters = " \t\n\f\r", bool solid = false);

  virtual ~NestedStringTokenizer() {}

public:
  /**
   * @brief Get the next available token.
   * If no token is availbale, throw an Exception.
   *
   * @return The next token if there is one.
   */
  const std::string& nextToken();


  /**
   * @brief This function is not supported for nested tokenizers.
   *
   * @return An empty string.
   */
  std::string unparseRemainingTokens() const { return ""; }
};
} // end of namespace bpp;
#endif // BPP_TEXT_NESTEDSTRINGTOKENIZER_H

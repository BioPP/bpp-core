// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_TEXT_STRINGTOKENIZER_H
#define BPP_TEXT_STRINGTOKENIZER_H

#include <deque>
#include <iostream>
#include <string>

#include "../Exceptions.h"

namespace bpp
{
/**
 * @brief A tokenizer for strings.
 *
 * Splits a string according to a given (set of) delimiter(s).
 */
class StringTokenizer
{
protected:
  /** @brief Where the tokens are stored. */
  std::deque<std::string> tokens_;
  std::deque<std::string> splits_;

  /** @brief the current position in the token list. */
  size_t currentPosition_;

public:
  /**
   * @brief Build a new StringTokenizer from a string.
   *
   * @param s                The string to parse.
   * @param delimiters       Chars that must be considered as delimiters.
   * @param solid            If true, delimiters is considered as a single bloc delimiter.
   * @param allowEmptyTokens Tell if empty tokens are allowed or should be ignored.
   */
  StringTokenizer(const std::string& s, const std::string& delimiters = " \t\n\f\r", bool solid = false, bool allowEmptyTokens = false);

  virtual ~StringTokenizer() {}

public:
  StringTokenizer() : tokens_(), splits_(), currentPosition_(0) {}

public:
  /**
   * @brief Get the next available token.
   * If no token is availbale, throw an Exception.
   *
   * @return The next token if there is one.
   */
  const std::string& nextToken()
  {
    if (!hasMoreToken()) throw Exception("No more token in tokenizer.");
    return tokens_[currentPosition_++];
  }

  /**
   * @brief Tell if some tokens are still available.
   * @return True if some tokens are still available.
   */
  bool hasMoreToken() const
  {
    return currentPosition_ < tokens_.size();
  }

  /**
   * @brief Tell how many tokens are available.
   *
   * @return the number of tokens available.
   */
  size_t numberOfRemainingTokens() const { return tokens_.size() - currentPosition_; }

  /**
   * @brief Get a particular token.
   *
   * Do not move the iterator.
   *
   * @param pos The index of the token.
   * @return the token at position 'pos'.
   */
  const std::string& getToken(size_t pos) const { return tokens_[pos]; }

  /**
   * @brief Retrieve all tokens.
   *
   * @return A reference toward the vector of tokens.
   */
  const std::deque<std::string>& getTokens() const { return tokens_; }

  /**
   * @brief remove all empty token from the current position.
   */
  void removeEmptyTokens();

  /**
   * @return The remaining tokens as if the original corresponding string was not parsed.
   */
  std::string unparseRemainingTokens() const;
};
} // end of namespace bpp.
#endif // BPP_TEXT_STRINGTOKENIZER_H

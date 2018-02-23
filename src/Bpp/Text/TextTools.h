//
// File: TextTools.h
// Authors:
//   Julien Dutheil
//   Francois Gindraud (2017)
// Created: 2003-08-08 12:57:50
// Last modified: 2017-06-26
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide basal and
  utilitary classes. This file belongs to the Bio++ Project.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_TEXT_TEXTTOOLS_H
#define BPP_TEXT_TEXTTOOLS_H

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace bpp
{

  /// Some utilitary functions that work on strings.
  namespace TextTools
  {
    /** @brief Tell if a string is empty.
     * A string is considered to be 'empty' if it is only made of white
     * spaces.
     * @param s The string to check.
     * @return True if the string has only white characters.
     */
    bool isEmpty(const std::string& s);

    /** @brief Make the string uppercase.
     * @param s The string to analyse.
     * @return A copy of the string with all chars uppercase.
     */
    std::string toUpper(const std::string& s);

    /** @brief Make the string lowercase.
     * @param s The string to analyse.
     * @return A copy of the string with all chars lowercase.
     */
    std::string toLower(const std::string& s);

    /** @brief Tell if a character is a white space or not.
     * @param c The character to check.
     * @return True if c is one of the following: ' ', '\\t', '\\n', '\\r', '\\f', '\\v'.
     */
    bool isWhiteSpaceCharacter(char c);

    /** @brief Remove all white spaces characters in a string.
     * @param s The string to parse.
     * @return A copy of 's' without white spaces characters.
     */
    std::string removeWhiteSpaces(const std::string& s);

    /** @brief Remove all white spaces characters at the beginning of a string.
     * @param s The string to parse.
     * @return A copy of 's' beginning with the first non-white character.
     */
    std::string removeFirstWhiteSpaces(const std::string& s);

    /** @brief Remove all white spaces characters at the end of a string.
     * @param s The string to parse.
     * @return A copy of 's' ending with the last non-white character.
     */
    std::string removeLastWhiteSpaces(const std::string& s);

    /** @brief Remove all white spaces characters at the beginning and the end of a string.
     * @param s The string to parse.
     * @return A copy of 's' beginning with the first non-white character and ending with the last one.
     */
    std::string removeSurroundingWhiteSpaces(const std::string& s);

    /** @brief Tell if a character is a new line character or not.
     * @param c The character to check.
     * @return True if c is one of the following: '\\n' or '\\r'.
     */
    bool isNewLineCharacter(char c);

    /** @brief Remove all new line characters in a string.
     * @param s The string to parse.
     * @return A copy of 's' without new line characters.
     */
    std::string removeNewLines(const std::string& s);

    /** @brief Remove all new line characters at the end of a string.
     * @param s The string to parse.
     * @return A copy of 's' ending with the last non-new line character.
     */
    std::string removeLastNewLines(const std::string& s);

    /** @brief Tell is a given character describes a decimal number.
     * @param c The character to check.
     * @return true if the given character is the reprensentation of a decimal number.
     */
    bool isDecimalNumber(char c);

    /** @brief Tell is a given character string describes a decimal number.
     * FIXME : for now, this parser will not recognize thousands delimiters, and not the scientific notation neither.
     * @param s The string to parse.
     * @param dec The decimal separator.
     * @param scientificNotation character to use for scientific notation (typically 'e' or 'E').
     * @return true if the given string is the representation of a decimal number.
     */
    bool isDecimalNumber(const std::string& s, char dec = '.', char scientificNotation = 'e');

    /** @brief Tell is a given character string describes a decimal integer.
     * FIXME: for now, this parser will not recognize thousands delimiters, and not the scientific notation neither.
     * @param s The string to parse.
     * @param scientificNotation character to use for scientific notation (typically 'e' or 'E').
     * @return true if the given string is the representation of a decimal integer.
     */
    bool isDecimalInteger(const std::string& s, char scientificNotation = 'e');

    /** @brief General template method to convert to a string.
     * @param t The object to convert.
     * @return A string equal to t.
     */
    template <class T>
    std::string toString(T t)
    {
      std::ostringstream oss;
      oss << t;
      return oss.str();
    }

    /** @brief Template string conversion.
     * @param t The object to convert.
     * @param precision To use (for numbers).
     * @return A string equal to t.
     */
    template <class T>
    std::string toString(T t, int precision)
    {
      std::ostringstream oss;
      oss << std::setprecision(precision) << t;
      return oss.str();
    }

    /** @brief General template method to convert from string.
     * @param s The string to convert.
     * @return An object from string t.
     */
    template <class T>
    T fromString(const std::string& s)
    {
      std::istringstream iss(s);
      T obj;
      iss >> obj;
      return obj;
    }

    /** @brief Convert from string to int.
     * @param s The string to parse.
     * @param scientificNotation character to use for scientific notation (typically 'e' or 'E').
     * @return The integer corresponding to s.
     * @throw Exception if the string does not specify a valid number.
     */
    int toInt(const std::string& s, char scientificNotation = 'e');

    /** @brief Convert from string to double.
     * @param s The string to parse.
     * @param dec The decimal separator.
     * @param scientificNotation character to use for scientific notation (typically 'e' or 'E').
     * @return The double corresponding to s.
     * @throw Exception if the string does not specify a valid number.
     */
    double toDouble(const std::string& s, char dec = '.', char scientificNotation = 'e');

    /** @brief Template to string conversion.
     * @param s The string to parse.
     * @return An object of class R corresponding to s.
     */
    template <class T>
    T to(const std::string& s)
    {
      std::istringstream iss(s);
      T t;
      iss >> t;
      return t;
    }

    /** @brief Send a string of size 'newSize', which is a copy of 's' truncated or
     * filled with character 'fill' at the end.
     * @param s       The string to parse.
     * @param newSize The new string size.
     * @param fill    The character to use to fill the string id length < newSize.
     * @return A string of size newsize which is a copy from the left of s.
     */

    std::string resizeRight(const std::string& s, size_t newSize, char fill = ' ');

    /** @brief Send a string of size 'newSize', which is a copy of 's' truncated or
     * filled with character 'fill' at the beginning.
     * @param s       The string to parse.
     * @param newSize The new string size.
     * @param fill    The character to use to fill the string id length < newSize.
     * @return A string of size newsize which is a copy from the right of s.
     */
    std::string resizeLeft(const std::string& s, size_t newSize, char fill = ' ');

    /** @brief Split a string into parts of size 'n'.
     * The last part may contain < n chars.
     * @param s The string to parse.
     * @param n The number of tokens.
     * @return A vector of strings with all tokens.
     */
    std::vector<std::string> split(const std::string& s, size_t n);

    /** @brief Remove substrings from a string.
     * All substrings beginning with blockBeginning
     * and ending with blockEnding will be removed.
     * Nesting blocks are allowed, the most extern block will be removed.
     * @param s The string to parse.
     * @param blockBeginning The character specifying the beginning of each block.
     * @param blockEnding    The character specifying the end of each block.
     * @return The string with all blocks removed.
     * @throw Exception If some blocks are not well formed.
     */
    std::string removeSubstrings(const std::string& s, char blockBeginning, char blockEnding);

    /** @brief Remove substrings from a string, unless they match some specific substrings.
     * All substrings beginning with blockBeginning
     * and ending with blockEnding will be removed, except if they begin with 
     * a string included in the vector exceptionsBeginning or end with a string
     * included in the vector exceptionsEnding.
     * Nesting blocks are allowed, the most extern block will be removed.
     * @param s The string to parse.
     * @param blockBeginning The character specifying the beginning of each block.
     * @param blockEnding    The character specifying the end of each block.
     * @param exceptionsBeginning A vector containing all strings specifying the beginning of blocks that should not be removed. 
     * @param exceptionsEnding A vector containing all strings specifying the ending of blocks that should not be removed.
     * @return The string with all blocks removed.
     * @throw Exception If some blocks are not well formed.
     */
    std::string removeSubstrings(const std::string& s, char blockBeginning, char blockEnding,
                                 std::vector<std::string>& exceptionsBeginning,
                                 std::vector<std::string>& exceptionsEnding);

    /**
    * @brief Remove all occurences of a character in a string.
    *
    * @param s The string to parse.
    * @param c The character to remove.
    * @return The string with all specified chars removed.
    */
    std::string removeChar(const std::string& s, char c);

    /**
     * @brief Count the occurences of a given pattern in a string.
     *
     * @param s The string to search.
     * @param pattern The pattern to use (this is a mere string, not a regexp!).
     * @return The number of occurences of 'pattern' in 's'.
     */
    std::size_t count(const std::string& s, const std::string& pattern);

    /**
     * @brief Tell is a string begins with a certain motif.
     *
     * @param s The string to search.
     * @param pattern The pattern to use (this is a mere string, not a regexp!).
     * @return true/false
     */
    bool startsWith(const std::string& s, const std::string& pattern);

    /**
     * @brief Tell is a string ends with a certain motif.
     *
     * @param s The string to search.
     * @param pattern The pattern to use (this is a mere string, not a regexp!).
     * @return true/false
     */
    bool endsWith(const std::string& s, const std::string& pattern);

    /**
     * @brief Tell is a string contains a certain motif.
     *
     * @param s The string to search.
     * @param pattern The pattern to use (this is a mere string, not a regexp!).
     * @return true/false
     */
    bool hasSubstring(const std::string& s, const std::string& pattern);

    /**
     * @brief Replacement of all non-overlapping occurrences of a certain motif in a string.
     *
     * @param target String to be modified
     * @param query The motif to look for
     * @param replacement The replacement string
     */
    void replaceAll(std::string& target, const std::string& query, const std::string& replacement);

  } // namespace TextTools
} // namespace bpp
#endif // BPP_TEXT_TEXTTOOLS_H

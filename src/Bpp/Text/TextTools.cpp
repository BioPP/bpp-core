//
// File: TextTools.cpp
// Authors:
//   Julien Dutheil
//   Francois Gindraud (2017)
// Created: 2003-08-08 12:57:50
// Last modified: 2017-06-27
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide utilitary
  classes. This file belongs to the Bio++ Project.
  
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

#include <algorithm>
#include <cctype>
#include <iterator>
#include <vector>

#include "../Exceptions.h"
#include "../Numeric/IntegerTools.h"
#include "TextTools.h"

namespace bpp
{
  namespace TextTools
  {
    /******************************************************************************/

    bool isEmpty(const std::string& s)
    {
      return std::all_of(s.begin(), s.end(), [](char c) { return std::isspace(c); });
    }

    /******************************************************************************/

    std::string toUpper(const std::string& s)
    {
      std::string result;
      result.reserve(s.size());
      std::transform(s.begin(), s.end(), std::back_inserter(result), [](char c) { return std::toupper(c); });
      return result;
    }

    /******************************************************************************/

    std::string toLower(const std::string& s)
    {
      std::string result;
      result.reserve(s.size());
      std::transform(s.begin(), s.end(), std::back_inserter(result), [](char c) { return std::tolower(c); });
      return result;
    }

    /******************************************************************************/

    bool isWhiteSpaceCharacter(char c) { return std::isspace(c); }

    /******************************************************************************/

    std::string removeWhiteSpaces(const std::string& s)
    {
      // Only copy non whitespace chars in new string
      std::string result;
      std::remove_copy_if(s.begin(), s.end(), std::back_inserter(result), [](char c) { return std::isspace(c); });
      return result;
    }

    /******************************************************************************/

    std::string removeFirstWhiteSpaces(const std::string& s)
    {
      // Copy s from first non whitespace to end.
      return std::string(std::find_if(s.begin(), s.end(), [](char c) { return !std::isspace(c); }), s.end());
    }

    /******************************************************************************/

    std::string removeLastWhiteSpaces(const std::string& s)
    {
      // Copy s from start to last non whitespace char
      auto lastNonWhitespace = std::find_if(s.rbegin(), s.rend(), [](char c) { return !std::isspace(c); });
      return std::string(s.begin(), lastNonWhitespace.base());
    }

    /******************************************************************************/

    std::string removeSurroundingWhiteSpaces(const std::string& s)
    {
      // Copy s from first non-whitespace to last non-whitespace
      auto isNotWhitespace = [](char c) { return !std::isspace(c); };
      auto firstNonWhitespace = std::find_if(s.begin(), s.end(), isNotWhitespace);
      auto lastNonWhitespace = std::find_if(
        s.rbegin(), std::reverse_iterator<std::string::const_iterator>(firstNonWhitespace), isNotWhitespace);
      return std::string(firstNonWhitespace, lastNonWhitespace.base());
    }

    /******************************************************************************/

    bool isNewLineCharacter(char c) { return (c == '\n') || (c == '\r'); }

    /******************************************************************************/

    std::string removeNewLines(const std::string& s)
    {
      // Only copy non newline chars in new string
      std::string result;
      std::remove_copy_if(s.begin(), s.end(), std::back_inserter(result), [](char c) { return isNewLineCharacter(c); });
      return result;
    }

    /******************************************************************************/

    std::string removeLastNewLines(const std::string& s)
    {
      // Copy s from start to last non newline char
      auto lastNonNewline = std::find_if(s.rbegin(), s.rend(), [](char c) { return !isNewLineCharacter(c); });
      return std::string(s.begin(), lastNonNewline.base());
    }

    /******************************************************************************/

    bool isDecimalNumber(char c) { return std::isdigit(c); }

    /******************************************************************************/

    bool isDecimalNumber(const std::string& s, char dec, char scientificNotation)
    {
      if (isEmpty(s))
        return false;

      std::size_t sepCount = 0;
      std::size_t sciCount = 0;
      std::size_t i = 0;
      if (s[0] == '-')
        i = 1;
      for (; i < s.size(); ++i)
      {
        char c = s[i];
        if (c == dec)
          sepCount++;
        else if (c == scientificNotation)
        {
          sciCount++;
          if (i == s.size() - 1)
            return false; // Must be sthg after scientific notation.
          c = s[i + 1];
          if (c == '-' || c == '+')
            i++;
          if (i == s.size() - 1)
            return false; // Must be sthg after scientific notation.
          if (sepCount == 0)
            sepCount = 1; // We do not want any dec in the exponent.
        }
        else if (!isDecimalNumber(c))
          return false;
        if (sepCount > 1 || sciCount > 1)
          return false;
      }
      return true;
    }

    /******************************************************************************/

    bool isDecimalInteger(const std::string& s, char scientificNotation)
    {
      if (isEmpty(s))
        return false;

      std::size_t sciCount = 0;
      std::size_t i = 0;
      if (s[0] == '-')
        i = 1;
      for (; i < s.size(); ++i)
      {
        char c = s[i];
        if (c == scientificNotation)
        {
          sciCount++;
          if (i == s.size() - 1)
            return false; // Must be sthg after scientific notation.
          c = s[i + 1];
          if (c == '-')
            return false; // Not an integer then!
          if (c == '+')
            i++;
          if (i == s.size() - 1)
            return false; // Must be sthg after scientific notation.
        }
        else if (!isDecimalNumber(c))
          return false;
        if (sciCount > 1)
          return false;
      }
      return true;
    }

    /******************************************************************************/

    int toInt(const std::string& s, char scientificNotation)
    {
      if (!isDecimalInteger(s, scientificNotation))
        throw Exception("TextTools::toInt(). Invalid number specification: " + s);
      return fromString<int>(s);
    }

    /******************************************************************************/

    double toDouble(const std::string& s, char dec, char scientificNotation)
    {
      if (!isDecimalNumber(s, dec, scientificNotation))
        throw Exception("TextTools::toDouble(). Invalid number specification: " + s);
      return fromString<double>(s);
    }

    /******************************************************************************/

    std::string resizeRight(const std::string& s, std::size_t newSize, char fill)
    {
      std::string result;
      result.reserve(newSize);
      if (newSize > s.size())
      {
        std::copy(s.begin(), s.end(), std::back_inserter(result));
        std::fill_n(std::back_inserter(result), newSize - s.size(), fill);
      }
      else
      {
        std::copy_n(s.begin(), newSize, std::back_inserter(result));
      }
      return result;
    }

    /******************************************************************************/

    std::string resizeLeft(const std::string& s, std::size_t newSize, char fill)
    {
      std::string result;
      result.reserve(newSize);
      if (newSize > s.size())
      {
        std::fill_n(std::back_inserter(result), newSize - s.size(), fill);
        std::copy(s.begin(), s.end(), std::back_inserter(result));
      }
      else
      {
        using diff_type = typename std::iterator_traits<decltype(s.begin())>::difference_type;
        std::copy(s.begin() + static_cast<diff_type>(s.size() - newSize), s.end(), std::back_inserter(result));
      }
      return result;
    }

    /******************************************************************************/

    std::vector<std::string> split(const std::string& s, std::size_t n)
    {
      using diff_type = typename std::iterator_traits<decltype(s.begin())>::difference_type;
      std::vector<std::string> v;
      auto nbChunks = IntegerTools::divideUp(s.size(), n);
      v.reserve(nbChunks);
      // Copy chunks by chunks, and add the last incomplete one if s.size () % n != 0
      auto nbCopiedChunks = IntegerTools::divideDown(s.size(), n);
      for (std::size_t i = 0; i < nbCopiedChunks; ++i)
        v.emplace_back(s.begin() + static_cast<diff_type>(i * n), s.begin() + static_cast<diff_type>((i + 1) * n));
      if (v.size() < nbChunks)
        v.emplace_back(s.begin() + static_cast<diff_type>(v.size() * n), s.end());
      return v;
    }

    /******************************************************************************/

    std::string removeSubstrings(const std::string& s, char blockBeginning, char blockEnding)
    {
      std::string result;
      std::size_t blockDepth = 0;
      for (std::size_t i = 0; i < s.size(); ++i)
      {
        auto c = s[i];
        if (c == blockBeginning)
        {
          blockDepth++;
        }
        else if (c == blockEnding)
        {
          if (blockDepth == 0)
            throw Exception(
              std::string("TextTools::removeSubstrings(): unmatched block closing character at position ") +
              std::to_string(i));
          blockDepth--;
        }
        else if (blockDepth == 0)
        {
          result += c;
        }
      }
      return result;
    }

    /******************************************************************************/

    std::string removeSubstrings(const std::string& s,
                                 char blockBeginning,
                                 char blockEnding,
                                 std::vector<std::string>& exceptionsBeginning,
                                 std::vector<std::string>& exceptionsEnding)
    {
      // TODO didn't upgrade... move to a parser like system ? it is very specific...
      std::string t;
      int blockCount = 0;
      std::size_t begPos = 0;
      for (std::size_t i = 0; i < s.size(); i++)
      {
        char current = s[i];
        if (current == blockBeginning)
        {
          bool except = false;
          for (std::size_t j = 0; j < exceptionsBeginning.size(); j++)
          {
            std::size_t pos = exceptionsBeginning[j].find(blockBeginning);
            if (pos != std::string::npos)
            {
              std::size_t left = i - pos;
              std::size_t right = i + exceptionsBeginning[j].length() - pos;
              if ((right < s.length() - 1) && (hasSubstring(s.substr(left, right), exceptionsBeginning[j])))
              {
                except = true;
                break;
              }
            }
          }
          if (!except)
          {
            blockCount++;
            t += s.substr(begPos, i - begPos);
          }
        }
        else if ((current == blockEnding) && (blockCount > 0))
        {
          for (std::size_t j = 0; j < exceptionsEnding.size(); j++)
          {
            std::size_t pos = exceptionsEnding[j].find(blockEnding);
            if (pos != std::string::npos)
            {
              std::size_t left = i - pos;
              std::size_t right = i + exceptionsEnding[j].length() - pos;
              if ((right < s.length() - 1) && (hasSubstring(s.substr(left, right), exceptionsEnding[j])))
              {
                break;
              }
            }
          }
          blockCount--;
          if (blockCount == 0)
          {
            begPos = i + 1;
          }
          else if (blockCount < 0)
            throw Exception("TextTools::removeSubstrings(). " +
                            std::string("Ending block character without corresponding beginning one at position ") +
                            toString((int)i) + ".");
        }
      }
      t += s.substr(begPos);
      return t;
    }

    /******************************************************************************/

    std::string removeChar(const std::string& s, char c)
    {
      std::string result;
      std::remove_copy(s.begin(), s.end(), std::back_inserter(result), c);
      return result;
    }

    /******************************************************************************/

    std::size_t count(const std::string& s, const std::string& pattern)
    {
      std::size_t count = 0;
      auto it = std::search(s.begin(), s.end(), pattern.begin(), pattern.end());
      while (it != s.end())
      {
        count++;
        it = std::search(it + 1, s.end(), pattern.begin(), pattern.end());
      }
      return count;
    }

    /******************************************************************************/

    bool startsWith(const std::string& s, const std::string& pattern)
    {
      if (s.size() < pattern.size())
        return false;
      return std::equal(pattern.begin(), pattern.end(), s.begin());
    }

    /******************************************************************************/

    bool endsWith(const std::string& s, const std::string& pattern)
    {
      if (s.size() < pattern.size())
        return false;
      return std::equal(pattern.rbegin(), pattern.rend(), s.rbegin());
    }

    /******************************************************************************/

    bool hasSubstring(const std::string& s, const std::string& pattern)
    {
      return std::search(s.begin(), s.end(), pattern.begin(), pattern.end()) != s.end();
    }

    /******************************************************************************/

    void replaceAll(std::string& target, const std::string& query, const std::string& replacement)
    {
      using diff_type = typename std::iterator_traits<decltype(target.begin())>::difference_type;
      if (query.empty())
        return;
      std::string result;
      auto it = target.begin();
      while (it != target.end())
      {
        // Find next start of pattern, copy s up to there, then append replacement, and move search after pattern.
        auto nextPattern = std::search(it, target.end(), query.begin(), query.end());
        std::copy(it, nextPattern, std::back_inserter(result));
        if (nextPattern != target.end())
        {
          result += replacement;
          it = nextPattern + static_cast<diff_type>(query.size());
        }
        else
        {
          it = nextPattern;
        }
      }
      target = std::move(result);
    }

    /******************************************************************************/
  } // namespace TextTools
} // namespace bpp

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "NestedStringTokenizer.h"
#include "TextTools.h"

using namespace bpp;

// From the STL:
#include <iostream>

using namespace std;

NestedStringTokenizer::NestedStringTokenizer(const std::string& s, const std::string& open, const std::string& end, const std::string& delimiters, bool solid) :
  StringTokenizer()
{
  int blocks = 0;
  string cache = "";
  if (!solid)
  {
    string::size_type index = s.find_first_not_of(delimiters, 0);
    while (index != s.npos)
    {
      string::size_type newIndex = s.find_first_of(delimiters, index);
      bool endBlockFound = false;
      while (!endBlockFound)
      {
        if (newIndex != s.npos)
        {
          string token = s.substr(index, newIndex - index);
          blocks += static_cast<int>(TextTools::count(token, open)) - static_cast<int>(TextTools::count(token, end));

          if (blocks == 0)
          {
            tokens_.push_back(cache + token);
            cache = ""; // reset cache.
            index = s.find_first_not_of(delimiters, newIndex);
            endBlockFound = true;
          }
          else
          {
            // Ignore this token until closing block is found
            cache += s.substr(index, newIndex - index + 1);
            index = newIndex + 1;
            newIndex = s.find_first_of(delimiters, index);
          }
        }
        else
        {
          string token = s.substr(index);
          blocks += static_cast<int>(TextTools::count(token, open)) - static_cast<int>(TextTools::count(token, end));
          if (blocks == 0)
          {
            auto s2 = cache + token;
            if (s2 == "\\t")
              s2 = "\t";
            tokens_.push_back(s2);
            cache = ""; // reset cache.
            index = newIndex;
            endBlockFound = true;
          }
          else
            throw Exception("NestedStringTokenizer (constructor). Unclosed block.");
        }
      }
    }
  }
  else
  {
    string::size_type index = 0;
    while (index != s.npos)
    {
      string::size_type newIndex = s.find(delimiters, index);
      bool endBlockFound = false;
      while (!endBlockFound)
      {
        if (newIndex != s.npos)
        {
          string token = s.substr(index, newIndex - index);
          blocks += static_cast<int>(TextTools::count(token, open)) - static_cast<int>(TextTools::count(token, end));

          if (blocks == 0)
          {
            auto s2 = cache + token;
            if (s2 == "\\t")
              s2 = "\t";
            tokens_.push_back(s2);
            cache = ""; // reset cache.
            index = newIndex + delimiters.size();
            endBlockFound = true;
          }
          else
          {
            // Ignore this token untill closing block is found
            cache += s.substr(index, newIndex - index + 1);
            index = newIndex + 1;
            newIndex = s.find(delimiters, index);
          }
        }
        else
        {
          string token = s.substr(index);
          blocks += static_cast<int>(TextTools::count(token, open)) - static_cast<int>(TextTools::count(token, end));
          if (blocks == 0)
          {
            auto s2 = cache + token;
            if (s2 == "\\t")
              s2 = "\t";
            tokens_.push_back(s2);
            cache = ""; // reset cache.
            index = newIndex;
            endBlockFound = true;
          }
          else
            throw Exception("Unclosed block.");
        }
      }
    }
  }
}

const std::string& NestedStringTokenizer::nextToken()
{
  if (!hasMoreToken())
    throw Exception("No more token in nested tokenizer.");
  return tokens_[currentPosition_++];
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "StringTokenizer.h"

using namespace bpp;
using namespace std;

StringTokenizer::StringTokenizer(const std::string& s, const std::string& delimiters, bool solid, bool allowEmptyTokens) :
  tokens_(),
  splits_(),
  currentPosition_(0)
{
  if (!solid)
  {
    string::size_type index = s.find_first_not_of(delimiters, 0);
    while (index != s.npos)
    {
      string::size_type newIndex = s.find_first_of(delimiters, index);
      if (newIndex != s.npos)
      {
        tokens_.push_back(s.substr(index, newIndex - index));
        if (!allowEmptyTokens)
          index = s.find_first_not_of(delimiters, newIndex);
        else
          index = newIndex + 1;
        splits_.push_back(s.substr(newIndex, index - newIndex));
      }
      else
      {
        tokens_.push_back(s.substr(index));
        index = newIndex;
      }
    }
  }
  else
  {
    string::size_type index = 0;
    while (index != s.npos)
    {
      string::size_type newIndex = s.find(delimiters, index);
      if (newIndex != s.npos)
      {
        tokens_.push_back(s.substr(index, newIndex - index));
        if (!allowEmptyTokens)
        {
          index = newIndex + delimiters.size();
          while (index != string::npos && s.substr(index, delimiters.size()) == delimiters)
            index += delimiters.size();
        }
        else
          index = newIndex + delimiters.size();
        splits_.push_back(s.substr(newIndex, index - newIndex));
      }
      else
      {
        tokens_.push_back(s.substr(index));
        index = newIndex;
      }
    }
  }
}

void StringTokenizer::removeEmptyTokens()
{
  for (size_t i = tokens_.size(); i > currentPosition_; i--)
  {
    if (tokens_[i - 1] == "")
      tokens_.erase(tokens_.begin() + static_cast<ptrdiff_t>(i - 1));
  }
}

std::string StringTokenizer::unparseRemainingTokens() const
{
  string s;
  for (size_t i = currentPosition_; i < tokens_.size() - 1; ++i)
  {
    s += tokens_[i] + splits_[i];
  }
  if (numberOfRemainingTokens() > 0)
    s += tokens_.back();
  return s;
}

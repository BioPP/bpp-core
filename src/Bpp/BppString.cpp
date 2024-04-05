// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <ostream>

#include "BppString.h"

namespace bpp
{
BppString::BppString() = default;
BppString::BppString(const char* value)
  : text_(value)
{}
BppString::BppString(const std::string& value)
  : text_(value)
{}
BppString& BppString::operator=(const char* value)
{
  text_ = value;
  return *this;
}
BppString& BppString::operator=(const std::string& value)
{
  text_ = value;
  return *this;
}

BppString* BppString::clone() const { return new BppString(*this); }

const std::string& BppString::toSTL() const { return text_; }

std::ostream& operator<<(std::ostream& out, const BppString& s)
{
  out << s.toSTL();
  return out;
}
} // namespace bpp

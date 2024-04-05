// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_BPPSTRING_H
#define BPP_BPPSTRING_H

#include <iosfwd>
#include <string>

#include "Clonable.h"

namespace bpp
{
/** @brief The BppString object class.
 * This class extends the stl::string class to support the Clonable interface.
 */
class BppString : public Clonable
{
private:
  std::string text_{};

public:
  BppString();
  BppString(const char* value);
  BppString(const std::string& value);
  BppString& operator=(const char* value);
  BppString& operator=(const std::string& value);
  BppString* clone() const;
  const std::string& toSTL() const;
};

std::ostream& operator<<(std::ostream& out, const BppString& s);
} // namespace bpp
#endif // BPP_BPPSTRING_H

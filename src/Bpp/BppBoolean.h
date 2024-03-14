// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_BPPBOOLEAN_H
#define BPP_BPPBOOLEAN_H

#include <ostream>

#include "Clonable.h"

namespace bpp
{
/** @brief The BppBoolean object class.
 * This class extends the bool type to support the Clonable interface.
 */
class BppBoolean : public virtual Clonable
{
private:
  bool value_;

public:
  BppBoolean(bool value = false)
    : value_(value)
  {}

  BppBoolean* clone() const { return new BppBoolean(*this); }

  const bool getValue() const { return value_; }
};

inline std::ostream& operator<<(std::ostream& out, const BppBoolean& s) { return out << s.getValue(); }
} // namespace bpp
#endif // BPP_BPPBOOLEAN_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_BPPVECTOR_H
#define BPP_BPPVECTOR_H

#include <utility>
#include <vector>

#include "Clonable.h"

namespace bpp
{
/** @brief The BppVector object class.
 * This class extends the std::vector class to support the Clonable interface.
 * @deprecated Do not use in new code anymore.
 */
template<typename T>
class BppVector :
  public Clonable,
  public std::vector<T>
{
public:
  template<typename ... Args>
  BppVector(Args&&... args) :
    std::vector<T>(std::forward<Args>(args)...)
  {}

  BppVector<T>* clone() const { return new BppVector<T>(*this); }

  const std::vector<T>& toSTL() const { return *this; }
  std::vector<T>& toSTL() { return *this; }
};
} // namespace bpp
#endif // BPP_BPPVECTOR_H

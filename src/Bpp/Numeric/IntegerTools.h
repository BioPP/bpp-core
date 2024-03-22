// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_INTEGERTOOLS_H
#define BPP_NUMERIC_INTEGERTOOLS_H

#include <type_traits>


namespace bpp
{
/// Namespace for integer utils.
namespace IntegerTools
{
/// Returns floor(n/divisor).
template<typename T>
T divideDown(T n, T divisor) noexcept
{
  static_assert (std::is_integral<T>::value, "T must be an integral type");
  return n / divisor;
}

/// Returns ceil (n/divisor).
template<typename T>
T divideUp(T n, T divisor) noexcept
{
  static_assert (std::is_integral<T>::value, "T must be an integral type");
  return divideDown(n + divisor - 1, divisor);
}

/// Round n to previous divisor multiple.
template<typename T>
T roundDown(T n, T divisor) noexcept
{
  static_assert (std::is_integral<T>::value, "T must be an integral type");
  return divisor * divideDown(n, divisor);
}

/// Round n to next divisor multiple.
template<typename T>
T roundUp(T n, T divisor) noexcept
{
  static_assert (std::is_integral<T>::value, "T must be an integral type");
  return divisor * divideUp(n, divisor);
}
}
} // namespace bpp
#endif // BPP_NUMERIC_INTEGERTOOLS_H

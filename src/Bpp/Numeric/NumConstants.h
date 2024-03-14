// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_NUMCONSTANTS_H
#define BPP_NUMERIC_NUMCONSTANTS_H

#include <cmath>
#include <limits>


namespace bpp
{
/**
 * @brief this static class contains several useful constant values.
 *
 * This classe uses function in order to avoid the infamous "static initialization order fiasco".
 * C++0x solves this...
 */
class NumConstants
{
public:
  /**
   * @name Golden ratio.
   *
   * The golden ratio, @f$\phi@f$ is equal to @f$\frac{1+\sqrt{5}}{2} = 1.6180339887498948482\ldots@f$.
   * We also define @f$R=\phi-1@f$ and @f$C = 1 - R@f$.
   * @{
   */
  static double GOLDEN_RATIO_PHI() { return (1. + sqrt(5.)) / 2.; }
  static double GOLDEN_RATIO_R() { return GOLDEN_RATIO_PHI() - 1.; }
  static double GOLDEN_RATIO_C() { return 1. - GOLDEN_RATIO_R(); }

  /** @} */

  static double MEGA() { return 1e6; }
  static double KILO() { return 1e3; }
  static double DECI() { return 1e-1; }
  static double CENTI() { return 1e-2; }
  static double MILLI() { return 1e-3; }
  static double MICRO() { return 1e-6; }
  static double NANO() { return 1e-9; }
  static double PICO() { return 1e-12; }

  static double SMALL() { return 1e-6; }
  static double TINY() { return 1e-12; }
  static double VERY_TINY() { return 1e-20; }
  static double VERY_BIG() { return static_cast<double>(1.7E+23); }

  /**
   * @name Define those constants in case they would not be available in stl/limits.
   *
   * @{
   */
  static double INF() { return std::numeric_limits<double>::has_infinity ? -std::log(0) : std::numeric_limits<double>::max(); }
  static double PINF() { return std::numeric_limits<double>::has_infinity ? -std::log(0) : std::numeric_limits<double>::max(); }
  static double MINF() { return std::numeric_limits<double>::has_infinity ? std::log(0) : std::numeric_limits<double>::min(); }
  static double NaN() { return NAN; }
  /** @} */

  static double PI() { return 3.141593; }
};
}// end of namespace bpp.
#endif // BPP_NUMERIC_NUMCONSTANTS_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_COLORTOOLS_H
#define BPP_GRAPHICS_COLORTOOLS_H


#include "../Exceptions.h"
#include "../Text/TextTools.h"
#include "RgbColor.h"

namespace bpp
{
/**
 * @brief Provide tools to deal with color objects.
 */
class ColorTools
{
public:
  ColorTools() {}
  virtual ~ColorTools() {}

public:
  /**
   * @brief Create a set of colors according to a gradient defined by two extrema.
   *
   * @param n Number of colors to output.
   * @param low First color in gradient.
   * @param high Last color in gradient.
   * @return A set of ordered colors defining a gradient.
   */
  static std::vector<RGBColor> gradient(unsigned int n, const RGBColor& low, const RGBColor& high);

  /**
   * @brief Create a set of colors according to a gradient defined by two extrema and a midpoint.
   *
   * @param n Number of colors to output.
   * @param low First color in gradient.
   * @param mid Midpoint color.
   * @param high Last color in gradient.
   * @return A set of ordered colors defining a gradient.
   */
  static std::vector<RGBColor> gradient(unsigned int n, const RGBColor& low, const RGBColor& mid, const RGBColor& high);

  /**
   * @return A gray color.
   * @param level Gray intensity ([0,1]).
   */
  static RGBColor gray(double level)
  {
    unsigned int i = (unsigned int)round(255 * level);
    return RGBColor(i, i, i);
  }

  /**
   * @brief Get a RGBColor from a cyan-magenta-yellow-key description.
   *
   * The following formula are used for the transformation:
   * @f[
   * \begin{array}{rcl}
   * r &=& 255 * (1 - c)(1 - k)\\
   * g &=& 255 * (1 - m)(1 - k)\\
   * b &=& 255 * (1 - y)(1 - k)
   * \end{array}
   * @f]
   *
   * @param c Cyan proportion.
   * @param m Magenta proportion.
   * @param y Yellow proportion.
   * @param k Black proportion.
   * @return A RGBColor object.
   */
  static RGBColor cmyk2rgb(double c, double m, double y, double k)
  {
    unsigned int r = static_cast<unsigned int>(round(255 * (1. - c) * (1. - k)));
    unsigned int g = static_cast<unsigned int>(round(255 * (1. - m) * (1. - k)));
    unsigned int b = static_cast<unsigned int>(round(255 * (1. - y) * (1. - k)));
    return RGBColor(r, g, b);
  }

public:
  static const RGBColor RED;
  static const RGBColor GREEN;
  static const RGBColor BLUE;
  static const RGBColor BLACK;
  static const RGBColor WHITE;
  static const RGBColor YELLOW;
  static const RGBColor CYAN;
  static const RGBColor MAGENTA;
  static const RGBColor ORANGE;
};
} // end of namespace bpp.
#endif // BPP_GRAPHICS_COLORTOOLS_H

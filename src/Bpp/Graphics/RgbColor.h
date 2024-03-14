// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_RGBCOLOR_H
#define BPP_GRAPHICS_RGBCOLOR_H



// From the STL:
#include <cmath>

#include "../Text/TextTools.h"
#include "../Exceptions.h"
#include "../Clonable.h"

namespace bpp
{
/**
 * @brief Describe a color according to its red, green and blue componants.
 */
class RGBColor :
  public virtual Clonable
{
protected:
  unsigned int red_;
  unsigned int green_;
  unsigned int blue_;

public:
  RGBColor(unsigned int red, unsigned int green, unsigned int blue) : red_(red), green_(green), blue_(blue) {}
  RGBColor() : red_(0), green_(0), blue_(0) {}
  virtual ~RGBColor() {}

  RGBColor* clone() const { return new RGBColor(*this); }

public:
  bool operator==(const RGBColor& color) const
  {
    return red_ == color.red_ && green_ == color.green_ && blue_ == color.blue_;
  }

  /**
   * @brief Comparison operator (for sorting purposes).
   *
   * The hexadecimal string representation is used for comparison.
   *
   * @param color The color to compare with.
   */
  bool operator<(const RGBColor& color) const
  {
    return toHex() < color.toHex();
  }

  /**
   * @brief Get the HTML-like, hexadecimal description of this color.
   */
  std::string toHex() const
  {
    std::string hex = "#";
    hex += decToHex(red_);
    hex += decToHex(green_);
    hex += decToHex(blue_);
    return hex;
  }

  /**
   * @brief Access to each color componant: 0=red, 1=green, 2=blue.
   */
  const unsigned int& operator[](unsigned int i) const
  {
    if (i == 0) return red_;
    if (i == 1) return green_;
    if (i == 2) return blue_;
    throw Exception("Invalid color index");
  }

  /**
   * @brief Access to each color componant: 0=red, 1=green, 2=blue.
   */
  unsigned int& operator[](unsigned int i)
  {
    if (i == 0) return red_;
    if (i == 1) return green_;
    if (i == 2) return blue_;
    throw Exception("Invalid color index");
  }

  /**
   * @brief Get a string description of the color, e.g. [R255,G0,B255].
   */
  std::string toString() const
  {
    return "[R" + TextTools::toString(red_) + ",G" + TextTools::toString(green_) + ",B" + TextTools::toString(blue_) + "]";
  }

protected:
  static std::string decToHex(unsigned int dec)
  {
    std::string hexa = "0123456789ABCDEF";
    std::string hex = "";
    while (dec > 15)
    {
      unsigned int tmp = dec - static_cast<unsigned int>(floor(static_cast<double>(dec) / 16.) * 16);
      hex = hexa[tmp] + hex;
      dec = static_cast<unsigned int>(floor(static_cast<double>(dec) / 16.));
    }
    hex = hexa[dec] + hex;
    if (hex.size() == 1) hex = "0" + hex;
    return hex;
  }
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_RGBCOLOR_H

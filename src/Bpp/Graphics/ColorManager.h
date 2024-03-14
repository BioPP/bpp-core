// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_COLORMANAGER_H
#define BPP_GRAPHICS_COLORMANAGER_H


#include "../Text/TextTools.h"
#include "ColorTools.h"
#include "RgbColor.h"

// From the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Associate special colors to a code.
 *
 * Instances of this interface are used in some vector format.
 */
template<class CodeType>
class ColorManager
{
public:
  ColorManager() {}
  virtual ~ColorManager() {}

public:
  /**
   * @param color The color to look for.
   * @return The code associated to a given color.
   */
  virtual CodeType getCode(const RGBColor& color) = 0;

  /**
   * @param code The code to look for.
   * @return The color associated to a given code.
   * @throw exception if the code is not valid.
   */
  virtual const RGBColor& getColor(CodeType& code) const = 0;

  /**
   * @return All valid codes.
   */
  virtual const std::vector<CodeType>& getCodes() const = 0;

  /**
   * @return All available colors.
   */
  virtual const std::vector<RGBColor>& getColors() const = 0;

  /**
   * @return The total number of colors available.
   */
  virtual size_t getNumberOfColors() const = 0;
};


/**
 * @brief Color manager for the XFig format.
 *
 * Default colors are coded from 0 to 31.
 * New colors may be added from code 32.
 */
class XFigColorManager :
  public ColorManager<unsigned int>
{
protected:
  unsigned int currentCode_;
  std::vector<RGBColor> colors_;
  std::vector<unsigned int> codes_;

public:
  XFigColorManager() :
    currentCode_(31),
    colors_(),
    codes_()
  {
    // Add "official" color codes, from 0 to 31:
    codes_.push_back(0); colors_.push_back(ColorTools::BLACK);
    codes_.push_back(1); colors_.push_back(ColorTools::BLUE);
    codes_.push_back(2); colors_.push_back(ColorTools::GREEN);
    codes_.push_back(3); colors_.push_back(ColorTools::CYAN);
    codes_.push_back(4); colors_.push_back(ColorTools::RED);
    codes_.push_back(5); colors_.push_back(ColorTools::MAGENTA);
    codes_.push_back(6); colors_.push_back(ColorTools::YELLOW);
    codes_.push_back(7); colors_.push_back(ColorTools::WHITE);
    codes_.push_back(8); colors_.push_back(RGBColor(0, 0, 140));
    codes_.push_back(9); colors_.push_back(RGBColor(0, 0, 173));
    codes_.push_back(10); colors_.push_back(RGBColor(0, 0, 206));
    codes_.push_back(11); colors_.push_back(RGBColor(132, 207, 205));
    codes_.push_back(12); colors_.push_back(RGBColor(0, 142, 0));
    codes_.push_back(13); colors_.push_back(RGBColor(0, 174, 0));
    codes_.push_back(14); colors_.push_back(RGBColor(0, 207, 0));
    codes_.push_back(15); colors_.push_back(RGBColor(0, 142, 140));
    codes_.push_back(16); colors_.push_back(RGBColor(0, 174, 173));
    codes_.push_back(17); colors_.push_back(RGBColor(0, 207, 206));
    codes_.push_back(18); colors_.push_back(RGBColor(140, 0, 0));
    codes_.push_back(19); colors_.push_back(RGBColor(173, 0, 0));
    codes_.push_back(20); colors_.push_back(RGBColor(206, 0, 0));
    codes_.push_back(21); colors_.push_back(RGBColor(140, 0, 140));
    codes_.push_back(22); colors_.push_back(RGBColor(173, 0, 173));
    codes_.push_back(23); colors_.push_back(RGBColor(206, 0, 206));
    codes_.push_back(24); colors_.push_back(RGBColor(132, 48, 0));
    codes_.push_back(25); colors_.push_back(RGBColor(156, 65, 0));
    codes_.push_back(26); colors_.push_back(RGBColor(189, 97, 0));
    codes_.push_back(27); colors_.push_back(RGBColor(255, 130, 132));
    codes_.push_back(28); colors_.push_back(RGBColor(255, 158, 156));
    codes_.push_back(29); colors_.push_back(RGBColor(255, 190, 189));
    codes_.push_back(30); colors_.push_back(RGBColor(255, 223, 222));
    codes_.push_back(31); colors_.push_back(RGBColor(255, 215, 0));
  }
  virtual ~XFigColorManager() {}

public:
  unsigned int getCode(const RGBColor& color)
  {
    for (unsigned int i = 0; i < colors_.size(); i++)
    {
      if (colors_[i] == color)
      {
        return codes_[i];
      }
    }
    currentCode_++;
    colors_.push_back(color);
    codes_.push_back(currentCode_);
    return currentCode_;
  }
  const RGBColor& getColor(unsigned int& code) const
  {
    for (unsigned int i = 0; i < codes_.size(); i++)
    {
      if (codes_[i] == code)
      {
        return colors_[i];
      }
    }
    throw Exception("XFigColorManager::getColor. No color associated to this code: " + TextTools::toString(code));
  }
  const std::vector<unsigned int>& getCodes() const { return codes_; }
  const std::vector<RGBColor>& getColors() const { return colors_; }
  size_t getNumberOfColors() const { return colors_.size(); }
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_COLORMANAGER_H

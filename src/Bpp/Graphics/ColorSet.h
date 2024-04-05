// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_COLORSET_H
#define BPP_GRAPHICS_COLORSET_H


#include "../Utils/MapTools.h"
#include "RgbColor.h"

// From the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Specify a set of color definitions.
 */
class ColorSet
{
public:
  ColorSet() {}
  virtual ~ColorSet() {}

public:
  /**
   * @brief Get the color object corresponding to a given name.
   *
   * @param name The name of the color to look for.
   * @return The color associated to the given name.
   * @throw Exception if the name is not assigned to any color.
   */
  virtual const RGBColor& getColor(const std::string& name) const = 0;

  /**
   * @brief Get the ith color object in the set.
   *
   * @param index The index of the color to look for.
   * @return The color associated to the given index.
   * @throw IndexOutOfBoundsException if the index is not assigned to any color.
   */
  virtual const RGBColor& getColor(unsigned int index) const = 0;

  /**
   * @return All valid color names.
   */
  virtual std::vector<std::string> getColorNames() const = 0;

  /**
   * @return The total number of colors available.
   */
  virtual size_t getNumberOfColors() const = 0;
};


/**
 * @brief Partial implementation of the ColorSet interface.
 *
 * Derivative classes just have to fill the colors_ map in the constructor of the class.
 */
class AbstractColorSet :
  public ColorSet
{
protected:
  std::map<std::string, RGBColor> colors_;

public:
  AbstractColorSet() : colors_() {}
  virtual ~AbstractColorSet() {}

public:
  const RGBColor& getColor(const std::string& name) const
  {
    std::map<std::string, RGBColor>::const_iterator it = colors_.find(name);
    if (it != colors_.end()) return it->second;
    else throw Exception("AbstractColorSet::getColor(name): no color with name " + name);
  }

  const RGBColor& getColor(unsigned int index) const
  {
    if (index >= colors_.size()) throw IndexOutOfBoundsException("AbstractColorSet::getColor(index): invalid index.", index, 0, colors_.size() - 1);
    std::map<std::string, RGBColor>::const_iterator it = colors_.begin();
    for (unsigned int i = 0; i < index; i++) { it++;}
    return it->second;
  }

  std::vector<std::string> getColorNames() const { return MapTools::getKeys(colors_); }

  size_t getNumberOfColors() const { return colors_.size(); }
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_COLORSET_H

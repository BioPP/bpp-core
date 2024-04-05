// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_DEFAULTCOLORSET_H
#define BPP_GRAPHICS_DEFAULTCOLORSET_H


#include "ColorSet.h"
#include "ColorTools.h"

namespace bpp
{
/**
 * @brief Default color definitions.
 */
class DefaultColorSet :
  public AbstractColorSet
{
public:
  DefaultColorSet()
  {
    colors_["black"]   = ColorTools::BLACK;
    colors_["red"]     = ColorTools::RED;
    colors_["green"]   = ColorTools::GREEN;
    colors_["blue"]    = ColorTools::BLUE;
    colors_["yellow"]  = ColorTools::YELLOW;
    colors_["magenta"] = ColorTools::MAGENTA;
    colors_["cyan"]    = ColorTools::CYAN;
    colors_["white"]   = ColorTools::WHITE;
  }
  virtual ~DefaultColorSet() {}
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_DEFAULTCOLORSET_H

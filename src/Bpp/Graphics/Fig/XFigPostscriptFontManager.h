// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_FIG_XFIGPOSTSCRIPTFONTMANAGER_H
#define BPP_GRAPHICS_FIG_XFIGPOSTSCRIPTFONTMANAGER_H


#include "../Font/FontManager.h"

namespace bpp
{
/**
 * @brief Postscript font manager for the XFig format.
 */
class XFigPostscriptFontManager :
  public AbstractFontManager<int>
{
public:
  XFigPostscriptFontManager();

  virtual ~XFigPostscriptFontManager() {}
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_FIG_XFIGPOSTSCRIPTFONTMANAGER_H

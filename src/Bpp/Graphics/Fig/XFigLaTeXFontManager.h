// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_FIG_XFIGLATEXFONTMANAGER_H
#define BPP_GRAPHICS_FIG_XFIGLATEXFONTMANAGER_H


#include "../Font/FontManager.h"

namespace bpp
{
/**
 * @brief LaTeX font manager for the XFig format.
 */
class XFigLaTeXFontManager :
  public AbstractFontManager<int>
{
public:
  XFigLaTeXFontManager();

  virtual ~XFigLaTeXFontManager() {}
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_FIG_XFIGLATEXFONTMANAGER_H

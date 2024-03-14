// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_LATEX_DVIPSCOLORSET_H
#define BPP_GRAPHICS_LATEX_DVIPSCOLORSET_H


#include "../ColorSet.h"

namespace bpp
{
/**
 * @brief The dvips color definitions.
 *
 * Source: dvipsnam.def, from the TeXlive LaTeX distribution.
 */
class DvipsColorSet :
  public AbstractColorSet
{
public:
  DvipsColorSet();
  virtual ~DvipsColorSet() {}
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_LATEX_DVIPSCOLORSET_H

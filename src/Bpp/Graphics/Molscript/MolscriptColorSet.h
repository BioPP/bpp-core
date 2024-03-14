// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_MOLSCRIPT_MOLSCRIPTCOLORSET_H
#define BPP_GRAPHICS_MOLSCRIPT_MOLSCRIPTCOLORSET_H


#include "../ColorSet.h"

namespace bpp
{
/**
 * @brief Molscript color definitions.
 */
class MolscriptColorSet :
  public AbstractColorSet
{
public:
  MolscriptColorSet();
  virtual ~MolscriptColorSet() {}
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_MOLSCRIPT_MOLSCRIPTCOLORSET_H

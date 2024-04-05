// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_R_RCOLORSET_H
#define BPP_GRAPHICS_R_RCOLORSET_H


#include "../ColorSet.h"

namespace bpp
{
/**
 * @brief R color definitions.
 */
class RColorSet :
  public AbstractColorSet
{
public:
  /**
   * The following R code has been used to generate this code:
   * @code
   * c<-colors()
   * file<-"Rcolors.cpp"
   * unlink(file)
   * for(i in c)
   * {
   *   r<-col2rgb(i)["red",1]
   *   g<-col2rgb(i)["green",1]
   *   b<-col2rgb(i)["blue",1]
   *   cat(file=file, append=TRUE, paste("  _colors[\"",i,"\"] = RGBColor(",r ,", ",g,", ", b, ");\n",sep=""));
   * }
   * @endcode
   */
  RColorSet();
  virtual ~RColorSet() {}
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_R_RCOLORSET_H

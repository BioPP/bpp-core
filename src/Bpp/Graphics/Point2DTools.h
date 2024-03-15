// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_POINT2DTOOLS_H
#define BPP_GRAPHICS_POINT2DTOOLS_H


// From local
#include "Point2D.h"

namespace bpp
{
/**
 * @brief Some functions to deal with Point2D.
 *
 * @author Sylvain Gaillard
 */

class CoordsTools
{
public:
  /**
   * @brief Get the distance between two Coord objects.
   *
   * @param co1 A Point2D object.
   * @param co2 An other Point2D object.
   * @return the distance between the 2 points as a double
   */
  template<class T> static double getDistanceBetween(const Point2D<T>& co1, const Point2D<T>& co2)
  {
    T base, height;
    base = co1.getX() - co2.getX();
    height = co1.getY() - co2.getY();
    base = base * base;
    height = height * height;
    return sqrt((double)base + (double)height);
  }
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_POINT2DTOOLS_H

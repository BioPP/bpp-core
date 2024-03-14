// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_POINT2D_H
#define BPP_GRAPHICS_POINT2D_H


#include "../Clonable.h"

namespace bpp
{
/**
 * @brief The Point2D class.
 *
 * This is a simple class designed to store the coordinates of a point.
 * The type of the two coordinates is defined as a template.
 *
 * @author Sylvain Gaillard, Julien Dutheil
 */
template<class T> class Point2D :
  public virtual Clonable
{
private:
  T x_;
  T y_;

public:
  // Constructors and destructor :

  /**
   * @brief Build a new Point2D from two values.
   *
   * The two values are set to 0 if no parametre is given to the constructor.
   *
   * @param x The longitude or abscissa.
   * @param y The latitude or ordinate.
   */
  Point2D<T>(const T x = 0, const T y = 0) : x_(x), y_(y) {}

  /**
   * @brief Destroy the Point2D object.
   */
  virtual ~Point2D() {}

public:
  // Methodes

  /**
   * @brief Implement the Clonable interface.
   */
  Point2D<T>* clone() const { return new Point2D(*this); }

  /**
   * @brief Set the two values.
   */
  void setCoord(const T x, const T y);

  /**
   * @brief Set only the longitude.
   */
  void setX(const T x) { x_ = x; }

  /**
   * @brief Set only the latitude.
   */
  void setY(const T y) { y_ = y; }

  /**
   * @brief Get the longitude.
   */
  const T& getX() const { return x_; }

  /**
   * @brief Get the latitude.
   */
  const T& getY() const { return y_; }

  /**
   * @brief Compares two Point2D objets.
   *
   * Return true if the coordinates of the 2 Point2D are equals.
   */
  bool hasSameCoordsAs(const Point2D<T>& coord) const
  {
    return x_ == coord.x_ && y_ == coord.y_;
  }

  /**
   * @brief The == operator.
   *
   * Return true if the coordinates of the 2 Point2Ds are equals.
   * Does the same as the asSameCoords() methode.
   */
  virtual bool operator==(const Point2D<T>& coord) const
  {
    return hasSameCoordsAs(coord);
  }

  /**
   * @brief The != operator.
   */
  virtual bool operator!=(const Point2D<T>& coord) const
  {
    return !hasSameCoordsAs(coord);
  }
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_POINT2D_H

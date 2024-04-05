// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_GRAPHICDEVICE_H
#define BPP_GRAPHICS_GRAPHICDEVICE_H


#include "Font/Font.h"
#include "RgbColor.h"

namespace bpp
{
class UnvalidFlagException : public virtual Exception
{
public:
  UnvalidFlagException(const std::string& text) : Exception(text) {}
};


/**
 * @brief Interface for all plotting devices.
 *
 * Implement this interface to support new formats.
 */
class GraphicDevice
{
public:
  GraphicDevice() {}
  virtual ~GraphicDevice() {}

public:
  /**
   * @brief Start the painting.
   */
  virtual void begin() = 0;

  /**
   * @brief End the painting.
   *
   * Depending on the implementation of the device,
   * the call of this method might be required before
   * the plotting commands become visible.
   */
  virtual void end() = 0;

  /**
   * @param xu The x expansion factor. The actual unit actually depends on the implementation.
   */
  virtual void setXUnit(double xu) = 0;

  /**
   * @param yu The y expansion factor. The actual unit actually depends on the implementation.
   */
  virtual void setYUnit(double yu) = 0;

  /**
   * @return The x expansion factor. The actual unit actually depends on the implementation.
   */
  virtual double getXUnit() const = 0;

  /**
   * @return The y expansion factor. The actual unit actually depends on the implementation.
   */
  virtual double getYUnit() const = 0;

  virtual void setCurrentForegroundColor(const RGBColor& color) = 0;
  virtual void setCurrentBackgroundColor(const RGBColor& color) = 0;
  virtual void setCurrentFont(const Font& font) = 0;
  virtual void setCurrentPointSize(unsigned int size) = 0;
  virtual void setCurrentLineType(short type) = 0;
  virtual void setCurrentLayer(int layerIndex) = 0;

  virtual const RGBColor& getCurrentForegroundColor() const = 0;
  virtual const RGBColor& getCurrentBackgroundColor() const = 0;
  virtual const Font& getCurrentFont() const = 0;
  virtual unsigned int getCurrentPointSize() const = 0;
  virtual short getCurrentLineType() const = 0;
  virtual int getCurrentLayer() const = 0;


  /**
   * @brief Draw a line between two points.
   *
   * This method uses the current foreground color and the current line type.
   *
   * @param x1 x coordinate 1
   * @param y1 y coordinate 1
   * @param x2 x coordinate 2
   * @param y2 y coordinate 2
   */
  virtual void drawLine(double x1, double y1, double x2, double y2) = 0;

  /**
   * @brief Draw a rectangle.
   *
   * This method uses the current foreground color and the current line type for drawing the stroke of the rectangle,
   * and the current background color for filling the rectangle.
   *
   * @param x x coordinate
   * @param y y coordinate
   * @param width The rectangle width
   * @param height The rectangle height
   * @param fill Filling type (one of FILL_EMPTY, FILL_FILLED or FILL_PATTERN).
   */
  virtual void drawRect(double x, double y, double width, double height, short fill = FILL_EMPTY) = 0;

  /**
   * @brief Draw a circle.
   *
   * This method uses the current foreground color and the current line type for drawing the stroke of the circle,
   * and the current background color for filling the circle.
   *
   * @param x x coordinate of the center
   * @param y y coordinate of the center
   * @param radius The circle radius
   * @param fill Filling type (one of FILL_EMPTY, FILL_FILLED or FILL_PATTERN).
   */
  virtual void drawCircle(double x, double y, double radius, short fill = FILL_EMPTY) = 0;

  /**
   * @brief Draw some characters.
   *
   * This method uses the current foreground color.
   *
   * @param x x coordinate
   * @param y y coordinate
   * @param text The characters to draw
   * @param hpos Horizontal adjustment, one of TEXT_HORIZONTAL_LEFT, TEXT_HORIZONTAL_CENTER or TEXT_HORIZONTAL_RIGHT.
   * @param vpos Vertical adjustment, one of TEXT_VERTICAL_LEFT, TEXT_VERTICAL_CENTER or TEXT_VERTICAL_RIGHT.
   * @param angle Angle i radian to rotate the text.
   */
  virtual void drawText(double x, double y, const std::string& text, short hpos = TEXT_HORIZONTAL_LEFT, short vpos = TEXT_VERTICAL_BOTTOM, double angle = 0) = 0;

  /**
   * @brief Add a comment in the output.
   *
   * @param comment Comment text.
   */
  virtual void comment(const std::string& comment) = 0;

public:
  static short TEXT_HORIZONTAL_CENTER;
  static short TEXT_HORIZONTAL_LEFT;
  static short TEXT_HORIZONTAL_RIGHT;
  static short TEXT_VERTICAL_CENTER;
  static short TEXT_VERTICAL_BOTTOM;
  static short TEXT_VERTICAL_TOP;

  static short FILL_EMPTY;
  static short FILL_FILLED;
  static short FILL_PATTERN;

  static short LINE_SOLID;
  static short LINE_DASHED;
  static short LINE_DOTTED;
};
} // end of namespace bpp.
#endif // BPP_GRAPHICS_GRAPHICDEVICE_H

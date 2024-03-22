// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_ABSTRACTGRAPHICDEVICE_H
#define BPP_GRAPHICS_ABSTRACTGRAPHICDEVICE_H


#include "GraphicDevice.h"

namespace bpp
{
/**
 * @brief Partial implementation of the GraphicDevice interface.
 *
 * Implement this interface to support new formats.
 */
class AbstractGraphicDevice :
  public virtual GraphicDevice
{
private:
  double xUnit_;
  double yUnit_;
  RGBColor fgColor_;
  RGBColor bgColor_;
  Font font_;
  unsigned int pointSize_;
  short lineType_;
  int currentLayer_;

public:
  AbstractGraphicDevice() : xUnit_(1.), yUnit_(1.),
    fgColor_(0, 0, 0), bgColor_(0, 0, 0), font_(), pointSize_(1), lineType_(LINE_SOLID), currentLayer_(-1)
  {}

  virtual ~AbstractGraphicDevice() {}

public:
  void setXUnit(double xu) { xUnit_ = xu; }
  void setYUnit(double yu) { yUnit_ = yu; }
  double getXUnit() const { return xUnit_; }
  double getYUnit() const { return yUnit_; }

  void setCurrentForegroundColor(const RGBColor& color) { fgColor_ = color; }
  void setCurrentBackgroundColor(const RGBColor& color) { bgColor_ = color; }
  void setCurrentFont(const Font& font) { font_ = font; }
  void setCurrentPointSize(unsigned int size) { pointSize_ = size; }
  void setCurrentLineType(short type)
  {
    if       (type == LINE_SOLID) lineType_ = type;
    else if (type == LINE_DASHED) lineType_ = type;
    else if (type == LINE_DOTTED) lineType_ = type;
    else throw Exception("AbstractGraphicDevice::setCurrentLineType. Unknown line type: " + TextTools::toString(type));
  }
  void setCurrentLayer(int layerIndex) { currentLayer_ = layerIndex; }

  const RGBColor& getCurrentForegroundColor() const { return fgColor_; }
  const RGBColor& getCurrentBackgroundColor() const { return bgColor_; }
  const Font& getCurrentFont() const { return font_; }
  unsigned int getCurrentPointSize() const { return pointSize_; }
  short getCurrentLineType() const { return lineType_; }
  int getCurrentLayer() const { return currentLayer_; }

protected:
  double x_(double x) const { return x * xUnit_; }
  double y_(double y) const { return y * yUnit_; }

  double revx_(double x) const { return x / xUnit_; }
  double revy_(double y) const { return y / yUnit_; }
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_ABSTRACTGRAPHICDEVICE_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_SVG_SVGGRAPHICDEVICE_H
#define BPP_GRAPHICS_SVG_SVGGRAPHICDEVICE_H


#include "../AbstractGraphicDevice.h"
#include "../ColorTools.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief SVG plotting format.
 */
class SvgGraphicDevice :
  public AbstractGraphicDevice
{
private:
  std::ostream& out_;
  std::map<int, std::vector<std::string>, std::greater<int> > layers_; // Layer display as in xfig
  bool inkscapeEnabled_;
  double minx_, maxx_, miny_, maxy_;
  std::map<short int, std::string> fontStyles_;
  std::map<short int, std::string> fontWeights_;

public:
  SvgGraphicDevice(std::ostream& out, bool inkscapeEnabled = false) :
    out_(out),
    layers_(),
    inkscapeEnabled_(inkscapeEnabled),
    minx_(0), maxx_(0), miny_(0), maxy_(0),
    fontStyles_(), fontWeights_()
  {
    fontStyles_[Font::STYLE_NORMAL] = "";
    fontStyles_[Font::STYLE_ITALIC] = "italic";
    fontWeights_[Font::WEIGHT_NORMAL] = "";
    fontWeights_[Font::WEIGHT_BOLD] = "bold";
  }

  virtual ~SvgGraphicDevice() {}

public:
  void begin();
  void end();

  void drawLine(double x1, double y1, double x2, double y2);
  void drawRect(double x, double y, double width, double height, short fill = FILL_EMPTY);
  void drawCircle(double x, double y, double radius, short fill = FILL_EMPTY);
  void drawText(double x, double y, const std::string& text, short hpos = TEXT_HORIZONTAL_LEFT, short vpos = TEXT_VERTICAL_BOTTOM, double angle = 0);
  void comment(const std::string& text)
  {
    layers_[getCurrentLayer()].push_back("<!-- " + text + " -->");
  }

public:
  static std::string colorToText(const RGBColor& color)
  {
    return "rgb(" + TextTools::toString(color[0]) + "," + TextTools::toString(color[1]) + "," + TextTools::toString(color[2]) + ")";
  }
};
} // end of namespace bpp.
#endif // BPP_GRAPHICS_SVG_SVGGRAPHICDEVICE_H

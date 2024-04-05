// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_FIG_XFIGGRAPHICDEVICE_H
#define BPP_GRAPHICS_FIG_XFIGGRAPHICDEVICE_H


#include "../AbstractGraphicDevice.h"
#include "../ColorManager.h"
#include "XFigLaTeXFontManager.h"
#include "XFigPostscriptFontManager.h"

// From the STL:
#include <map>
#include <iostream>

namespace bpp
{
/**
 * @brief XFig plotting format.
 */
class XFigGraphicDevice :
  public AbstractGraphicDevice
{
private:
  std::ostream& out_;
  std::vector<std::string> content_;
  XFigColorManager colorManager_;
  XFigLaTeXFontManager latexFontManager_;
  XFigPostscriptFontManager postscriptFontManager_;
  unsigned int fgColorCode_;
  unsigned int bgColorCode_;
  int fontCode_;
  unsigned int fontSize_;
  unsigned int fontFlag_;
  short lineTypeCode_;

public:
  XFigGraphicDevice(std::ostream& out) :
    out_(out),
    content_(),
    colorManager_(),
    latexFontManager_(),
    postscriptFontManager_(),
    fgColorCode_(0),
    bgColorCode_(0),
    fontCode_(-1),
    fontSize_(12),
    fontFlag_(FONTFLAG_POSTSCRIPT),
    lineTypeCode_(LINE_SOLID)
  {
    setCurrentLayer(0);
  }

  virtual ~XFigGraphicDevice() {}

public:
  void begin();
  void end();

  void setCurrentForegroundColor(const RGBColor& color);
  void setCurrentBackgroundColor(const RGBColor& color);
  void setCurrentFont(const Font& font);
  void setCurrentLineType(short type)
  {
    if (type == LINE_SOLID) lineTypeCode_ = 0;
    else if (type == LINE_DASHED) lineTypeCode_ = 1;
    else if (type == LINE_DOTTED) lineTypeCode_ = 2;
    else throw Exception("XFigGraphicDevice::setCurrentLineType. Unknown line type: " + TextTools::toString(type));
    AbstractGraphicDevice::setCurrentLineType(type);
  }
  void drawLine(double x1, double y1, double x2, double y2);
  void drawRect(double x, double y, double width, double height, short fill = FILL_EMPTY);
  void drawCircle(double x, double y, double radius, short fill = FILL_EMPTY);
  void drawText(double x, double y, const std::string& text, short hpos = TEXT_HORIZONTAL_LEFT, short vpos = TEXT_VERTICAL_BOTTOM, double angle = 0);
  void comment(const std::string& text)
  {
    content_.push_back("#" + text);
  }

  // Specific:
  void setFontFlag(unsigned int flag) { fontFlag_ = flag; }

protected:
  int getFillCode(short fill);

public:
  static const unsigned int FONTFLAG_LATEX;
  static const unsigned int FONTFLAG_POSTSCRIPT;
};
} // end of namespace bpp;
#endif // BPP_GRAPHICS_FIG_XFIGGRAPHICDEVICE_H

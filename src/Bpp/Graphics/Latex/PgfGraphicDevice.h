// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_LATEX_PGFGRAPHICDEVICE_H
#define BPP_GRAPHICS_LATEX_PGFGRAPHICDEVICE_H


#include "../AbstractGraphicDevice.h"
#include "../ColorTools.h"

// From the STL:
#include <map>
#include <vector>
#include <string>

namespace bpp
{
/**
 * @brief LaTeX Portable Graphic Format (Pgf) plotting format.
 */
class PgfGraphicDevice :
  public AbstractGraphicDevice
{
private:
  std::ostream& out_;
  std::string fgColorStr_;
  std::string bgColorStr_;
  std::vector<std::string> content_;
  std::vector<int> layers_;
  std::map<const RGBColor, std::string> colorIndex_;
  unsigned int colorCount_;
  bool useLayers_;
  bool contentStarted_;
  mutable std::map<short int, std::string> fontShapes_;
  mutable std::map<short int, std::string> fontSeries_;

public:
  /**
   * @brief Build a new Pgf device object.
   *
   * Coordinates in Pgf are the same as in LaTeX, so it can be any of cm, mm, in, pt, px, etc.
   * For compatibility with other devices, the constructor takes as input the scale of the drawing, as cm per points.
   * All coordinates and widths will be multiplied by the factor in the output file.
   *
   * @param out The output stream.
   * @param unit The unit length.
   */
  PgfGraphicDevice(std::ostream& out, double unit);

  virtual ~PgfGraphicDevice() {}

public:
  void begin();
  void end();

  void setCurrentForegroundColor(const RGBColor& color);
  void setCurrentBackgroundColor(const RGBColor& color);
  void setCurrentFont(const Font& font);
  void setCurrentPointSize(unsigned int size);
  void setCurrentLineType(short type);
  void setCurrentLayer(int layerIndex);

  void drawLine(double x1, double y1, double x2, double y2);
  void drawRect(double x, double y, double width, double height, short fill = FILL_EMPTY);
  void drawCircle(double x, double y, double radius, short fill = FILL_EMPTY);
  void drawText(double x, double y, const std::string& text, short hpos = TEXT_HORIZONTAL_LEFT, short vpos = TEXT_VERTICAL_BOTTOM, double angle = 0);
  void comment(const std::string& text)
  {
    content_.push_back("%" + text);
  }

public:
  const std::string& getFontShape(const Font& font);
};
} // end of namespace bpp.
#endif // BPP_GRAPHICS_LATEX_PGFGRAPHICDEVICE_H

//
// File: PgfGraphicDevice.cpp
// Created by: Julien Dutheil
// Created on: Thu Jun 19 2008
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

This software is a computer program whose purpose is to provide utilitary
classes. This file belongs to the Bio++ Project.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#include "PgfGraphicDevice.h"
using namespace bpp;

#include<algorithm>
using namespace std;


PgfGraphicDevice::PgfGraphicDevice(std::ostream& out, double unit) :
  out_(out),
  fgColorStr_("black"),
  bgColorStr_("white"),
  content_(),
  layers_(),
  colorIndex_(),
  colorCount_(0),
  useLayers_(false),
  contentStarted_(false),
  fontShapes_(),
  fontSeries_()
{
  colorIndex_[ColorTools::BLACK]   = "black";
  colorIndex_[ColorTools::WHITE]   = "white";
  colorIndex_[ColorTools::BLUE]    = "blue";
  colorIndex_[ColorTools::RED]     = "red";
  colorIndex_[ColorTools::GREEN]   = "green";
  colorIndex_[ColorTools::YELLOW]  = "yellow";
  colorIndex_[ColorTools::CYAN]    = "cyan";
  colorIndex_[ColorTools::MAGENTA] = "magenta";

  fontShapes_[Font::STYLE_NORMAL]  = "n";
  fontShapes_[Font::STYLE_ITALIC]  = "it";
  fontSeries_[Font::WEIGHT_NORMAL] = "m";
  fontSeries_[Font::WEIGHT_BOLD]   = "bx";

  setCurrentFont(Font("cmtt", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12));
  setXUnit(unit);
  setYUnit(unit);
}

void PgfGraphicDevice::begin()
{
  content_.clear();
  layers_.clear();
  colorIndex_.clear();
  colorCount_ = 0;
  useLayers_ = false;
  contentStarted_ = false;
}

bool comp( int a, int b ) { return a > b; } ;
void PgfGraphicDevice::end()
{
  ostringstream oss;
  if (useLayers_)
    oss << "\\end{pgfonlayer}{" << TextTools::toString(getCurrentLayer()) << "}" << endl;
  content_.push_back(oss.str());

  //Header:
  out_ << "\\documentclass{article}" << endl;
  out_ << "% This figure was generated by the Bio++ Pgf Graphic Device." << endl;
  out_ << "% Althought this file can be compiled 'as is' it may not be displayed correctly, depending on the size of the picture." << endl;
  out_ << "% You may consider copying the pgfpicture environment to your own LaTeX file and play with pgf settings (e.g. the pgfpages module)." << endl;
  out_ << "% You can also use the geometry package, for instance:" << endl;
  out_ << "% \\usepackage[a3paper]{geometry}" << endl;
  out_ << "\\usepackage{pgf}" << endl;
  for(map<const RGBColor, string>::iterator it = colorIndex_.begin(); it != colorIndex_.end(); it++)
  {
    if(it->second.substr(0,3) == "use")
      out_ << "\\definecolor{" << it->second << "}{rgb}{" << it->first[0]/255. << "," << it->first[1]/255. << "," << it->first[2]/255. << "}" << endl;    
  }
  out_ << "\\begin{document}" << endl;

  //Declare and set layers:
  if (useLayers_)
  {
    string tmp;
    sort(layers_.begin(), layers_.end(), comp);
    for(unsigned int i = 0; i < layers_.size(); i++)
    {
      if(i > 0) tmp += ",";
      tmp += TextTools::toString(layers_[i]);
      out_ << "\\pgfdeclarelayer{" << layers_[i] << "}" << endl;
    }
    out_ << "\\pgfsetlayers{" << tmp << "}" << endl;
  }

  //Start picture:
  out_ << "\\begin{pgfpicture}" << endl;
  out_ << "\\pgfsetxvec{\\pgfpoint{" << getXUnit() << "cm}{0cm}}" << endl;
  out_ << "\\pgfsetyvec{\\pgfpoint{0cm}{-" << getYUnit() << "cm}}" << endl;
  
  for(unsigned int i = 0; i < content_.size(); i++)
  {
    out_ << content_[i] << endl;
  }
  
  out_ << "\\end{pgfpicture}" << endl;
  out_ << "\\end{document}" << endl;
}

void PgfGraphicDevice::setCurrentForegroundColor(const RGBColor& color)
{
  map<const RGBColor, string>::iterator it = colorIndex_.find(color);
  if(it != colorIndex_.end())
  {
    fgColorStr_ = it->second;
  }
  else
  {
    colorCount_++;
    fgColorStr_ = "usercolor" + TextTools::toString(colorCount_);
    colorIndex_[color] = fgColorStr_;
  }
  AbstractGraphicDevice::setCurrentForegroundColor(color);
  ostringstream oss;
  oss << "\\pgfsetstrokecolor{" << fgColorStr_ << "}" << endl;
  content_.push_back(oss.str());
}

void PgfGraphicDevice::setCurrentBackgroundColor(const RGBColor& color)
{
  map<const RGBColor, string>::iterator it = colorIndex_.find(color);
  if (it != colorIndex_.end())
  {
    bgColorStr_ = it->second;
  }
  else
  {
    colorCount_++;
    bgColorStr_ = "usercolor" + TextTools::toString(colorCount_);
    colorIndex_[color] = bgColorStr_;
  }
  AbstractGraphicDevice::setCurrentBackgroundColor(color);
  ostringstream oss;
  oss << "\\pgfsetfillcolor{" << bgColorStr_ << "}" << endl;
  content_.push_back(oss.str());
}

void PgfGraphicDevice::setCurrentFont(const Font& font)
{
  AbstractGraphicDevice::setCurrentFont(font);
  ostringstream oss;
  oss << "\\fontfamily{" << font.getFamily() << "}" << endl;
  oss << "\\fontseries{" << fontSeries_[font.getSeries()] << "}" << endl;
  oss << "\\fontshape{"  << fontShapes_[font.getShape()] << "}"  << endl;
  oss << "\\fontsize{"   << font.getSize() << "}{" << font.getSize() << "}" << endl;
  oss << "\\selectfont"  << endl;
  content_.push_back(oss.str());
}

void PgfGraphicDevice::setCurrentPointSize(unsigned int size)
{
  AbstractGraphicDevice::setCurrentPointSize(size);
  ostringstream oss;
  oss << "\\pgfsetlinewidth{" << x_(size) << "}" << endl;
  content_.push_back(oss.str());
}

void PgfGraphicDevice::setCurrentLineType(short type)
{ 
  AbstractGraphicDevice::setCurrentLineType(type);
  if(type == LINE_SOLID)
  {
    ostringstream oss;
    oss << "\\pgfsetdash{}{0pt}" << endl;
    content_.push_back(oss.str());
  }
  else if(type == LINE_DASHED)
  {
    ostringstream oss;
    oss << "\\pgfsetdash{{3mm}{2mm}}{0pt}" << endl;
    content_.push_back(oss.str());
  }
  else if(type == LINE_DOTTED)
  {
    ostringstream oss;
    oss << "\\pgfsetdash{{" << (x_(getCurrentPointSize())) << "}{" << (x_(getCurrentPointSize())) << "}}{0pt}" << endl;
    content_.push_back(oss.str());
  }
  else throw Exception("PgfGraphicDevice::setCurrentLineType. Unknown line type: " + TextTools::toString(type));
}

void PgfGraphicDevice::setCurrentLayer(int layerIndex)
{
  if (!useLayers_ && contentStarted_)
    throw Exception("PgfGraphicDevice::setCurrentLayer. A layer is specified after some content has been already added, this would result in a corrupted display.");
  ostringstream oss;
  if (useLayers_)
    oss << "\\end{pgfonlayer}{" << TextTools::toString(getCurrentLayer()) << "}" << endl;
  oss << "\\begin{pgfonlayer}{" << TextTools::toString(layerIndex) << "}" << endl;
  //We have to recall the current color values for this layer:
  oss << "\\pgfsetstrokecolor{" << fgColorStr_ << "}" << endl;
  oss << "\\pgfsetfillcolor{" << bgColorStr_ << "}" << endl;
  content_.push_back(oss.str());
  AbstractGraphicDevice::setCurrentLayer(layerIndex);
  if (find(layers_.begin(), layers_.end(), layerIndex) == layers_.end())
    layers_.push_back(layerIndex);
  useLayers_ = true;
}

void PgfGraphicDevice::drawLine(double x1, double y1, double x2, double y2)
{
  ostringstream oss;
  oss << "\\pgfpathmoveto{\\pgfpointxy{" << x1 << "}{" << y1 << "}}" << endl;
  oss << "\\pgfpathlineto{\\pgfpointxy{" << x2 << "}{" << y2 << "}}" << endl;
  oss << "\\pgfpathclose" << endl;
  oss << "\\pgfusepath{stroke}" << endl;
  content_.push_back(oss.str());
  contentStarted_ = true;
}
 
void PgfGraphicDevice::drawRect(double x, double y, double width, double height, short fill)
{
  ostringstream oss;
  oss << "\\pgfpathrectangle{\\pgfpointxy{" << x << "}{" << y << "}}{\\pgfpointxy{" << width << "}{" << height << "}}" << endl;
  if(fill == FILL_FILLED)
    oss << "\\pgfusepath{stroke,fill}" << endl;
  else
    oss << "\\pgfusepath{stroke}" << endl;
  content_.push_back(oss.str());
  contentStarted_ = true;
}

void PgfGraphicDevice::drawCircle(double x, double y, double radius, short fill)
{
  ostringstream oss;
  oss << "\\pgfpathcircle{\\pgfpointxy{" << x << "}{" << y << "}}{" << radius << "}" << endl;
  if(fill == FILL_FILLED)
    oss << "\\pgfusepath{stroke,fill}" << endl;
  else
    oss << "\\pgfusepath{stroke}" << endl;
  content_.push_back(oss.str());
  contentStarted_ = true;
}

void PgfGraphicDevice::drawText(double x, double y, const std::string& text, short hpos, short vpos, double angle)
{
  string anchor;
  if(vpos == TEXT_VERTICAL_BOTTOM)
    anchor = "bottom";
  else if(vpos == TEXT_VERTICAL_TOP)
    anchor = "top";
  else if(vpos == TEXT_VERTICAL_CENTER)
    anchor = "base";
  else throw UnvalidFlagException("PgfGraphicDevice::drawText. Invalid vertical alignment option.");
  if(hpos == TEXT_HORIZONTAL_LEFT)
    anchor += ",left";
  else if(hpos == TEXT_HORIZONTAL_RIGHT)
    anchor += ",right";
  else if(hpos == TEXT_HORIZONTAL_CENTER)
    anchor += "";
  else throw UnvalidFlagException("PgfGraphicDevice::drawText. Invalid horizontal alignment option.");

  ostringstream oss;
  oss << "\\pgftransformrotate{" << angle << "}" << endl;
  oss << "\\pgftext[" << anchor << ",at=\\pgfpointxy{" << x << "}{" << y << "}]{\\textcolor{" << fgColorStr_ << "}" << text << "}" << endl;
  content_.push_back(oss.str());
  contentStarted_ = true;
}


// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "ColorTools.h"

using namespace std;
using namespace bpp;

const RGBColor ColorTools::RED    = RGBColor(255, 0, 0);
const RGBColor ColorTools::GREEN  = RGBColor(0, 255, 0);
const RGBColor ColorTools::BLUE   = RGBColor(0, 0, 255);
const RGBColor ColorTools::BLACK  = RGBColor(0, 0, 0);
const RGBColor ColorTools::WHITE  = RGBColor(255, 255, 255);
const RGBColor ColorTools::YELLOW = RGBColor(255, 255, 0);
const RGBColor ColorTools::CYAN   = RGBColor(0, 255, 255);
const RGBColor ColorTools::MAGENTA = RGBColor(255, 0, 255);
const RGBColor ColorTools::ORANGE = RGBColor(255, 127, 0);

/******************************************************************************/

std::vector<RGBColor> ColorTools::gradient(unsigned int n, const RGBColor& low, const RGBColor& high)
{
  vector<RGBColor> colors(n);
  for (unsigned int i = 0; i < n - 1; ++i)
  {
    colors[i][0] = low[0] + static_cast<unsigned int>(
      floor(
          static_cast<double>(i) * static_cast<double>(static_cast<int>(high[0]) - static_cast<int>(low[0]))
          / static_cast<double>(n - 1)
          )
      );
    colors[i][1] = low[1] + static_cast<unsigned int>(
      floor(
          static_cast<double>(i) * static_cast<double>(static_cast<int>(high[1]) - static_cast<int>(low[1]))
          / static_cast<double>(n - 1)
          )
      );
    colors[i][2] = low[2] + static_cast<unsigned int>(
      floor(
          static_cast<double>(i) * static_cast<double>(static_cast<int>(high[2]) - static_cast<int>(low[2]))
          / static_cast<double>(n - 1)
          )
      );
  }
  colors[n - 1] = high;
  return colors;
}

/******************************************************************************/

std::vector<RGBColor> ColorTools::gradient(unsigned int n, const RGBColor& low, const RGBColor& mid, const RGBColor& high)
{
  unsigned int lower = n / 2;
  unsigned int upper = n - lower;
  vector<RGBColor> colors1 = gradient(lower, low, mid);
  vector<RGBColor> colors2 = gradient(upper + 1, mid, high);
  for (size_t i = 1; i < colors2.size(); i++)
  {
    colors1.push_back(colors2[i]);
  }
  return colors1;
}

/******************************************************************************/

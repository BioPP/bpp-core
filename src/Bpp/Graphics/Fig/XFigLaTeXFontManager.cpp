// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "XFigLaTeXFontManager.h"

using namespace bpp;

XFigLaTeXFontManager::XFigLaTeXFontManager()
{
  // Add "official" font codes, from 0 to 5:
  registerFont_(Font("Default", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12), 0);
  registerFont_(Font("Roman", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12), 1);
  registerFont_(Font("Bold", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12), 2);
  registerFont_(Font("Italic", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12), 3);
  registerFont_(Font("Sans Serif", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12), 4);
  registerFont_(Font("Typewriter", Font::STYLE_NORMAL, Font::WEIGHT_NORMAL, 12), 5);
}

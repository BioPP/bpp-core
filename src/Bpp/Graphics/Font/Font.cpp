// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "Font.h"

using namespace bpp;

const short int Font::STYLE_NORMAL  = 0;
const short int Font::STYLE_ITALIC  = 1;

const short int Font::WEIGHT_NORMAL = 0;
const short int Font::WEIGHT_BOLD   = 1;

void Font::init_()
{
  styleDesc_[STYLE_NORMAL] = "normal";
  styleDesc_[STYLE_ITALIC] = "italic";
  weightDesc_[WEIGHT_NORMAL] = "normal";
  weightDesc_[WEIGHT_BOLD] = "bold";
}

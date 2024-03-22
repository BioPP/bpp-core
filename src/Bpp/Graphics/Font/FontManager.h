// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_FONT_FONTMANAGER_H
#define BPP_GRAPHICS_FONT_FONTMANAGER_H


#include "Font.h"

// From the STL:
#include <vector>

#include "../../Text/TextTools.h"
#include "../../Exceptions.h"

namespace bpp
{
/**
 * @brief Associate special fonts to a code.
 *
 * Instances of this interface are used in some vector format.
 */
template<class CodeType>
class FontManager
{
public:
  FontManager() {}
  virtual ~FontManager() {}

public:
  /**
   * @param font The font to look for.
   * @return The code associated to a given font.
   */
  virtual CodeType getCode(const Font& font) const = 0;

  /**
   * @param code The code to look for.
   * @return The font associated to a given code.
   * @throw exception if the code is not valid.
   */
  virtual const Font& getFont(CodeType& code) const = 0;

  /**
   * @return All valid codes.
   */
  virtual std::vector<CodeType> getCodes() const = 0;

  /**
   * @return All available fonts.
   */
  virtual std::vector<Font> getFonts() const = 0;

  /**
   * @return The total number of fonts available.
   */
  virtual size_t getNumberOfFonts() const = 0;
};


template<class CodeType>
class AbstractFontManager :
  public virtual FontManager<CodeType>
{
private:
  std::vector<Font> fonts_;
  std::vector<CodeType> codes_;

public:
  AbstractFontManager() : fonts_(), codes_() {}

public:
  CodeType getCode(const Font& font) const
  {
    for (unsigned int i = 0; i < fonts_.size(); i++)
    {
      if (fonts_[i] == font)
      {
        return codes_[i];
      }
    }
    throw Exception("AbstractFontManager::getCode. Unknown font: " + font.toString());
  }

  const Font& getFont(int& code) const
  {
    for (unsigned int i = 0; i < codes_.size(); i++)
    {
      if (codes_[i] == code)
      {
        return fonts_[i];
      }
    }
    throw Exception("AbstractFontManager::getColor. No font associated to this code: " + TextTools::toString(code));
  }
  std::vector<CodeType> getCodes() const { return codes_; }
  std::vector<Font> getFonts() const { return fonts_; }
  size_t getNumberOfFonts() const { return fonts_.size(); }

protected:
  void registerFont_(const Font& font, int code)
  {
    codes_.push_back(code);
    fonts_.push_back(font);
  }
};
} // end of namespace.
#endif // BPP_GRAPHICS_FONT_FONTMANAGER_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPHICS_FONT_FONT_H
#define BPP_GRAPHICS_FONT_FONT_H


#include "../../Clonable.h"
#include "../../Text/TextTools.h"

// From the STL:
#include <map>
#include <string>

namespace bpp
{
/**
 * @brief Data structure for fonts.
 */
class Font :
  public virtual Clonable
{
private:
  std::string family_;
  short int style_;
  short int weight_;
  unsigned int size_;
  mutable std::map<short int, std::string> styleDesc_;
  mutable std::map<short int, std::string> weightDesc_;

public:
  Font(const std::string& family = "Default", short int style = STYLE_NORMAL, short int weight = WEIGHT_NORMAL, unsigned int size = 12) :
    family_(family), style_(style), weight_(weight), size_(size), styleDesc_(), weightDesc_()
  {
    init_();
  }

  virtual ~Font() {}

  Font* clone() const { return new Font(*this); }

private:
  void init_();

public:
  bool operator==(const Font& font) const
  {
    return family_ == font.family_
           &&  style_ == font.style_
           && weight_ == font.weight_;
  }

  /**
   * @return The family component of this font.
   */
  const std::string& getFamily() const { return family_; }

  /**
   * @return The style component of this font.
   */
  short int getStyle() const { return style_; }

  /**
   * @brief Alias function for getStyle.
   * @return The shape component of this font.
   */
  short int getShape() const { return style_; }

  /**
   * @return The weight component of this font.
   */
  short int getWeight() const { return weight_; }

  /**
   * @brief Alias function for getWeight
   * @return The series component of this font.
   */
  short int getSeries() const { return weight_; }

  /**
   * @return The size component of this font.
   */
  const unsigned int& getSize() const { return size_; }

  /**
   * @param family The family component of this font.
   */
  void setFamily(const std::string& family) { family_ = family; }

  /**
   * @param style The style component of this font.
   */
  void setStyle(short int style) { style_ = style; }

  /**
   * @brief Alias function for setStyle.
   * @param shape The shape component of this font.
   */
  void setShape(short int shape) { style_ = shape; }


  /**
   * @param weight The weight component of this font.
   */
  void setWeight(short int weight) { weight_ = weight; }

  /**
   * @brief Alias function for setWeight.
   * @param series The series component of this font.
   */
  void setSeries(short int series) { weight_ = series; }

  /**
   * @param size The size component of this font.
   */
  void setSize(unsigned int size) { size_ = size; }

  /**
   * @return A text description of this font (family type size).
   */
  std::string toString() const
  {
    return family_ + "/" + styleDesc_[style_] + "/" + weightDesc_[weight_] + "/" + TextTools::toString(size_);
  }

public:
  static const short int STYLE_NORMAL;
  static const short int STYLE_ITALIC;

  static const short int WEIGHT_NORMAL;
  static const short int WEIGHT_BOLD;
};
} // end of namespace bpp.
#endif // BPP_GRAPHICS_FONT_FONT_H

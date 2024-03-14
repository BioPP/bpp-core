// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_CLONABLE_H
#define BPP_CLONABLE_H



/**
 * @mainpage
 *
 * @par
 * This library contains Bio++ core interfaces, classes and functions.
 * It provides a general interface called bpp::Clonable, which allows dynamic copy of objects.
 * Wrapper classes for numbers, strings and vectors that implement this interface are provided, and called
 * respectively bpp::Number, bpp::BppString and bpp::BppVector.
 *
 * @par
 * The bpp::Exception class is the most general exception class used in Bio++, and provides only a single text information.
 * Several more specialized exception classes are also provided, like
 * - bpp::IOException for input/output errors, usually when reading from or writing to files,
 * - bpp::NullPointerException when a NULL pointer is unexpectedly provided,
 * - bpp::BadNumberException and bpp::BadIntegerException, dealing with number problems (bounds, format, etc),
 * - and more...
 *
 * @par
 * This library also includes several submodules:
 * - The Text module contains: functions to deal with character strings (bpp::StringTokenizer and the bpp::TextTools static class),
 * like pattern matching, concatenation, number conversion, etc.
 * - The Graphics module provides support for graphical output. This includes the general bpp::GraphicDevice interface, with currently three implementations for XFig (bpp::XFigGraphicDevice), SVG (bpp::SVGGraphicDevice) and PGF (bpp::PGFGraphicDevice) formats.
 * Classes to deal with colors and fonts were created (see bpp::RGBColor, bpp::ColorSet, bpp::ColorTools and bpp::Font).
 * - The App module contains classes and tools that provide powerful methods to parse command-line options and more.
 * - The Numeric module is quite large and provides interfaces, classes and functions for numerical calculus.
 *   The tools included are general enough to be useful beyond biology.
 *   The originality of this module, unless many existing libraries, is to be fully object-oriented.
 *   Available methods include:
 *   - Vector operations, based on the stl::vector class, see bpp::VectorTools,
 *   - Matrix data and operations, see the bpp::Matrix interface and bpp::MatrixTools class,
 *   - Functions are implemented using several general classes:
 *     - the bpp::Function interface and its derivatives,
 *     - the bpp::Parameter, bpp::Constraint and bpp::ParameterList classes, and the bpp::Parametrizable interface
 *       provide a very general way to deal with parameters
 *     - Function operations, see the bpp::Optimizer interface, and the bpp::AbstractNumericalDerivative class for
 *       numerical differenciation,
 *     - Discrete probability distributions are implemented via the bpp::DiscreteDistribution interface
 *     - Several random numbers generators are available through the bpp::RandomNumberFactory interface, and the
 *       bpp::RandomTools static class provides useful methods to deal with random numbers.
 *
 *
 *
 */

namespace bpp
{
/** @brief The Clonable interface (allow an object to be cloned).
 *
 * A clone is a deep (or hard) copy of an object.
 * This interface provides a method that dynamically creates a copy of itself
 * and send a pointer toward it.
 *
 * This method allows an object to be copied when you do not now its class.
 */
class Clonable
{
public:
  virtual ~Clonable() = default;

  /** @brief Create a copy of this object and send a pointer to it.
   * @return A pointer toward the copy object.
   */
  virtual Clonable* clone() const = 0;
};
} // namespace bpp
#endif // BPP_CLONABLE_H

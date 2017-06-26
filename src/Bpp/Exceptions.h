//
// File: Exceptions.h
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Sylvain Gaillard
//   Francois Gindraud (2017)
// Last modified: 2017-06-26
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide utilitary
  classes. This file belongs to the Bio++ Project.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_EXCEPTIONS_H
#define BPP_EXCEPTIONS_H

#include <array>
#include <stdexcept>
#include <string>

namespace bpp
{
  /** @brief Exception base class.
   * Overload exception constructor (to control the exceptions mechanism).
   * Destructor is already virtual (from std::exception)
   */
  class Exception : public std::exception
  {
  private:
    std::string message_;

  public:
    /** @brief Build a new Exception.
     * @param text A message to be passed to the exception hierarchy.
     */
    Exception(std::string text);
    /** @brief Method to get the message of the exception (STL method redefinition).
     * @return The message passed to the exception hierarchy.
     */
    const char* what() const noexcept override;
    /// Access the message as a std::string.
    const std::string& message() const noexcept;
  };

  /// The base class exception for IO error.
  class IOException : public Exception
  {
  public:
    /** @brief Build a new IOException.
     * @param text A message to be passed to the exception hierarchy.
     */
    IOException(std::string text);
  };

  /** @brief The base class exception for NULL pointer error.
   * This exception may be thrown when an unexpected NULL pointer is found.
   */
  class NullPointerException : public Exception
  {
  public:
    /** @brief Build a new NullPointerException.
     * @param text A message to be passed to the exception hierarchy.
     */
    NullPointerException(std::string text);
  };

  /// The base class exception for zero division error.
  class ZeroDivisionException : public Exception
  {
  public:
    /** @brief Build a new ZeroDivisionException.
     * @param text A message to be passed to the exception hierarchy.
     */
    ZeroDivisionException(std::string text);
  };

  /// Number exception: integers.
  class BadIntegerException : public Exception
  {
  private:
    int badInt_;

  public:
    /** @brief Build a new BadIntegerException.
     * @param text   A message to be passed to the exception hierarchy.
     * @param badInt The faulty integer.
     */
    BadIntegerException(std::string text, int badInt);
    /** @brief Get the integer that threw this exception.
     * @return The faulty integer.
     */
    int getBadInteger() const;
  };

  /// Number exception: doubles.
  class BadNumberException : public Exception
  {
  private:
    double badNumber_;

  public:
    /** @brief Build a new BadNumberException.
     * @param text      A message to be passed to the exception hierarchy.
     * @param badNumber The faulty number.
     */
    BadNumberException(std::string text, double badNumber);
    /** @brief Get the number that threw this exception.
     * @return The faulty number.
     */
    double getBadNumber() const;
  };

  /// Number format exception.
  class NumberFormatException : public Exception
  {
  private:
    std::string badNumber_;

  public:
    /** @brief Build a new NumberFormatException.
     * @param text      A message to be passed to the exception hierarchy.
     * @param badNumber The faulty number.
     */
    NumberFormatException(std::string text, std::string badNumber);
    /** @brief Get the number that threw this exception.
     * @return The faulty number.
     */
    const std::string& getBadNumber() const;
  };

  /// Index out of bounds exception class.
  class IndexOutOfBoundsException : public Exception
  {
  private:
    std::size_t badIndex_;
    std::array<std::size_t, 2> bounds_;

  public:
    /** @brief Build a new IndexOutOfBoundsException.
     * @param text   A message to be passed to the exception hierarchy.
     * @param badInt The faulty integer.
     * @param lowerBound Lower limit.
     * @param upperBound Upper limit.
     */
    IndexOutOfBoundsException(std::string text, std::size_t badInt, std::size_t lowerBound, std::size_t upperBound);
    /// Get the bounds.
    const std::array<std::size_t, 2>& getBounds() const;
    std::size_t getBadIndex() const;
  };

  /// Wrong size exception class.
  class BadSizeException : public Exception
  {
  private:
    std::size_t badSize_;
    std::size_t correctSize_;

  public:
    /** @brief Build a new BadSizeException.
     * @param text   A message to be passed to the exception hierarchy.
     * @param badSize The faulty size.
     * @param correctSize The expected size.
     */
    BadSizeException(std::string text, std::size_t badSize, std::size_t correctSize);

    std::size_t getBadSize() const;
    std::size_t getCorrectSize() const;
  };

  /// Out of range exception class.
  class OutOfRangeException : public Exception
  {
  private:
    double badValue_;
    std::array<double, 2> bounds_;

  public:
    /** @brief Build a new OutOfRangeException.
     * @param text   A message to be passed to the exception hierarchy.
     * @param badValue The faulty value.
     * @param lowerBound Lower limit.
     * @param upperBound Upper limit.
     */
    OutOfRangeException(std::string text, double badValue, double lowerBound, double upperBound);

    double getBadValue () const;
    double getLowerBound() const;
    double getUpperBound() const;
  };

  /// This exception is sent when a given method is not implemented.
  class NotImplementedException : public Exception
  {
  public:
    /** @brief Build a new NotImplementedException.
     * @param text A message to be passed to the exception hierarchy.
     */
    NotImplementedException(std::string text);
  };
} // namespace bpp
#endif // BPP_EXCEPTIONS_H

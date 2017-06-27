//
// File: Exceptions.cpp
// Authors:
//   Guillaume Deuchst
//   Julien Dutheil
//   Sylvain Gaillard
//   Francois Gindraud (2017)
// Created: 2017-03-28 00:00:00
// Last modified: 2017-06-27
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

#include <string>
#include <utility>

#include "Exceptions.h"

namespace bpp
{
  Exception::Exception(std::string text)
    : message_(std::move(text))
  {
  }
  const char* Exception::what() const noexcept { return message_.c_str(); }
  const std::string& Exception::message() const noexcept { return message_; }

  IOException::IOException(std::string text)
    : Exception(std::move(text))
  {
  }

  NullPointerException::NullPointerException(std::string text)
    : Exception(std::move(text))
  {
  }

  ZeroDivisionException::ZeroDivisionException(std::string text)
    : Exception(std::move(text))
  {
  }

  BadIntegerException::BadIntegerException(std::string text, int badInt)
    : Exception(std::move(text) + '(' + std::to_string(badInt) + ')')
    , badInt_(badInt)
  {
  }
  int BadIntegerException::getBadInteger() const { return badInt_; }

  BadNumberException::BadNumberException(std::string text, double badNumber)
    : Exception(std::move(text) + '(' + std::to_string(badNumber) + ')')
    , badNumber_(badNumber)
  {
  }
  double BadNumberException::getBadNumber() const { return badNumber_; }

  NumberFormatException::NumberFormatException(std::string text, std::string badNumber)
    : Exception(std::move(text) + '(' + std::move(badNumber) + ')')
    , badNumber_(badNumber)
  {
  }
  const std::string& NumberFormatException::getBadNumber() const { return badNumber_; }

  IndexOutOfBoundsException::IndexOutOfBoundsException(std::string text, std::size_t badInt, std::size_t lowerBound,
                                                       std::size_t upperBound)
    : Exception("out of [" + std::to_string(lowerBound) + ", " + std::to_string(upperBound) + "])" + std::move(text))
    , badIndex_(badInt)
    , bounds_{{lowerBound, upperBound}}
  {
  }
  const std::array<std::size_t, 2>& IndexOutOfBoundsException::getBounds() const { return bounds_; }
  std::size_t IndexOutOfBoundsException::getBadIndex() const { return badIndex_; }

  BadSizeException::BadSizeException(std::string text, std::size_t badSize, std::size_t correctSize)
    : Exception("Incorrect size " + std::to_string(badSize) + ", expected " + std::to_string(correctSize) + ". " +
                std::move(text))
    , badSize_(badSize)
    , correctSize_(correctSize)
  {
  }
  std::size_t BadSizeException::getBadSize() const { return badSize_; }
  std::size_t BadSizeException::getCorrectSize() const { return correctSize_; }

  OutOfRangeException::OutOfRangeException(std::string text, double badValue, double lowerBound, double upperBound)
    : Exception(std::to_string(badValue) + " out of [" + std::to_string(lowerBound) + ", " +
                std::to_string(upperBound) + "])" + std::move(text))
    , badValue_(badValue)
    , bounds_{{lowerBound, upperBound}}
  {
  }
  double OutOfRangeException::getBadValue() const { return badValue_; }
  double OutOfRangeException::getLowerBound() const { return bounds_[0]; }
  double OutOfRangeException::getUpperBound() const { return bounds_[1]; }

  NotImplementedException::NotImplementedException(std::string text)
    : Exception(std::move(text))
  {
  }
} // namespace bpp

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_NUMBER_H
#define BPP_NUMERIC_NUMBER_H

#include <string>

#include "../Clonable.h"
#include "../Text/TextTools.h"

namespace bpp
{
/**
 * @brief The Number interface.
 *
 * This template class may be used to deal with number in an object way.
 */
class BppNumberI : public Clonable
{
public:
  BppNumberI() {}

  virtual ~BppNumberI() {}

public:
  virtual BppNumberI* clone() const = 0;

public:
  virtual std::string toString() const = 0;
};


class BppNotANumber : public virtual BppNumberI
{
public:
  BppNotANumber() {}

  virtual ~BppNotANumber() {}

public:
  virtual BppNotANumber* clone() const { return new BppNotANumber(); }

public:
  virtual std::string toString() const { return "NaN"; }
};


/**
 * @brief The Number object template class.
 *
 * This template class may be used to deal with number in an object way.
 */
template<class T> class Number : public virtual BppNumberI
{
protected:
  /** @brief The value of this parameter. */
  T value_;

public:
  /**
   * @brief Build a new Number object with a specific value.
   *
   * @param value The value that the Number must have.
   */
  Number(const T& value = 0) : value_(value) {}

  virtual ~Number() {}

  Number<T>& operator=(const T& t)
  {
    value_ = t;
    return *this;
  }

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  Number<T>* clone() const { return new Number<T>(value_); }
  /** @} */

public:
  /**
   * @brief Get the value of this number.
   *
   * @return The value of this number.
   */
  T getValue() const { return value_; }

  std::string toString() const { return TextTools::toString(value_); }
};

/**
 * @brief An object wrapper for double values.
 */
class BppDouble : public virtual Number<double>
{
public:
  /**
   * @brief Build a new BppDouble number object with a specific value.
   *
   * @param value The value that the Number must have.
   */
  BppDouble(double value = 0) : Number<double>(value) {}

  virtual ~BppDouble() {}

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  BppDouble* clone() const { return new BppDouble(*this); }
  /** @} */
};

/**
 * @brief An object wrapper for integer values.
 */
class BppInteger : public virtual Number<int>
{
public:
  /**
   * @brief Build a new BppInteger number object with a specific value.
   *
   * @param value The value that the Number must have.
   */
  BppInteger(int value = 0) : Number<int>(value) {}

  virtual ~BppInteger() {}

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  BppInteger* clone() const { return new BppInteger(*this); }
  /** @} */
};

/**
 * @brief An object wrapper for unsigned integer values.
 */
class BppUnsignedInteger : public virtual Number<unsigned int>
{
public:
  /**
   * @brief Build a new BppUnsignedInteger number object with a specific value.
   *
   * @param value The value that the Number must have.
   */
  BppUnsignedInteger(unsigned int value = 0) : Number<unsigned int>(value) {}

  virtual ~BppUnsignedInteger() {}

public:
  /**
   * @name The Clonable interface.
   *
   * @{
   */
  BppUnsignedInteger* clone() const { return new BppUnsignedInteger(*this); }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_NUMBER_H

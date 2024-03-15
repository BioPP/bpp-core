// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PARAMETEREXCEPTIONS_H
#define BPP_NUMERIC_PARAMETEREXCEPTIONS_H


// From Utils:
#include "../Exceptions.h"

// From the STL:
#include <string>

namespace bpp
{
class Parameter;

/**
 * @brief The parameter exception base class.
 *
 * @see Exception
 */
class ParameterException :
  public Exception
{
private:
  const Parameter* parameter_;

public:
  // Class constructors and destructor:
  /**
   * @brief Build a new ParameterException object.
   *
   * @param text A message to be passed to the exception hierarchy.
   * @param param A const pointer toward the parameter that threw the exception.
   */
  ParameterException(const std::string& text, const Parameter* param);

  ParameterException(const ParameterException& pe) :
    Exception(pe),
    parameter_(pe.parameter_) {} // We explicitely do not want to make a hard copy of the pointer here.

  ParameterException& operator=(const ParameterException& pe)
  {
    Exception::operator=(pe);
    parameter_ = pe.parameter_;
    return *this;
  }

  virtual ~ParameterException() {}

public:
  /**
   * @brief Get the parameter that threw the exception.
   *
   * @return A const pointer toward the parameter.
   */
  virtual const Parameter* parameter() const;
};

/**
 * @brief Exception thrown when a value do not match a given constraint.
 *
 * @see Constraint
 */
class ConstraintException : public ParameterException
{
private:
  double badValue_;

public:
  // Class constructor and destructor:
  /**
   * @brief Build a new ConstraintException object.
   *
   * @param text     A message to be passed to the exception hierarchy.
   * @param param    A const pointer toward the parameter that threw the exception.
   * @param badValue The value that doesn't match the constraint.
   */
  ConstraintException(const std::string& text, const Parameter* param, double badValue);

  virtual ~ConstraintException() {}

public:
  /**
   * @brief Get the value that doesn't match the constraint.
   *
   * @return The faulty value.
   */
  virtual double getBadValue() const;
};

/*******************************************************************************/

/**
 * @brief Exception thrown when a parameter is not found,
 * for instance in a ParameterList.
 */
class ParameterNotFoundException : public Exception
{
private:
  const std::string parameter_;

public:
  // Class constructors and destructor:
  /**
   * @brief Build a new ParameterNotFoundException object.
   *
   * @param text     A message to be passed to the exception hierarchy.
   * @param param    The name of the parameter not found.
   */
  ParameterNotFoundException(const std::string& text, const std::string& param = "");

  virtual ~ParameterNotFoundException() {}

public:
  /**
   * @brief Get the name of the parameter not found.
   *
   * @return The parameter name.
   */
  virtual std::string parameter() const;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PARAMETEREXCEPTIONS_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PARAMETRIZABLE_H
#define BPP_NUMERIC_PARAMETRIZABLE_H


// From Utils:
#include "../Clonable.h"

// From the STL:
#include <string>

#include "ParameterList.h"

namespace bpp
{
/**
 * @brief This is the interface for all objects that imply parameters.
 *
 * @see Parameter, ParameterList
 */
class Parametrizable :
  public virtual Clonable
{
public:
  Parametrizable() {}
  virtual ~Parametrizable() {}

public:
  /**
   * @brief Tell if there is a parameter with specified name.
   *
   * @param name The name of the parameter to look for.
   * @return y/n.
   */
  virtual bool hasParameter(const std::string& name) const = 0;

  /**
   * @brief Get all parameters available.
   *
   * @see getIndependentParameters if some parameters are aliased.
   * @return A list with all parameters available.
   */
  virtual const ParameterList& getParameters() const = 0;

  /**
   * @brief Get the parameter with specified name.
   *
   * @param name The name of the parameter to look for.
   * @return The parameter with given name.
   * @throw ParameterNotFoundException if no parameter with this name is found.
   */
  virtual const Parameter& parameter(const std::string& name) const = 0;

  /**
   * @brief Get the value for parameter of name 'name'.
   *
   * @param name The name of the parameter.
   * @return the value of parameter <i>name</i>.
   */
  virtual double getParameterValue(const std::string& name) const
  = 0;

  /**
   * @brief Set the parameters values to be equals to those of <i>parameters</i>.
   *
   * The list must contain exactly the same parameters (ie same names)
   * than the parameters available.
   *
   * @param parameters A list with all parameters.
   * @throw ParameterNotFoundException If a some parameter in the list is not in <i>params</i>.
   * @throw ConstraintException If a value in <i>parameters</i> does not match the constraint in the
   * corresponding parameter in the list.
   */
  virtual void setAllParametersValues(const ParameterList& parameters) = 0;

  /**
   * @brief Set the value of parameter with name <i>name</i> to be equal to <i>value</i>.
   *
   * @param name the name of the parameter to set.
   * @param value The value of the parameter.
   * @throw ParameterNotFoundException If no parameter in the list has the name <i>name</i>.
   * @throw ConstraintException If <i>value</i> does not match the constraint associated to
   * parameter <i>name</i>.
   */
  virtual void setParameterValue(const std::string& name, double value) = 0;

  /**
   * @brief Update the parameters from <i>parameters</i>.
   *
   * <i>parameters</i> must be a subset of all parameters available.
   *
   * @param parameters A list containing all parameters to update.
   * @throw ParameterNotFoundException If a some parameter in <i>params</i> is not in the list.
   * @throw ConstraintException If a value in <i>parameters</i> does not match the constraint in the
   * corresponding parameter in the list.
   */
  virtual void setParametersValues(const ParameterList& parameters) = 0;

  /**
   * @brief Update the parameters from <i>parameters</i>.
   *
   * Only common parameters with <i>parameters</i> will be updated.
   *
   * @param parameters A list of parameters.
   * @return True if at least one parameter value has been changed.
   * @throw ConstraintException If a value in <i>parameters</i> does not match the constraint in the
   * corresponding parameter in the list.
   */
  virtual bool matchParametersValues(const ParameterList& parameters) = 0;

  /**
   * @brief Remove the constraint associated with one parameter, if any.
   *
   * @param name The name of the parameter to look for.
   * @throw ParameterNotFoundException if no parameter with this name is found.
   */
  virtual void removeConstraint(const std::string& name) = 0;

  /**
   * @brief Set/Change the constraint associated with one parameter.
   *
   * @param name The name of the parameter to look for.
   * @param constraint A pointer to the constraint (may be null)
   * @throw ParameterNotFoundException if no parameter with this name is found.
   */
  virtual void setConstraint(const std::string& name, std::shared_ptr<ConstraintInterface> constraint) = 0;

  /**
   * @brief Get the number of parameters.
   *
   * @see getNumberOfIndependentParameters If some parameters are aliased.
   * @return The number of parameters.
   */
  virtual size_t getNumberOfParameters() const = 0;

  /**
   * @brief Set the namespace for the parameter names.
   *
   * @param prefix The 'namespace', that is a prefix to add to all parameter names.
   * If parameter names are already prefixed, the new prefix will be used instead.
   */
  virtual void setNamespace(const std::string& prefix) = 0;

  /**
   * @return The current namespace used. This is an empty string if no namespace is currently defined.
   */
  virtual std::string getNamespace() const = 0;

  /**
   * @brief Resolves a parameter name according to the current namespace.
   *
   * @return The parameter name without the namespace prefix, if any.
   */
  virtual std::string getParameterNameWithoutNamespace(const std::string& name) const = 0;

protected:
  /**
   * @brief Get all parameters available.
   *
   * @see getIndependentParameters if some parameters are aliased.
   * @return A list with all parameters available.
   */

  virtual ParameterList& getParameters_() = 0;
};

/**
 * @brief A low-level implementation of the Parametrizable interface with void functions.
 *
 * @see Parameter, ParameterList, Parametrizable
 */
class ParametrizableAdapter :
  public virtual Parametrizable
{
protected:
  ParameterList parameters_;
  Parameter parameter_;

public:
  ParametrizableAdapter() : parameters_(), parameter_() {}
  virtual ~ParametrizableAdapter() {}

public:
  /**
   * @name The Parametrizable interface.
   *
   * @{
   */
  bool hasParameter(const std::string& name) const override { return parameters_.hasParameter(name); }
  const ParameterList& getParameters() const override { return parameters_; }
  const Parameter& parameter(const std::string& name) const override { return parameter_; }
  double getParameterValue(const std::string& name) const override { return 0; }
  void setAllParametersValues(const ParameterList& parameters) override {}
  void setParameterValue(const std::string& name, double value) override {}
  void setParametersValues(const ParameterList& parameters) override {}
  bool matchParametersValues(const ParameterList& parameters) override { return false; }
  void removeConstraint(const std::string& name) override {}
  void setConstraint(const std::string& name, std::shared_ptr<ConstraintInterface> constraint) override {}
  size_t getNumberOfParameters() const override { return 0; }
  void setNamespace(const std::string& prefix) override {}
  std::string getNamespace() const override { return ""; }
  std::string getParameterNameWithoutNamespace(const std::string& name) const override { return name; }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PARAMETRIZABLE_H

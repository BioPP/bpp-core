// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_ABSTRACTPARAMETRIZABLE_H
#define BPP_NUMERIC_ABSTRACTPARAMETRIZABLE_H


#include "Parametrizable.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief A partial implementation of the Parametrizable interface.
 *
 * Parameters are stored in a protected ParameterList object.
 *
 * The abstract fireParameterChanged() method is provided so that the derived class
 * know when a parameter has changed, and can be updated.
 * All methods call the corresponding method in ParameterList and then call the
 * fireParameterChanged() method.
 */
class AbstractParametrizable :
  public virtual Parametrizable
{
private:
  ParameterList parameters_;
  std::string prefix_;

public:
  AbstractParametrizable(const std::string& prefix) : parameters_(), prefix_(prefix) {}

  virtual ~AbstractParametrizable() {}

public:
  bool hasParameter(const std::string& name) const override { return parameters_.hasParameter(prefix_ + name); }

  const ParameterList& getParameters() const override { return parameters_; }

  const Parameter& parameter(const std::string& name) const override
  {
    return parameters_.parameter(prefix_ + name);
  }

  const std::shared_ptr<Parameter>& getParameter(const std::string& name) const
  {
    return parameters_.getParameter(prefix_ + name);
  }

  double getParameterValue(const std::string& name) const override
  {
    return parameter(name).getValue();
  }

  void setAllParametersValues(const ParameterList& parameters) override
  {
    parameters_.setAllParametersValues(parameters);
    fireParameterChanged(parameters);
  }

  void setParameterValue(const std::string& name, double value) override
  {
    parameters_.setParameterValue(prefix_ + name, value);
    fireParameterChanged(parameters_.createSubList(prefix_ + name));
  }

  void setParametersValues(const ParameterList& parameters) override
  {
    parameters_.setParametersValues(parameters);
    fireParameterChanged(parameters);
  }

  bool matchParametersValues(const ParameterList& parameters) override
  {
    std::unique_ptr< std::vector<size_t> > updatedParameters(new std::vector<size_t>());
    bool test = parameters_.matchParametersValues(parameters, updatedParameters.get());
    if (test)
      fireParameterChanged(parameters.shareSubList(*updatedParameters));
    return test;
  }

  void removeConstraint(const std::string& name) override
  {
    getParameter_(name).removeConstraint();
  }

  void setConstraint(const std::string& name, std::shared_ptr<ConstraintInterface> constraint) override
  {
    getParameter_(name).setConstraint(constraint);
  }

  size_t getNumberOfParameters() const override { return parameters_.size(); }

  void setNamespace(const std::string& prefix) override;

  std::string getNamespace() const override { return prefix_; }

  std::string getParameterNameWithoutNamespace(const std::string& name) const override;

  /**
   * @brief Notify the class when one or several parameters have changed.
   *
   * @param parameters A ParameterList object with parameters that changed.
   */
  virtual void fireParameterChanged(const ParameterList& parameters) {}

protected:
  virtual void addParameter_(Parameter* parameter)
  {
    if (parameter)
      parameters_.addParameter(parameter);
  }

  virtual void addParameters_(const ParameterList& parameters)
  {
    parameters_.addParameters(parameters);
  }

  virtual void shareParameter_(const std::shared_ptr<Parameter>& parameter)
  {
    parameters_.shareParameter(parameter);
  }

  virtual void shareParameters_(const ParameterList& parameters)
  {
    parameters_.shareParameters(parameters);
  }

  virtual void includeParameters_(const ParameterList& parameters)
  {
    parameters_.includeParameters(parameters);
  }

  virtual void deleteParameter_(size_t index)
  {
    if (index >= parameters_.size())
      throw IndexOutOfBoundsException("AbstractParametrizable::deleteParameter_.", index, 0, parameters_.size() - 1);
    parameters_.deleteParameter(index);
  }

  virtual void deleteParameter_(std::string& name)
  {
    parameters_.deleteParameter(name);
  }

  virtual void deleteParameters_(const std::vector<std::string>& names)
  {
    parameters_.deleteParameters(names);
  }

  void resetParameters_()
  {
    parameters_.reset();
  }

  /**
   * @param name The name of the parameter.
   * @return A reference toward the corresponding parameter.
   * @throw ParameterNotFoundException If no parameter with that name is found in the list.
   */
  Parameter& getParameter_(const std::string& name)
  {
    if (!hasParameter(name))
      throw ParameterNotFoundException("AbstractParametrizable::getParameter_().", prefix_ + name);
    return parameters_.parameter(prefix_ + name);
  }

  /**
   * @param name The name of the parameter, including its namespace.
   * @return A reference toward the corresponding parameter.
   * @throw ParameterNotFoundException If no parameter with that name is found in the list.
   */
  Parameter& getParameterWithNamespace_(const std::string& name)
  {
    return getParameter_(name);
  }
  /**
   * @param name The name of the parameter, including its namespace.
   * @return A reference toward the corresponding parameter.
   * @throw ParameterNotFoundException If no parameter with that name is found in the list.
   */
  const Parameter& getParameterWithNamespace_(const std::string& name) const
  {
    return parameter(name);
  }

  Parameter& getParameter_(size_t index)
  {
    if (index >= parameters_.size())
      throw IndexOutOfBoundsException("AbstractParametrizable::getParameter_.", index, 0, parameters_.size() - 1);
    return parameters_[index];
  }

  const Parameter& getParameter_(size_t index) const
  {
    if (index >= parameters_.size())
      throw IndexOutOfBoundsException("AbstractParametrizable::getParameter_.", index, 0, parameters_.size() - 1);
    return parameters_[index];
  }


  ParameterList& getParameters_() override { return parameters_; }

  /**
   * @return The shared_ptr parameter at a given position.
   * @warning No check is performed on the validity of the index given as input!
   */
  const std::shared_ptr<Parameter>& getParameter(size_t i) const
  {
    return parameters_.getParameter(i);
  }

  std::shared_ptr<Parameter>& getParameter(size_t i)
  {
    return parameters_.getParameter(i);
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_ABSTRACTPARAMETRIZABLE_H

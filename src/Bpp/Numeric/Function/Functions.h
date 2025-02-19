// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_FUNCTIONS_H
#define BPP_NUMERIC_FUNCTION_FUNCTIONS_H


#include "../AbstractParametrizable.h"
#include "../ParameterExceptions.h"
#include "../ParameterList.h"
#include "../Parametrizable.h"

// From Utils:
#include "../../Clonable.h"
#include "../../Exceptions.h"

// From the STL:
#include <cmath>
#include <memory>

namespace bpp
{
/**
 * @brief This is the function abstract class.
 *
 * This class provides the interface for function objet
 * and a default implementation of the f() function.
 *
 * The f() function sends the value of the function according to a
 * given set of parameters.
 *
 * However for complexe function like likelihood for instance,
 * computing the function value takes some time, and one do not want
 * to perform several times the computation for an identical set of
 * parameters.
 * The setParameters() method hence allows to set the parameter value
 * for which the function is to be computed, perform the computation
 * and store the results.
 * The getValue() methods send the result of the computation.
 * One may hence access to the result of the computation by calling the
 * getvalue() method without re-computating the function.
 * The f(parameters) function is a shortcut for
 * @code
 * setParameters(parameters);
 * return getValue();
 * @endcode
 * for convinience.
 *
 * @see Parameter, ParameterList
 */
class FunctionInterface :
  public virtual Parametrizable
{
public:
  FunctionInterface() {}
  virtual ~FunctionInterface() {}

public:
  /**
   * @brief Set the point where the function must be computed.
   *
   * @param parameters The parameter set to pass to the function.
   */
  virtual void setParameters(const ParameterList& parameters) = 0;

  /**
   * @brief Get the value of the function at the current point.
   *
   * @return The value of the function.
   * @throw Exception If no point is specified or if an error occured.
   */
  virtual double getValue() const = 0;

  /**
   * @brief Get the value of the function according to a given set of parameters.
   *
   * @param parameters The parameter set to pass to the function.
   * @return The value of the function with the given parameter set.
   * @throw Exception If an error occured.
   */
  virtual double f(const ParameterList& parameters)
  {
    setParameters(parameters);
    return getValue();
  }

  friend class FunctionWrapper;
};

/**
 * @brief This is the abstract class for first order derivable functions.
 *
 * This class adds the getFirstOrderDerivative() and df() shortcut functions.
 */
class FirstOrderDerivable :
  public virtual FunctionInterface
{
public:
  FirstOrderDerivable() {}
  virtual ~FirstOrderDerivable() {}

  FirstOrderDerivable* clone() const = 0;

public:
  /**
   * @brief Tell if derivatives must be computed.
   *
   * @param yn yes/no
   */
  virtual void enableFirstOrderDerivatives(bool yn) = 0;

  /**
   * @brief Tell if derivatives must be computed.
   *
   * @return yes/no
   */
  virtual bool enableFirstOrderDerivatives() const = 0;

  /**
   * @brief Get the derivative of the function at the current point.
   *
   * @param variable   The name of the @f$ x @f$ variable in @f$ \frac{df}{dx} @f$.
   * @return The value of the function.
   * @throw Exception If no point is specified or if an error occured.
   */
  virtual double getFirstOrderDerivative(const std::string& variable) const = 0;

  /**
   * @brief Get the value of the first derivative of the function
   * according to a given set of parameters.
   *
   * @param variable   The name of the @f$ x @f$ variable in @f$ \frac{df}{dx} @f$.
   * @param parameters The parameter set to pass to the function.
   * @return The value of the function with the given parameter set.
   * @throw Exception If an error occured.
   */
  virtual double df(const std::string& variable, const ParameterList& parameters)
  {
    setParameters(parameters);
    return getFirstOrderDerivative(variable);
  }
};

/**
 * @brief This is the abstract class for second order derivable functions.
 *
 * This class adds the getSecondOrderDerivative() and d2f() shortcut functions.
 * Cross derivative functions are also provided.
 */
class SecondOrderDerivable :
  public virtual FirstOrderDerivable
{
public:
  SecondOrderDerivable() {}
  virtual ~SecondOrderDerivable() {}

  SecondOrderDerivable* clone() const = 0;

public:
  /**
   * @brief Tell if derivatives must be computed.
   *
   * @param yn yes/no
   */
  virtual void enableSecondOrderDerivatives(bool yn) = 0;

  /**
   * @brief Tell if derivatives must be computed.
   *
   * @return yes/no
   */
  virtual bool enableSecondOrderDerivatives() const = 0;

  /**
   * @brief Get the second order derivative of the function at the current point.
   *
   * @param variable   The name of the @f$ x @f$ variable in @f$ \frac{\partial^2 f}{\partial x^2} @f$.
   * @return The value of the function.
   * @throw Exception If no point is specified or if an error occured.
   */
  virtual double getSecondOrderDerivative(const std::string& variable) const = 0;

  /**
   * @brief Get the value of the second order derivative of the function
   * according to a given set of parameters.
   *
   * @param variable   The name of the @f$ x @f$ variable in @f$ \frac{\partial^2 f}{\partial x^2} @f$.
   * @param parameters The parameter set to pass to the function.
   * @return The value of the function with the given parameter set.
   * @throw Exception If an error occured.
   */
  virtual double d2f(const std::string& variable, const ParameterList& parameters)
  {
    setParameters(parameters);
    return getSecondOrderDerivative(variable);
  }

  /**
   * @brief Get the value of the cross derivative of the function
   * according to a given set of parameters.
   *
   * @param variable1  The name of the @f$ x @f$ variable in @f$ \frac{\partial^2 f}{\partial x \partial y} @f$.
   * @param variable2  The name of the @f$ y @f$ variable in @f$ \frac{\partial^2 f}{\partial x \partial y} @f$.
   * @return The value of the function with the given parameter set.
   * @throw Exception If an error occured.
   */
  virtual double getSecondOrderDerivative(const std::string& variable1, const std::string& variable2) const = 0;

  /**
   * @brief Get the value of the cross derivative of the function
   * according to a given set of parameters.
   *
   * @param variable1  The name of the @f$ x @f$ variable in @f$ \frac{\partial^2 f}{\partial x \partial y} @f$.
   * @param variable2  The name of the @f$ y @f$ variable in @f$ \frac{\partial^2 f}{\partial x \partial y} @f$.
   * @param parameters The parameter set to pass to the function.
   * @return The value of the function with the given parameter set.
   * @throw Exception If an error occured.
   */
  virtual double d2f(const std::string& variable1, const std::string& variable2, const ParameterList& parameters)
  {
    setParameters(parameters);
    return getSecondOrderDerivative(variable1, variable2);
  }
};

/**
 * @brief General class that wraps a function into another one.
 * This class is meant to be derivated and just provides a general framework.
 */
class FunctionWrapper :
  public virtual FunctionInterface
{
protected:
  std::shared_ptr<FunctionInterface> function_;

public:
  FunctionWrapper(std::shared_ptr<FunctionInterface> function) : function_(function) {}
  FunctionWrapper(const FunctionWrapper& fw) : function_(fw.function_) {}
  FunctionWrapper& operator=(const FunctionWrapper& fw)
  {
    function_ = fw.function_;
    return *this;
  }

public:
  virtual const FunctionInterface& function() const
  {
    return *function_;
  }
  virtual FunctionInterface& function()
  {
    return *function_;
  }
  std::shared_ptr<const FunctionInterface> getFunction() const
  {
    return function_;
  }
  std::shared_ptr<FunctionInterface> getFunction()
  {
    return function_;
  }


  bool hasParameter(const std::string& name) const override
  {
    return function_->hasParameter(name);
  }

  void setParameters(const ParameterList& parameters) override
  {
    function_->setParameters(parameters);
  }

  const ParameterList& getParameters() const override
  {
    return function_->getParameters();
  }

  const Parameter& parameter(const std::string& name) const override
  {
    return function_->parameter(name);
  }

  double getValue() const override
  {
    return function_->getValue();
  }

  double f(const ParameterList& parameters) override
  {
    return function_->f(parameters);
  }

  double getParameterValue(const std::string& name) const override
  {
    return function_->getParameterValue(name);
  }

  void setAllParametersValues(const ParameterList& parameters) override
  {
    function_->setAllParametersValues(parameters);
  }

  void setParameterValue(const std::string& name, double value) override
  {
    function_->setParameterValue(name, value);
  }

  void setParametersValues(const ParameterList& parameters) override
  {
    function_->setParametersValues(parameters);
  }

  bool matchParametersValues(const ParameterList& parameters) override
  {
    return function_->matchParametersValues(parameters);
  }

  void removeConstraint(const std::string& name) override
  {
    return function_->removeConstraint(name);
  }

  void setConstraint(const std::string& name, std::shared_ptr<ConstraintInterface> constraint) override
  {
    return function_->setConstraint(name, constraint);
  }

  size_t getNumberOfParameters() const override
  {
    return function_->getNumberOfParameters();
  }

  void setNamespace(const std::string& prefix) override
  {
    function_->setNamespace(prefix);
  }

  std::string getNamespace() const override
  {
    return function_->getNamespace();
  }

  std::string getParameterNameWithoutNamespace(const std::string& name) const override
  {
    return function_->getParameterNameWithoutNamespace(name);
  }

protected:
  ParameterList& getParameters_() override
  {
    return function_->getParameters_();
  }
};


/**
 * @brief General class that wraps a function into another one.
 * This class is meant to be derivated and just provides a general framework.
 */
class FirstOrderDerivableWrapper :
  public FunctionWrapper,
  public virtual FirstOrderDerivable
{
public:
  FirstOrderDerivableWrapper(std::shared_ptr<FirstOrderDerivable> function) :
    FunctionWrapper(function) {}

public:
  void enableFirstOrderDerivatives(bool yn)
  {
    std::dynamic_pointer_cast<FirstOrderDerivable>(function_)->enableFirstOrderDerivatives(yn);
  }

  bool enableFirstOrderDerivatives() const
  {
    return std::dynamic_pointer_cast<FirstOrderDerivable>(function_)->enableFirstOrderDerivatives();
  }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    return std::dynamic_pointer_cast<FirstOrderDerivable>(function_)->getFirstOrderDerivative(variable);
  }
};


/**
 * @brief General class that wraps a function into another one.
 * This class is meant to be derivated and just provides a general framework.
 */
class SecondOrderDerivableWrapper :
  public FirstOrderDerivableWrapper,
  public virtual SecondOrderDerivable
{
public:
  SecondOrderDerivableWrapper(std::shared_ptr<SecondOrderDerivable> function) :
    FirstOrderDerivableWrapper(function) {}

public:
  void enableSecondOrderDerivatives(bool yn)
  {
    std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->enableSecondOrderDerivatives(yn);
  }

  bool enableSecondOrderDerivatives() const
  {
    return std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->enableSecondOrderDerivatives();
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    return std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->getSecondOrderDerivative(variable);
  }

  double getSecondOrderDerivative(const std::string& variable1, const std::string& variable2) const
  {
    return std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->getSecondOrderDerivative(variable1, variable2);
  }
};


/**
 * @brief Wrapper class for optimization under constraints.
 *
 * Catch any ConstraintException thrown and send +inf.
 */
class InfinityFunctionWrapper :
  public FunctionWrapper
{
protected:
  mutable bool constraintMatch_;

public:
  InfinityFunctionWrapper(std::shared_ptr<FunctionInterface> function) :
    FunctionWrapper(function),
    constraintMatch_(false) {}
  virtual ~InfinityFunctionWrapper() {}

  InfinityFunctionWrapper* clone() const override { return new InfinityFunctionWrapper(*this); }

public:
  void setParameters(const ParameterList& parameters) override
  {
    try
    {
      function_->setParameters(parameters);
      constraintMatch_ = false;
    }
    catch (ConstraintException& ce)
    {
      constraintMatch_ = true;
    }
  }

  double getValue() const override
  {
    return constraintMatch_ ? -std::log(0.) :  function_->getValue();
  }

  double f(const ParameterList& parameters) override
  {
    setParameters(parameters);
    return getValue();
  }

  void setAllParametersValues(const ParameterList& parameters) override
  {
    try
    {
      function_->setAllParametersValues(parameters);
      constraintMatch_ = false;
    }
    catch (ConstraintException& ce)
    {
      constraintMatch_ = true;
    }
  }

  void setParameterValue(const std::string& name, double value) override
  {
    try
    {
      function_->setParameterValue(name, value);
      constraintMatch_ = false;
    }
    catch (ConstraintException& ce)
    {
      constraintMatch_ = true;
    }
  }

  void setParametersValues(const ParameterList& parameters) override
  {
    try
    {
      function_->setParametersValues(parameters);
      constraintMatch_ = false;
    }
    catch (ConstraintException& ce)
    {
      constraintMatch_ = true;
    }
  }

  bool matchParametersValues(const ParameterList& parameters) override
  {
    try
    {
      bool test = function_->matchParametersValues(parameters);
      constraintMatch_ = false;
      return test;
    }
    catch (ConstraintException& ce)
    {
      constraintMatch_ = true;
      return false;
    }
  }
};

/**
 * @brief Wrapper class for optimization under constraints.
 *
 * Catch any ConstraintException thrown and send +inf.
 */
class InfinityDerivableFirstOrderWrapper :
  public virtual InfinityFunctionWrapper
{
public:
  InfinityDerivableFirstOrderWrapper(std::shared_ptr<FirstOrderDerivable> function) :
    InfinityFunctionWrapper(function) {}
  virtual ~InfinityDerivableFirstOrderWrapper() {}

  InfinityDerivableFirstOrderWrapper* clone() const { return new InfinityDerivableFirstOrderWrapper(*this); }

public:
  double getFirstOrderDerivative(const std::string& variable) const
  {
    return constraintMatch_ ? -std::log(0.) : (std::dynamic_pointer_cast<FirstOrderDerivable>(function_)->getFirstOrderDerivative(variable));
  }

  double df(const std::string& variable, const ParameterList& parameters)
  {
    setParameters(parameters);
    return getFirstOrderDerivative(variable);
  }
};

/**
 * @brief Wrapper class for optimization under constraints.
 *
 * Catch any ConstraintException thrown and send +inf.
 */
class InfinityDerivableSecondOrderWrapper :
  public virtual InfinityDerivableFirstOrderWrapper
{
public:
  InfinityDerivableSecondOrderWrapper(std::shared_ptr<FirstOrderDerivable> function) :
    InfinityFunctionWrapper(function),
    InfinityDerivableFirstOrderWrapper(function) {}
  virtual ~InfinityDerivableSecondOrderWrapper() {}

  InfinityDerivableSecondOrderWrapper* clone() const { return new InfinityDerivableSecondOrderWrapper(*this); }

public:
  double getSecondOrderDerivative(const std::string& variable) const
  {
    return constraintMatch_ ? -std::log(0.) : (std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->getSecondOrderDerivative(variable));
  }

  double d2f(const std::string& variable, const ParameterList& parameters)
  {
    setParameters(parameters);
    return getSecondOrderDerivative(variable);
  }

  double getSecondOrderDerivative(const std::string& variable1, const std::string& variable2) const
  {
    return constraintMatch_ ? -std::log(0.) : (std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->getSecondOrderDerivative(variable1, variable2));
  }

  double d2f(const std::string& variable1, const std::string& variable2, const ParameterList& parameters)
  {
    setParameters(parameters);
    return getSecondOrderDerivative(variable1, variable2);
  }
};


/**
 * @brief A simple function with two parameters, mostly for testing and debugging :)
 *
 * @author Julien Dutheil.
 */
class TestFunction :
  public virtual FunctionInterface,
  public AbstractParametrizable
{
public:
  TestFunction(double x = 0, double y = 0) :
    AbstractParametrizable("")
  {
    addParameter_(new Parameter("x", x));
    addParameter_(new Parameter("y", y));
  }

  Clonable* clone() const { return new TestFunction(*this); }

  void setParameters(const ParameterList& parameters)
  {
    matchParametersValues(parameters);
  }

  double getValue() const
  {
    double x = parameter("x").getValue();
    double y = parameter("y").getValue();
    return x * x + y * y;
  }

  void fireParameterChanged(const ParameterList& parameters) {}
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_FUNCTIONS_H

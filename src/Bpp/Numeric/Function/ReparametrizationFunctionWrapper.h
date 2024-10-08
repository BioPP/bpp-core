// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_REPARAMETRIZATIONFUNCTIONWRAPPER_H
#define BPP_NUMERIC_FUNCTION_REPARAMETRIZATIONFUNCTIONWRAPPER_H


#include "../AbstractParametrizable.h"
#include "../TransformedParameter.h"
#include "Functions.h"

namespace bpp
{
/**
 * @brief Function wrapper that remove simple constraints on parameters.
 *
 * This function takes as input another function and reparametrize it when possible.
 * currently, only constraint of the type ]a, b[ where a and b can be +/- inf.
 */
class ReparametrizationFunctionWrapper :
  public virtual FunctionInterface,
  public AbstractParametrizable
{
protected:
  std::shared_ptr<FunctionInterface> function_;
  ParameterList functionParameters_;

public:
  /**
   * @brief Build a new reparametrization wrapper for the given function, using all available parameters.
   *
   * @param function The function to reparametrize.
   * @param verbose Print some information.
   */
  ReparametrizationFunctionWrapper(std::shared_ptr<FunctionInterface> function, bool verbose = true) :
    AbstractParametrizable(function->getNamespace()),
    function_(function),
    functionParameters_(function->getParameters())
  {
    init_(verbose);
  }

  /**
   * @brief Build a new reparametrization wrapper for the given function, using only the specified parameters.
   *
   * @param function The function to reparametrize.
   * @param parameters The list of parameters that will be reparametrized. The intersection with the
   * list of function parameters will be used in the reparametrized function. Any other parameters
   * (in the given list or in the original function) will be ignored.
   * @param verbose Print some information.
   */
  ReparametrizationFunctionWrapper(std::shared_ptr<FunctionInterface> function, const ParameterList& parameters, bool verbose = true) :
    AbstractParametrizable(function->getNamespace()),
    function_(function),
    functionParameters_(function->getParameters().getCommonParametersWith(parameters))
  {
    init_(verbose);
  }

  ReparametrizationFunctionWrapper(const ReparametrizationFunctionWrapper& rfw) :
    AbstractParametrizable(rfw),
    function_(rfw.function_),
    functionParameters_(rfw.functionParameters_) {}

  ReparametrizationFunctionWrapper& operator=(const ReparametrizationFunctionWrapper& rfw)
  {
    AbstractParametrizable::operator=(rfw),
    function_ = rfw.function_;
    functionParameters_ = rfw.functionParameters_;
    return *this;
  }

  virtual ~ReparametrizationFunctionWrapper() {}

  ReparametrizationFunctionWrapper* clone() const { return new ReparametrizationFunctionWrapper(*this); }

private:
  void init_(bool verbose);

public:
  virtual const FunctionInterface& function() const { return *function_; }

  virtual FunctionInterface& function() { return *function_; }

  virtual std::shared_ptr<const FunctionInterface> getFunction() const { return function_; }

  virtual std::shared_ptr<FunctionInterface> getFunction() { return function_; }

  void setParameters(const ParameterList& parameters)
  {
    //      parameters.printParameters(std::cout);
    matchParametersValues(parameters);
    // We only set parameters that have been changed:
    //      functionParameters_.printParameters(std::cout);
    function_->setParameters(functionParameters_.createSubList(parameters.getParameterNames()));
  }

  double getValue() const
  {
    return function_->getValue();
  }

  void fireParameterChanged (const ParameterList& parameters);
};

/**
 * @brief Function wrapper that remove simple constraints on parameters. Also transform first order derivatives.
 *
 * This function takes as input another function and reparametrize it when possible.
 * currently, only constraint of the type ]a, b[ where a and b can be +/- inf.
 */
class ReparametrizationDerivableFirstOrderWrapper :
  public virtual FirstOrderDerivable,
  public ReparametrizationFunctionWrapper
{
public:
  /**
   * @brief Build a new reparametrization wrapper for the given function, using all available parameters.
   *
   * @param function The function to reparametrize.
   * @param verbose Print some information.
   */
  ReparametrizationDerivableFirstOrderWrapper(std::shared_ptr<FirstOrderDerivable> function, bool verbose = true) :
    ReparametrizationFunctionWrapper(function, verbose)
  {}

  /**
   * @brief Build a new reparametrization wrapper for the given function, using only the specified parameters.
   *
   * @param function The function to reparametrize.
   * @param parameters The list of parameters that will be reparametrized. The intersection with the
   * list of function parameters will be used in the reparametrized function. Any other parameters
   * (in the given list or in the original function) will be ignored.
   * @param verbose Print some information.
   */
  ReparametrizationDerivableFirstOrderWrapper(std::shared_ptr<FirstOrderDerivable> function, const ParameterList& parameters, bool verbose = true) :
    ReparametrizationFunctionWrapper(function, parameters, verbose)
  {}

  virtual ~ReparametrizationDerivableFirstOrderWrapper() {}

  ReparametrizationDerivableFirstOrderWrapper* clone() const { return new ReparametrizationDerivableFirstOrderWrapper(*this); }

private:
  void init_(bool verbose);

public:
  void enableFirstOrderDerivatives(bool yn) { std::dynamic_pointer_cast<FirstOrderDerivable>(function_)->enableFirstOrderDerivatives(yn); }

  bool enableFirstOrderDerivatives() const { return std::dynamic_pointer_cast<const FirstOrderDerivable>(function_)->enableFirstOrderDerivatives(); }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    return std::dynamic_pointer_cast<const FirstOrderDerivable>(function_)->getFirstOrderDerivative(variable)
           * dynamic_cast<const TransformedParameter&>(parameter(variable)).getFirstOrderDerivative();
  }
};


/**
 * @brief Function wrapper that remove simple constraints on parameters. Also transform first and second order derivatives.
 *
 * This function takes as input another function and reparametrize it when possible.
 * currently, only constraint of the type ]a, b[ where a and b can be +/- inf.
 */
class ReparametrizationDerivableSecondOrderWrapper :
  public virtual SecondOrderDerivable,
  public ReparametrizationDerivableFirstOrderWrapper
{
public:
  /**
   * @brief Build a new reparametrization wrapper for the given function, using all available parameters.
   *
   * @param function The function to reparametrize.
   * @param verbose Print some information.
   */
  ReparametrizationDerivableSecondOrderWrapper(std::shared_ptr<SecondOrderDerivable> function, bool verbose = true) :
    ReparametrizationDerivableFirstOrderWrapper(function, verbose)
  {}

  /**
   * @brief Build a new reparametrization wrapper for the given function, using only the specified parameters.
   *
   * @param function The function to reparametrize.
   * @param parameters The list of parameters that will be reparametrized. The intersection with the
   * list of function parameters will be used in the reparametrized function. Any other parameters
   * (in the given list or in the original function) will be ignored.
   * @param verbose Print some information.
   */
  ReparametrizationDerivableSecondOrderWrapper(std::shared_ptr<SecondOrderDerivable> function, const ParameterList& parameters, bool verbose = true) :
    ReparametrizationDerivableFirstOrderWrapper(function, parameters, verbose)
  {}

  virtual ~ReparametrizationDerivableSecondOrderWrapper() {}

  ReparametrizationDerivableSecondOrderWrapper* clone() const { return new ReparametrizationDerivableSecondOrderWrapper(*this); }

private:
  void init_(bool verbose);

public:
  void enableSecondOrderDerivatives(bool yn) { std::dynamic_pointer_cast<SecondOrderDerivable>(function_)->enableSecondOrderDerivatives(yn); }

  bool enableSecondOrderDerivatives() const { return std::dynamic_pointer_cast<const SecondOrderDerivable>(function_)->enableSecondOrderDerivatives(); }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    return std::dynamic_pointer_cast<const SecondOrderDerivable>(function_)->getSecondOrderDerivative(variable)
           * std::pow(dynamic_cast<const TransformedParameter&>(parameter(variable)).getFirstOrderDerivative(), 2)
           + std::dynamic_pointer_cast<const SecondOrderDerivable>(function_)->getFirstOrderDerivative(variable)
           * dynamic_cast<const TransformedParameter&>(parameter(variable)).getSecondOrderDerivative();
  }

  double getSecondOrderDerivative(const std::string& variable1, const std::string& variable2) const
  {
    return std::dynamic_pointer_cast<const SecondOrderDerivable>(function_)->getSecondOrderDerivative(variable1, variable2)
           * dynamic_cast<const TransformedParameter&>(parameter(variable1)).getFirstOrderDerivative()
           * dynamic_cast<const TransformedParameter&>(parameter(variable2)).getFirstOrderDerivative();
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_REPARAMETRIZATIONFUNCTIONWRAPPER_H

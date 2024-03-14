// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_FUNCTIONOPERATOR_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_FUNCTIONOPERATOR_H

#include <memory>

#include "../Functions.h"

namespace bpp
{
/**
 * @brief Implements a double operator (ie leaf in the computation
 * tree) where value comes from a function.
 *
 */

template<class F>
class FunctionOperator :
  public Operator
{
private:
  F& func_;

  std::string name_;

  double getValue_(std::true_type) const
  {
    return func_.getValue();
  }

  double getValue_(std::false_type) const
  {
    return 0;
  }

  double getFirstOrderDerivative_(const std::string& variable, std::true_type) const
  {
    return func_.getFirstOrderDerivative(variable);
  }

  double getFirstOrderDerivative_(const std::string& variable, std::false_type) const
  {
    return 0;
  }

  double getSecondOrderDerivative_(const std::string& variable, std::true_type) const
  {
    return func_.getSecondOrderDerivative(variable);
  }

  double getSecondOrderDerivative_(const std::string& variable, std::false_type) const
  {
    return 0;
  }

public:
  FunctionOperator(F& func, std::string name) :
    func_(func), name_(name)
  {}

  FunctionOperator* clone() const
  {
    return new FunctionOperator(*this);
  }

  double getValue() const
  {
    return getValue_(std::integral_constant<bool, std::is_base_of<FunctionInterface, F>::value>{});
  }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    return getFirstOrderDerivative_(variable, std::integral_constant<bool, std::is_base_of<FirstOrderDerivable, F>::value>{});
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    return getSecondOrderDerivative_(variable, std::integral_constant<bool, std::is_base_of<SecondOrderDerivable, F>::value>{});
  }

  std::string getName() const
  {
    return name_;
  }

  std::string output() const
  {
    return name_;
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_FUNCTIONOPERATOR_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_MATHOPERATOR_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_MATHOPERATOR_H

#include <cmath>
#include <memory>


namespace bpp
{
/**
 * @brief Implements a unary operator that applies a math (described
 * in cmath) operator.
 *
 */

class MathOperator :
  public Operator
{
private:
  /*
   * @brief pointer to function.
   *  If null, identity function is used.
   */

  double (* func_)(double);

  std::string name_;

  std::shared_ptr<Operator> son_;

public:
  MathOperator(double (*func)(double), std::string name, std::shared_ptr<Operator> son) :
    func_(func), name_(name), son_(son)
  {}

  MathOperator* clone() const
  {
    return new MathOperator(*this);
  }

  std::shared_ptr<Operator> getSon()
  {
    return son_;
  }

  double getValue() const
  {
    if (func_)
      return (*func_)(son_->getValue());
    else
      return son_->getValue();
  }

  std::string getName() const
  {
    return name_;
  }

  /**
   * @brief 1st order derivative
   *
   */
  double getFirstOrderDerivative(const std::string& variable) const
  {
    double v = son_->getValue();
    double d = son_->getFirstOrderDerivative(variable);

    if (name_ == "exp")
      return d * exp(v);
    else if (name_ == "log")
      return d / v;
    else
      throw Exception("MathOperator::getFirstOrderDerivative : unknown function " + name_);
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    double v = son_->getValue();
    double d = son_->getFirstOrderDerivative(variable);
    double d2 = son_->getSecondOrderDerivative(variable);

    if (name_ == "exp")
      return (d2 + d * d) * exp(v);
    else if (name_ == "log")
      return (d2 * v - d * d) / (v * v);
    else
      throw Exception("MathOperator::getFirstOrderDerivative : unknown function " + name_);
    return 0;
  }


  std::string output() const
  {
    return name_ + "(" + son_->output() + ")";
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_MATHOPERATOR_H

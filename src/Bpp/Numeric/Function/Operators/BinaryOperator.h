// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_BINARYOPERATOR_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_BINARYOPERATOR_H

#include <memory>

#include "Operator.h"

namespace bpp
{
/**
 * @brief Binary arithmetic operator for numerical computation.
 *
 */

class BinaryOperator :
  public Operator
{
private:
  char symb_;

  std::shared_ptr<Operator> left_, right_;

public:
  BinaryOperator(char symb, std::shared_ptr<Operator> left, std::shared_ptr<Operator> right) :
    symb_(symb), left_(left), right_(right)
  {}

  BinaryOperator* clone() const
  {
    return new BinaryOperator(*this);
  }

  std::shared_ptr<Operator> getLeftSon()
  {
    return left_;
  }

  std::shared_ptr<Operator> getRightSon()
  {
    return right_;
  }

  char getSymbol() const
  {
    return symb_;
  }

  double getValue() const
  {
    switch (symb_)
    {
    case '+':
      return left_->getValue() + right_->getValue();
    case '-':
      return left_->getValue() - right_->getValue();
    case '/':
      if (right_->getValue() == 0)
        return 0;

      return left_->getValue() / right_->getValue();
    case '*':
      return left_->getValue() * right_->getValue();
    default:
      return 0;
    }
  }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    double dl = left_->getFirstOrderDerivative(variable);
    double dr = right_->getFirstOrderDerivative(variable);
    double l = left_->getValue();
    double r = right_->getValue();

    switch (symb_)
    {
    case '+':
      return dl + dr;
    case '-':
      return dl - dl;
    case '/':
      if (r == 0)
        return 0;

      return (dl * r - dr * l ) / (r * r);
    case '*':
      return dl * r + dr * l;
    default:
      return 0;
    }
    return 0;
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    double d2l = left_->getSecondOrderDerivative(variable);
    double d2r = right_->getSecondOrderDerivative(variable);
    double l = left_->getValue();
    double r = right_->getValue();
    double dl = left_->getFirstOrderDerivative(variable);
    double dr = right_->getFirstOrderDerivative(variable);

    double r2 = r * r;
    double r3 = r * r2;

    switch (symb_)
    {
    case '+':
      return d2l + d2r;
    case '-':
      return d2l - d2r;

    case '/':
      if (r == 0)
        return 0;

      return (d2l * r - d2r * l ) / r2 - ( 2 * dl * ( dl * r - dr * l) ) / r3;

    case '*':
      return d2l * r + d2r * l + 2 * dr * dl;
    default:
      return 0;
    }
  }

  std::string output() const
  {
    return "(" + left_->output() + " " + symb_ + " " + right_->output() + ")";
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_BINARYOPERATOR_H

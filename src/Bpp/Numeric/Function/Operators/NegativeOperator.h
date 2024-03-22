// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_NEGATIVEOPERATOR_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_NEGATIVEOPERATOR_H

#include <memory>


namespace bpp
{
/**
 * @brief Negative value for an operator
 *
 */

class NegativeOperator :
  public Operator
{
private:
  std::shared_ptr<Operator> son_;

public:
  NegativeOperator(std::shared_ptr<Operator> son) :
    son_(son)
  {}

  NegativeOperator* clone() const
  {
    return new NegativeOperator(*this);
  }

  std::shared_ptr<Operator> getSon()
  {
    return son_;
  }

  double getValue() const
  {
    return -son_->getValue();
  }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    return -son_->getFirstOrderDerivative(variable);
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    return -son_->getSecondOrderDerivative(variable);
  }

  std::string output() const
  {
    return "-" + son_->output();
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_NEGATIVEOPERATOR_H

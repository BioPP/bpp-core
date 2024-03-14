// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_CONSTANTOPERATOR_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_CONSTANTOPERATOR_H

#include <memory>

#include "../../../Text/TextTools.h"

namespace bpp
{
/**
 * @brief Constant (ie leaf) operator.
 *
 */

class ConstantOperator :
  public Operator
{
private:
  const double value_;

public:
  ConstantOperator(double value) :
    value_(value) {}

  ConstantOperator* clone() const
  {
    return new ConstantOperator(*this);
  }

  double getValue() const
  {
    return value_;
  }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    return 0;
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    return 0;
  }

  std::string output() const
  {
    return TextTools::toString(value_);
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_CONSTANTOPERATOR_H

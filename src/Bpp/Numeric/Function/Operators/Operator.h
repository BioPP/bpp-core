// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_OPERATOR_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_OPERATOR_H

#include <string>

#include "../../../Clonable.h"

namespace bpp
{
/**
 * @brief Interface of operator for numerical computation.
 *
 */

class Operator :
  public virtual Clonable
{
public:
  virtual ~Operator() {}

  virtual double getValue() const = 0;

  virtual double getFirstOrderDerivative(const std::string& variable) const = 0;

  virtual double getSecondOrderDerivative(const std::string& variable) const = 0;

  virtual std::string output() const = 0;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_OPERATOR_H

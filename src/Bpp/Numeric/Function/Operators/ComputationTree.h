// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPERATORS_COMPUTATIONTREE_H
#define BPP_NUMERIC_FUNCTION_OPERATORS_COMPUTATIONTREE_H

#include <memory>

#include "../../../Graph/AssociationTreeGraphImplObserver.h"
#include "../Functions.h"
#include "Operator.h"

namespace bpp
{
/**
 * @brief Defines a Computation Tree based on Operators.
 *
 */

class ComputationTree :
  public AssociationTreeGlobalGraphObserver<Operator, short>
{
private:
  std::shared_ptr<Operator> readFormula_(const std::string& formula, const std::map<std::string, std::shared_ptr<FunctionInterface>>& functionNames);

public:
  /*
   * @brief Tree for numerical computation given a formula (such as
   * 2*f+g), and given a map from function name (f) to real
   * function.
   *
   */

  ComputationTree(const std::string& formula, const std::map<std::string, std::shared_ptr<FunctionInterface>>& functionNames);

  ComputationTree* clone() const
  {
    return new ComputationTree(*this);
  }

  double getValue() const
  {
    return getRoot()->getValue();
  }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    return getRoot()->getFirstOrderDerivative(variable);
  }

  double getSecondOrderDerivative(const std::string& variable) const
  {
    return getRoot()->getSecondOrderDerivative(variable);
  }

  void readFormula(const std::string& formula, const std::map<std::string, std::shared_ptr<FunctionInterface>>& functionNames)
  {
    readFormula_(formula, functionNames);
  }

  std::string output() const;

  /*
   * Return true if all binary operators are '+' or '-'
   *
   */

  bool isAllSum();
};
}
#endif // BPP_NUMERIC_FUNCTION_OPERATORS_COMPUTATIONTREE_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_BFGSMULTIDIMENSIONS_H
#define BPP_NUMERIC_FUNCTION_BFGSMULTIDIMENSIONS_H


#include "../VectorTools.h"
#include "AbstractOptimizer.h"
#include "DirectionFunction.h"

namespace bpp
{
/**
 * @brief Broyden–Fletcher–Goldfarb–Shanno (BFGS) optimization method.
 *
 * with a modification on the bounds taken from:
 *  An active set limited memory BFGS algorithm for large-scale bound
 *    constrained optimization, Yunhai Xiao & Dong-Hui Li. Math Meth
 *    Oper Res (2008) 67:443­454
 */

class BfgsMultiDimensions :
  public AbstractOptimizer
{
protected:
  // double gtol_;
  double slope_;

  // vectors of the Lower & Upper bounds of the parameters
  Vdouble Up_, Lo_;

  mutable Vdouble p_, gradient_, xi_, dg_, hdg_;
  mutable VVdouble hessian_;

  mutable std::shared_ptr<DirectionFunction> f1dim_;

public:
  BfgsMultiDimensions(std::shared_ptr<FirstOrderDerivable> function);

  virtual ~BfgsMultiDimensions() {}

  BfgsMultiDimensions* clone() const override { return new BfgsMultiDimensions(*this); }

public:
  /**
   * @name From AbstractOptimizer.
   *
   * @{
   */
  void doInit(const ParameterList& params) override;

  double doStep() override;
  /** @} */

  const FirstOrderDerivable& firstOrderDerivableFunction() const
  {
    if (function_) { 
      return *dynamic_pointer_cast<const FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("BfgsMultiDimensions::firstOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }

  FirstOrderDerivable& firstOrderDerivableFunction()
  {
    if (function_) { 
      return *dynamic_pointer_cast<FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("BfgsMultiDimensions::firstOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }
  
  std::shared_ptr<const FirstOrderDerivable> getFirstOrderDerivableFunction() const
  {
    return dynamic_pointer_cast<const FirstOrderDerivable>(function_);
  }

  std::shared_ptr<FirstOrderDerivable> getFirstOrderDerivableFunction()
  {
    return dynamic_pointer_cast<FirstOrderDerivable>(function_);
  }

  void getGradient(std::vector<double>& gradient) const;

private:
  /**
   * Sets the direction for linesearch in case of bounds
   * To be used after gradient_ & pi_ are computed
   */
  void setDirection();
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_BFGSMULTIDIMENSIONS_H

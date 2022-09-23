//
// File: BfgsMultiDimensions.h
// Authors:
//   Laurent Guéguen
// Created: 2010-12-16 13:49:00
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 19, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for numerical calculus.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

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

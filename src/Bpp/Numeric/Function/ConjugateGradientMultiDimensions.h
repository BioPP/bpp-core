// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_CONJUGATEGRADIENTMULTIDIMENSIONS_H
#define BPP_NUMERIC_FUNCTION_CONJUGATEGRADIENTMULTIDIMENSIONS_H


#include "AbstractOptimizer.h"
#include "BrentOneDimension.h"
#include "DirectionFunction.h"

namespace bpp
{
/**
 * @brief Conjugate gradient optimization method.
 *
 * A description of the algorithm can be found in:
 * <pre>
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
 * (ISBN 0-521-43108-5)
 * </pre>
 * or there:
 * <a href="http://en.wikipedia.org/wiki/Conjugate_gradient">http://en.wikipedia.org/wiki/Conjugate_gradient</a>.
 */
class ConjugateGradientMultiDimensions :
  public AbstractOptimizer
{
protected:
  BrentOneDimension optimizer_; // One dimensional optimizer.
  std::vector<double> xi_, h_, g_;
  std::shared_ptr<DirectionFunction> f1dim_;

public:
  ConjugateGradientMultiDimensions(std::shared_ptr<FirstOrderDerivable> function);

  virtual ~ConjugateGradientMultiDimensions() {}

  ConjugateGradientMultiDimensions* clone() const { return new ConjugateGradientMultiDimensions(*this); }

public:
  /**
   * @name From AbstractOptimizer.
   *
   * @{
   */
  void doInit(const ParameterList& params);

  double doStep();
  /** @} */

  const FirstOrderDerivable& firstOrderDerivableFunction() const
  {
    if (function_) { 
      return *dynamic_pointer_cast<const FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("ConjugateGradientMultiDimensions::firstOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }

  FirstOrderDerivable& firstOrderDerivableFunction()
  {
    if (function_) { 
      return *dynamic_pointer_cast<FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("ConjugateGradientMultiDimensions::firstOrderDerivableFunction() : no function associated to this optimizer.");
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

};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_CONJUGATEGRADIENTMULTIDIMENSIONS_H

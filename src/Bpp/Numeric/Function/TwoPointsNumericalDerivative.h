// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_TWOPOINTSNUMERICALDERIVATIVE_H
#define BPP_NUMERIC_FUNCTION_TWOPOINTSNUMERICALDERIVATIVE_H


#include "AbstractNumericalDerivative.h"
#include "Functions.h"

// From the STL:
#include <map>
#include <vector>
#include <string>

namespace bpp
{
/**
 * @brief Two points numerical derivative function wrapper.
 *
 * Numerical derivatives use two points to compute the (first order) derivatives.
 * @f$x_0@f$ is the focus point and @f$x_{+1} = x_0+h@f$ (or @f$x_0-h@f$, if constrained on the right).
 * Corresponding function values are @f$f_0@f$, @f$f_{-1}@f$ and @f$f_{+1/-1}@f$ respectively.
 * The derivatives are then computed using the central formulas:
 * @f{eqnarray*}
 * \dfrac{\partial   f}{\partial x  } &=& \dfrac{f_{+1}-f_{0}}{h}\mathrm{\ or}\\
 * \dfrac{\partial   f}{\partial x  } &=& \dfrac{f_{0}-f_{-1}}{h}\\
 * @f}
 * This class does not allow computation of second order derivatives.
 *
 * The @f$h@f$ parameter is computed in a parameter dependent manner:
 * @f$ h = x \times e@f$, with @f$x \neq 0@f$ being the current parameter value.
 * If @f$x = 0@f$, @f$h = e@f$.
 * Default value is provided for @f$e@f$ and corresponds to the _h field.
 * The default value works fine in most cases, but you may want to change it using the setInterval method.
 *
 * @see AbstractNumericalDerivative, ThreePointsNumericalDerivative, FivePointsNumericalDerivative
 */
class TwoPointsNumericalDerivative :
  public AbstractNumericalDerivative
{
private:
  double f1_, f2_;

public:
  TwoPointsNumericalDerivative(std::shared_ptr<FunctionInterface> function) :
    AbstractNumericalDerivative(function),
    f1_(),
    f2_() {}
  TwoPointsNumericalDerivative(std::shared_ptr<FirstOrderDerivable> function) :
    AbstractNumericalDerivative(function),
    f1_(),
    f2_() {}
  virtual ~TwoPointsNumericalDerivative() {}

  TwoPointsNumericalDerivative* clone() const override
  {
    return new TwoPointsNumericalDerivative(*this);
  }

public:
  double getValue() const override { return f1_; }

  /**
   * @name The DerivableSecondOrder interface
   *
   * @{
   */
  double getSecondOrderDerivative(const std::string& variable) const override
  {
    throw Exception("Second order derivative not avalaible with two points method.");
  }

  double getSecondOrderDerivative(const std::string& variable1, const std::string& variable2) const override
  {
    throw Exception("Unimplemented cross derivative.");
  }
  /** @} */

protected:
  void updateDerivatives(const ParameterList& parameters) override;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_TWOPOINTSNUMERICALDERIVATIVE_H

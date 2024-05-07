// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_THREEPOINTSNUMERICALDERIVATIVE_H
#define BPP_NUMERIC_FUNCTION_THREEPOINTSNUMERICALDERIVATIVE_H


#include "NumericalDerivative.h"
#include "Functions.h"

// From the STL:
#include <map>
#include <vector>
#include <string>

namespace bpp
{
/**
 * @brief Three points numerical derivative function wrapper.
 *
 * Numerical derivatives use three points to compute the derivatives.
 * @f$x_0@f$ is the focus point, @f$x_{-1} = x_0-h@f$ and @f$x_{+1} = x_0+h@f$
 * are other points, with function values @f$f_0@f$, @f$f_{-1}@f$ and @f$f_{+1}@f$ respectively.
 * The derivatives are then computed using the central formulas:
 * @f{eqnarray*}
 * \dfrac{\partial   f}{\partial x  } &=& \dfrac{f_{+1}-f_{-1}}{2h}\\
 * \dfrac{\partial^2 f}{\partial x^2} &=& \dfrac{f_{+1}-2f_0+f_{-1}}{h^2}\\
 * @f}
 * In case of border limit (when @f$x_{-1}@f$ or @f$x_{+1}@f$ are not computable),
 * the foreward and backward computations are performed, respectively:
 * @f{eqnarray*}
 * \dfrac{\partial   f}{\partial x  } &=& \dfrac{f_{+1}-f_0}{h}\\
 * \dfrac{\partial^2 f}{\partial x^2} &=& \dfrac{f_{+2}-2f_{+1}+f_0}{h^2}\\
 * @f}
 * and
 * @f{eqnarray*}
 * \dfrac{\partial   f}{\partial x  } &=& \dfrac{f_0-f_{-1}}{h}\\
 * \dfrac{\partial^2 f}{\partial x^2} &=& \dfrac{f_0-2f_{-1}+f_{-2}}{h^2}\\
 * @f}
 *
 * The @f$h@f$ parameter is computed in a parameter dependent manner:
 * @f$ h = x \times e@f$, with @f$x \neq 0@f$ being the current parameter value.
 * If @f$x = 0@f$, @f$h = e@f$.
 * Default value is provided for @f$e@f$ and corresponds to the _h field.
 * The default value works fine in most cases, but you may want to change it using the setInterval method.
 *
 * @see AbstractNumericalDerivative
 */
class ThreePointsNumericalDerivative :
  public AbstractNumericalDerivative
{
private:
  double f1_, f2_, f3_, f11_, f22_, f12_, f21_;

public:
  ThreePointsNumericalDerivative (std::shared_ptr<FunctionInterface> function) :
    AbstractNumericalDerivative(function),
    f1_(),
    f2_(),
    f3_(),
    f11_(),
    f22_(),
    f12_(),
    f21_() {}
  ThreePointsNumericalDerivative(std::shared_ptr<FirstOrderDerivable> function) :
    AbstractNumericalDerivative(function),
    f1_(),
    f2_(),
    f3_(),
    f11_(),
    f22_(),
    f12_(),
    f21_() {}
  ThreePointsNumericalDerivative(std::shared_ptr<SecondOrderDerivable> function) :
    AbstractNumericalDerivative(function),
    f1_(),
    f2_(),
    f3_(),
    f11_(),
    f22_(),
    f12_(),
    f21_() {}
  virtual ~ThreePointsNumericalDerivative() {}

  ThreePointsNumericalDerivative* clone() const override
  {
    return new ThreePointsNumericalDerivative(*this);
  }

public:
  double getValue() const override
  {
    return f2_;
  }

protected:
  void updateDerivatives(const ParameterList& parameters) override;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_THREEPOINTSNUMERICALDERIVATIVE_H

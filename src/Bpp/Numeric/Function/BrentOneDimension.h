// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_BRENTONEDIMENSION_H
#define BPP_NUMERIC_FUNCTION_BRENTONEDIMENSION_H


#include "AbstractOptimizer.h"

namespace bpp
{
/**
 * @brief Brent's optimization for one parameter.
 *
 * A description of the algorithm can be found in:
 * <pre>
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
 * (ISBN 0-521-43108-5)
 * </pre>
 * or there: <a href="http://en.wikipedia.org/wiki/Brent's_method">http://en.wikipedia.org/wiki/Brent's_method</a>.
 */
class BrentOneDimension :
  public AbstractOptimizer
{
public:
  class BODStopCondition :
    public AbstractOptimizationStopCondition
  {
public:
    BODStopCondition(BrentOneDimension* bod) :
      AbstractOptimizationStopCondition(bod)
    {
      tolerance_ = bod->tol2;
      burnin_ = 3;
    }
    virtual ~BODStopCondition() {}

    BODStopCondition* clone() const { return new BODStopCondition(*this); }

public:
    bool isToleranceReached() const;
    double getCurrentTolerance() const;
  };

  enum Bracketing
  {
    BRACKET_OUTWARD = 0,
    BRACKET_INWARD = 1
  };

  friend class BODStopCondition;

protected:
  double a, b, d, e, etemp, fu, fv, fw, fx, p, q, r, tol1, tol2, u, v, w, x, xm;
  double _xinf, _xsup;
  bool isInitialIntervalSet_;
  Bracketing bracketing_;

public:
  BrentOneDimension(std::shared_ptr<FunctionInterface> function = nullptr);
  virtual ~BrentOneDimension() {}

  BrentOneDimension* clone() const { return new BrentOneDimension(*this); }

public:
  /**
   * @name The Optimizer interface.
   *
   * @{
   */

  /**
   * @brief Initialize optimizer.
   *
   * Brent's algorithm needs 2 initial guesses, so you must call the
   * setInitialInterval() method first. This function actually performs:
   * <ul>
   * <li>Parameter list actualisation;</li>
   * <li>Initial bracketting;</li>
   * <li>Function evaluation count reseting.</li>
   * </ul>
   */
  double optimize(); // redefinition
  /** @} */

  void doInit(const ParameterList& params);

  double doStep();

public:
  /**
   * @name Specific method
   *
   * @{
   */

  /**
   * @brief Set intial search interval.
   *
   * @param inf Minimum value.
   * @param sup Maximum value.
   */
  void setInitialInterval(double inf, double sup);
  /** @} */

  /**
   * @return 'true' if the initial interval has been correctly set.
   */
  bool isInitialIntervalSet() const { return isInitialIntervalSet_; }

  /**
   * @brief Get the brackeitng method
   */
  BrentOneDimension::Bracketing getBracketing() const { return bracketing_; }

  /**
   * @brief Set the brackeitng method
   */
  void setBracketing(BrentOneDimension::Bracketing bracketing)  { bracketing_ = bracketing; }

public:
  static double ZEPS;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_BRENTONEDIMENSION_H

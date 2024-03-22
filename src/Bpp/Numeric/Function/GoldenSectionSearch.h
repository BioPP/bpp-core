// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_GOLDENSECTIONSEARCH_H
#define BPP_NUMERIC_FUNCTION_GOLDENSECTIONSEARCH_H


#include "AbstractOptimizer.h"

namespace bpp
{
/**
 * @brief Golden Section Search optimization algorithm for one parameter.
 *
 * A description of the algorithm can be found in:
 * <pre>
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
 * (ISBN 0-521-43108-5)
 * </pre>
 * or there:
 * <a href="http://en.wikipedia.org/wiki/Golden_section_search">http://en.wikipedia.org/wiki/Golden_section_search</a>.
 */
class GoldenSectionSearch :
  public AbstractOptimizer
{
public:
  class GSSStopCondition :
    public AbstractOptimizationStopCondition
  {
public:
    GSSStopCondition(GoldenSectionSearch* gss) :
      AbstractOptimizationStopCondition(gss) {}
    virtual ~GSSStopCondition() {}

    GSSStopCondition* clone() const { return new GSSStopCondition(*this); }

public:
    bool isToleranceReached() const;
    double getCurrentTolerance() const;
  };

  friend class GSSStopCondition;

private:
  double f1, f2, x0, x1, x2, x3;
  double xinf_, xsup_;
  bool isInitialIntervalSet_;

public:
  GoldenSectionSearch(std::shared_ptr<FunctionInterface> function);
  virtual ~GoldenSectionSearch() {}

  GoldenSectionSearch* clone() const override { return new GoldenSectionSearch(*this); }

public:
  /**
   * @name The Optimizer interface.
   *
   * @{
   */

  /**
   * @brief Initialize optimizer.
   *
   * The golden section search needs 2 initial guesses, so you must call the
   * setInitialInterval() method first. This function actually performs:
   * <ul>
   * <li>Parameter list actualisation;</li>
   * <li>Initial bracketting;</li>
   * <li>Function evaluation count reseting.</li>
   * </ul>
   */
  double getFunctionValue() const override;
  /** @} */

  void doInit(const ParameterList& params) override;

  double doStep() override;

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

protected:
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_GOLDENSECTIONSEARCH_H

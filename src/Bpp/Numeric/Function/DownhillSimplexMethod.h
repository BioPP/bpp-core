// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_DOWNHILLSIMPLEXMETHOD_H
#define BPP_NUMERIC_FUNCTION_DOWNHILLSIMPLEXMETHOD_H


#include "../VectorTools.h"
#include "AbstractOptimizer.h"

// From the STL:
#include <cmath>

namespace bpp
{
/**
 * @brief This implements the Downhill Simplex method in multidimensions.
 *
 * A description of the algorithm can be found in:
 * <pre>
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
 * (ISBN 0-521-43108-5)
 * </pre>
 * or there:
 * <a href="http://en.wikipedia.org/wiki/Nelder-Mead_method">http://en.wikipedia.org/wiki/Nelder-Mead_method</a>.
 */
class DownhillSimplexMethod :
  public AbstractOptimizer
{
public:
  class DSMStopCondition :
    public AbstractOptimizationStopCondition
  {
public:
    DSMStopCondition(DownhillSimplexMethod* dsm) :
      AbstractOptimizationStopCondition(dsm) {}
    virtual ~DSMStopCondition() {}

    DSMStopCondition* clone() const { return new DSMStopCondition(*this); }

public:
    bool isToleranceReached() const { return getCurrentTolerance() < tolerance_; }
    double getCurrentTolerance() const;
  };

  friend class DSMStopCondition;

private:
  class Simplex
  {
private:
    std::vector<ParameterList> parameters_;

public:
    // Class constructor and destructor:
    Simplex() : parameters_() {}
    virtual ~Simplex() {}

public:
    // Methods:
    const ParameterList& operator[](size_t i) const { return parameters_[i]; }
    ParameterList& operator[](size_t i) { return parameters_[i]; }
    void resize(size_t size) { parameters_.resize(size); }
    size_t getDimension() const { return parameters_[0].size(); }
  };

protected:
  Simplex simplex_;
  Vdouble y_;
  ParameterList pSum_;
  unsigned int iHighest_, iNextHighest_, iLowest_;

public:
  /**
   * @brief Build a new Downhill Simplex optimizer.
   *
   * @param function A pointer toward an object implementing the Optimizable interface.
   */
  DownhillSimplexMethod(std::shared_ptr<FunctionInterface> function);

  virtual ~DownhillSimplexMethod() {}

  DownhillSimplexMethod* clone() const { return new DownhillSimplexMethod(*this); }

public:
  /**
   * @name The Optimizer interface.
   *
   * @{
   */

  /**
   * @brief Multidimensional minimization of the function function_ by the
   * downhill simplex method of Nelder and Mead.
   */
  double optimize();
  /** @} */

  void doInit(const ParameterList& params);

  double doStep();

protected:
  /**
   * @name Specific inner methods
   *
   * @{
   */

  /**
   * @brief Update the pSum_ variable.
   */
  ParameterList getPSum();

  /**
   * @brief Extrapolates by a factor fac through the face of the simplex from the high point.
   * Try the new point and replaces the high point if it is better.
   *
   * @param fac Extrapolation factor.
   * @return The value of the function for the new point.
   */
  double tryExtrapolation(double fac);

  /** @} */
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_DOWNHILLSIMPLEXMETHOD_H

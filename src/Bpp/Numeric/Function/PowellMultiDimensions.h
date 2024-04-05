// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_POWELLMULTIDIMENSIONS_H
#define BPP_NUMERIC_FUNCTION_POWELLMULTIDIMENSIONS_H


#include "../VectorTools.h"
#include "AbstractOptimizer.h"
#include "DirectionFunction.h"

namespace bpp
{
/**
 * @brief Powell's multi-dimensions optimization algorithm for one parameter.
 *
 * A description of the algorithm can be found for example in:
 * <pre>
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
 * (ISBN 0-521-43108-5)
 * </pre>
 */
class PowellMultiDimensions :
  public AbstractOptimizer
{
public:
  class PMDStopCondition :
    public AbstractOptimizationStopCondition
  {
public:
    PMDStopCondition(PowellMultiDimensions* pmd) :
      AbstractOptimizationStopCondition(pmd) {}
    virtual ~PMDStopCondition() {}

    PMDStopCondition* clone() const { return new PMDStopCondition(*this); }

public:
    bool isToleranceReached() const;
    double getCurrentTolerance() const;
  };

  friend class PMDStopCondition;

protected:
  double fp_;
  double fret_;
  ParameterList pt_;
  VVdouble xi_;

  unsigned int ncom_;
  ParameterList pcom_, xicom_;
  std::shared_ptr<DirectionFunction> f1dim_;

public:
  PowellMultiDimensions(std::shared_ptr<FunctionInterface> function);
  virtual ~PowellMultiDimensions() {}

  PowellMultiDimensions* clone() const { return new PowellMultiDimensions(*this); }

public:
  /**
   * @name The Optimizer interface.
   *
   * @{
   */
  double optimize();
  /** @} */

  void doInit(const ParameterList& params);

  double doStep();
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_POWELLMULTIDIMENSIONS_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_SIMPLENEWTONMULTIDIMENSIONS_H
#define BPP_NUMERIC_FUNCTION_SIMPLENEWTONMULTIDIMENSIONS_H


#include "AbstractOptimizer.h"
#include "NewtonOneDimension.h"

namespace bpp
{
/**
 * @brief This Optimizer is a simple multi-dimensions optimizer, calling
 * the Newton one dimensional optimizer on each parameter.
 *
 * The one-dimensional optimizer used is NewtonOneDimension.
 */
class SimpleNewtonMultiDimensions :
  public AbstractOptimizer
{
private:
  size_t nbParams_;

  NewtonOneDimension optimizer_; // One dimensional optimizer.

public:
  SimpleNewtonMultiDimensions(std::shared_ptr<SecondOrderDerivable> function);

  virtual ~SimpleNewtonMultiDimensions() {}

  SimpleNewtonMultiDimensions* clone() const override { return new SimpleNewtonMultiDimensions(*this); }

public:
  /**
   * @name The Optimizer interface.
   *
   * @{
   */
  void setFunction(std::shared_ptr<FunctionInterface> function) override;
  /** @} */

  void doInit(const ParameterList& params) override;

  double doStep() override;

  /**
   * @return The optimizer used to optimize each parameter.
   */
  OptimizerInterface& oneDimensionOptimizer() { return optimizer_; }
  /**
   * @return The optimizer used to optimize each parameter.
   */
  const OptimizerInterface& oneDimensionOptimizer() const { return optimizer_; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_SIMPLENEWTONMULTIDIMENSIONS_H

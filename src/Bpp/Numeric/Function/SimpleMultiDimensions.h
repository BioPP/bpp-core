// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_SIMPLEMULTIDIMENSIONS_H
#define BPP_NUMERIC_FUNCTION_SIMPLEMULTIDIMENSIONS_H


#include "AbstractOptimizer.h"
#include "BrentOneDimension.h"

namespace bpp
{
/**
 * @brief This Optimizer is a very simple multi-dimensions optimizer, calling
 * a one dimensional optimizer on each parameter.
 *
 * The one-dimensional optimizer used is BrentOneDimension.
 * Consider using PowellMultiDimensions optimizer for a more efficient modified version of the algorithm.
 */
class SimpleMultiDimensions :
  public AbstractOptimizer
{
private:
  size_t nbParams_;

  BrentOneDimension optimizer_; // One dimensional optimizer.

public:
  SimpleMultiDimensions(std::shared_ptr<FunctionInterface> function);

  virtual ~SimpleMultiDimensions() {}

  SimpleMultiDimensions* clone() const override { return new SimpleMultiDimensions(*this); }

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
#endif // BPP_NUMERIC_FUNCTION_SIMPLEMULTIDIMENSIONS_H

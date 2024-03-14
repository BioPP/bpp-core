
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

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_NEWTONBACKTRACKONEDIMENSION_H
#define BPP_NUMERIC_FUNCTION_NEWTONBACKTRACKONEDIMENSION_H


#include "AbstractOptimizer.h"

namespace bpp
{
/**
 * @brief Newton's backtrack nearly optimization for one parameter.
 *
 *
 * This algorithm looks for a value that 'sufficiently low' enough
 * near the minimum of the function, but does not look for the
 * minimum. It needs the first derivative of a function.
 *
 *  Search a 'sufficiently low' value for a function in a given
 *  direction.
 *
 * This function implements the algorithm described for example in page 385 of
 *
 * <pre>
 * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
 * (ISBN 0-521-43108-5)
 * </pre>
 */

class NewtonBacktrackOneDimension :
  public AbstractOptimizer
{
public:
  class NBODStopCondition :
    public AbstractOptimizationStopCondition
  {
public:
    NBODStopCondition(NewtonBacktrackOneDimension* bod) :
      AbstractOptimizationStopCondition(bod) {}
    virtual ~NBODStopCondition() {}

    NBODStopCondition* clone() const { return new NBODStopCondition(*this); }

public:
    void init() {}
    bool isToleranceReached() const { return false; }
    double getCurrentTolerance() const { return 0; }
  };

  friend class NBODStopCondition;

public:
  /**
   * @brief Constructor
   * @param function  a pointer to the function
   * @param slope the slope of the backtrack
   * @param test the inverse factor on the stop tolerance
   */
  NewtonBacktrackOneDimension(std::shared_ptr<FunctionInterface> function, double slope, double test);
  virtual ~NewtonBacktrackOneDimension() {}

  NewtonBacktrackOneDimension* clone() const { return new NewtonBacktrackOneDimension(*this); }

private:
  double fold_, f_, a_, alam_, alamin_, alam2_, b_, disc_, f2_, rhs1_, rhs2_, slope_, test_, tmplam_;

public:
  void doInit(const ParameterList& params);

  double doStep();

  const FirstOrderDerivable& firstOrderDerivableFunction() const
  {
    if (function_) { 
      return *dynamic_pointer_cast<const FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("NewtonBacktrackOneDimension::firstOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }

  FirstOrderDerivable& firstOrderDerivableFunction()
  {
    if (function_) { 
      return *dynamic_pointer_cast<FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("NewtonBacktrackOneDimension::firstOrderDerivableFunction() : no function associated to this optimizer.");
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

};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_NEWTONBACKTRACKONEDIMENSION_H

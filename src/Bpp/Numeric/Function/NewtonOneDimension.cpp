// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Text/TextTools.h"
#include "../NumTools.h"
#include "NewtonOneDimension.h"

using namespace bpp;

/******************************************************************************/

NewtonOneDimension::NewtonOneDimension(std::shared_ptr<SecondOrderDerivable> function) :
  AbstractOptimizer(function),
  _param(),
  _maxCorrection(10)
{
  setDefaultStopCondition_(make_shared<FunctionStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
  nbEvalMax_ = 10000;
}

/******************************************************************************/

void NewtonOneDimension::doInit(const ParameterList& params)
{
  // Set the initial value (no use here! Use setInitialValues() instead).
  if (params.size() != 1)
    throw Exception("NewtonOneDimension::init(). This optimizer only deals with one parameter.");
  _param = params[0].getName();
  currentValue_ = getFunction()->f(getParameters());
  getStopCondition()->init();
}

/******************************************************************************/

double NewtonOneDimension::doStep()
{
  double movement;
  ParameterList newPoint = getParameters();
  ParameterList bckPoint = getFunction()->getParameters();
  double newValue;
  double firstOrderDerivative = firstOrderDerivableFunction().getFirstOrderDerivative(_param);
  double secondOrderDerivative = secondOrderDerivableFunction().getSecondOrderDerivative(_param);
  if (secondOrderDerivative <= 0)
  {
    printMessage("!!! Second order derivative is negative (" + TextTools::toString(getParameters()[0].getValue()) + "). No move performed.");
    // movements[i] = 0;  // We want to reach a minimum, not a maximum!
    // My personnal improvement:
    movement = -firstOrderDerivative / secondOrderDerivative;
  }
  else
    movement = firstOrderDerivative / secondOrderDerivative;
  if (std::isnan(movement))
  {
    printMessage("!!! Non derivable point. No move performed. (f=" + TextTools::toString(currentValue_) + ", d1=" + TextTools::toString(firstOrderDerivative) + ", d2=" + TextTools::toString(secondOrderDerivative) + ").");
    movement = 0; // Either first or second order derivative is infinity. This may happen when the function == inf at this point.
  }
  newPoint[0].setValue(getParameters()[0].getValue() - movement);
  newValue = getFunction()->f(newPoint);

  // Check newValue:
  unsigned int count = 0;
  while (newValue > currentValue_)
  {
    // Restore previous point (all parameters in case of global constraint):
    getFunction()->setParameters(bckPoint);

    count++;
    if (count >= _maxCorrection)
    {
      printMessage("!!! Felsenstein-Churchill correction applied too much time. Stopping here. Convergence probably not reached.");
      tolIsReached_ = true;
      return currentValue_;
      // throw Exception("NewtonOneDimension::step(). Felsenstein-Churchill correction applied more than 10000 times.");
    }
    printMessage("!!! Function at new point is greater than at current point: " + TextTools::toString(newValue) + ">" + TextTools::toString(currentValue_) + ". Applying Felsenstein-Churchill correction, value = " + TextTools::toString(newPoint[0].getValue()));
    movement = movement / 2;
    newPoint[0].setValue(getParameters()[0].getValue() - movement);
    newValue = getFunction()->f(newPoint);
  }

  getParameters_() = newPoint; // Function as been set to newPoint by the call of f(newPoint).
  return newValue;
}

/******************************************************************************/

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../NumTools.h"
#include "../VectorTools.h"
#include "OptimizationStopCondition.h"
#include "Optimizer.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

ParametersStopCondition::ParametersStopCondition(
    const OptimizerInterface* optimizer) :
  AbstractOptimizationStopCondition(optimizer),
  lastParametersEstimates_(),
  newParametersEstimates_()
{
  init();
  if (newParametersEstimates_.size() == 0)
  {
    cout << "DEBUG: WARNING!!! No parameter passed to ParametersStopCondition constructor. "
         << "Be sure to have initialized the Optimizer first!" << endl;
  }
}

ParametersStopCondition::ParametersStopCondition(
    const OptimizerInterface* optimizer,
    double tolerance) :
  AbstractOptimizationStopCondition(optimizer, tolerance),
  lastParametersEstimates_(),
  newParametersEstimates_()
{
  init();
  if (newParametersEstimates_.size() == 0)
  {
    cout << "DEBUG: WARNING!!! No parameter passed to ParametersStopCondition constructor. "
         << "Be sure to have initialized the Optimizer first!" << endl;
  }
}

ParametersStopCondition::ParametersStopCondition(
    const OptimizerInterface* optimizer,
    int burnin) :
  AbstractOptimizationStopCondition(optimizer, burnin),
  lastParametersEstimates_(),
  newParametersEstimates_()
{
  init();
  if (newParametersEstimates_.size() == 0)
  {
    cout << "DEBUG: WARNING!!! No parameter passed to ParametersStopCondition constructor. "
         << "Be sure to have initialized the Optimizer first!" << endl;
  }
}

ParametersStopCondition::ParametersStopCondition(
    const OptimizerInterface* optimizer,
    double tolerance,
    int burnin) :
  AbstractOptimizationStopCondition(optimizer, tolerance, burnin),
  lastParametersEstimates_(),
  newParametersEstimates_()
{
  init();
  if (newParametersEstimates_.size() == 0)
  {
    cout << "DEBUG: WARNING!!! No parameter passed to ParametersStopCondition constructor. "
         << "Be sure to have initialized the Optimizer first!" << endl;
  }
}

/******************************************************************************/

void ParametersStopCondition::init()
{
  AbstractOptimizationStopCondition::init();
  if (optimizer_->getFunction() != 0)
    newParametersEstimates_ = optimizer_->getParameters();
}

/******************************************************************************/

bool ParametersStopCondition::isToleranceReached() const
{
  callCount_++;
  lastParametersEstimates_ = newParametersEstimates_;
  newParametersEstimates_  = optimizer_->getParameters();
  if (callCount_ <= burnin_)
    return false;
  for (unsigned int i = 0; i < newParametersEstimates_.size(); i++)
  {
    Parameter& p = newParametersEstimates_[i];
    double lastEstimate = lastParametersEstimates_.parameter(p.getName()).getValue();
    double newEstimate = p.getValue();
    double tol = NumTools::abs<double>(newEstimate - lastEstimate);
    if (tol > tolerance_)
    {
      return false;
    }
  }
  return true;
}

/******************************************************************************/

double ParametersStopCondition::getCurrentTolerance() const
{
  if (callCount_ > burnin_)
  {
    double maxTol = 0.;
    for (unsigned int i = 0; i < newParametersEstimates_.size(); i++)
    {
      Parameter& p = newParametersEstimates_[i];
      double lastEstimate = lastParametersEstimates_.parameter(p.getName()).getValue();
      double newEstimate = p.getValue();
      double tol = NumTools::abs<double>(newEstimate - lastEstimate);
      if (tol > maxTol)
        maxTol = tol;
    }
    return maxTol;
  }
  else
  {
    return std::max(tolerance_, 1.);
  }
}

/******************************************************************************/

FunctionStopCondition::FunctionStopCondition(
    const OptimizerInterface* optimizer) :
  AbstractOptimizationStopCondition(optimizer),
  lastFunctionValue_(-log(0.)),
  newFunctionValue_(-log(0.))
{
  init();
}

FunctionStopCondition::FunctionStopCondition(
    const OptimizerInterface* optimizer,
    double tolerance) :
  AbstractOptimizationStopCondition(optimizer, tolerance),
  lastFunctionValue_(-log(0.)),
  newFunctionValue_(-log(0.))
{
  init();
}

FunctionStopCondition::FunctionStopCondition(
    const OptimizerInterface* optimizer,
    int burnin) :
  AbstractOptimizationStopCondition(optimizer, burnin),
  lastFunctionValue_(-log(0.)),
  newFunctionValue_(-log(0.))
{
  init();
}

FunctionStopCondition::FunctionStopCondition(
    const OptimizerInterface* optimizer,
    double tolerance,
    int burnin) :
  AbstractOptimizationStopCondition(optimizer, tolerance, burnin),
  lastFunctionValue_(-log(0.)),
  newFunctionValue_(-log(0.))
{
  init();
}

FunctionStopCondition::~FunctionStopCondition() {}

/******************************************************************************/

void FunctionStopCondition::init()
{
  AbstractOptimizationStopCondition::init();
  newFunctionValue_ = -log(0.);
  if (optimizer_->getFunction() != 0)
  {
    newFunctionValue_ = optimizer_->getFunctionValue();
  }
}

/******************************************************************************/

bool FunctionStopCondition::isToleranceReached() const
{
  callCount_++;
  lastFunctionValue_ = newFunctionValue_;
  newFunctionValue_  = optimizer_->getFunctionValue();
  if (callCount_ <= burnin_)
    return false;
  double tol = NumTools::abs<double>(newFunctionValue_ - lastFunctionValue_);
  return tol < tolerance_;
}

/******************************************************************************/

double FunctionStopCondition::getCurrentTolerance() const
{
  if (callCount_ > burnin_)
    return NumTools::abs<double>(newFunctionValue_ - lastFunctionValue_);
  else
    return std::max(tolerance_, 1.);
}

/******************************************************************************/

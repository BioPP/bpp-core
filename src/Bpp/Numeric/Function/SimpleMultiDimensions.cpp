// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

/******************************************************************************/

#include "SimpleMultiDimensions.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

SimpleMultiDimensions::SimpleMultiDimensions(std::shared_ptr<FunctionInterface> function) :
  AbstractOptimizer(function), nbParams_(0), optimizer_(function)
{
  setDefaultStopCondition_(make_shared<FunctionStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
  setOptimizationProgressCharacter("");
}

/******************************************************************************/

void SimpleMultiDimensions::setFunction(std::shared_ptr<FunctionInterface> function)
{
  AbstractOptimizer::setFunction(function);
  optimizer_.setFunction(function);
  getStopCondition()->init();
}

/******************************************************************************/

void SimpleMultiDimensions::doInit(const ParameterList& params)
{
  nbParams_ = params.size();
  if (nbParams_ == 0)
    return;

  // Initialize optimizers:
  unsigned int nbEvalMax = nbEvalMax_ / static_cast<unsigned int>(nbParams_);
  optimizer_.setMaximumNumberOfEvaluations(nbEvalMax);
  optimizer_.setProfiler(getProfiler());
  optimizer_.setMessageHandler(getMessageHandler());
  optimizer_.getStopCondition()->setTolerance(getStopCondition()->getTolerance());
  optimizer_.setConstraintPolicy(getConstraintPolicy());
  optimizer_.setVerbose(getVerbose() > 0 ? getVerbose() - 1 : 0);
  optimizer_.setInitialInterval(0., 1.);
  getFunction()->setParameters(getParameters());
}

/******************************************************************************/

double SimpleMultiDimensions::doStep()
{
  double f = getFunction()->getValue();
  for (unsigned int i = 0; i < nbParams_; i++)
  {
    if (getVerbose() > 0)
    {
      cout << getParameters()[i].getName() << ":";
      cout.flush();
    }
    // Re-init optimizer according to new values:
    double v = getParameters()[i].getValue();
    double t = std::max(0.000001, std::min(std::abs(v), getStopCondition()->getTolerance()));
    optimizer_.setInitialInterval(v - t, v + t);
    optimizer_.init(getParameters().createSubList(i));

    // Optimize through this dimension:
    f = optimizer_.optimize();
    if (getVerbose() > 0)
      cout << endl;
    getParameters_().matchParametersValues(getFunction()->getParameters());
    nbEval_ += optimizer_.getNumberOfEvaluations();
  }
  tolIsReached_ = nbParams_ <= 1;
  return f;
}

/******************************************************************************/

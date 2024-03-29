// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../App/ApplicationTools.h"
#include "../../Text/TextTools.h"
#include "../AutoParameter.h"
#include "AbstractOptimizer.h"

// From the STL:
#include <iomanip>
#include <time.h>

using namespace std;
using namespace bpp;

/******************************************************************************/

AbstractOptimizer::AbstractOptimizer(std::shared_ptr<FunctionInterface> function) :
  function_(function),
  parameters_(),
  messageHandler_(ApplicationTools::message),
  profiler_(ApplicationTools::message),
  constraintPolicy_(AutoParameter::CONSTRAINTS_KEEP),
  stopCondition_(0), defaultStopCondition_(0),
  verbose_(true), isInitialized_(false), startTime_(), listeners_(),
  updateParameters_(false), stepChar_("*"),
  nbEvalMax_(1000000), nbEval_(0),
  currentValue_(0), tolIsReached_(false)
{}

/******************************************************************************/

AbstractOptimizer::AbstractOptimizer(const AbstractOptimizer& opt) :
  function_(opt.function_),
  parameters_(opt.parameters_),
  messageHandler_(opt.messageHandler_),
  profiler_(opt.profiler_),
  constraintPolicy_(opt.constraintPolicy_),
  stopCondition_(0), defaultStopCondition_(0),
  verbose_(opt.verbose_),
  isInitialized_(opt.isInitialized_),
  startTime_(opt.startTime_),
  listeners_(), // We do not copy listeners!
  updateParameters_(opt.updateParameters_),
  stepChar_(opt.stepChar_),
  nbEvalMax_(opt.nbEvalMax_),
  nbEval_(opt.nbEval_),
  currentValue_(opt.currentValue_),
  tolIsReached_(opt.tolIsReached_)
{
  if (opt.stopCondition_)
  {
    stopCondition_ = shared_ptr<OptimizationStopCondition>(opt.stopCondition_->clone());
    stopCondition_->setOptimizer(this);
  }
  else
    stopCondition_        = nullptr;

  if (opt.defaultStopCondition_)
  {
    defaultStopCondition_ = shared_ptr<OptimizationStopCondition>(opt.defaultStopCondition_->clone());
    defaultStopCondition_->setOptimizer(this);
  }
  else
    defaultStopCondition_ = nullptr;
  // In case of AutoParameter instances, we must actualize the pointers toward _messageHandler:
  //
  if (isInitialized_)
  {
    if (constraintPolicy_ == AutoParameter::CONSTRAINTS_AUTO)
      autoParameter();
    else if (constraintPolicy_ == AutoParameter::CONSTRAINTS_IGNORE)
      ignoreConstraints();
  }
}

/******************************************************************************/

AbstractOptimizer& AbstractOptimizer::operator=(const AbstractOptimizer& opt)
{
  function_               = opt.function_;
  parameters_             = opt.parameters_;
  messageHandler_         = opt.messageHandler_;
  profiler_               = opt.profiler_;
  constraintPolicy_       = opt.constraintPolicy_;
  tolIsReached_           = opt.tolIsReached_;
  if (opt.stopCondition_)
  {
    stopCondition_        = shared_ptr<OptimizationStopCondition>(opt.stopCondition_->clone());
    stopCondition_->setOptimizer(this);
  }
  else
    stopCondition_        = nullptr;

  if (opt.defaultStopCondition_)
  {
    defaultStopCondition_ = shared_ptr<OptimizationStopCondition>(opt.defaultStopCondition_->clone());
    defaultStopCondition_->setOptimizer(this);
  }
  else
    defaultStopCondition_ = nullptr;

  nbEvalMax_              = opt.nbEvalMax_;
  nbEval_                 = opt.nbEval_;
  verbose_                = opt.verbose_;
  isInitialized_          = opt.isInitialized_;
  // In case of AutoParameter instances, we must actualize the pointers toward messageHandler_:
  if (isInitialized_)
  {
    if (constraintPolicy_ == AutoParameter::CONSTRAINTS_AUTO)
      autoParameter();
    else if (constraintPolicy_ == AutoParameter::CONSTRAINTS_IGNORE)
      ignoreConstraints();
  }
  startTime_              = opt.startTime_;
  listeners_.resize(0); // Reset listener list, do not copy it!
  updateParameters_       = opt.updateParameters_;
  stepChar_               = opt.stepChar_;
  return *this;
}

/******************************************************************************/

void AbstractOptimizer::init(const ParameterList& params)
{
  if (!function_)
    throw Exception("AbstractOptimizer::init. Optimizer currently has no function.");
  // We do this in order to keep original constraints:
  parameters_ = params;
  // More secure, but too slow:
  // parameters_ = function_->getParameters().createSubList(params.getParameterNames());
  // parameters_.matchParametersValues(params);
  if (constraintPolicy_ == AutoParameter::CONSTRAINTS_AUTO)
    autoParameter();
  else if (constraintPolicy_ == AutoParameter::CONSTRAINTS_IGNORE)
    ignoreConstraints();
  doInit(params);
  nbEval_ = 0;
  tolIsReached_ = false;
  isInitialized_ = true;
  time(&startTime_);
  currentValue_ = function_->getValue();

  profile("Step\t");
  for (unsigned int i = 0; i < parameters_.size(); i++)
  {
    profile(parameters_[i].getName() + "\t");
  }
  profileln("Function\tTime");

  // Parameters must be assigned by doInit:

  printPoint(parameters_, currentValue_);

  // Initialize the StopCondition:
  stopCondition_->init();
  fireOptimizationInitializationPerformed(OptimizationEvent(this));
}

/******************************************************************************/

double AbstractOptimizer::step()
{
  currentValue_ = doStep();
  printPoint(parameters_, currentValue_);
  fireOptimizationStepPerformed(OptimizationEvent(this));
  if (listenerModifiesParameters())
  {
    if (!updateParameters_)
      parameters_.matchParametersValues(function_->getParameters());
    // else already done!
    // _currentValue = function_->getValue();
    // Often useless, but avoid some bizare behaviour in particular cases:
    currentValue_ = function_->f(parameters_);
  }
  tolIsReached_ = tolIsReached_ || stopCondition_->isToleranceReached();
  return currentValue_;
}

/**************************************************************************/

double AbstractOptimizer::optimize()
{
  if (!isInitialized_)
    throw Exception("AbstractOptimizer::optimize. Optimizer not initialized: call the 'init' method first!");
  tolIsReached_ = false;
  for (nbEval_ = 1; nbEval_ < nbEvalMax_ && !tolIsReached_; nbEval_++)
  {
    if (verbose_)
      ApplicationTools::displayUnlimitedGauge(nbEval_, "Optimizing... ");
    step();
  }
  return currentValue_;
}

/******************************************************************************/

void AbstractOptimizer::profile(double v)
{
  if (profiler_)
    *profiler_ << v;
}

/******************************************************************************/

void AbstractOptimizer::profileln(double v)
{
  if (profiler_)
    (*profiler_ << v).endLine();
}

/******************************************************************************/

void AbstractOptimizer::profile(unsigned int v)
{
  if (profiler_)
    *profiler_ << v;
}
/******************************************************************************/

void AbstractOptimizer::profileln(unsigned int v)
{
  if (profiler_)
    (*profiler_ << v).endLine();
}

/******************************************************************************/

void AbstractOptimizer::profile(const std::string& s)
{
  if (profiler_)
    *profiler_ << s;
}

/******************************************************************************/

void AbstractOptimizer::profileln(const std::string& s)
{
  if (profiler_)
    (*profiler_ << s).endLine();
}

/******************************************************************************/

void AbstractOptimizer::printPoint(const ParameterList& params, double value)
{
  size_t ndim = params.size();
  profile(nbEval_);
  profile("\t");
  for (size_t j = 0; j < ndim; j++)
  {
    profile(TextTools::toString(params[j].getValue()));
    profile("\t");
  }
  profile(value);
  profile("\t");
  time_t seconds;
  time(&seconds);
  profileln(difftime(seconds, startTime_));
}

/******************************************************************************/

void AbstractOptimizer::printMessage(const std::string& message)
{
  if (messageHandler_)
    (*messageHandler_ << message).endLine();
}

/******************************************************************************/

void AbstractOptimizer::autoParameter()
{
  for (size_t i = 0; i < parameters_.size(); ++i)
  {
    AutoParameter ap(parameters_[i]);
    ap.setMessageHandler(messageHandler_);
    parameters_.setParameter(i, ap);
  }
}

/******************************************************************************/

void AbstractOptimizer::ignoreConstraints()
{
  for (size_t i = 0; i < parameters_.size(); ++i)
  {
    parameters_[i].removeConstraint();
  }
}

/******************************************************************************/

void AbstractOptimizer::fireOptimizationInitializationPerformed(const OptimizationEvent& event)
{
  for (auto listener : listeners_)
  {
    listener->optimizationInitializationPerformed(event);
  }
}

/******************************************************************************/

void AbstractOptimizer::fireOptimizationStepPerformed(const OptimizationEvent& event)
{
  for (auto listener : listeners_)
  {
    listener->optimizationStepPerformed(event);
  }
}

/******************************************************************************/

bool AbstractOptimizer::listenerModifiesParameters() const
{
  for (auto listener : listeners_)
  {
    if (listener->listenerModifiesParameters())
      return true;
  }
  return false;
}

/******************************************************************************/

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

/**************************************************************************/

#include "MetaOptimizer.h"
#include "../../App/ApplicationTools.h"

using namespace bpp;
using namespace std;

/**************************************************************************/

string MetaOptimizerInfos::IT_TYPE_STEP = "step";
string MetaOptimizerInfos::IT_TYPE_FULL = "full";

/**************************************************************************/

MetaOptimizer::MetaOptimizer(
    shared_ptr<FunctionInterface> function,
    unique_ptr<MetaOptimizerInfos> desc,
    unsigned int n) :
  AbstractOptimizer(function),
  optDesc_(std::move(desc)), optParameters_(optDesc_->getNumberOfOptimizers()),
  nbParameters_(optDesc_->getNumberOfOptimizers()), n_(n),
  precisionStep_(-1.), stepCount_(0), initialValue_(-1.)
{
  setDefaultStopCondition_(make_shared<FunctionStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
  precisionStep_ = log10(getStopCondition()->getTolerance()) / n_;
  setOptimizationProgressCharacter("");
}

/**************************************************************************/

MetaOptimizer::MetaOptimizer(
    const MetaOptimizer& opt) :
  AbstractOptimizer(opt),
  optDesc_(dynamic_cast<MetaOptimizerInfos*>(opt.optDesc_->clone())),
  optParameters_(opt.optParameters_),
  nbParameters_(opt.nbParameters_),
  n_(opt.n_),
  precisionStep_(opt.precisionStep_),
  stepCount_(opt.stepCount_),
  initialValue_(opt.initialValue_)
{}

/**************************************************************************/

MetaOptimizer& MetaOptimizer::operator=(
    const MetaOptimizer& opt)
{
  AbstractOptimizer::operator=(opt);
  optDesc_.reset(dynamic_cast<MetaOptimizerInfos*>(opt.optDesc_->clone()));
  optParameters_ = opt.optParameters_;
  nbParameters_  = opt.nbParameters_;
  n_             = opt.n_;
  precisionStep_ = opt.precisionStep_;
  stepCount_     = opt.stepCount_;
  initialValue_  = opt.initialValue_;
  return *this;
}

/**************************************************************************/

MetaOptimizer::~MetaOptimizer() {}

/**************************************************************************/

void MetaOptimizer::doInit(const ParameterList& parameters)
{
  optParameters_.resize(optDesc_->getNumberOfOptimizers());
  for (unsigned int i = 0; i < optDesc_->getNumberOfOptimizers(); ++i)
  {
    optParameters_[i].reset();
    for (size_t j = 0; j < optDesc_->getParameterNames(i).size(); ++j)
    {
      string pname = optDesc_->getParameterNames(i)[j];
      if (parameters.hasParameter(pname))
      {
        optParameters_[i].addParameter(parameters.parameter(pname));
      }
    }
    nbParameters_[i] = optParameters_[i].size();
  }

  // Initialize optimizers:
  for (unsigned int i = 0; i < optDesc_->getNumberOfOptimizers(); ++i)
  {
    if (nbParameters_[i] > 0)
    {
      OptimizerInterface& opt = optDesc_->optimizer(i);
      opt.updateParameters(updateParameters());
      opt.setProfiler(getProfiler());
      opt.setMessageHandler(getMessageHandler());
      opt.setConstraintPolicy(getConstraintPolicy());
      opt.setVerbose(getVerbose() > 0 ? getVerbose() - 1 : 0);
    }
  }

  // Actualize parameters:
  getParameters_().matchParametersValues(getFunction()->getParameters());

  getFunction()->setParameters(getParameters());
  initialValue_ = getFunction()->getValue();
  // Reset counter:
  stepCount_ = 1;
  // Recompute step if precision has changed:
  precisionStep_ = (log10(getStopCondition()->getTolerance()) - log10(initialValue_)) / n_;
}

/**************************************************************************/

double MetaOptimizer::doStep()
{
  stepCount_++;

  int tolTest = 0;
  double tol = getStopCondition()->getTolerance();
  if (stepCount_ <= n_)
  {
    tol = initialValue_ * pow(10, stepCount_ * precisionStep_);
  }

  for (unsigned int i = 0; i < optDesc_->getNumberOfOptimizers(); ++i)
  {
    if (nbParameters_[i] > 0)
    {
      if (getVerbose() > 1 && ApplicationTools::message)
      {
        (ApplicationTools::message->endLine() << optDesc_->getName(i)).endLine();
        ApplicationTools::message->flush();
      }
      if (optDesc_->requiresFirstOrderDerivatives(i))
        dynamic_cast<FirstOrderDerivable&>(function()).enableFirstOrderDerivatives(true);
      if (optDesc_->requiresSecondOrderDerivatives(i))
        dynamic_cast<SecondOrderDerivable&>(function()).enableSecondOrderDerivatives(true);

      optParameters_[i].matchParametersValues(getParameters());
      OptimizerInterface& opt = optDesc_->optimizer(i);
      opt.getStopCondition()->setTolerance(tol);
      opt.init(optParameters_[i]);
      if (optDesc_->getIterationType(i) == MetaOptimizerInfos::IT_TYPE_STEP)
        opt.step();
      else if (optDesc_->getIterationType(i) == MetaOptimizerInfos::IT_TYPE_FULL)
        opt.optimize();
      else
        throw Exception("MetaOptimizer::step. Unknown iteration type specified.");
      nbEval_ += opt.getNumberOfEvaluations();
      if (optDesc_->requiresFirstOrderDerivatives(i))
        dynamic_cast<FirstOrderDerivable&>(function()).enableFirstOrderDerivatives(false);
      if (optDesc_->requiresSecondOrderDerivatives(i))
        dynamic_cast<SecondOrderDerivable&>(function()).enableSecondOrderDerivatives(false);
      if (getVerbose() > 1)
        cout << endl;

      getParameters_().matchParametersValues(opt.getParameters());
    }
    tolTest += nbParameters_[i] > 0 ? 1 : 0;
  }
  tolIsReached_ = (tolTest == 1);

  return getFunction()->getValue();
}

/**************************************************************************/

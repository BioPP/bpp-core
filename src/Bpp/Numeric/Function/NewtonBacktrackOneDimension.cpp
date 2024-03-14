// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Text/TextTools.h"
#include "../NumTools.h"
#include "NewtonBacktrackOneDimension.h"

using namespace bpp;

/******************************************************************************/

NewtonBacktrackOneDimension::NewtonBacktrackOneDimension(std::shared_ptr<FunctionInterface> function, double slope, double test) :
  AbstractOptimizer(function),
  fold_(0), f_(0), a_(0), alam_(0), alamin_(0), alam2_(0), b_(0), disc_(0), f2_(0), rhs1_(0), rhs2_(0), slope_(slope), test_(test), tmplam_(0)

{
  setDefaultStopCondition_(make_shared<NBODStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
  setMaximumNumberOfEvaluations(10000);
}

/******************************************************************************/

void NewtonBacktrackOneDimension::doInit(const ParameterList& params)
{
  // Set the initial value (no use here! Use setInitialValues() instead).
  if (params.size() != 1)
    throw Exception("NewtonBacktrackOneDimension::init(). This optimizer only deals with one parameter.");
  fold_ = getFunction()->f(getParameters());
  getStopCondition()->setTolerance(getStopCondition()->getTolerance() / test_);
  alamin_ = getStopCondition()->getTolerance();
  alam_ = 1;
}

/******************************************************************************/

double NewtonBacktrackOneDimension::doStep()
{
  if (alam_ < alamin_)
  {
    getParameter_(0).setValue(0);
    tolIsReached_ = true;
    return fold_;
  }

  getParameter_(0).setValue(alam_);
  f_ = getFunction()->f(getParameters());

  if (f_ <= fold_ + alam_ * 0.0001 * slope_)
  {
    tolIsReached_ = true;
    return f_;
  }

  if (alam_ == 1)
  {
    tmplam_ = -slope_ / (2.0 * (f_ - fold_ - slope_));
    f2_ = f_;
    alam_ = tmplam_ > 0.1 ? tmplam_ : 0.1;
    return f_;
  }

  rhs1_ = f_ - fold_ - alam_ * slope_;
  rhs2_ = f2_ - fold_ - alam2_ * slope_;

  a_ = (rhs1_ / (alam_ * alam_) - rhs2_ / (alam2_ * alam2_)) / (alam_ - alam2_);
  b_ = (-alam2_ * rhs1_ / (alam_ * alam_) + alam_ * rhs2_ / (alam2_ * alam2_)) / (alam_ - alam2_);

  if (a_ == 0.0)
    tmplam_ = -slope_ / (2.0 * b_);
  else
  {
    disc_ = b_ * b_ - 3.0 * a_ * slope_;
    if (disc_ < 0.0)
      tmplam_ = 0.5 * alam_;
    else if (b_ <= 0)
      tmplam_ = (-b_ + sqrt(disc_)) / (3.0 * a_);
    else
      tmplam_ = -slope_ / (b_ + sqrt(disc_));
  }
  if (tmplam_ > 0.5 * alam_)
    tmplam_ = 0.5 * alam_;

  alam2_ = alam_;
  f2_ = f_;
  alam_ = tmplam_ > 0.1 * alam_ ? tmplam_ : 0.1 * alam_;

  return f_;
}

/******************************************************************************/

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "ConstantDistribution.h"

using namespace bpp;

#include <iostream>

using namespace std;

/******************************************************************************/

ConstantDistribution::ConstantDistribution(double value) :
  AbstractDiscreteDistribution(1, "Constant."),
  value_(value)
{
  addParameter_(new Parameter("Constant.value", value));
  distribution_[value_] = 1; // One single class  with probability 1.
}

ConstantDistribution::ConstantDistribution(const ConstantDistribution& cd) :
  AbstractDiscreteDistribution(cd),
  value_(cd.value_)
{}

ConstantDistribution& ConstantDistribution::operator=(const ConstantDistribution& cd)
{
  AbstractDiscreteDistribution::operator=(cd);
  value_ = cd.value_;

  return *this;
}

/******************************************************************************/

void ConstantDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);

  value_ = getParameterValue("value");
  distribution_.clear();
  distribution_[value_] = 1; // One single class of rate 1 with probability 1.
}

/******************************************************************************/

void ConstantDistribution::restrictToConstraint(const ConstraintInterface& c)
{
  if (getNumberOfParameters() == 0)
    return;

  const IntervalConstraint* pi = dynamic_cast<const IntervalConstraint*>(&c);
  if (!pi)
    throw Exception("ConstantDistribution::restrictToConstraint: Non-interval exception");

  if (!pi->isCorrect(getParameterValue("value")))
    throw ConstraintException("Impossible to restrict to Constraint", &getParameter_("value"), getParameterValue("value"));

  AbstractDiscreteDistribution::restrictToConstraint(c);

  getParameter_("value").setConstraint(intMinMax_);
}

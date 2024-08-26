// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <cmath>
#include <algorithm>

#include "Parameter.h"

// From Utils:
#include "../Text/TextTools.h"

using namespace bpp;

#include <iostream>
using namespace std;

/******************************************************************************/

ParameterEvent::ParameterEvent(Parameter* parameter) : parameter_(parameter) {}

/** Constructors: *************************************************************/

Parameter::Parameter(const std::string& name, double value, std::shared_ptr<ConstraintInterface> constraint, double precision) :
  name_(name), value_(0), precision_(0), constraint_(constraint), listeners_()
{
  setValue(value);
  setPrecision(precision);
}

Parameter::Parameter(const Parameter& p) :
  name_(p.name_),
  value_(p.value_),
  precision_(p.precision_),
  constraint_(p.constraint_),
  listeners_(p.listeners_)
{}

Parameter& Parameter::operator=(const Parameter& p)
{
  name_           = p.name_;
  value_          = p.value_;
  precision_      = p.precision_;
  constraint_     = p.constraint_;
  listeners_      = p.listeners_;
  return *this;
}

/** Destructor: ***************************************************************/

Parameter::~Parameter() {}

/** Value: ********************************************************************/

void Parameter::setValue(double value)
{
  if (std::abs(value - value_) > precision_ / 2)
  {
    if (constraint_ && !constraint_->isCorrect(value))
      throw ConstraintException("Parameter::setValue", this, value);
    value_ = value;
    ParameterEvent event(this);
    fireParameterValueChanged(event);
  }
}

/** Precision: ********************************************************************/

void Parameter::setPrecision(double precision)
{
  precision_ = (precision < 0) ? 0 : precision;
}

/** Constraint: ***************************************************************/

void Parameter::setConstraint(std::shared_ptr<ConstraintInterface> constraint)
{
  if (constraint != nullptr && !constraint->isCorrect(value_))
    throw ConstraintException("Parameter::setConstraint", this, value_);

  constraint_ = constraint;
}


std::shared_ptr<ConstraintInterface> Parameter::removeConstraint()
{
  auto c = constraint_;
  constraint_ = nullptr;
  return c;
}

/******************************************************************************/

void Parameter::removeParameterListener(const std::string& listenerId)
{
  listeners_.erase(std::remove_if(listeners_.begin(), listeners_.end(),
      [&listenerId](std::shared_ptr<ParameterListener>& pl) {
    return pl->getId() == listenerId; // put your condition here
  }), listeners_.end());
}

/******************************************************************************/

bool Parameter::hasParameterListener(const std::string& listenerId)
{
  for (auto& listener : listeners_)
  {
    if (listener->getId() == listenerId)
      return true;
  }
  return false;
}

/******************************************************************************/

const std::shared_ptr<IntervalConstraint> Parameter::R_PLUS(new IntervalConstraint(true, 0, true));
const std::shared_ptr<IntervalConstraint> Parameter::R_PLUS_STAR(new IntervalConstraint(true, 0, false));
const std::shared_ptr<IntervalConstraint> Parameter::R_MINUS(new IntervalConstraint(false, 0, true));
const std::shared_ptr<IntervalConstraint> Parameter::R_MINUS_STAR(new IntervalConstraint(false, 0, false));
const std::shared_ptr<IntervalConstraint> Parameter::PROP_CONSTRAINT_IN(new IntervalConstraint(0, 1, true, true));
const std::shared_ptr<IntervalConstraint> Parameter::PROP_CONSTRAINT_EX(new IntervalConstraint(0, 1, false, false));

/******************************************************************************/

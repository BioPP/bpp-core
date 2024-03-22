// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <typeinfo>

#include "../../App/ApplicationTools.h"
#include "ReparametrizationFunctionWrapper.h"

using namespace bpp;
using namespace std;

void ReparametrizationFunctionWrapper::init_(bool verbose)
{
  for (size_t i = 0; i < functionParameters_.size(); i++)
  {
    Parameter& p = functionParameters_[i];
    auto constraint = p.getConstraint();
    const string name = p.getName();
    double value = p.getValue();
    if (!constraint)
    {
      if (verbose)
        ApplicationTools::displayMessage("Parameter " + name + " does not need to be transformed.");
      addParameter_(new PlaceboTransformedParameter(name, value));
    }
    else
    {
      auto interval = dynamic_pointer_cast<IntervalConstraint>(constraint);
      if (interval)
      {
        bool isInfinite = (!interval->finiteLowerBound()) || (!interval->finiteUpperBound());
        if (!isInfinite)
        {
          if (!interval->strictLowerBound() && !interval->strictUpperBound())
          {
            // Case 1: [a,b]
            // This solves an issue if the original value is at the bound.
            double correctedValue = value;
            if (abs(value - interval->getLowerBound()) < NumConstants::TINY())
              correctedValue = interval->getLowerBound() + NumConstants::TINY();
            if (abs(value - interval->getUpperBound()) < NumConstants::TINY())
              correctedValue = interval->getUpperBound() - NumConstants::TINY();
            IntervalTransformedParameter* pp = new IntervalTransformedParameter(name, correctedValue, interval->getLowerBound(), interval->getUpperBound());
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was tanh transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
          else if (interval->strictLowerBound() && interval->strictUpperBound())
          {
            // Case 2: ]a,b[
            // We have to correct the bound in order to prevent numerical issues.
            // It can happens that the original bound is matched because of rounding errors.
            IntervalTransformedParameter* pp = new IntervalTransformedParameter(name, value, interval->getLowerBound() + NumConstants::TINY(), interval->getUpperBound() - NumConstants::TINY());
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was tanh transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
          else if (!interval->strictLowerBound() && interval->strictUpperBound())
          {
            // Case 3: [a,b[
            // This solves an issue if the original value is at the bound.
            double correctedValue = value;
            if (abs(value - interval->getLowerBound()) < NumConstants::TINY())
              correctedValue = interval->getLowerBound() + NumConstants::TINY();
            IntervalTransformedParameter* pp = new IntervalTransformedParameter(name, correctedValue, interval->getLowerBound(), interval->getUpperBound() - NumConstants::TINY());
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was tanh transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
          else if (interval->strictLowerBound() && !interval->strictUpperBound())
          {
            // Case 4: ]a,b]
            // This solve an issue if the original value is at the bound.
            double correctedValue = value;
            if (abs(value - interval->getUpperBound()) < NumConstants::TINY())
              correctedValue = interval->getUpperBound() - NumConstants::TINY();
            IntervalTransformedParameter* pp = new IntervalTransformedParameter(name, correctedValue, interval->getLowerBound() + NumConstants::TINY(), interval->getUpperBound());
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was tanh transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
        }
        else
        {
          if (interval->strictLowerBound() && !interval->finiteUpperBound())
          {
            // Case 5: ]a, +inf[
            RTransformedParameter* pp = new RTransformedParameter(name, value, interval->getLowerBound() + NumConstants::TINY(), true);
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was log transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
          else if (!interval->strictLowerBound() && !interval->finiteUpperBound())
          {
            // Case 6: [a, +inf[
            // This solve an issue if the original value is at the bound.
            double correctedValue = value;
            if (abs(value - interval->getLowerBound()) < NumConstants::TINY())
              correctedValue = interval->getLowerBound() + NumConstants::TINY();
            RTransformedParameter* pp = new RTransformedParameter(name, correctedValue, interval->getLowerBound(), true);
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was log transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
          else if (!interval->finiteLowerBound() && interval->strictUpperBound())
          {
            // Case 7: ]-inf, a[
            RTransformedParameter* pp = new RTransformedParameter(name, value, interval->getUpperBound() - NumConstants::TINY(), false);
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was log transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
          else if (!interval->finiteLowerBound() && !interval->strictUpperBound())
          {
            // Case 8: ]-inf, a]
            // This solve an issue if the original value is at the bound.
            double correctedValue = value;
            if (abs(value - interval->getUpperBound()) < NumConstants::TINY())
              correctedValue = interval->getUpperBound() - NumConstants::TINY();
            RTransformedParameter* pp = new RTransformedParameter(name, correctedValue, interval->getUpperBound(), false);
            addParameter_(pp);
            if (verbose)
              ApplicationTools::displayMessage("Parameter " + name + " was log transformed: " + TextTools::toString(value) + "->" + TextTools::toString(pp->getValue()));
          }
        }
      }
      else
      {
        // Nothing found
        if (verbose)
        {
          ApplicationTools::displayWarning("No transformation found for this constraint '" + constraint->getDescription() + "'! Parameter " + p.getName());
        }
        addParameter_(new PlaceboTransformedParameter(name, value));
      }
    }
  }
}

void ReparametrizationFunctionWrapper::fireParameterChanged(const ParameterList& parameters)
{
  // Recompute function parameters:
  // We could update only the parameter that actually changed,
  // but that would implied a quick sort on parameter names (nlog(n))
  // whereas using a loop over the set is in o(n). It should hence be
  // more efficient in most cases.
  for (size_t i = 0; i < getNumberOfParameters(); ++i)
  {
    double x = dynamic_cast<TransformedParameter&>(getParameter_(i)).getOriginalValue();
    try
    {
      functionParameters_[i].setValue(x);
    }
    catch (ConstraintException& ce)
    {
      (*ApplicationTools::error << "Oups, value " << x << " led to a constraint exception. The transformed value was " << getParameter_(i).getValue()).endLine();
      throw ce;
    }
  }
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <iostream>

#include "AutoParameter.h"
#include "NumConstants.h"

using namespace std;

// Utils:
#include "../Text/TextTools.h"
#include "../App/ApplicationTools.h"

using namespace bpp;

/******************************************************************************/

string AutoParameter::CONSTRAINTS_AUTO   = "auto";
string AutoParameter::CONSTRAINTS_IGNORE = "ignore";
string AutoParameter::CONSTRAINTS_KEEP   = "keep";

/** Constructors: *************************************************************/

AutoParameter::AutoParameter(const std::string& name, double value, std::shared_ptr<ConstraintInterface> constraint) :
  Parameter(name, value, constraint), messageHandler_(ApplicationTools::message) {}

AutoParameter::AutoParameter(const Parameter& p) : Parameter(p), messageHandler_(ApplicationTools::message) {}

AutoParameter::AutoParameter(const AutoParameter& p) : Parameter(p), messageHandler_(p.messageHandler_) {}

AutoParameter& AutoParameter::operator=(const AutoParameter& p)
{
  Parameter::operator=(p);
  messageHandler_ = p.messageHandler_;
  return *this;
}

/******************************************************************************/

void AutoParameter::setValue(double value)
{
  try
  {
    // First we try to assign this value:
    Parameter::setValue(value);
  }
  catch (ConstraintException& ce)
  {
    // Aie, there's a pb here...
    if (messageHandler_)
    {
      (*messageHandler_) << "Constraint match at parameter ";
      (*messageHandler_) << name_;
      (*messageHandler_) << ", badValue = ";
      (*messageHandler_) << ce.getBadValue();
      (*messageHandler_) << " ";
      (*messageHandler_ << constraint_->getDescription()).endLine();
    }
    double limit = constraint_->getAcceptedLimit(value);
    try
    { // We try to assign the limit then.
      Parameter::setValue(limit);
    }
    catch (ConstraintException& ce2)
    {
      // Aie, the limit is not reachable, so we perform a smaller step...
      // Parameter::setValue((getValue() + limit) / 2);
      try
      {
        // Try on the right:
        Parameter::setValue(limit + NumConstants::TINY());
      }
      catch (ConstraintException& ce3)
      {
        // Try on the left:
        Parameter::setValue(limit - NumConstants::TINY());
      }
    }
  }
}

/******************************************************************************/

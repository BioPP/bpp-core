// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "Parameter.h"
#include "ParameterExceptions.h"

// From Utils:
#include "../Text/TextTools.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

ParameterException::ParameterException(const std::string& text, const Parameter* param) :
  Exception("ParameterException: " + text + (param != 0 ? "(" + param->getName() + ")" : string(""))),
  parameter_(param) {}

const Parameter* ParameterException::parameter() const { return parameter_; }

/******************************************************************************/

ConstraintException::ConstraintException(const std::string& text, const Parameter* param, double badValue) :
  ParameterException("ConstraintException: " + text + "(" + TextTools::toString(badValue) + ")"
                     + (param->hasConstraint() ? param->getConstraint()->getDescription() : "no constraint"), param),
  badValue_(badValue) {}

double ConstraintException::getBadValue() const { return badValue_; }

/******************************************************************************/

ParameterNotFoundException::ParameterNotFoundException(const string& text, const string& param) :
  Exception("ParameterNotFoundException: " + text + (!TextTools::isEmpty(param) ? "(" + param + ")" : string(""))),
  parameter_(param) {}

std::string ParameterNotFoundException::parameter() const { return parameter_; }

/******************************************************************************/

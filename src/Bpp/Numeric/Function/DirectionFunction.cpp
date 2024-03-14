// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "DirectionFunction.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

void DirectionFunction::setParameters(const ParameterList& params)
{
  params_ = params;
  double x = params_[0].getValue();
  for (unsigned int j = 0; j < p_.size(); j++)
  {
    //      cout << p_[j].getValue() << " " << x << " " << xi_[j] << endl;
    xt_[j].setValue((p_[j].getValue()) + x * xi_[j]);
  }
  function_->setParameters(xt_);
}

/******************************************************************************/

double DirectionFunction::getValue() const
{
  return function_->getValue();
}

/******************************************************************************/

const ParameterList& DirectionFunction::getParameters() const
{
  return params_;
}

ParameterList& DirectionFunction::getParameters_()
{
  return params_;
}

/******************************************************************************/

void DirectionFunction::init(const ParameterList& p, const vector<double>& xi)
{
  p_ = p;
  xi_ = xi;
  if (constraintPolicy_ == AutoParameter::CONSTRAINTS_AUTO)
    autoParameter();
  else if (constraintPolicy_ == AutoParameter::CONSTRAINTS_IGNORE)
    ignoreConstraints();
  xt_ = p_;
}

/******************************************************************************/

void DirectionFunction::autoParameter()
{
  for (unsigned int i = 0; i < p_.size(); ++i)
  {
    AutoParameter ap(p_[i]);
    ap.setMessageHandler(messenger_);
    p_.setParameter(i, ap);
  }
}

/******************************************************************************/

void DirectionFunction::ignoreConstraints()
{
  for (unsigned int i = 0; i < p_.size(); i++)
  {
    p_[i].removeConstraint();
  }
}

/******************************************************************************/

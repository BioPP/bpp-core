// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "BfgsMultiDimensions.h"
#include "OneDimensionOptimizationTools.h"

using namespace bpp;

/******************************************************************************/

BfgsMultiDimensions::BfgsMultiDimensions(std::shared_ptr<FirstOrderDerivable> function) :
  AbstractOptimizer(function),
  // gtol_(gtol),
  slope_(0),
  Up_(),
  Lo_(),
  p_(),
  gradient_(),
  xi_(),
  dg_(),
  hdg_(),
  hessian_(),
  f1dim_(new DirectionFunction(function))
{
  setDefaultStopCondition_(make_shared<FunctionStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
  setOptimizationProgressCharacter(".");
}

/******************************************************************************/

void BfgsMultiDimensions::doInit(const ParameterList& params)
{
  size_t nbParams = params.size();
  p_.resize(nbParams);
  gradient_.resize(nbParams);
  xi_.resize(nbParams);
  dg_.resize(nbParams);
  hdg_.resize(nbParams);
  Up_.resize(nbParams);
  Lo_.resize(nbParams);

  hessian_.resize(nbParams);
  for (size_t i = 0; i < nbParams; i++)
  {
    hessian_[i].resize(nbParams);
  }

  for (size_t i = 0; i < nbParams; i++)
  {
    auto cp = params[i].getConstraint();
    if (!cp)
    {
      Up_[i] = NumConstants::VERY_BIG();
      Lo_[i] = -NumConstants::VERY_BIG();
    }
    else
    {
      Up_[i] = cp->getAcceptedLimit(NumConstants::VERY_BIG()) - NumConstants::TINY();
      Lo_[i] = cp->getAcceptedLimit(-NumConstants::VERY_BIG()) + NumConstants::TINY();
    }
  }

  firstOrderDerivableFunction().enableFirstOrderDerivatives(true);
  function().setParameters(params);

  getGradient(gradient_);

  for (size_t i = 0; i < nbParams; ++i)
  {
    p_[i] = getParameters()[i].getValue();

    for (size_t j = 0; j < nbParams; ++j)
    {
      hessian_[i][j] = 0.0;
    }
    hessian_[i][i] = 1.0;
  }


  // jdutheil 01/09/22: what is this for?
  // double sum = 0;
  // for (size_t i = 0; i < nbParams; i++)
  // {
  //  sum += p_[i] * p_[i];
  // }
}

/******************************************************************************/

double BfgsMultiDimensions::doStep()
{
  double f;
  size_t n = getParameters().size();
  // Loop over iterations.

  unsigned int i;

  for (i = 0; i < n; ++i)
  {
    p_[i] = getParameters()[i].getValue();
  }

  setDirection();

  firstOrderDerivableFunction().enableFirstOrderDerivatives(false);
  nbEval_ += OneDimensionOptimizationTools::lineSearch(f1dim_,
        getParameters_(), xi_,
        gradient_,
        // getStopCondition()->getTolerance(),
        0, 0,
        getVerbose() > 0 ? getVerbose() - 1 : 0);
  firstOrderDerivableFunction().enableFirstOrderDerivatives(true);

  for (i = 0; i < n; ++i)
  {
    xi_[i] = getParameters_()[i].getValue() - p_[i];
  }

  f = getFunction()->f(getParameters());
  if (f > currentValue_)
  {
    printMessage("!!! Function increase !!!");
    printMessage("!!! Optimization might have failed. Try to reparametrize your function to remove constraints.");
    tolIsReached_ = true;
    return f;
  }

  if (tolIsReached_)
  {
    return f;
  }

  // double temp, test = 0.0;
  // for (i = 0; i < n; i++)
  // {
  //  temp = xi_[i];
  //  if (p_[i] > 1.0)
  //    temp /= p_[i];
  //  if (temp > test)
  //    test = temp;
  // }

  // if (test < 1e-7)
  // {
  //  tolIsReached_ = true;
  //  return f;
  // }

  for (i = 0; i < n; i++)
  {
    dg_[i] = gradient_[i];
  }

  getGradient(gradient_);
  // test = 0.0;

  // for (i = 0; i < n; i++)
  // {
  //  temp = abs(gradient_[i]);
  //  if (abs(p_[i]) > 1.0)
  //    temp /= p_[i];
  //  if (temp > test)
  //    test = temp;
  // }

  // if (f > 1.0)
  //  test /= f;

  // if (test < gtol_)
  // {
  //  tolIsReached_ = true;
  //  return f;
  // }

  for (i = 0; i < n; i++)
  {
    dg_[i] = gradient_[i] - dg_[i];
  }

  for (i = 0; i < n; i++)
  {
    hdg_[i] = 0;
    for (unsigned int j = 0; j < n; j++)
    {
      hdg_[i] += hessian_[i][j] * dg_[j];
    }
  }

  double fae(0), fac(0), sumdg(0), sumxi(0);

  for (i = 0; i < n; i++)
  {
    fac += dg_[i] * xi_[i];
    fae += dg_[i] * hdg_[i];
    sumdg += dg_[i] * dg_[i];
    sumxi += xi_[i] * xi_[i];
  }

  if (fac > sqrt(1e-7 * sumdg * sumxi))
  {
    fac = 1.0 / fac;
    double fad = 1.0 / fae;
    for (i = 0; i < n; i++)
    {
      dg_[i] = fac * xi_[i] - fad * hdg_[i];
    }
    for (i = 0; i < n; i++)
    {
      for (unsigned int j = i; j < n; j++)
      {
        hessian_[i][j] += fac * xi_[i] * xi_[j] - fad * hdg_[i] * hdg_[j] + fae * dg_[i] * dg_[j];
        hessian_[j][i] = hessian_[i][j];
      }
    }
  }

  return f;
}

/******************************************************************************/

void BfgsMultiDimensions::getGradient(std::vector<double>& gradient) const
{
  for (unsigned int i = 0; i < gradient.size(); i++)
  {
    gradient[i] = firstOrderDerivableFunction().getFirstOrderDerivative(getParameters()[i].getName());
  }
}

/******************************************************************************/

void BfgsMultiDimensions::setDirection()
{
  size_t nbParams = getParameters().size();

  for (size_t i = 0; i < nbParams; ++i)
  {
    xi_[i] = 0;
    for (size_t j = 0; j < nbParams; ++j)
    {
      xi_[i] -= hessian_[i][j] * gradient_[j];
    }
  }

  double v = 1, alpmax = 1;
  for (size_t i = 0; i < nbParams; ++i)
  {
    if ((xi_[i] > 0) && (p_[i] + NumConstants::TINY() * xi_[i] < Up_[i]))
      v = (Up_[i] - p_[i]) / xi_[i];
    else if ((xi_[i] < 0) && (p_[i] + NumConstants::TINY() * xi_[i] > Lo_[i]))
      v = (Lo_[i] - p_[i]) / xi_[i];
    if (v < alpmax)
      alpmax = v;
  }

  for (size_t i = 0; i < nbParams; i++)
  {
    if (p_[i] + NumConstants::TINY() * xi_[i] >= Up_[i])
      xi_[i] = Up_[i] - p_[i];
    else if (p_[i] + NumConstants::TINY() * xi_[i] <= Lo_[i])
      xi_[i] = Lo_[i] - p_[i];
    else
      xi_[i] *= alpmax;
  }
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../VectorTools.h"
#include "ConjugateGradientMultiDimensions.h"
#include "OneDimensionOptimizationTools.h"

using namespace bpp;

/******************************************************************************/

ConjugateGradientMultiDimensions::ConjugateGradientMultiDimensions(std::shared_ptr<FirstOrderDerivable> function) :
  AbstractOptimizer(function), optimizer_(function),
  xi_(), h_(), g_(), f1dim_(new DirectionFunction(function))
{
  setDefaultStopCondition_(make_shared<FunctionStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
}

/******************************************************************************/

void ConjugateGradientMultiDimensions::doInit(const ParameterList& params)
{
  size_t nbParams = params.size();
  g_.resize(nbParams);
  h_.resize(nbParams);
  xi_.resize(nbParams);
  firstOrderDerivableFunction().enableFirstOrderDerivatives(true);
  function().setParameters(params);
  getGradient(xi_);

  for (size_t i = 0; i < nbParams; ++i)
  {
    g_[i]  = -xi_[i];
    xi_[i] = h_[i] = g_[i];
  }
}

/******************************************************************************/

double ConjugateGradientMultiDimensions::doStep()
{
  double gg, gam, f, dgg;
  size_t n = getParameters().size();
  // Loop over iterations.
  firstOrderDerivableFunction().enableFirstOrderDerivatives(false);
  nbEval_ += OneDimensionOptimizationTools::lineMinimization(f1dim_,
                                                             getParameters_(), xi_, getStopCondition()->getTolerance(),
                                                             0, 0, getVerbose() > 0 ? getVerbose() - 1 : 0);

  firstOrderDerivableFunction().enableFirstOrderDerivatives(true);
  f = getFunction()->f(getParameters());

  if (tolIsReached_)
  {
    return f;
  }
  getGradient(xi_);

  dgg = gg = 0.0;
  for (unsigned j = 0; j < n; j++)
  {
    gg += g_[j] * g_[j];
    /* dgg += xi[j] * xi[j]; */ // This statement for Fletcher-Reeves.
    dgg += (xi_[j] + g_[j]) * xi_[j]; // This statement for Polak-Ribiere.
  }

  if (gg == 0.0)
  {
    // Unlikely. If gradient is exactly zero then
    return f;
  }
  gam = dgg / gg;

  if (!(std::isnan(gam) || std::isinf(gam)))
  {
    for (unsigned int j = 0; j < n; j++)
    {
      g_[j] = -xi_[j];
      xi_[j] = h_[j] = g_[j] + gam * h_[j];
    }
  }

  return f;
}

/******************************************************************************/

void ConjugateGradientMultiDimensions::getGradient(std::vector<double>& gradient) const
{
  for (size_t i = 0; i < gradient.size(); ++i)
  {
    gradient[i] = firstOrderDerivableFunction().getFirstOrderDerivative(getParameters()[i].getName());
  }
}

/******************************************************************************/

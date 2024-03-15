// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../NumConstants.h"
#include "../NumTools.h"
#include "BrentOneDimension.h"
#include "NewtonBacktrackOneDimension.h"
#include "OneDimensionOptimizationTools.h"

using namespace bpp;
using namespace std;

/******************************************************************************
*                              The Point class                               *
******************************************************************************/
inline void BracketPoint::set(double xval, double fval) { this->x = xval; this->f = fval; }

/******************************************************************************
*                             The Bracket class                              *
******************************************************************************/
inline void Bracket::setA(double xa, double fa) { a.set(xa, fa); }
inline void Bracket::setB(double xb, double fb) { b.set(xb, fb); }
inline void Bracket::setC(double xc, double fc) { c.set(xc, fc); }

/******************************************************************************/

Bracket OneDimensionOptimizationTools::bracketMinimum(
  double a,
  double b,
  FunctionInterface& function,
  ParameterList parameters)
{
  Bracket bracket;
  // Copy the parameter to use.
  bracket.a.x = a;
  parameters[0].setValue(bracket.a.x); bracket.a.f = function.f(parameters);
  bracket.b.x = b;
  parameters[0].setValue(bracket.b.x); bracket.b.f = function.f(parameters);

  while (std::isnan(bracket.b.f) || std::isinf(bracket.b.f))
  {
    bracket.b.x /= 1.1;
    parameters[0].setValue(bracket.b.x); bracket.b.f = function.f(parameters);
  }

  if (bracket.b.f > bracket.a.f)
  {
    // Switch roles of first and second point so that we can go downhill
    // in the direction from a to b.
    NumTools::swap<double>(bracket.a.x, bracket.b.x);
    NumTools::swap<double>(bracket.a.f, bracket.b.f);
  }

  // First guess for third point:
  bracket.c.x = bracket.b.x + NumConstants::GOLDEN_RATIO_PHI() * (bracket.b.x - bracket.a.x);
  parameters[0].setValue(bracket.c.x); bracket.c.f = function.f(parameters);


  // Keep returning here until we bracket:
  while (bracket.b.f > bracket.c.f)
  {
    // Compute xu by parabolic extrapolation from a, b, c. TINY is used to prevent
    // any possible division by 0.
    double r = (bracket.b.x - bracket.a.x) * (bracket.b.f - bracket.c.f);
    double q = (bracket.b.x - bracket.c.x) * (bracket.b.f - bracket.a.f);

    double xu = bracket.b.x - ((bracket.b.x - bracket.c.x) * q - (bracket.b.x - bracket.a.x) * r) /
                (2.0 * NumTools::sign(NumTools::max(NumTools::abs(q - r), NumConstants::VERY_TINY()), q - r));
    double xulim = (bracket.b.x) + GLIMIT * (bracket.c.x - bracket.b.x);
    double fu;

    // We don't go farther than this.
    // Test various possibilities:
    if ((bracket.b.x - xu) * (xu - bracket.c.x) > 0.0)
    {
      parameters[0].setValue(xu); fu = function.f(parameters);
      if (fu < bracket.c.f)
      {
        bracket.setA(bracket.b.x, bracket.b.f);
        bracket.setB(xu, fu);
        return bracket;
      }
      else if (fu > bracket.b.f)
      {
        bracket.setC(xu, fu);
        return bracket;
      }
      // Parabolic fit was no use.
      // Use default magnification.
      xu = bracket.c.x + NumConstants::GOLDEN_RATIO_PHI() * (bracket.c.x - bracket.b.x);
      parameters[0].setValue(xu); fu = function.f(parameters);
    }
    else if ((bracket.c.x - xu) * (xu - xulim) > 0.0)
    {
      // Parabolic fit is between point 3 and its allowed limit.
      parameters[0].setValue(xu); fu = function.f(parameters);
      if (fu < bracket.c.f)
      {
        NumTools::shift<double>(bracket.b.x, bracket.c.x, xu, bracket.c.x + NumConstants::GOLDEN_RATIO_PHI() * (bracket.c.x - bracket.b.x));
        parameters[0].setValue(xu);
        NumTools::shift<double>(bracket.b.f, bracket.c.f, fu, function.f(parameters));
      }
    }
    else if ((xu - xulim) * (xulim - bracket.c.x) >= 0.0)
    {
      // Limit parabolic xu to maximum allowed value.
      xu = xulim;
      parameters[0].setValue(xu); fu = function.f(parameters);
    }
    else
    {
      // Reject parabolic xu, use default magnification.
      xu = bracket.c.x + NumConstants::GOLDEN_RATIO_PHI() * (bracket.c.x - bracket.b.x);
      parameters[0].setValue(xu); fu = function.f(parameters);
    }
    // Eliminate oldest point and continue.
    NumTools::shift<double>(bracket.a.x, bracket.b.x, bracket.c.x, xu);
    NumTools::shift<double>(bracket.a.f, bracket.b.f, bracket.c.f, fu);
  }
  return bracket;
}

/******************************************************************************/

Bracket OneDimensionOptimizationTools::inwardBracketMinimum(
  double a,
  double b,
  FunctionInterface& function,
  ParameterList parameters,
  uint intervalsNum)
{
  Bracket bracket;
  // Copy the parameter to use.
  bracket.a.x = a;
  parameters[0].setValue(bracket.a.x); bracket.a.f = function.f(parameters);
  bracket.b.x = b;
  parameters[0].setValue(bracket.b.x); bracket.b.f = function.f(parameters);

  while (std::isnan(bracket.b.f) || std::isinf(bracket.b.f))
  {
    bracket.b.x /= 1.1;
    parameters[0].setValue(bracket.b.x); bracket.b.f = function.f(parameters);
  }

  double bestMiddleX, bestMiddleF;
  double curr, fcurr, jump;

  // look for bracket.c.x by scanning n possible assignments and assigining c the best one over the examined values
  curr = bracket.a.x;
  fcurr = bracket.a.f;
  bestMiddleX = (bracket.a.f < bracket.b.f ? bracket.a.x : bracket.b.x); // determine the currently optimal point with respect to f out of a and b
  bestMiddleF = (bracket.a.f < bracket.b.f ? bracket.a.f : bracket.b.f); // determine the currently optimal point with respect to f out of a and b
  jump = (b - a) / static_cast<double>(intervalsNum); // Determine the spacing appropriate to the mesh.
  for (size_t i = 1; i <= intervalsNum; i++)
  { // Loop over all intervals
    curr += jump;
    parameters[0].setValue(curr); fcurr = function.f(parameters);
    // If c yields better likelihood than a and b
    if (fcurr < bestMiddleF)
    {
      bestMiddleX = curr;
      bestMiddleF = fcurr;
    }
  }
  bracket.c.x = bestMiddleX;
  parameters[0].setValue(bracket.c.x); bracket.c.f = function.f(parameters);
  return bracket;
}

/******************************************************************************/

unsigned int OneDimensionOptimizationTools::lineMinimization(
  std::shared_ptr<DirectionFunction> f1dim,
  ParameterList& parameters,
  std::vector<double>& xi,
  double tolerance,
  std::shared_ptr<OutputStream> profiler,
  std::shared_ptr<OutputStream> messenger,
  unsigned int verbose)
{
  // Initial guess for brackets:
  double ax = 0.;
  double xx = 0.01;

  f1dim->setConstraintPolicy(AutoParameter::CONSTRAINTS_AUTO);
  f1dim->setMessageHandler(messenger);
  f1dim->init(parameters, xi);
  BrentOneDimension bod(f1dim);
  bod.setMessageHandler(messenger);
  bod.setProfiler(profiler);
  bod.setVerbose(verbose >= 1 ? 1 : 0);
  bod.setOptimizationProgressCharacter(".");
  bod.getStopCondition()->setTolerance(0.01);
  bod.setInitialInterval(ax, xx);
  bod.setConstraintPolicy(AutoParameter::CONSTRAINTS_KEEP);
  ParameterList singleParameter;
  singleParameter.addParameter(Parameter("x", 0.0));
  bod.init(singleParameter);
  bod.optimize();
  // Update parameters:
  // parameters.matchParametersValues(f1dim.getFunction()->getParameters());

  double xmin = f1dim->getParameters()[0].getValue();
  for (size_t j = 0; j < parameters.size(); ++j)
  {
    xi[j] *= xmin;
    parameters[j].setValue(parameters[j].getValue() + xi[j]);
  }
  return bod.getNumberOfEvaluations();
}

/******************************************************************************/

unsigned int OneDimensionOptimizationTools::lineSearch(
  std::shared_ptr<DirectionFunction> f1dim,
  ParameterList& parameters,
  std::vector<double>& xi,
  std::vector<double>& gradient,
  std::shared_ptr<OutputStream> profiler,
  std::shared_ptr<OutputStream> messenger,
  unsigned int verbose)
{
  size_t size = xi.size();

  f1dim->setConstraintPolicy(AutoParameter::CONSTRAINTS_AUTO);
  f1dim->setMessageHandler(messenger);
  f1dim->init(parameters, xi);

  double slope = 0;
  for (size_t i = 0; i < size; i++)
  {
    slope += xi[i] * gradient[i];
  }

  //  if (slope>=0)
  //  throw Exception("Slope problem in OneDimensionOptimizationTools::lineSearch. Slope="+TextTools::toString(slope));

  double x, temp, test = 0;
  for (size_t i = 0; i < size; ++i)
  {
    x = abs(parameters[i].getValue());
    temp = abs(xi[i]);
    if (x > 1.0)
      temp /= x;
    if (temp > test)
      test = temp;
  }

  NewtonBacktrackOneDimension nbod(f1dim, slope, test);

  nbod.setMessageHandler(messenger);
  nbod.setProfiler(profiler);
  nbod.setVerbose(verbose >= 1 ? 1 : 0);
  nbod.setOptimizationProgressCharacter(".");
  nbod.getStopCondition()->setTolerance(0.0001);
  //  nbod.setInitialInterval(ax, xx);
  nbod.setConstraintPolicy(AutoParameter::CONSTRAINTS_KEEP);
  ParameterList singleParameter;
  singleParameter.addParameter(Parameter("x", 0.0));
  nbod.init(singleParameter);
  nbod.optimize();
  // Update parameters:
  // parameters.matchParametersValues(f1dim.getFunction()->getParameters());

  double xmin = f1dim->getParameters()[0].getValue();
  for (unsigned int j = 0; j < parameters.size(); ++j)
  {
    xi[j] *= xmin;
    parameters[j].setValue(parameters[j].getValue() + xi[j]);
  }

  return nbod.getNumberOfEvaluations();
}

/******************************************************************************/

double OneDimensionOptimizationTools::GLIMIT = 100.0;

/******************************************************************************/

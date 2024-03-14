// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../NumTools.h"
#include "DownhillSimplexMethod.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

double DownhillSimplexMethod::DSMStopCondition::getCurrentTolerance() const
{
  const DownhillSimplexMethod* dsm = dynamic_cast<const DownhillSimplexMethod*>(optimizer_);
  double rTol = 2.0 * NumTools::abs(dsm->y_[dsm->iHighest_] - dsm->y_[dsm->iLowest_]) /
                (NumTools::abs(dsm->y_[dsm->iHighest_]) + NumTools::abs(dsm->y_[dsm->iLowest_]));
  return rTol;
}

/******************************************************************************/

DownhillSimplexMethod::DownhillSimplexMethod(std::shared_ptr<FunctionInterface> function) :
  AbstractOptimizer(function), simplex_(), y_(), pSum_(), iHighest_(0), iNextHighest_(0), iLowest_(0)
{
  // Default values:
  nbEvalMax_ = 5000;
  setDefaultStopCondition_(make_shared<DSMStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
}

/******************************************************************************/

void DownhillSimplexMethod::doInit(const ParameterList& params)
{
  size_t nDim = getParameters().size();
  nbEval_ = 0;

  // Initialize the simplex:
  simplex_.resize(nDim + 1);
  y_.resize(nDim + 1);
  double lambda = 0.2; // 20% of the parameter value.
  for (unsigned int i = 1; i < nDim + 1; i++)
  {
    // Copy the vector...
    simplex_[i] = getParameters();
    // ... and set the initial values.
    for (unsigned int j = 0; j < nDim; j++)
    {
      // Hummm... that does not work when the first point is 0!!! where does this come from???
      // simplex_[i][j].setValue(getParameters()[j].getValue() * (1. + (j == i - 1 ? lambda : 0.)));
      simplex_[i][j].setValue(getParameters()[j].getValue() + (j == i - 1 ? lambda : 0.));
    }
    // Compute the corresponding f value:
    y_[i] = getFunction()->f(simplex_[i]);
    nbEval_++;
  }
  // Last function evaluation, setting current value:
  simplex_[0] = getParameters();
  y_[0] = getFunction()->f(simplex_[0]);
  nbEval_++;

  pSum_ = getPSum();
}

/******************************************************************************/

double DownhillSimplexMethod::doStep()
{
  // The number of dimensions of the parameter space:
  size_t nDim = simplex_.getDimension();
  size_t mpts = nDim + 1;

  iLowest_ = 0;
  // First we must determine which point is the highest (worst),
  // next-highest, and lowest (best), by looping over the points
  // in the simplex.
  if (y_[0] > y_[1])
  {
    iHighest_ = 0;
    iNextHighest_ = 1;
  }
  else
  {
    iHighest_ = 1;
    iNextHighest_ = 0;
  }

  for (unsigned int i = 0; i < mpts; i++)
  {
    if (y_[i] <= y_[iLowest_])
      iLowest_ = i;
    if (y_[i] > y_[iHighest_])
    {
      iNextHighest_ = iHighest_;
      iHighest_ = i;
    }
    else if (y_[i] > y_[iNextHighest_] && i != iHighest_)
      iNextHighest_ = i;
  }

  // Set current best point:
  getParameters_() = simplex_[iLowest_];

  // Begin a new iteration.
  // First extrapolate by a factor -1 through the face of the simplex
  // across from high point, i.e., reflect the simplex from the high point.</p>

  double yTry = tryExtrapolation(-1.0);
  if (yTry <= y_[iLowest_])
  {
    // Expansion.
    yTry = tryExtrapolation(2.0);
  }
  else if (yTry >= y_[iNextHighest_])
  {
    // Contraction.
    double ySave = y_[iHighest_];
    yTry = tryExtrapolation(0.5);
    if (yTry >= ySave)
    {
      for (size_t i = 0; i < mpts; i++)
      {
        if (i != iLowest_)
        {
          for (size_t j = 0; j < nDim; j++)
          {
            pSum_[j].setValue(0.5 * (simplex_[i][j].getValue() + simplex_[iLowest_][j].getValue()));
            simplex_[i][j].setValue(pSum_[j].getValue());
          }
          y_[i] = getFunction()->f(pSum_);
          nbEval_++;
        }
      }
      nbEval_ += static_cast<unsigned int>(nDim);
      pSum_ = getPSum();
    }
  }

  return y_[iLowest_];
}

/******************************************************************************/

double DownhillSimplexMethod::optimize()
{
  AbstractOptimizer::optimize();

  // set best shot:
  return getFunction()->f(simplex_[iLowest_]);
}

/******************************************************************************/

ParameterList DownhillSimplexMethod::getPSum()
{
  size_t ndim = simplex_.getDimension();
  size_t mpts = ndim + 1;

  // Get a copy...
  ParameterList pSum = getParameters();
  // ... and initializes it.
  for (size_t j = 0; j < ndim; j++)
  {
    double sum = 0.;
    for (size_t i = 0; i < mpts; i++)
    {
      sum += simplex_[i][j].getValue();
    }
    pSum[j].setValue(sum);
  }
  return pSum;
}

/******************************************************************************/

double DownhillSimplexMethod::tryExtrapolation(double fac)
{
  size_t ndim = simplex_.getDimension();
  double fac1, fac2, yTry;

  fac1 = (1.0 - fac) / static_cast<double>(ndim);
  fac2 = fac1 - fac;

  // Get a copy...
  ParameterList pTry = getParameters();
  // and initialize it:
  for (size_t j = 0; j < ndim; j++)
  {
    pTry[j].setValue(pSum_[j].getValue() * fac1 - simplex_[iHighest_][j].getValue() * fac2);
  }
  // Now compute the function for this new set of parameters:
  yTry = getFunction()->f(pTry);
  nbEval_++;

  // Then test this new point:
  if (yTry < y_[iHighest_])
  {
    y_[iHighest_] = yTry;
    for (size_t j = 0; j < ndim; j++)
    {
      pSum_[j].setValue(pSum_[j].getValue() + pTry[j].getValue() - simplex_[iHighest_][j].getValue());
      simplex_[iHighest_][j].setValue(pTry[j].getValue());
    }
  }
  return yTry;
}

/******************************************************************************/

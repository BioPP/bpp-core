// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../NumTools.h"
#include "BrentOneDimension.h"
#include "OneDimensionOptimizationTools.h"
#include "PowellMultiDimensions.h"

using namespace bpp;

/******************************************************************************/

bool PowellMultiDimensions::PMDStopCondition::isToleranceReached() const
{
  callCount_++;
  if (callCount_ <= burnin_)
    return false;
  return getCurrentTolerance() < tolerance_;
}

double PowellMultiDimensions::PMDStopCondition::getCurrentTolerance() const
{
  // NRC Test for done:
  const PowellMultiDimensions* pmd = dynamic_cast<const PowellMultiDimensions*>(optimizer_);
  double fp   = pmd->fp_;
  double fret = pmd->fret_;
  return 2.0 * NumTools::abs(fp - fret) / (NumTools::abs(fp) + NumTools::abs(fret));
}

/******************************************************************************/

PowellMultiDimensions::PowellMultiDimensions(std::shared_ptr<FunctionInterface> function) :
  AbstractOptimizer(function), fp_(0), fret_(0), pt_(), xi_(), ncom_(0), pcom_(), xicom_(), f1dim_(new DirectionFunction(function))
{
  setDefaultStopCondition_(make_shared<PMDStopCondition>(this));
  setStopCondition(getDefaultStopCondition());
}

/******************************************************************************/

void PowellMultiDimensions::doInit(const ParameterList& params)
{
  // Build the initial matrix:
  size_t n = params.size();
  xi_.resize(n);
  for (size_t i = 0; i < n; ++i)
  {
    // Copy the parameter list:
    xi_[i].resize(n);
    for (unsigned int j = 0; j < n; ++j)
    {
      // Set the directions to unit vectors:
      xi_[i][j] = (j == i) ? 1 : 0;
    }
  }

  // Starting point:
  fret_ = getFunction()->f(getParameters());
  pt_   = getParameters();
}

/******************************************************************************/

double PowellMultiDimensions::doStep()
{
  size_t n = getParameters().size();
  fp_ = fret_;
  size_t ibig = 0;
  double del = 0.0; // Will be the biggest function decrease
  Vdouble xit(n);

  // In each iteration, loop over all directions in the set.
  double fptt;
  for (size_t i = 0; i < n; i++)
  {
    // Copy the direction:
    for (size_t j = 0; j < n; j++)
    {
      xit[j] = xi_[j][i];
    }
    fptt = fret_;
    nbEval_ += OneDimensionOptimizationTools::lineMinimization(
		   f1dim_, getParameters_(), xit, getStopCondition()->getTolerance(),
       0, getMessageHandler(), getVerbose() > 0 ? getVerbose() - 1 : 0);
    fret_ = function().f(getParameters());
    if (getVerbose() > 2)
      printPoint(getParameters(), fret_);
    if (fret_ > fp_)
      throw Exception("DEBUG: PowellMultiDimensions::doStep(). Line minimization failed!");
    if (fptt - fret_ > del)
    {
      del = fptt - fret_;
      ibig = i;
    }
  }

  ParameterList ptt = getParameters();
  for (size_t j = 0; j < n; ++j)
  {
    ptt[j].setValue(2.0 * getParameters()[j].getValue() - pt_[j].getValue());
    xit[j] = getParameters()[j].getValue() - pt_[j].getValue();
    pt_[j].setValue(getParameters()[j].getValue());
  }
  fptt = getFunction()->f(ptt);
  if (fptt < fp_)
  {
    double t = 2.0 * (fp_ - 2.0 * fret_ + fptt) * NumTools::sqr(fp_ - fret_ - del) - del * NumTools::sqr(fp_ - fptt);
    if (t < 0.0)
    {
      // cout << endl << "New direction: drection " << ibig << " removed." << endl;
      nbEval_ += OneDimensionOptimizationTools::lineMinimization(f1dim_,
                                                                 getParameters_(), xit, getStopCondition()->getTolerance(),
                                                                 0, getMessageHandler(), getVerbose() > 0 ? getVerbose() - 1 : 0);
      fret_ = getFunction()->f(getParameters());
      if (fret_ > fp_)
        throw Exception("DEBUG: PowellMultiDimensions::doStep(). Line minimization failed!");
      for (size_t j = 0; j < n; ++j)
      {
        xi_[j][ibig]  = xi_[j][n - 1];
        xi_[j][n - 1] = xit[j];
      }
    }
  }
  else
    getFunction()->setParameters(getParameters());

  return fret_;
}

/******************************************************************************/

double PowellMultiDimensions::optimize()
{
  AbstractOptimizer::optimize();
  // Apply best parameter:
  return getFunction()->f(getParameters());
}

/******************************************************************************/

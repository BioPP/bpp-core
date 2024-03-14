// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "TwoPointsNumericalDerivative.h"

using namespace bpp;
using namespace std;

void TwoPointsNumericalDerivative::updateDerivatives(const ParameterList& parameters)
{
  if (computeD1_ && variables_.size() > 0)
  {
    if (function1_)
      function1_->enableFirstOrderDerivatives(false);
    if (function2_)
      function2_->enableSecondOrderDerivatives(false);
    function_->setParameters(parameters);
    f1_ = function_->getValue();
    if ((abs(f1_) >= NumConstants::VERY_BIG()) || std::isnan(f1_))
    {
      for (size_t i = 0; i < variables_.size(); ++i)
      {
        der1_[i] = log(-1);
        der2_[i] = log(-1);
      }
      return;
    }

    string lastVar;
    bool functionChanged = false;
    ParameterList p;
    bool start = true;
    for (size_t i = 0; i < variables_.size(); ++i)
    {
      string var = variables_[i];
      if (!parameters.hasParameter(var))
        continue;
      if (!start)
      {
        vector<string> vars(2);
        vars[0] = var;
        vars[1] = lastVar;
        p = parameters.createSubList(vars);
      }
      else
      {
        p = parameters.createSubList(var);
        start = false;
      }
      lastVar = var;
      functionChanged = true;
      double value = function_->getParameterValue(var);
      double h = -(1. + std::abs(value)) * h_;
      if (abs(h) < p[0].getPrecision())
        h = h < 0 ? -p[0].getPrecision() : p[0].getPrecision();
      double hf2(0);
      unsigned int nbtry = 0;

      // Compute one other point:
      while (hf2 == 0)
      {
        try
        {
          p[0].setValue(value + h);
          function_->setParameters(p); // also reset previous parameter...

          p = p.createSubList(0);
          f2_ = function_->getValue();
          if ((abs(f2_) >= NumConstants::VERY_BIG()) || std::isnan(f2_))
            throw ConstraintException("f2_ too large", &p[0], f2_);
          else
            hf2 = h;
        }
        catch (ConstraintException& ce)
        {
          if (++nbtry == 10) // no possibility to compute derivatives
            break;
          else if (h < 0)
            h = -h;                       // try on the right
          else
            h /= -2;                        // try again on the left with smaller interval
        }
      }

      der1_[i] = (f2_ - f1_) / h;
    }
    // Reset last parameter and compute analytical derivatives if any:
    if (function1_)
      function1_->enableFirstOrderDerivatives(computeD1_);
    if (functionChanged)
      function_->setParameters(parameters.createSubList(lastVar));
  }
  else
  {
    // Reset initial value and compute analytical derivatives if any.
    if (function1_)
      function1_->enableFirstOrderDerivatives(computeD1_);
    if (function2_)
      function2_->enableSecondOrderDerivatives(computeD2_);
    // Just in case derivatives are not computed:
    function_->setParameters(parameters);
    f1_ = function_->getValue();
  }
}

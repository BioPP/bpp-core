//
// File: TwoPointsNumericalDerivative.cpp
// Authors:
//   Julien Dutheil
// Created: 2007-05-28 10:33:00
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for numerical calculus.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/


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

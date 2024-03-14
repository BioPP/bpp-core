// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "../NumConstants.h"
#include "../Random/RandomTools.h"
#include "ExponentialDiscreteDistribution.h"

using namespace bpp;

// From the STL:
#include <cmath>
using namespace std;

/****************************************************************/

ExponentialDiscreteDistribution::ExponentialDiscreteDistribution(size_t n, double lambda) :
  AbstractDiscreteDistribution(n, "Exponential."),
  lambda_(lambda)
{
  addParameter_(new Parameter("Exponential.lambda", lambda, Parameter::R_PLUS));

  intMinMax_->setLowerBound(0, true);
  discretize();
}

/******************************************************************************/

void ExponentialDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  lambda_ = getParameterValue("lambda");
  discretize();
}

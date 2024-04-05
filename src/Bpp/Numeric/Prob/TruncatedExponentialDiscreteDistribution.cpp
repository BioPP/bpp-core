// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "../Random/RandomTools.h"
#include "TruncatedExponentialDiscreteDistribution.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

TruncatedExponentialDiscreteDistribution::TruncatedExponentialDiscreteDistribution(size_t n, double lambda, double truncationPoint) :
  AbstractDiscreteDistribution(n, "TruncExponential."),
  lambda_(lambda),
  tp_(truncationPoint),
  cond_(1 - exp(-lambda_ * tp_))
{
  addParameter_(new Parameter("TruncExponential.tp", truncationPoint, Parameter::R_PLUS));
  addParameter_(new Parameter("TruncExponential.lambda", lambda,  Parameter::R_PLUS));

  intMinMax_->setLowerBound(0, true);
  intMinMax_->setUpperBound(tp_, false);

  discretize();
}

/******************************************************************************/

void TruncatedExponentialDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  lambda_ = getParameterValue("lambda");
  tp_ = getParameterValue("tp");

  intMinMax_->setUpperBound(tp_, false);
  cond_ = 1 - exp(-lambda_ * tp_);

  discretize();
}

/******************************************************************************/

void TruncatedExponentialDiscreteDistribution::restrictToConstraint(const ConstraintInterface& c)
{
  AbstractDiscreteDistribution::restrictToConstraint(c);
  getParameter_("tp").setConstraint(intMinMax_);
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "../NumConstants.h"
#include "../Random/RandomTools.h"
#include "BetaDiscreteDistribution.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

BetaDiscreteDistribution::BetaDiscreteDistribution(size_t n, double alpha, double beta, short discretization) :
  AbstractDiscreteDistribution(n, NumConstants::VERY_TINY(), "Beta.", discretization),
  alpha_(alpha),
  beta_(beta),
  diffln_(0)
{
  addParameter_(new Parameter("Beta.alpha", alpha, std::make_shared<IntervalConstraint>(true, 0.0001, true)));

  // For precision issues, beta cannot be too low
  addParameter_(new Parameter("Beta.beta", beta, std::make_shared<IntervalConstraint>(true, 0.0001, true)));
  intMinMax_->setLowerBound(0, true);
  intMinMax_->setUpperBound(1, true);

  diffln_ = exp(RandomTools::lnBeta(alpha_ + 1, beta_) - RandomTools::lnBeta(alpha_, beta_));
  discretize();
}

/******************************************************************************/

void BetaDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  alpha_ = getParameterValue("alpha");
  beta_ = getParameterValue("beta");

  if (alpha_ <= 1 && intMinMax_->getLowerBound() == 0)
    intMinMax_->setLowerBound(precision(), false);

  if (beta_ <= 1 && intMinMax_->getUpperBound() == 1)
    intMinMax_->setUpperBound(1 - precision(), false);

  diffln_ = exp(RandomTools::lnBeta(alpha_ + 1, beta_) - RandomTools::lnBeta(alpha_, beta_));
  discretize();
}

/******************************************************************************/

double BetaDiscreteDistribution::qProb(double x) const
{
  return RandomTools::qBeta(x, alpha_, beta_);
}

double BetaDiscreteDistribution::pProb(double x) const
{
  return RandomTools::pBeta(x, alpha_, beta_);
}

double BetaDiscreteDistribution::Expectation(double a) const
{
  return RandomTools::pBeta(a, alpha_ + 1, beta_) * diffln_;
}

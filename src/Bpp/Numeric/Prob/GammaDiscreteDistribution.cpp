// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "../NumConstants.h"
#include "../Random/RandomTools.h"
#include "GammaDiscreteDistribution.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

GammaDiscreteDistribution::GammaDiscreteDistribution(size_t n, double alpha, double beta, double minimumAlpha, double minimumBeta, bool paramOffset, double offset) :
  AbstractDiscreteDistribution(n, "Gamma."),
  alpha_(alpha),
  beta_(beta),
  offset_(offset),
  ga1_(1)
{
  // We use a lower bound of 0.0001 for alpha and beta to prohibe errors due to computer
  // floating precision: if alpha is quite low (gamma -> constant), some classes
  // may have the same category value, leading to a classe number lower than expected.
  // NB: if this is the case, then a warning is shown. This may happen in optimization
  // algorithms.

  if (n <= 1)
    throw Exception("At least 2 classes for Gamma discrete distribution.");

  addParameter_(new Parameter("Gamma.alpha", alpha, std::make_shared<IntervalConstraint>(1, minimumAlpha, true)));
  addParameter_(new Parameter("Gamma.beta", beta, std::make_shared<IntervalConstraint>(1, minimumBeta, true)));
  if (paramOffset)
    addParameter_(new Parameter("Gamma.offset", offset));

  ga1_ = exp(RandomTools::lnGamma(alpha_ + 1) - RandomTools::lnGamma(alpha_));

  intMinMax_->setLowerBound(offset_, true);
  discretize();
}

GammaDiscreteDistribution::GammaDiscreteDistribution(const GammaDiscreteDistribution& gdd) :
  AbstractDiscreteDistribution(gdd),
  alpha_(gdd.alpha_),
  beta_(gdd.beta_),
  offset_(gdd.offset_),
  ga1_(gdd.ga1_)
{}

GammaDiscreteDistribution& GammaDiscreteDistribution::operator=(const GammaDiscreteDistribution& gdd)
{
  AbstractDiscreteDistribution::operator=(gdd);
  alpha_ = gdd.alpha_;
  beta_ = gdd.beta_;
  offset_ = gdd.offset_;
  ga1_ = gdd.ga1_;

  return *this;
}

GammaDiscreteDistribution::~GammaDiscreteDistribution() {}

/******************************************************************************/

void GammaDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  alpha_ = getParameterValue("alpha");
  beta_ = getParameterValue("beta");
  if (hasParameter("offset"))
    offset_ = getParameterValue("offset");
  ga1_ = exp(RandomTools::lnGamma(alpha_ + 1) - RandomTools::lnGamma(alpha_));

  discretize();
}

/******************************************************************************/

// Adapted from function DiscreteGamma of Yang

double GammaDiscreteDistribution::qProb(double x) const
{
  return offset_ + RandomTools::qGamma(x, alpha_, beta_);
}


double GammaDiscreteDistribution::pProb(double x) const
{
  return RandomTools::pGamma(x - offset_, alpha_, beta_);
}

double GammaDiscreteDistribution::Expectation(double a) const
{
  return RandomTools::pGamma(a - offset_, alpha_ + 1, beta_) / beta_ * ga1_ + (offset_ > 0 ? offset_ * RandomTools::pGamma(a - offset_, alpha_, beta_) : 0);
}

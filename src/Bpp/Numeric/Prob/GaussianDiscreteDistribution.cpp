// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "../NumConstants.h"
#include "../Random/RandomTools.h"
#include "GaussianDiscreteDistribution.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

GaussianDiscreteDistribution::GaussianDiscreteDistribution(size_t n, double mu, double sigma) :
  AbstractDiscreteDistribution(n, "Gaussian."), mu_(mu), sigma_(sigma)
{
  addParameter_(new Parameter("Gaussian.mu", mu));
  addParameter_(new Parameter("Gaussian.sigma", sigma, Parameter::R_PLUS_STAR));
  discretize();
}

GaussianDiscreteDistribution::GaussianDiscreteDistribution(const GaussianDiscreteDistribution& gdd) :
  AbstractDiscreteDistribution(gdd),
  mu_(gdd.mu_),
  sigma_(gdd.sigma_)
{}

GaussianDiscreteDistribution& GaussianDiscreteDistribution::operator=(const GaussianDiscreteDistribution& gdd)
{
  AbstractDiscreteDistribution::operator=(gdd);
  mu_ = gdd.mu_;
  sigma_ = gdd.sigma_;

  return *this;
}

GaussianDiscreteDistribution::~GaussianDiscreteDistribution() {}

/******************************************************************************/

void GaussianDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  mu_ = getParameterValue("mu");
  sigma_ = getParameterValue("sigma");
  discretize();
}

/******************************************************************************/

double GaussianDiscreteDistribution::qProb(double x) const
{
  return RandomTools::qNorm(x, mu_, sigma_);
}

double GaussianDiscreteDistribution::pProb(double x) const
{
  return RandomTools::pNorm(x, mu_, sigma_);
}

double GaussianDiscreteDistribution::Expectation(double a) const
{
  return -sigma_ / sqrt(2 * M_PI) * exp(-pow((a - mu_) / sigma_, 2) / 2)
         + mu_ * RandomTools::pNorm(a, mu_, sigma_);
}

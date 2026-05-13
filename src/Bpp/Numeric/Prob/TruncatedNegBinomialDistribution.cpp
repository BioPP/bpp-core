// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "TruncatedNegBinomialDistribution.h"

#include <Bpp/Numeric/Parameter.h>
#include <cmath>
#include <stdexcept>

using namespace bpp;
using namespace std;

TruncatedNegBinomialDistribution::TruncatedNegBinomialDistribution(
  double r,
  double mu,
  size_t maxK
) :
  AbstractDiscreteDistribution(maxK, "NegativeBinomial."),
  maxK_(maxK),
  mu_(mu)
{
  if (r <= 0.0)
    throw Exception("NegativeBinomial: r must be > 0.");
  if (mu_ <= 0.0)
    throw Exception("NegativeBinomial: mu must be > 0.");
  if (maxK_ == 0)
    throw Exception("NegativeBinomial: maxK must be >= 1.");

  addParameter_(new Parameter("NegativeBinomial.r", r, Parameter::R_PLUS_STAR));

  updateDistribution_();
}

TruncatedNegBinomialDistribution::TruncatedNegBinomialDistribution(
  const TruncatedNegBinomialDistribution& nbdd
) :
  AbstractDiscreteDistribution(nbdd),
  maxK_(nbdd.maxK_),
  mu_(nbdd.mu_)
{}

double TruncatedNegBinomialDistribution::getR() const
{
  return getParameterValue("r");
}

void TruncatedNegBinomialDistribution::setR(double r)
{
  setParameterValue("r", r);
}

void TruncatedNegBinomialDistribution::setMaxK(size_t maxK)
{
  if (maxK == 0)
    throw Exception("NegativeBinomial: maxK must be >= 1.");

  maxK_ = maxK;
  updateDistribution_();
}

void TruncatedNegBinomialDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  updateDistribution_();
}

void TruncatedNegBinomialDistribution::updateDistribution_()
{
  distribution_.clear();

  const double r  = getR();
  const double p  = r / (r + mu_); // success probability derived from r and fixed mean
  const double lp = std::log(p);
  const double l1mp = std::log(1.0 - p);

  // Fill unnormalized log-probabilities, then exponentiate
  for (size_t k = 0; k < maxK_; ++k)
  {
    double logProb = std::lgamma(static_cast<double>(k) + r)
                   - std::lgamma(static_cast<double>(k) + 1.0)
                   - std::lgamma(r)
                   + r * lp
                   + static_cast<double>(k) * l1mp;
    distribution_[static_cast<double>(k)] = std::exp(logProb);
  }

  // Normalize over truncated support
  double sum = 0.0;
  for (const auto& kv : distribution_)
    sum += kv.second;

  if (sum <= 0.0)
    throw Exception("NegativeBinomial: normalization failed.");

  for (auto& kv : distribution_)
    kv.second /= sum;
}

double TruncatedNegBinomialDistribution::pProb(double x) const
{
  double cdf = 0.0;
  for (const auto& kv : distribution_)
  {
    if (kv.first <= x)
      cdf += kv.second;
    else
      break;
  }
  return cdf;
}

double TruncatedNegBinomialDistribution::qProb(double p) const
{
  if (p < 0.0 || p > 1.0)
    throw Exception("NegativeBinomial::qProb(): p must be in [0,1].");

  double cdf = 0.0;
  for (const auto& kv : distribution_)
  {
    cdf += kv.second;
    if (cdf >= p)
      return kv.first;
  }

  return distribution_.rbegin()->first;
}

double TruncatedNegBinomialDistribution::Expectation(double a) const
{
  double num = 0.0;
  double den = 0.0;

  for (const auto& kv : distribution_)
  {
    const double x = kv.first;
    const double prob = kv.second;

    if (x < a)
      continue;

    num += x * prob;
    den += prob;
  }

  if (den == 0.0)
    return 0.0;

  return num / den;
}

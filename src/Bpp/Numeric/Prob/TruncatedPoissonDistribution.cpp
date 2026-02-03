#include "TruncatedPoissonDistribution.h"

#include <cmath>
#include <stdexcept>

using namespace bpp;
using namespace std;

TruncatedPoissonDistribution::TruncatedPoissonDistribution(
  double lambda,
  size_t maxK
) :
  AbstractDiscreteDistribution(maxK, "TruncatedPoisson."),
  maxK_(maxK)
{
  if (lambda <= 0.0)
    throw Exception("TruncatedPoisson: lambda must be > 0.");

  if (maxK_ == 0)
    throw Exception("TruncatedPoisson: maxK must be >= 1.");

  addParameter_(new Parameter("TruncatedPoisson.lambda", lambda, Parameter::R_PLUS_STAR));

  updateDistribution_();
}

TruncatedPoissonDistribution::TruncatedPoissonDistribution(
  const TruncatedPoissonDistribution& tpdd
) :
  AbstractDiscreteDistribution(tpdd),
  maxK_(tpdd.maxK_)
{}

double TruncatedPoissonDistribution::getLambda() const
{
  return getParameterValue("lambda");
}

void TruncatedPoissonDistribution::setLambda(double lambda)
{
  setParameterValue("lambda", lambda);
}

void TruncatedPoissonDistribution::setMaxK(size_t maxK)
{
  if (maxK == 0)
    throw Exception("TruncatedPoisson: maxK must be >= 1.");

  maxK_ = maxK;
  updateDistribution_();
}

void TruncatedPoissonDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  setLambda(getParameterValue("lambda"));
  updateDistribution_();
}

void TruncatedPoissonDistribution::updateDistribution_()
{
  distribution_.clear();

  const double lambda = getLambda();

  double p = std::exp(-lambda); 
  distribution_[0.0] = p;

  for (size_t k = 1; k < maxK_; ++k)
  {
    p *= lambda / static_cast<double>(k);
    distribution_[static_cast<double>(k)] = p;
  }

  // Compute normalization over truncated support
  double sum = 0.0;
  for (const auto& kv : distribution_)
    sum += kv.second;

  if (sum <= 0.0)
    throw Exception("TruncatedPoisson: normalization failed.");

  // Normalize probabilities to sum to 1
  for (auto& kv : distribution_)
    kv.second /= sum;
}

double TruncatedPoissonDistribution::pProb(double x) const
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

double TruncatedPoissonDistribution::qProb(double p) const
{
  if (p < 0.0 || p > 1.0)
    throw Exception("TruncatedPoisson::qProb(): p must be in [0,1].");

  double cdf = 0.0;
  for (const auto& kv : distribution_)
  {
    cdf += kv.second;
    if (cdf >= p)
      return kv.first;
  }

  // Return max category if p ≈ 1
  return distribution_.rbegin()->first;
}

double TruncatedPoissonDistribution::Expectation(double a) const
{
  double num = 0.0;
  double den = 0.0;

  for (const auto& kv : distribution_)
  {
    const double x = kv.first;
    const double p = kv.second;

    if (x < a)
      continue;

    num += x * p;
    den += p;
  }

  if (den == 0.0)
    return 0.0;

  return num / den;
}
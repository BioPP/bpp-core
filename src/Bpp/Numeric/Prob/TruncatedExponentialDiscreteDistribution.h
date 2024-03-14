// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_TRUNCATEDEXPONENTIALDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_TRUNCATEDEXPONENTIALDISCRETEDISTRIBUTION_H


#include "../Constraints.h"
#include "../Random/RandomTools.h"
#include "ExponentialDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Truncated (on the right) Exponential
 * distribution, where the probabilities are given the exponential,
 * conditioned by the upper limit.
 *
 * This distribution has two parameters: the traditional exponential law parameter,
 * and the abscissa of the truncation. The distribution will be truncated on the right
 * of this point.
 */

class TruncatedExponentialDiscreteDistribution :
  public AbstractDiscreteDistribution
{
protected:
  double lambda_;

  double tp_;

  /*
   * Probability of the condition given x < tp_.
   *
   */

  double cond_;

public:
  /**
   * @brief Build a new truncated exponential discrete distribution.
   * @param n the number of categories to use.
   * @param lambda The lambda parameter
   * @param truncationPoint The truncation point
   *
   * The Parameters are: lambda @f$ \in [0.000001;\infty[ @f$ and tp .@f$ \in [0;\infty[ @f$
   *
   */

  TruncatedExponentialDiscreteDistribution(size_t n, double lambda = 1., double truncationPoint = 10);

  TruncatedExponentialDiscreteDistribution(const TruncatedExponentialDiscreteDistribution& dist) :
    AbstractDiscreteDistribution(dist),
    lambda_(dist.lambda_),
    tp_(dist.tp_),
    cond_(dist.cond_)
  {}

  TruncatedExponentialDiscreteDistribution& operator=(const TruncatedExponentialDiscreteDistribution& dist)
  {
    AbstractDiscreteDistribution::operator=(dist);
    lambda_ = dist.lambda_;
    tp_ = dist.tp_;
    cond_ = dist.cond_;
    return *this;
  }

  ~TruncatedExponentialDiscreteDistribution(){}

  TruncatedExponentialDiscreteDistribution* clone() const { return new TruncatedExponentialDiscreteDistribution(*this); }

public:
  std::string getName() const {return "TruncExponential";}

  void fireParameterChanged(const ParameterList& parameters);

  double randC() const
  {
    double x = RandomTools::randExponential(1. / getParameterValue("lambda"));
    while (!intMinMax_->isCorrect(x))
      x = RandomTools::randExponential(1. / getParameterValue("lambda"));

    return x;
  }

  double pProb(double x) const
  {
    if (x >= tp_)
      return 1.;
    else
      return (1. - exp(-lambda_ * x)) / cond_;
  }

  double qProb(double x) const
  {
    if (x == 1)
      return tp_;
    else
      return -log(1. - cond_ * x) / lambda_;
  }

  double Expectation(double a) const
  {
    if (a < tp_)
      return (1. / lambda_ - exp(-a * lambda_) * (a + 1. / lambda_)) / cond_;
    else
      return (1. / lambda_ - exp(-tp_ * lambda_) * (tp_ + 1. / lambda_)) / cond_;
  }

  void restrictToConstraint(const ConstraintInterface& c);
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_TRUNCATEDEXPONENTIALDISCRETEDISTRIBUTION_H

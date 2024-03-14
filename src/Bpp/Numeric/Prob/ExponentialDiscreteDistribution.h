// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_EXPONENTIALDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_EXPONENTIALDISCRETEDISTRIBUTION_H


#include "../Constraints.h"
#include "../Random/RandomTools.h"
#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Exponential distribution.
 *
 * @author Julien Dutheil
 */
class ExponentialDiscreteDistribution :
  public AbstractDiscreteDistribution
{
protected:
  double lambda_;

public:
  /**
   * @brief Build a new discretized exponential distribution.
   * @param n the number of categories to use.
   * @param lambda The lambda parameter.
   *
   * The Parameter is: lambda @f$ \in [0;\infty[ @f$.
   *
   */

  ExponentialDiscreteDistribution(size_t n, double lambda = 1.);

  ExponentialDiscreteDistribution(const ExponentialDiscreteDistribution& dist) :
    AbstractDiscreteDistribution(dist),
    lambda_(dist.lambda_)
  {}

  ExponentialDiscreteDistribution& operator=(const ExponentialDiscreteDistribution& dist)
  {
    AbstractDiscreteDistribution::operator=(dist);
    lambda_ = dist.lambda_;
    return *this;
  }

  ~ExponentialDiscreteDistribution(){}

  ExponentialDiscreteDistribution* clone() const { return new ExponentialDiscreteDistribution(*this); }

public:
  std::string getName() const {return "Exponential";}

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
    return 1. - exp(-lambda_ * x);
  }

  double qProb(double x) const
  {
    return -log(1. - x) / lambda_;
  }

  double Expectation(double a) const
  {
    return 1. / lambda_ - exp(-a * lambda_) * (a + 1. / lambda_);
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_EXPONENTIALDISCRETEDISTRIBUTION_H

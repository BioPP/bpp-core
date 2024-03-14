// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_GAUSSIANDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_GAUSSIANDISCRETEDISTRIBUTION_H


#include "../Constraints.h"
#include "../Random/RandomTools.h"
#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Gaussian distribution.
 *
 * @author Laurent Guéguen
 */
class GaussianDiscreteDistribution :
  public AbstractDiscreteDistribution
{
private:
  double mu_, sigma_;

public:
  /**
   * @brief Build a new discretized normal distribution.
   * @param n the number of categories to use.
   * @param mu The mean parameter.
   * @param sigma The standard deviation parameter.
   *
   * The Parameters are: mu @f$ \in ]-\infty;\infty[ @f$ and sigma
   * @f$ \in ]0;\infty[ @f$.
   *
   */
  GaussianDiscreteDistribution(size_t n, double mu = 0., double sigma = 1.);

  GaussianDiscreteDistribution(const GaussianDiscreteDistribution&);

  GaussianDiscreteDistribution& operator=(const GaussianDiscreteDistribution&);

  virtual ~GaussianDiscreteDistribution();

  GaussianDiscreteDistribution* clone() const { return new GaussianDiscreteDistribution(*this); }

public:
  std::string getName() const {return "Gaussian";}

  void fireParameterChanged(const ParameterList& parameters);

  double randC() const
  {
    return RandomTools::randGaussian(mu_, sigma_);
  }

  double qProb(double x) const;

  double pProb(double x) const;

  double Expectation(double a) const;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_GAUSSIANDISCRETEDISTRIBUTION_H

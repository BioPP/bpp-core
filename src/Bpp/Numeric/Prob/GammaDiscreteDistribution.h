// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_GAMMADISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_GAMMADISCRETEDISTRIBUTION_H


#include "../Constraints.h"
#include "../Random/RandomTools.h"
#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Gamma distribution with an offset.
 *
 * @author Julien Dutheil, Laurent Guéguen, with original code from Tal Pupko and Ziheng Yang.
 */

class GammaDiscreteDistribution :
  public AbstractDiscreteDistribution
{
private:
  double alpha_, beta_;

  double offset_;

  // To prevent useless computations
  double ga1_;

public:
  std::string getName() const {return "Gamma";}

  /**
   * @brief Build a new discretized gamma distribution.
   * @param n the number of categories to use (n>1).
   * @param alpha The alpha parameter (shape)
   * @param beta The beta parameter (rate)
   * @param minimumAlpha The minimum allowed value for parameter alpha.
   * @param minimumBeta The minimum allowed value for parameter beta.
   * @param paramOffset bool if the distribution has an offset
   *               parameter (default: false).
   * @param offset The distribution is offset + Gamma (default 0).
   *         P(X<x)=pGamma(x-offset,alpha,beta)
   *
   * The Parameters are: alpha and beta @f$ \in [minimumBound;\infty[ @f$.
   * Small values of alpha and/or beta can lead to discretization issues.
   *
   *
   * If @f$ alpha > 1 @f$, the minimum value of the distribution is
   * set to offset+1e-12, otherwise it is offset.
   */
  GammaDiscreteDistribution(size_t n, double alpha = 1., double beta = 1., double minimumAlpha = 0.05, double minimumBeta = 0.05, bool paramOffset = false, double offset = 0);

  /**
   * @brief Build a new discretized gamma distribution for a set of user-defined bounds
   * @param alpha The alpha parameter (shape)
   * @param beta The beta parameter (rate)
   * @param minimumAlpha The minimum allowed value for parameter alpha.
   * @param minimumBeta The minimum allowed value for parameter beta.
   *
   * The Parameters are: alpha and beta @f$ \in [minimumBound;\infty[ @f$.
   * Small values of alpha and/or beta can lead to discretization issues.
   */
  GammaDiscreteDistribution(const std::vector<double>& bounds, double alpha = 1., double beta = 1., double minimumAlpha = 0.0001, double minimumBeta = 0.0001);

  GammaDiscreteDistribution(const GammaDiscreteDistribution&);

  GammaDiscreteDistribution& operator=(const GammaDiscreteDistribution&);

  virtual ~GammaDiscreteDistribution();

  GammaDiscreteDistribution* clone() const { return new GammaDiscreteDistribution(*this); }

  void fireParameterChanged(const ParameterList& parameters);

  double randC() const
  {
    double x = RandomTools::randGamma(getParameterValue("alpha"),
          getParameterValue("beta"));
    while (!intMinMax_->isCorrect(x))
      x = RandomTools::randGamma(getParameterValue("alpha"),
            getParameterValue("beta"));

    return x + offset_;
  }

  double qProb(double x) const;

  double pProb(double x) const;

  double Expectation(double a) const;

  /**
   * @brief Set the discretization policy.
   *
   * @param discretization Either DISCRETIZATION_EQUAL_PROB or DISCRETIZATION_EQUAL_INTERVAL. 
   * @warning Default is set to EQUAL_PROB. Changing to EQUAL_INTERVAL necessitates to call restrictToConstraint with a finite interval first.
   */
  void setDiscretizationPolicy(short discretization) { discretizationScheme_ = discretization; };
};

} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_GAMMADISCRETEDISTRIBUTION_H

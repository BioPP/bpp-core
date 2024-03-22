// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_BETADISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_BETADISCRETEDISTRIBUTION_H


#include "../Random/RandomTools.h"
#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Beta distribution with parameters alpha and
 * beta, on a given interval. On default, the interval is @f$ [0;1]
 * @f$, but it can be restricted.
 *
 * The minimum (resp maximum) value of this distribution is set to
 * 1e-12 (resp 1-1e-12) if alpha>1 (resp beta>1), otherwise it is 0
 * (resp 1).
 *
 * The Parameters are: alpha and beta @f$ \in [0.0001;\infty[ @f$.
 * @author Laurent Guéguen
 */

class BetaDiscreteDistribution :
  public AbstractDiscreteDistribution
{
private:
  double alpha_, beta_;

  /* for computation economy */

  double diffln_;

public:
  /**
   * @brief Build a new discretized beta distribution.
   *
   * @param n the number of categories to use.
   * @param alpha The alpha parameter.
   * @param beta The beta parameter.
   * @param discretization The discretization scheme to use.
   */
  BetaDiscreteDistribution(size_t n, double alpha = 1, double beta = 1, short discretization = DISCRETIZATION_EQUAL_PROB_WHEN_POSSIBLE);

  BetaDiscreteDistribution(const BetaDiscreteDistribution&) = default;

  BetaDiscreteDistribution& operator=(const BetaDiscreteDistribution&) = default;

  BetaDiscreteDistribution* clone() const override { return new BetaDiscreteDistribution(*this); }

public:
  std::string getName() const override { return "Beta";}

  void fireParameterChanged(const ParameterList& parameters) override;

  double randC() const override
  {
    double x = RandomTools::randBeta(getParameterValue("alpha"),
          getParameterValue("beta"));
    while (!intMinMax_->isCorrect(x))
      x = RandomTools::randBeta(getParameterValue("alpha"),
            getParameterValue("beta"));
    return x;
  }

  double qProb(double x) const override;

  double pProb(double x) const override;

  double Expectation(double a) const override;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_BETADISCRETEDISTRIBUTION_H

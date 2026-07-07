// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_TRUNCATEDNEGBINOMIALDISTRIBUTION_H
#define BPP_NUMERIC_PROB_TRUNCATEDNEGBINOMIALDISTRIBUTION_H

#include <Bpp/Numeric/Prob/AbstractDiscreteDistribution.h>

namespace bpp
{

/**
 * @brief Truncated negative binomial distribution.
 *
 * The dispersion parameter r is real-valued (> 0) and registered as a bpp
 * parameter for numerical optimization. The success probability p is derived
 * analytically from r and the empirical mean mu, which is estimated from the data.
 *
 * @author Roye Tadmor
 */
class TruncatedNegBinomialDistribution :
  public AbstractDiscreteDistribution
{
private:
  size_t maxK_; // upper truncation (exclusive)
  double mu_;   // fixed empirical mean; used to derive p = r / (r + mu)

public:
  /**
   * @param r    Dispersion parameter (> 0); will be numerically optimized.
   * @param mu   Empirical mean of the data (> 0); fixed — used to derive p.
   * @param maxK Number of categories (support is {0, 1, ..., maxK-1}).
   */
  TruncatedNegBinomialDistribution(double r, double mu, size_t maxK);

  TruncatedNegBinomialDistribution(const TruncatedNegBinomialDistribution&);

  TruncatedNegBinomialDistribution* clone() const { return new TruncatedNegBinomialDistribution(*this); }

public:
  std::string getName() const { return "NegativeBinomial"; }

  double pProb(double x) const;
  double qProb(double p) const;
  double Expectation(double a) const;

  double getR() const;
  void   setR(double r);

  double getMu()   const { return mu_; }
  size_t getMaxK() const { return maxK_; }
  void   setMaxK(size_t maxK);

protected:
  void fireParameterChanged(const ParameterList& parameters) override;

private:
  void updateDistribution_();
};

} // namespace bpp

#endif // BPP_NUMERIC_PROB_TRUNCATEDNEGBINOMIALDISTRIBUTION_H

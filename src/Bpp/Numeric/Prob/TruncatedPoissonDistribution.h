#ifndef BPP_NUMERIC_PROB_TRUNCATEDPOISSONDISTRIBUTION_H
#define BPP_NUMERIC_PROB_TRUNCATEDPOISSONDISTRIBUTION_H


#include "AbstractDiscreteDistribution.h"

namespace bpp
{

  /**
 * @brief Truncated Poisson distribution.
 * Usually the Poisson distribution has infinite support, so to make it computationally tractable
 * we consider a truncated version of the distribution, where the support is {0...maxK_}
 * 
 * The CDF is computed and normalized such that the sum of probabilities of every category is 1.
 * 
 * @author Roye Tadmor
 */

class TruncatedPoissonDistribution :
  public AbstractDiscreteDistribution
{
private:
  size_t maxK_; // upper truncation

public:
  /**
   * @brief Build a new discretized normal distribution.
   * @param maxK the number of categories to use.
   * @param lambda The lambda parameter.
   *
   */
  TruncatedPoissonDistribution(
    double lambda,
    size_t maxK
  );

TruncatedPoissonDistribution* clone() const { return new TruncatedPoissonDistribution(*this); }

TruncatedPoissonDistribution(const TruncatedPoissonDistribution&);

public:
  std::string getName() const {return "TruncatedPoisson";}

  double pProb(double x) const;
  double qProb(double p) const;
  double Expectation(double a) const;

  double getLambda() const;
  void setLambda(double lambda);

  size_t getMaxK() const { return maxK_; }
  void setMaxK(size_t maxK);

protected:
  void fireParameterChanged(const ParameterList& parameters) override;

private:
  void updateDistribution_();
};

}


#endif // BPP_NUMERIC_PROB_TRUNCATEDPOISSONDISTRIBUTION_H

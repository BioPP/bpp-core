// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_MIXTUREOFDISCRETEDISTRIBUTIONS_H
#define BPP_NUMERIC_PROB_MIXTUREOFDISCRETEDISTRIBUTIONS_H


#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief A Discrete distribution object defined by a vector of
 * Discrete Distributions and a set of probabilities for these
 * Discrete Distributions.
 *
 * The non-null values of the MixtureOfDiscreteDistributions are all
 * the non-null values of the Discrete Distributions, with
 * probabilities equal to their probabilities in each Discrete
 * Distribution multiplied by the specific probability of this
 * Distribution.
 *
 * Parameters:
 *
 * For the probabilities: they are called \c "theta1",... and defined
 * as @f$ \theta_{i \in 1..\textrm{size-1}} @f$ such that probability
 * of value @f$i @f$ is @f$ (1-\theta_1).(1-\theta_2)...\theta_{i} @f$
 *
 * For the values: they are the parameters of the Discrete
 * Distributions, prefixed by the index in the vector of the Discrete
 * Distributions.
 *
 */
class MixtureOfDiscreteDistributions :
  public AbstractDiscreteDistribution
{
protected:
  std::vector<std::unique_ptr<DiscreteDistributionInterface>> vdd_;

  std::vector<double> probas_;

  std::vector<std::string> vNestedPrefix_;

public:
  /**
   * @brief Builds a new MixtureOfDiscreteDistributions object from a
   * vector of Discrete Distributions and a vector of probabilities.
   *
   * @param distributions The vector of pointers to Discrete
   * Distributions.
   * @param probas The vector of probabilities.
   */
  MixtureOfDiscreteDistributions(const std::vector<std::unique_ptr<DiscreteDistributionInterface>>& distributions, const std::vector<double>& probas);

  virtual ~MixtureOfDiscreteDistributions();

  MixtureOfDiscreteDistributions(const MixtureOfDiscreteDistributions& mdd);

  MixtureOfDiscreteDistributions& operator=(const MixtureOfDiscreteDistributions& mdd);

  MixtureOfDiscreteDistributions* clone() const { return new MixtureOfDiscreteDistributions(*this); }

public:
  std::string getName() const {return "Mixture"; }

  /**
   * @brief Returns the number of discrete distributions in the
   * mixture.
   */
  size_t getNumberOfDistributions() const { return vdd_.size(); }

  /**
   * @brief Returns a pointer to the n-th discrete distribution in the mixture.
   *
   * @param n tne number of the distribution in the mixture;
   */
  const DiscreteDistributionInterface& nDistribution(size_t n) const
  {
    return *vdd_[n];
  }

  /**
   * @brief Returns the probability of the n-th discrete distribution in the mixture.
   *
   * @param n the number of the distribution in the mixture;
   */
  double getNProbability(size_t n) const
  {
    return probas_[n];
  }

  /**
   * @brief sets the number of categories of EACH submodel to
   * nbClasses, so the number of categories of the mixture is the sum
   * of all these numbers.
   *
   */

  void setNumberOfCategories(size_t nbClasses);

  void fireParameterChanged(const ParameterList& parameters);

  double qProb(double x) const;

  double pProb(double x) const;

  double Expectation(double a) const;

  void setMedian(bool median);

  void restrictToConstraint(const ConstraintInterface& c);

  void discretize();

  void setNamespace(const std::string& prefix);

protected:
  void updateDistribution();
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_MIXTUREOFDISCRETEDISTRIBUTIONS_H

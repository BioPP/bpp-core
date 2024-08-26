// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_HMMLIKELIHOOD_H
#define BPP_NUMERIC_HMM_HMMLIKELIHOOD_H


// From NumCalc:
#include "../Function/Functions.h"
#include "../VectorTools.h"

#include "HmmStateAlphabet.h"
#include "HmmTransitionMatrix.h"
#include "HmmEmissionProbabilities.h"

namespace bpp
{
/**
 * @brief Basal interface for Hidden Markov Models likelihood computation.
 *
 * HmmLikelihood classes compute the probability of data according to parameters (likelihood),
 * using the so-called forward recursion:
 *
 * - Initialisation: @f[ f_0({\cal A}_0), \ldots, f_0({\cal A}_n) @f] The initial frequencies, set to the equilibrium frequencies of the chain.
 * - Recursion (for i=1 to l, the length of the sequence data): @f[ f_i({\cal A}_y) = e_y(D_i) \sum_{x=1}^n f_{i-1}({\cal A}_x) \cdot p_{x,y} @f]
 * - Termination: @f[ \Pr(D) = \sum_{x=1}^n f_l({\cal A}_x) @f]
 * where @f$ {\cal A}_{1..n} @f$ denotes the hidden states of the alphabet, @f$ e_y(D_i) @f$ the
 * probability of the data at position i conditioned on hidden state y (emission probabilities)
 * and @f$ p_{x,y} @f$ is the probability of havving hidden state y at state i+1 knowing there
 * is hidden state x at position i (transition probabilities). These essential elements are given
 * respectively by the HmmEmissionProbabilities and HmmTransitionMatrix object associated to this
 * class. Both objects have to share the same HmmStateAlphabet instance, which describes all
 * allowed hidden states.
 *
 * The HmmLikelihood interface provides essentially two major methods:
 * - A method to retrieve the likelihood value (parameter estimation)
 * - Two methods to retrieve the posterio probabilities of each state using the forward and backward conditionnal likelihoods (posterior decoding).
 */

class HmmLikelihood :
  public virtual SecondOrderDerivable
{
public:
  virtual HmmLikelihood* clone() const = 0;

  virtual const HmmStateAlphabet& hmmStateAlphabet() const = 0;
  virtual HmmStateAlphabet& hmmStateAlphabet() = 0;

  virtual std::shared_ptr<const HmmStateAlphabet> getHmmStateAlphabet() const = 0;
  virtual std::shared_ptr<HmmStateAlphabet> getHmmStateAlphabet() = 0;

  virtual const HmmTransitionMatrix& hmmTransitionMatrix() const = 0;
  virtual HmmTransitionMatrix& hmmTransitionMatrix() = 0;

  virtual std::shared_ptr<const HmmTransitionMatrix> getHmmTransitionMatrix() const = 0;
  virtual std::shared_ptr<HmmTransitionMatrix> getHmmTransitionMatrix() = 0;

  virtual const HmmEmissionProbabilities& hmmEmissionProbabilities() const = 0;
  virtual HmmEmissionProbabilities& hmmEmissionProbabilities() = 0;

  virtual std::shared_ptr<const HmmEmissionProbabilities> getHmmEmissionProbabilities() const = 0;
  virtual std::shared_ptr<HmmEmissionProbabilities> getHmmEmissionProbabilities() = 0;

  virtual void getHiddenStatesPosteriorProbabilities(std::vector<std::vector<double>>& probs, bool append) const = 0;

  virtual Vdouble getHiddenStatesPosteriorProbabilitiesForASite(size_t site) const = 0;

  virtual double getLogLikelihood() const = 0;

  virtual double getDLogLikelihood() const = 0;

  virtual double getD2LogLikelihood() const = 0;

  /**
   * @brief Get the likelihood for a site, and its derivatives.
   *
   * @param site The site index to analyse.
   * @return The likelihood for site <i>site</i>.
   */
  virtual double getLikelihoodForASite(size_t site) const = 0;

  virtual double getDLogLikelihoodForASite(size_t site) const = 0;

  virtual double getD2LogLikelihoodForASite(size_t site) const = 0;

  /**
   * @brief Get the likelihood for each site.
   *
   * @return A vector with all likelihoods for each site.
   */
  virtual Vdouble getLikelihoodForEachSite() const = 0;

  virtual const std::vector<size_t>& getBreakPoints() const = 0;

  virtual void setBreakPoints(const std::vector<size_t>& breakPoints) = 0;

protected:
  virtual void computeDLikelihood_() const = 0;

  virtual void computeD2Likelihood_() const = 0;
};


/**
 * @brief partial impmementation of Hmm Likelihoods.
 */

class AbstractHmmLikelihood :
  public virtual HmmLikelihood
{
protected:
  mutable double dLogLik_;
  mutable std::string dVariable_;

  mutable double d2LogLik_;
  mutable std::string d2Variable_;

public:
  AbstractHmmLikelihood();

  AbstractHmmLikelihood(const AbstractHmmLikelihood& adhlik);

  AbstractHmmLikelihood& operator=(const AbstractHmmLikelihood& adhlik);

  /**
   * @{
   *
   * @brief From FirstOrder:
   *
   */
  void enableFirstOrderDerivatives(bool yn) {}

  bool enableFirstOrderDerivatives() const { return true;}

  double getFirstOrderDerivative(const std::string& variable) const;

  double getDLogLikelihood() const
  {
    return dLogLik_;
  }

  /**
   * @}
   */

  /**
   * @{
   *
   * @brief From SecondOrder:
   */
  void enableSecondOrderDerivatives(bool yn) {}

  bool enableSecondOrderDerivatives() const {return true;}

  double getSecondOrderDerivative(const std::string& variable) const;

  double getD2LogLikelihood() const
  {
    return d2LogLik_;
  }

  double getSecondOrderDerivative(const std::string& variable1, const std::string& variable2) const
  {
    throw (NotImplementedException("AbstractHmmLikelihood::getSecondOrderDerivative is not defined for 2 variables."));
  }

  /**
   * @}
   */
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_HMM_HMMLIKELIHOOD_H

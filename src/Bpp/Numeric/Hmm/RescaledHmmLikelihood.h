// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_RESCALEDHMMLIKELIHOOD_H
#define BPP_NUMERIC_HMM_RESCALEDHMMLIKELIHOOD_H


#include "../AbstractParametrizable.h"
#include "../Matrix/Matrix.h"
#include "HmmLikelihood.h"

// From the STL:
#include <vector>
#include <memory>

namespace bpp
{
/**
 * @brief A simple implementation of hidden Markov models recursion.
 *
 * This implementation uses the rescaling method described in Durbin et al "Biological sequence analysis", Cambridge University Press.
 * It also offer the possibility to specify "breakpoints", where the chain will be reset to the equilibrium frequencies.
 */
class RescaledHmmLikelihood :
  public virtual AbstractHmmLikelihood,
  public AbstractParametrizable
{
private:
  /**
   * @brief The alphabet describing the hidden states.
   */
  std::shared_ptr<HmmStateAlphabet> hiddenAlphabet_;
  std::shared_ptr<HmmTransitionMatrix> transitionMatrix_;
  std::shared_ptr<HmmEmissionProbabilities> emissionProbabilities_;

  /**
   * @brief The likelihood arrays.
   *
   */

  /**
   * @brief forward likelihood
   *
   * likelihood_[i * nbStates_ + j] corresponds to Pr(x_1...x_i, y_i=j)/Pr(x_1...x_i),
   * where the x are the observed states, and y the hidden states.
   */

  std::vector<double> likelihood_;

  /**
   * @brief derivatec of forward likelihood
   *
   * dlikelihood_[i][j] corresponds to d(Pr(x_1...x_i, y_i=j))/Pr(x_1...x_i)),
   * where the x are the observed states, and y the hidden states.
   */

  mutable std::vector<std::vector<double>> dLikelihood_;
  mutable std::vector<std::vector<double>> d2Likelihood_;

  /**
   * @brief backward likelihood
   *
   * backLikelihood_[i][j] corresponds to Pr(x_i+1...x_n | yi=j),
   * where the x are the observed states, and y the hidden states.
   */

  mutable std::vector<std::vector<double>> backLikelihood_;
  mutable bool backLikelihoodUpToDate_;

  /**
   * @brief scales for likelihood computing
   *
   * scales_[i * nbStates_ + j] corresponds to Pr(x_1...x_i)/Pr(x_1...x_{i-1})
   * where the x are the observed states.
   */

  std::vector<double> scales_;

  mutable std::vector<double> dScales_;
  mutable std::vector<double> d2Scales_;
  double logLik_;

  std::vector<size_t> breakPoints_;

  size_t nbStates_, nbSites_;

public:
  /**
   * @brief Build a new RescaledHmmLikelihood object.
   *
   * @warning the HmmTransitionMatrix and HmmEmissionProbabilities object passed as argument must be non-null
   * and point toward the same HmmStateAlphabet instance. The three object will be copied if needed, and
   * deleted when the hmm likelihood objet is deleted. You should secure a copy before if you don't want them to
   * be destroyed with this object.
   */
  RescaledHmmLikelihood(
      std::shared_ptr<HmmStateAlphabet> hiddenAlphabet,
      std::shared_ptr<HmmTransitionMatrix> transitionMatrix,
      std::shared_ptr<HmmEmissionProbabilities> emissionProbabilities,
      const std::string& prefix);

  RescaledHmmLikelihood(const RescaledHmmLikelihood& lik) :
    AbstractHmmLikelihood(lik),
    AbstractParametrizable(lik),
    hiddenAlphabet_(dynamic_cast<HmmStateAlphabet*>(lik.hiddenAlphabet_->clone())),
    transitionMatrix_(dynamic_cast<HmmTransitionMatrix*>(lik.transitionMatrix_->clone())),
    emissionProbabilities_(dynamic_cast<HmmEmissionProbabilities*>(lik.emissionProbabilities_->clone())),
    likelihood_(lik.likelihood_),
    dLikelihood_(lik.dLikelihood_),
    d2Likelihood_(lik.d2Likelihood_),
    backLikelihood_(lik.backLikelihood_),
    backLikelihoodUpToDate_(lik.backLikelihoodUpToDate_),
    scales_(lik.scales_),
    dScales_(lik.dScales_),
    d2Scales_(lik.d2Scales_),
    logLik_(lik.logLik_),
    breakPoints_(lik.breakPoints_),
    nbStates_(lik.nbStates_),
    nbSites_(lik.nbSites_)
  {
    // Now adjust pointers:
    transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_);
    emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_);
  }

  RescaledHmmLikelihood& operator=(const RescaledHmmLikelihood& lik)
  {
    AbstractHmmLikelihood::operator=(lik);
    AbstractParametrizable::operator=(lik);
    hiddenAlphabet_        = std::unique_ptr<HmmStateAlphabet>(dynamic_cast<HmmStateAlphabet*>(lik.hiddenAlphabet_->clone()));
    transitionMatrix_      = std::unique_ptr<HmmTransitionMatrix>(dynamic_cast<HmmTransitionMatrix*>(lik.transitionMatrix_->clone()));
    emissionProbabilities_ = std::unique_ptr<HmmEmissionProbabilities>(dynamic_cast<HmmEmissionProbabilities*>(lik.emissionProbabilities_->clone()));
    likelihood_            = lik.likelihood_;
    dLikelihood_           = lik.dLikelihood_;
    d2Likelihood_          = lik.d2Likelihood_;
    backLikelihood_        = lik.backLikelihood_;
    backLikelihoodUpToDate_ = lik.backLikelihoodUpToDate_;
    scales_                = lik.scales_;
    dScales_               = lik.dScales_;
    d2Scales_              = lik.d2Scales_;
    logLik_                = lik.logLik_;
    breakPoints_           = lik.breakPoints_;
    nbStates_              = lik.nbStates_;
    nbSites_               = lik.nbSites_;

    // Now adjust pointers:
    transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_);
    emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_);
    return *this;
  }

  virtual ~RescaledHmmLikelihood() {}

  RescaledHmmLikelihood* clone() const override { return new RescaledHmmLikelihood(*this); }

public:
  const HmmStateAlphabet& hmmStateAlphabet() const override { return *hiddenAlphabet_; }
  std::shared_ptr<const HmmStateAlphabet> getHmmStateAlphabet() const override { return hiddenAlphabet_; }

  HmmStateAlphabet& hmmStateAlphabet() override { return *hiddenAlphabet_; }
  std::shared_ptr<HmmStateAlphabet> getHmmStateAlphabet() override { return hiddenAlphabet_; }

  const HmmTransitionMatrix& hmmTransitionMatrix() const override { return *transitionMatrix_; }
  std::shared_ptr<const HmmTransitionMatrix> getHmmTransitionMatrix() const override { return transitionMatrix_; }

  HmmTransitionMatrix& hmmTransitionMatrix() override { return *transitionMatrix_; }
  std::shared_ptr<HmmTransitionMatrix> getHmmTransitionMatrix() override { return transitionMatrix_; }

  const HmmEmissionProbabilities& hmmEmissionProbabilities() const override { return *emissionProbabilities_; }
  std::shared_ptr<const HmmEmissionProbabilities> getHmmEmissionProbabilities() const override { return emissionProbabilities_; }

  HmmEmissionProbabilities& hmmEmissionProbabilities() override { return *emissionProbabilities_; }
  std::shared_ptr<HmmEmissionProbabilities> getHmmEmissionProbabilities() override { return emissionProbabilities_; }

  void setBreakPoints(const std::vector<size_t>& breakPoints) override
  {
    breakPoints_ = breakPoints;
    computeForward_();
    backLikelihoodUpToDate_ = false;
  }

  const std::vector<size_t>& getBreakPoints() const override { return breakPoints_; }

  void setParameters(const ParameterList& pl) override
  {
    setParametersValues(pl);
  }

  double getValue() const override { return -logLik_; }

  double getLogLikelihood() const override { return logLik_; }

  double getLikelihoodForASite(size_t site) const override;

  double getDLogLikelihoodForASite(size_t site) const override;

  double getD2LogLikelihoodForASite(size_t site) const override;

  Vdouble getLikelihoodForEachSite() const override;

  void setNamespace(const std::string& nameSpace) override;

  void fireParameterChanged(const ParameterList& pl) override;

  Vdouble getHiddenStatesPosteriorProbabilitiesForASite(size_t site) const override;

  void getHiddenStatesPosteriorProbabilities(std::vector<std::vector<double>>& probs, bool append = false) const override;

protected:
  void computeForward_();
  void computeBackward_() const;


  void computeDLikelihood_() const override
  {
    computeDForward_();
  }

  void computeD2Likelihood_() const override
  {
    computeD2Forward_();
  }

  void computeDForward_() const;

  void computeD2Forward_() const;
};
}
#endif // BPP_NUMERIC_HMM_RESCALEDHMMLIKELIHOOD_H

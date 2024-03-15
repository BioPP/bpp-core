// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_LOWMEMORYRESCALEDHMMLIKELIHOOD_H
#define BPP_NUMERIC_HMM_LOWMEMORYRESCALEDHMMLIKELIHOOD_H


#include "../AbstractParametrizable.h"
#include "../Matrix/Matrix.h"
#include "HmmLikelihood.h"

// From the STL:
#include <vector>
#include <memory>

namespace bpp
{
/**
 * @brief A modified implementation of the RescaledHmmLikelihood implementation, with lower memory usage.
 *
 * This implementation is similar to the one used in the RescaledHmmLikelihood class,
 * but does not store the full likelihood array. The benefit of it is a significantly reduced
 * memory usage, allowing to compute likelihood for very large data sets.
 *
 * The drawback is that this class can't compute posterior
 * probabilities, neither derivatives of the likelihoods, and can
 * hence only be used to compute likelihoods.
 *
 */

class LowMemoryRescaledHmmLikelihood :
  public AbstractHmmLikelihood,
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
   * @brief The likelihood array.
   *
   * Here we use two arrays for the i and i-1 positions
   */
  std::vector<double> likelihood1_;
  std::vector<double> likelihood2_;
  double logLik_;
  size_t maxSize_;

  std::vector<size_t> breakPoints_;

  size_t nbStates_, nbSites_;

public:
  /**
   * @brief Build a new LowMemoryRescaledHmmLikelihood object.
   *
   * @warning the HmmTransitionMatrix and HmmEmissionProbabilities object passed as argument must be non-null
   * and point toward the same HmmStateAlphabet instance. The three object will be copied if needed, and
   * deleted when the hmm likelihood objet is deleted. You should secure a copy before if you don't want them to
   * be destroyed with this object.
   *
   * @param hiddenAlphabet The hidden states alphabet to use.
   * @param transitionMatrix The transition matrix to use.
   * @param emissionProbabilities The emission probabilities to use.
   * @param prefix A namespace for parameter names.
   * @param maxSize the maximum size of the vector of scales. If this size is exceeded, then a temporary likelihood computation is made and stored, and the vector is reset.
   * the size of the vector specify the memory usage of the class. A two low value can lead to numerical precision errors.
   */
  LowMemoryRescaledHmmLikelihood(
    std::shared_ptr<HmmStateAlphabet> hiddenAlphabet,
    std::shared_ptr<HmmTransitionMatrix> transitionMatrix,
    std::shared_ptr<HmmEmissionProbabilities> emissionProbabilities,
    const std::string& prefix,
    size_t maxSize = 1000000);

  LowMemoryRescaledHmmLikelihood(const LowMemoryRescaledHmmLikelihood& lik) :
    AbstractHmmLikelihood(lik),
    AbstractParametrizable(lik),
    hiddenAlphabet_(dynamic_cast<HmmStateAlphabet*>(lik.hiddenAlphabet_->clone())),
    transitionMatrix_(dynamic_cast<HmmTransitionMatrix*>(lik.transitionMatrix_->clone())),
    emissionProbabilities_(dynamic_cast<HmmEmissionProbabilities*>(lik.emissionProbabilities_->clone())),
    likelihood1_(lik.likelihood1_),
    likelihood2_(lik.likelihood2_),
    logLik_(lik.logLik_),
    maxSize_(lik.maxSize_),
    breakPoints_(lik.breakPoints_),
    nbStates_(lik.nbStates_),
    nbSites_(lik.nbSites_)
  {
    // Now adjust pointers:
    transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_);
    emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_);
  }

  LowMemoryRescaledHmmLikelihood& operator=(const LowMemoryRescaledHmmLikelihood& lik)
  {
    AbstractHmmLikelihood::operator=(lik);
    AbstractParametrizable::operator=(lik);
    hiddenAlphabet_        = std::unique_ptr<HmmStateAlphabet>(dynamic_cast<HmmStateAlphabet*>(lik.hiddenAlphabet_->clone()));
    transitionMatrix_      = std::unique_ptr<HmmTransitionMatrix>(dynamic_cast<HmmTransitionMatrix*>(lik.transitionMatrix_->clone()));
    emissionProbabilities_ = std::unique_ptr<HmmEmissionProbabilities>(dynamic_cast<HmmEmissionProbabilities*>(lik.emissionProbabilities_->clone()));
    likelihood1_           = lik.likelihood1_;
    likelihood2_           = lik.likelihood2_;
    logLik_                = lik.logLik_;
    maxSize_               = lik.maxSize_;
    breakPoints_           = lik.breakPoints_;
    nbStates_              = lik.nbStates_;
    nbSites_               = lik.nbSites_;

    // Now adjust pointers:
    transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_);
    emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_);
    return *this;
  }

  virtual ~LowMemoryRescaledHmmLikelihood() {}

  LowMemoryRescaledHmmLikelihood* clone() const override { return new LowMemoryRescaledHmmLikelihood(*this); }

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
  }

  const std::vector<size_t>& getBreakPoints() const override { return breakPoints_; }

  void setParameters(const ParameterList& pl) override
  {
    setParametersValues(pl);
  }

  double getValue() const override { return -logLik_; }

  double getLogLikelihood() const override { return logLik_; }

  void setNamespace(const std::string& nameSpace) override;

  void fireParameterChanged(const ParameterList& pl) override;

  double getLikelihoodForASite(size_t site) const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::getLikelihoodForASite. This class can't compute posterior probabilities, use RescaledHmmLikelihood instead."));
  }


  Vdouble getLikelihoodForEachSite() const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::getLikelihoodForEachSite. This class can't compute posterior probabilities, use RescaledHmmLikelihood instead."));
  }

  Vdouble getHiddenStatesPosteriorProbabilitiesForASite(size_t site) const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::getHiddenStatesPosteriorProbabilitiesForASite. This class can't compute posterior probabilities, use RescaledHmmLikelihood instead."));
  }

  void getHiddenStatesPosteriorProbabilities(std::vector< std::vector<double> >& probs, bool append = false) const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::getHiddenStatesPosteriorProbabilities. This class can't compute posterior probabilities, use RescaledHmmLikelihood instead."));
  }

protected:
  void computeForward_();

  void computeDLikelihood_() const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::computeDLikelihood_. Use RescaledHmmLikelihood instead."));
  }

  void computeD2Likelihood_() const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::computeD2Likelihood_. Use RescaledHmmLikelihood instead."));
  }

  double getDLogLikelihoodForASite(size_t site) const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::getDLogLikelihoodForASite. Use RescaledHmmLikelihood instead."));
    return 0;
  }

  double getD2LogLikelihoodForASite(size_t site) const override
  {
    throw (NotImplementedException("LowMemoryRescaledHmmLikelihood::getD2LogLikelihoodForASite. Use RescaledHmmLikelihood instead."));
    return 0;
  }
};
}
#endif // BPP_NUMERIC_HMM_LOWMEMORYRESCALEDHMMLIKELIHOOD_H

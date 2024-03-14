
/*
  Copyright or © or Copr. Bio++ Development Team, (November 16, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for phylogenetic data analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_NUMERIC_HMM_LOGSUMHMMLIKELIHOOD_H
#define BPP_NUMERIC_HMM_LOGSUMHMMLIKELIHOOD_H


#include "../AbstractParametrizable.h"
#include "../Matrix/Matrix.h"
#include "../NumTools.h"
#include "HmmLikelihood.h"

// From the STL:
#include <vector>
#include <memory>

namespace bpp
{
/**
 * @brief A simple implementation of hidden Markov models recursion.
 *
 * This implementation uses the logsum method described in Durbin et al "Biological sequence analysis", Cambridge University Press,
 * and further developped in Tobias P. Mann "Numerically Stable Hidden Markov Model Implementation" (2006) http://bozeman.genome.washington.edu/compbio/mbt599_2006/hmm_scaling_revised.pdf .
 * It also offer the possibility to specify "breakpoints", where the chain will be reset to the equilibrium frequencies.
 *
 * Although probably more numerically accurate, this method is slower than the rescaling, as it involves one exponentiation per site and per hidden state!
 *
 * @see RescaledHmmLikelihood
 */
class LogsumHmmLikelihood :
  public virtual AbstractHmmLikelihood,
  public AbstractParametrizable
{
protected:
  /**
   * @brief The alphabet describing the hidden states.
   */
  std::shared_ptr<HmmStateAlphabet> hiddenAlphabet_;
  std::shared_ptr<HmmTransitionMatrix> transitionMatrix_;
  std::shared_ptr<HmmEmissionProbabilities> emissionProbabilities_;

  /**
   * @brief The likelihood array.
   *
   * logLikelihood_[i * nbStates_ + j] corresponds to log(Pr(x1...xi, yi=j)),
   * where the x are the observed states, and y the hidden states.
   */
  std::vector<double> logLikelihood_;
  std::vector<double> partialLogLikelihoods_;
  double logLik_;

  /**
   * @brief The DLogLikelihood arrays.
   *
   * dLogLikelihood_[i][j] corresponds to d(log(Pr(x1...xi, yi=j))),
   * where the x are the observed states, and y the hidden states.
   *
   * d2LogLikelihood_[i][j] corresponds to d2(log(Pr(x1...xi, yi=j))),
   * where the x are the observed states, and y the hidden states.
   *
   */
  mutable std::vector< std::vector<double> > dLogLikelihood_;
  mutable std::vector<double> partialDLogLikelihoods_;

  mutable std::vector< std::vector<double> > d2LogLikelihood_;
  mutable std::vector<double> partialD2LogLikelihoods_;

  /**
   * @brief backward logLikelihood
   *
   * backLogLikelihood_[i][j] corresponds to log(Pr(x_i+1...x_n | yi=j)),
   * where the x are the observed states, and y the hidden states.
   */
  mutable std::vector<std::vector<double> > backLogLikelihood_;
  mutable bool backLogLikelihoodUpToDate_;

  std::vector<size_t> breakPoints_;

  size_t nbStates_, nbSites_;

public:
  /**
   * @brief Build a new LogsumHmmLikelihood object.
   *
   * @warning the HmmTransitionMatrix and HmmEmissionProbabilities
   * object passed as argument must be non-null and point toward the
   * same HmmStateAlphabet instance.
   */
  LogsumHmmLikelihood(
    std::shared_ptr<HmmStateAlphabet> hiddenAlphabet,
    std::shared_ptr<HmmTransitionMatrix> transitionMatrix,
    std::shared_ptr<HmmEmissionProbabilities> emissionProbabilities,
    const std::string& prefix = "");

  LogsumHmmLikelihood(const LogsumHmmLikelihood& lik) :
    AbstractHmmLikelihood(lik),
    AbstractParametrizable(lik),
    hiddenAlphabet_(),
    transitionMatrix_(),
    emissionProbabilities_(),
    logLikelihood_(lik.logLikelihood_),
    partialLogLikelihoods_(lik.partialLogLikelihoods_),
    logLik_(lik.logLik_),
    dLogLikelihood_(lik.dLogLikelihood_),
    partialDLogLikelihoods_(lik.partialDLogLikelihoods_),
    d2LogLikelihood_(lik.d2LogLikelihood_),
    partialD2LogLikelihoods_(lik.partialD2LogLikelihoods_),
    backLogLikelihood_(lik.backLogLikelihood_),
    backLogLikelihoodUpToDate_(lik.backLogLikelihoodUpToDate_),
    breakPoints_(lik.breakPoints_),
    nbStates_(lik.nbStates_),
    nbSites_(lik.nbSites_)
  {
    hiddenAlphabet_ = std::shared_ptr<HmmStateAlphabet>(lik.hiddenAlphabet_->clone());
    transitionMatrix_ = std::shared_ptr<HmmTransitionMatrix>(lik.transitionMatrix_->clone());
    emissionProbabilities_ = std::shared_ptr<HmmEmissionProbabilities>(lik.emissionProbabilities_->clone());

    // Now adjust pointers:
    transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_);
    emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_);
  }

  LogsumHmmLikelihood& operator=(const LogsumHmmLikelihood& lik)
  {
    AbstractHmmLikelihood::operator=(lik);
    AbstractParametrizable::operator=(lik);

    hiddenAlphabet_            = std::shared_ptr<HmmStateAlphabet>(lik.hiddenAlphabet_->clone());
    transitionMatrix_          = std::shared_ptr<HmmTransitionMatrix>(lik.transitionMatrix_->clone());
    emissionProbabilities_     = std::shared_ptr<HmmEmissionProbabilities>(lik.emissionProbabilities_->clone());

    logLikelihood_             = lik.logLikelihood_;
    partialLogLikelihoods_     = lik.partialLogLikelihoods_;
    dLogLikelihood_            = lik.dLogLikelihood_;
    partialDLogLikelihoods_    = lik.partialDLogLikelihoods_;
    d2LogLikelihood_           = lik.d2LogLikelihood_;
    partialD2LogLikelihoods_   = lik.partialD2LogLikelihoods_;
    backLogLikelihood_         = lik.backLogLikelihood_;
    backLogLikelihoodUpToDate_ = lik.backLogLikelihoodUpToDate_;
    logLik_                    = lik.logLik_;
    breakPoints_               = lik.breakPoints_;
    nbStates_                  = lik.nbStates_;
    nbSites_                   = lik.nbSites_;

    // Now adjust pointers:
    transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_);
    emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_);
    return *this;
  }

  virtual ~LogsumHmmLikelihood() {}


  LogsumHmmLikelihood* clone() const override { return new LogsumHmmLikelihood(*this); }

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
    backLogLikelihoodUpToDate_ = false;
  }

  const std::vector<size_t>& getBreakPoints() const override { return breakPoints_; }

  void setParameters(const ParameterList& pl) override
  {
    setParametersValues(pl);
  }

  double getValue() const override { return -logLik_; }

  double getLogLikelihood() const override { return logLik_; }

  double getLikelihoodForASite(size_t site) const override;

  double getDLogLikelihoodForASite(size_t site) const override ;

  double getD2LogLikelihoodForASite(size_t site) const override;

  Vdouble getLikelihoodForEachSite() const override;

  void setNamespace(const std::string& nameSpace) override;

  void fireParameterChanged(const ParameterList& pl) override;

  Vdouble getHiddenStatesPosteriorProbabilitiesForASite(size_t site) const override;

  void getHiddenStatesPosteriorProbabilities(std::vector< std::vector<double> >& probs, bool append = false) const override;

  void computeLikelihood();

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
#endif // BPP_NUMERIC_HMM_LOGSUMHMMLIKELIHOOD_H

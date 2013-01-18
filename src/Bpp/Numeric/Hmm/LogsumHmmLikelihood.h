//
// File: LogsumHmmLikelihood.h
// Created by: Julien Dutheil
// Created on: Mon Nov 23 11:27 2009
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 16, 2004)

This software is a computer program whose purpose is to provide classes
for phylogenetic data analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _LOGSUMHMMLIKELIHOOD_H_
#define _LOGSUMHMMLIKELIHOOD_H_

#include "HmmLikelihood.h"
#include "../AbstractParametrizable.h"
#include "../NumTools.h"
#include "../Matrix/Matrix.h"

//From the STL:
#include <vector>
#include <memory>

namespace bpp {

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
  class LogsumHmmLikelihood:
    public virtual HmmLikelihood,
    public AbstractParametrizable
  {
    private:
      /**
       * @brief The alphabet describing the hidden states.
       */
      std::auto_ptr<HmmStateAlphabet> hiddenAlphabet_;
      std::auto_ptr<HmmTransitionMatrix> transitionMatrix_;
      std::auto_ptr<HmmEmissionProbabilities> emissionProbabilities_;

      /**
       * @brief The likelihood array.
       *
       * logLikelihood_[i * nbStates_ + j] corresponds to Pr(x1...xi, yi=j),
       * where the x are the observed states, and y the hidden states.
       */
      std::vector<double> logLikelihood_;
      std::vector<double> partialLogLikelihoods_;
      double logLik_;

      std::vector<size_t> breakPoints_;

      size_t nbStates_, nbSites_;

    public:
      /**
       * @brief Build a new LogsumHmmLikelihood object.
       *
       * @warning the HmmTransitionMatrix and HmmEmissionProbabilities object passed as argument must be non-null
       * and point toward the same HmmStateAlphabet instance. The three object will be copied if needed, and
       * deleted when the hmm likelihood objet is deleted. You should secure a copy before if you don't want them to
       * be destroyed with this object.
       */
      LogsumHmmLikelihood(
          HmmStateAlphabet* hiddenAlphabet,
          HmmTransitionMatrix* transitionMatrix,
          HmmEmissionProbabilities* emissionProbabilities,
          const std::string& prefix) throw (Exception);

      LogsumHmmLikelihood(const LogsumHmmLikelihood& lik):
        AbstractParametrizable(lik),
        hiddenAlphabet_(dynamic_cast<HmmStateAlphabet*>(lik.hiddenAlphabet_->clone())),
        transitionMatrix_(dynamic_cast<HmmTransitionMatrix*>(lik.transitionMatrix_->clone())),
        emissionProbabilities_(dynamic_cast<HmmEmissionProbabilities*>(lik.emissionProbabilities_->clone())),
        logLikelihood_(lik.logLikelihood_),
        partialLogLikelihoods_(lik.partialLogLikelihoods_),
        logLik_(lik.logLik_),
        breakPoints_(lik.breakPoints_),
        nbStates_(lik.nbStates_),
        nbSites_(lik.nbSites_)
      {
        // Now adjust pointers:
        transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_.get());
        emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_.get());
      }

      LogsumHmmLikelihood& operator=(const LogsumHmmLikelihood& lik)
      {
        AbstractParametrizable::operator =(lik);
        hiddenAlphabet_        = std::auto_ptr<HmmStateAlphabet>(dynamic_cast<HmmStateAlphabet*>(lik.hiddenAlphabet_->clone()));
        transitionMatrix_      = std::auto_ptr<HmmTransitionMatrix>(dynamic_cast<HmmTransitionMatrix*>(lik.transitionMatrix_->clone()));
        emissionProbabilities_ = std::auto_ptr<HmmEmissionProbabilities>(dynamic_cast<HmmEmissionProbabilities*>(lik.emissionProbabilities_->clone()));
        logLikelihood_         = lik.logLikelihood_;
        partialLogLikelihoods_ = lik.partialLogLikelihoods_;
        logLik_                = lik.logLik_;
        breakPoints_           = lik.breakPoints_;
        nbStates_              = lik.nbStates_;
        nbSites_               = lik.nbSites_;

        // Now adjust pointers:
        transitionMatrix_->setHmmStateAlphabet(hiddenAlphabet_.get());
        emissionProbabilities_->setHmmStateAlphabet(hiddenAlphabet_.get());
        return *this;
      }

      virtual ~LogsumHmmLikelihood() {}

#ifndef NO_VIRTUAL_COV
      LogsumHmmLikelihood*
#else
      Clonable*
#endif
      clone() const { return new LogsumHmmLikelihood(*this); }

    public:
      const HmmStateAlphabet& getHmmStateAlphabet() const { return *hiddenAlphabet_; }
      HmmStateAlphabet& getHmmStateAlphabet() { return *hiddenAlphabet_; }

      const HmmTransitionMatrix& getHmmTransitionMatrix() const { return *transitionMatrix_; }
      HmmTransitionMatrix& getHmmTransitionMatrix() { return *transitionMatrix_; }

      const HmmEmissionProbabilities& getHmmEmissionProbabilities() const { return *emissionProbabilities_; }
      HmmEmissionProbabilities& getHmmEmissionProbabilities() { return *emissionProbabilities_; }

      void setBreakPoints(const std::vector<size_t>& breakPoints) {
        breakPoints_ = breakPoints;
        computeForward_();
      }

      const std::vector<size_t>& getBreakPoints() const { return breakPoints_; }

      void setParameters(const ParameterList& pl) throw (Exception)
      {
        setParametersValues(pl);
      }

      double getValue() const throw (Exception) { return -logLik_; }

      double getLogLikelihood() const { return logLik_; }

      void fireParameterChanged(const ParameterList& pl);

      void getHiddenStatesPosteriorProbabilities(std::vector< std::vector<double> >& probs, bool append = false) const throw (Exception);

    protected:
      void computeForward_();
      void computeBackward_(std::vector< std::vector<double> >& b) const;

  };

}

#endif //_LOGSUMHMMLIKELIHOOD_H_


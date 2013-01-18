//
// File: LogsumHmmLikelihood.cpp
// Created by: Julien Dutheil
// Created on: Fri Oct 26 11:57 2007
//

/*
Copyright or © or Copr. CNRS, (November 16, 2004)

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

#include "LogsumHmmLikelihood.h"

// from the STL:
#include <iostream>
#include <algorithm>
using namespace bpp;
using namespace std;

LogsumHmmLikelihood::LogsumHmmLikelihood(
    HmmStateAlphabet* hiddenAlphabet,
    HmmTransitionMatrix* transitionMatrix,
    HmmEmissionProbabilities* emissionProbabilities,
    const std::string& prefix) throw (Exception):
  AbstractParametrizable(prefix),
  hiddenAlphabet_(hiddenAlphabet),
  transitionMatrix_(transitionMatrix),
  emissionProbabilities_(emissionProbabilities),
  logLikelihood_(),
  partialLogLikelihoods_(),
  logLik_(),
  breakPoints_(),
  nbStates_(),
  nbSites_()
{
  if (!hiddenAlphabet)        throw Exception("LogsumHmmLikelihood: null pointer passed for HmmStateAlphabet.");
  if (!transitionMatrix)      throw Exception("LogsumHmmLikelihood: null pointer passed for HmmTransitionMatrix.");
  if (!emissionProbabilities) throw Exception("LogsumHmmLikelihood: null pointer passed for HmmEmissionProbabilities.");
  if (!hiddenAlphabet_->worksWith(transitionMatrix->getHmmStateAlphabet()))
    throw Exception("LogsumHmmLikelihood: HmmTransitionMatrix and HmmEmissionProbabilities should point toward the same HmmStateAlphabet object.");
  if (!hiddenAlphabet_->worksWith(emissionProbabilities->getHmmStateAlphabet()))
    throw Exception("LogsumHmmLikelihood: HmmTransitionMatrix and HmmEmissionProbabilities should point toward the same HmmStateAlphabet object.");
  nbStates_ = hiddenAlphabet_->getNumberOfStates();
  nbSites_ = emissionProbabilities_->getNumberOfPositions();

  //Manage parameters:
  addParameters_(hiddenAlphabet_->getParameters());
  addParameters_(transitionMatrix_->getParameters());
  addParameters_(emissionProbabilities_->getParameters());

  //Init arrays:
  logLikelihood_.resize(nbSites_ * nbStates_);
  
  //Compute:
  computeForward_();
}

void LogsumHmmLikelihood::fireParameterChanged(const ParameterList& pl)
{
  bool alphabetChanged    = hiddenAlphabet_->matchParametersValues(pl);
  bool transitionsChanged = transitionMatrix_->matchParametersValues(pl);
  bool emissionChanged    = emissionProbabilities_->matchParametersValues(pl);
  // these lines are necessary because the transitions and emissions can depend on the alphabet.
  // we could use a StateChangeEvent, but this would result in computing some calculations twice in some cases
  // (when both the alphabet and other parameter changed).
  if (alphabetChanged && !transitionsChanged) transitionMatrix_->setParametersValues(transitionMatrix_->getParameters());
  if (alphabetChanged && !emissionChanged) emissionProbabilities_->setParametersValues(emissionProbabilities_->getParameters());
  
  computeForward_();
}

/***************************************************************************************************************************/

void LogsumHmmLikelihood::computeForward_()
{
  double x, a;
  vector<double> logTrans(nbStates_ * nbStates_);

  //Transition probabilities:
  for (size_t i = 0; i < nbStates_; i++)
  {
    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++)
      logTrans[ii + j] = log(transitionMatrix_->Pij(static_cast<int>(j), static_cast<int>(i)));
  }

  //Initialisation:
  const vector<double>* emissions = &(*emissionProbabilities_)(0);
  for (size_t j = 0; j < nbStates_; j++)
  {
    size_t jj = j * nbStates_;
    x = logTrans[jj] + log(transitionMatrix_->getEquilibriumFrequencies()[0]);
    for (size_t k = 1; k < nbStates_; k++)
    {
      a = logTrans[k + jj] + log(transitionMatrix_->getEquilibriumFrequencies()[k]);
      x = NumTools::logsum(x, a);
    }
    logLikelihood_[j] = log((*emissions)[j]) + x;
  }
 
  //Recursion:
  size_t nextBrkPt = nbSites_; //next break point
  vector<size_t>::const_iterator bpIt = breakPoints_.begin();
  if (bpIt != breakPoints_.end()) nextBrkPt = *bpIt;
  partialLogLikelihoods_.clear();
 
  for (size_t i = 1; i < nbSites_; i++)
  {
    size_t ii = i * nbStates_;
    size_t iip = (i - 1) * nbStates_;
    emissions = &(*emissionProbabilities_)(i);
    if (i < nextBrkPt)
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = logTrans[jj] + logLikelihood_[iip];
        for (size_t k = 1; k < nbStates_; k++)
        {
          a = logTrans[jj + k] + logLikelihood_[iip + k];
          x = NumTools::logsum(x, a);
        }
        logLikelihood_[ii + j] = log((*emissions)[j]) + x;
      }
    }
    else //Reset markov chain:
    {
      //Termination of previous segment:
      double tmpLog = logLikelihood_[(i - 1) * nbStates_];
      for (size_t k = 1; k < nbStates_; k++)
        tmpLog = NumTools::logsum(tmpLog, logLikelihood_[(i - 1) * nbStates_ + k]);
      partialLogLikelihoods_.push_back(tmpLog);

      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = logTrans[jj] + log(transitionMatrix_->getEquilibriumFrequencies()[0]);
        for (size_t k = 1; k < nbStates_; k++)
        {
          a = logTrans[jj + k] + log(transitionMatrix_->getEquilibriumFrequencies()[k]);
          x = NumTools::logsum(x, a);
        }
        logLikelihood_[ii + j] = log((*emissions)[j]) + x;
      }
      bpIt++;
      if (bpIt != breakPoints_.end()) nextBrkPt = *bpIt;
      else nextBrkPt = nbSites_;
    }
  }
  //Termination:
  double tmpLog = logLikelihood_[(nbSites_ - 1) * nbStates_];
  for (unsigned int k = 1; k < nbStates_; k++)
    tmpLog = NumTools::logsum(tmpLog, logLikelihood_[(nbSites_ - 1) * nbStates_ + k]);
  partialLogLikelihoods_.push_back(tmpLog);
  
  //Compute likelihood:
  logLik_ = 0;
  vector<double> copy = partialLogLikelihoods_; //We need to keep the original order for posterior decoding.
  sort(copy.begin(), copy.end());
  for (size_t i = copy.size(); i > 0; --i)
    logLik_ += copy[i - 1];
}

/***************************************************************************************************************************/

void LogsumHmmLikelihood::computeBackward_(std::vector< std::vector<double> >& b) const
{
  b.resize(nbSites_);
  for (size_t i = 0; i < nbSites_; i++)
  {
    b[i].resize(nbStates_);
  }
  double x;

  //Transition probabilities:
  vector<double> logTrans(nbStates_ * nbStates_);
  for (size_t i = 0; i < nbStates_; i++)
  {
    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++)
      logTrans[ii + j] = log(transitionMatrix_->Pij(static_cast<int>(i), static_cast<int>(j)));
  }


  //Initialisation:
  const vector<double>* emissions = 0;
  size_t nextBrkPt = 0; //next break point
  vector<size_t>::const_reverse_iterator bpIt = breakPoints_.rbegin();
  if (bpIt != breakPoints_.rend()) nextBrkPt = *bpIt;
  
  for (size_t k = 0; k < nbStates_; k++)
  {
    b[nbSites_ - 1][k] = 0.;
  }

  //Recursion:
  for (size_t i = nbSites_ - 1; i > 0; i--)
  {
    emissions = &(*emissionProbabilities_)(i);
    if (i > nextBrkPt)
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = log((*emissions)[0]) + logTrans[jj] + b[i][0];
        for (size_t k = 1; k < nbStates_; k++)
        {
          x = NumTools::logsum(x, log((*emissions)[k]) + logTrans[jj + k] + b[i][k]);
        }
        b[i - 1][j] = x;
      }    
    }
    else //Reset markov chain
    {
      for (unsigned int j = 0; j < nbStates_; j++)
      {
        b[i - 1][j] = 0.;
      }    
      bpIt++;
      if (bpIt != breakPoints_.rend()) nextBrkPt = *bpIt;
      else nextBrkPt = 0;
    }
  }
}

/***************************************************************************************************************************/

void LogsumHmmLikelihood::getHiddenStatesPosteriorProbabilities(std::vector< std::vector<double> >& probs, bool append) const throw (Exception)
{
  size_t offset = append ? probs.size() : 0;
  probs.resize(offset + nbSites_);
  for (size_t i = 0; i < nbSites_; i++)
  {
    probs[offset + i].resize(nbStates_);
  }

  vector< vector<double> > logB;
  computeBackward_(logB);
 
  size_t nextBrkPt = nbSites_; //next break point
  vector<size_t>::const_iterator bpIt = breakPoints_.begin();
  if (bpIt != breakPoints_.end()) nextBrkPt = *bpIt;
 
  vector<double>::const_iterator logLikIt = partialLogLikelihoods_.begin();
  for (size_t i = 0; i < nbSites_; i++)
  {
    if (i == nextBrkPt) {
      logLikIt++;
      bpIt++;
      if (bpIt != breakPoints_.end()) nextBrkPt = *bpIt;
      else nextBrkPt = nbSites_;
    }

    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++)
    {
      probs[offset + i][j] = exp(logLikelihood_[ii + j] + logB[i][j] - *logLikIt);
    }
  }
}

/***************************************************************************************************************************/


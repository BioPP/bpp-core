//
// File: RescaledHmmLikelihood.cpp
// Created by: Julien Dutheil
// Created on: Fri Oct 26 11:57 2007
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

#include "RescaledHmmLikelihood.h"

#include "../../App/ApplicationTools.h"

// from the STL:
#include <iostream>
#include <algorithm>
using namespace bpp;
using namespace std;

RescaledHmmLikelihood::RescaledHmmLikelihood(
    HmmStateAlphabet* hiddenAlphabet,
    HmmTransitionMatrix* transitionMatrix,
    HmmEmissionProbabilities* emissionProbabilities,
    const std::string& prefix) throw (Exception):
  AbstractParametrizable(prefix),
  hiddenAlphabet_(hiddenAlphabet),
  transitionMatrix_(transitionMatrix),
  emissionProbabilities_(emissionProbabilities),
  likelihood_(),
  scales_(),
  logLik_(),
  breakPoints_(),
  nbStates_(),
  nbSites_()
{
  if (!hiddenAlphabet)        throw Exception("RescaledHmmLikelihood: null pointer passed for HmmStateAlphabet.");
  if (!transitionMatrix)      throw Exception("RescaledHmmLikelihood: null pointer passed for HmmTransitionMatrix.");
  if (!emissionProbabilities) throw Exception("RescaledHmmLikelihood: null pointer passed for HmmEmissionProbabilities.");
  if (!hiddenAlphabet_->worksWith(transitionMatrix->getHmmStateAlphabet()))
    throw Exception("RescaledHmmLikelihood: HmmTransitionMatrix and HmmEmissionProbabilities should point toward the same HmmStateAlphabet object.");
  if (!hiddenAlphabet_->worksWith(emissionProbabilities->getHmmStateAlphabet()))
    throw Exception("RescaledHmmLikelihood: HmmTransitionMatrix and HmmEmissionProbabilities should point toward the same HmmStateAlphabet object.");
  nbStates_ = hiddenAlphabet_->getNumberOfStates();
  nbSites_ = emissionProbabilities_->getNumberOfPositions();

  //Manage parameters:
  addParameters_(hiddenAlphabet_->getParameters());
  addParameters_(transitionMatrix_->getParameters());
  addParameters_(emissionProbabilities_->getParameters());

  //Init arrays:
  likelihood_.resize(nbSites_ * nbStates_);
  scales_.resize(nbSites_);
  
  //Compute:
  computeForward_();
}

void RescaledHmmLikelihood::fireParameterChanged(const ParameterList& pl)
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

void RescaledHmmLikelihood::computeForward_()
{
  double x;
  vector<double> tmp(nbStates_);
  vector<double> lScales(nbSites_);
  vector<double> trans(nbStates_ * nbStates_);

  //Transition probabilities:
  for (size_t i = 0; i < nbStates_; i++)
  {
    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++) {
      trans[ii + j] = transitionMatrix_->Pij(static_cast<int>(j), static_cast<int>(i));
      if (isnan(trans[ii + j]))
        throw Exception("RescaledHmmLikelihood::computeForward_. NaN transition probability");
      if (trans[ii + j] < 0)
        throw Exception("RescaledHmmLikelihood::computeForward_. Negative transition probability: " + TextTools::toString(trans[ii + j]));
    }
  }

  //Initialisation:
  scales_[0] = 0;
  const vector<double>* emissions = &(*emissionProbabilities_)(0);
  for (size_t j = 0; j < nbStates_; j++)
  {
    size_t jj = j * nbStates_;
    x = 0;
    for (size_t k = 0; k < nbStates_; k++)
    {
      x += trans[k + jj] * transitionMatrix_->getEquilibriumFrequencies()[k];
      //cerr << j << "\t" << k << "\t" << trans[k + jj] << "\t" << transitionMatrix_->getEquilibriumFrequencies()[k] << "\t" << trans[k + jj] * transitionMatrix_->getEquilibriumFrequencies()[k] << "\t" << x << endl;  
    }
    tmp[j] = (*emissions)[j] * x;
    //cerr << "e[j]=" << (*emissions)[j] << "\t" << tmp[j] << endl;
    scales_[0] += tmp[j];
  }
  for (size_t j = 0; j < nbStates_; j++)
  {
    likelihood_[j] = tmp[j] / scales_[0];
  }
  lScales[0] = log(scales_[0]);
 
  //Recursion:
  size_t nextBrkPt = nbSites_; //next break point
  vector<size_t>::const_iterator bpIt = breakPoints_.begin();
  if (bpIt != breakPoints_.end()) nextBrkPt = *bpIt;
  
  double a;
  for (size_t i = 1; i < nbSites_; i++)
  {
    size_t ii = i * nbStates_;
    size_t iip = (i - 1) * nbStates_;
    scales_[i] = 0 ;
    emissions = &(*emissionProbabilities_)(i);
    if (i < nextBrkPt)
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = 0;
        for (size_t k = 0; k < nbStates_; k++)
        {
          a = trans[jj + k] * likelihood_[iip + k];
          //if (a < 0)
          //{
          //  (*ApplicationTools::warning << "Negative value for likelihood at " << i << ", state " << j << ": " << likelihood_[iip + k] << ", Pij = " << trans[jj + k]).endLine();
          //  a = 0;
          //}
          x += a;
        }
        tmp[j] = (*emissions)[j] * x;
        if (tmp[j] < 0)
        {
          (*ApplicationTools::warning << "Negative probability at " << i << ", state " << j << ": " << (*emissions)[j] << "\t" << x).endLine();
          tmp[j] = 0;
        }
        scales_[i] += tmp[j];
      }
    }
    else //Reset markov chain:
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = 0;
        for (size_t k = 0; k < nbStates_; k++)
        {
          a = trans[jj + k] * transitionMatrix_->getEquilibriumFrequencies()[k];
          //if (a < 0)
          //{
          //  (*ApplicationTools::warning << "Negative value for likelihood at " << i << ", state " << j << ": ,Pij = " << trans[jj + k]).endLine();
          //  a = 0;
          //}
          x += a;
        }
        tmp[j] = (*emissions)[j] * x;
        //if (tmp[j] < 0)
        //{
        //  (*ApplicationTools::warning << "Negative emission probability at " << i << ", state " << j << ": " << (*emissions)[j]).endLine();
        //  tmp[j] = 0;
        //}
        scales_[i] += tmp[j];
      }
      bpIt++;
      if (bpIt != breakPoints_.end()) nextBrkPt = *bpIt;
      else nextBrkPt = nbSites_;
    }

    for (size_t j = 0; j < nbStates_; j++)
    {
      if (scales_[i] > 0) likelihood_[ii + j] = tmp[j] / scales_[i];
      else                likelihood_[ii + j] = 0;
    }
    lScales[i] = log(scales_[i]);
  }
  greater<double> cmp;
  sort(lScales.begin(), lScales.end(), cmp);
  logLik_ = 0;
  for (size_t i = 0; i < nbSites_; ++i)
  {
    logLik_ += lScales[i];
  }
}

/***************************************************************************************************************************/

void RescaledHmmLikelihood::computeBackward_(std::vector< std::vector<double> >& b) const
{
  b.resize(nbSites_);
  for (size_t i = 0; i < nbSites_; i++)
  {
    b[i].resize(nbStates_);
  }
  double x;

  //Transition probabilities:
  vector<double> trans(nbStates_ * nbStates_);
  for (size_t i = 0; i < nbStates_; i++)
  {
    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++)
      trans[ii + j] = transitionMatrix_->Pij(static_cast<int>(i), static_cast<int>(j));
  }


  //Initialisation:
  const vector<double>* emissions = 0;
  size_t nextBrkPt = 0; //next break point
  vector<size_t>::const_reverse_iterator bpIt = breakPoints_.rbegin();
  if (bpIt != breakPoints_.rend()) nextBrkPt = *bpIt;
  
  for (size_t j = 0; j < nbStates_; j++)
  {
    x = 0;
    b[nbSites_ - 1][j] = 1.;
  }

  //Recursion:
  for (size_t i = nbSites_ - 1; i > 0; i--)
  {
    emissions = &(*emissionProbabilities_)(i);
    if (i > nextBrkPt)
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        x = 0;
        size_t jj = j * nbStates_;
        for (size_t k = 0; k < nbStates_; k++)
        {
          x += (*emissions)[k] * trans[jj + k] * b[i][k];
        }
        b[i-1][j] = x / scales_[i];
      }    
    }
    else //Reset markov chain
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        b[i-1][j] = 1.;
      }    
      bpIt++;
      if (bpIt != breakPoints_.rend()) nextBrkPt = *bpIt;
      else nextBrkPt = 0;
    }
  }
}

/***************************************************************************************************************************/

void RescaledHmmLikelihood::getHiddenStatesPosteriorProbabilities(std::vector< std::vector<double> >& probs, bool append) const throw (Exception)
{
  size_t offset = append ? probs.size() : 0;
  probs.resize(offset + nbSites_);
  for (size_t i = 0; i < nbSites_; i++)
  {
    probs[offset + i].resize(nbStates_);
  }

  vector< vector<double> > b;
  computeBackward_(b);
  
  for (size_t i = 0; i < nbSites_; i++)
  {
    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++)
    {
      probs[offset + i][j] = likelihood_[ii + j] * b[i][j];
    }
  }
}

/***************************************************************************************************************************/


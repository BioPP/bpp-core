// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "LowMemoryRescaledHmmLikelihood.h"

// from the STL:
#include <iostream>
#include <algorithm>
using namespace bpp;
using namespace std;

LowMemoryRescaledHmmLikelihood::LowMemoryRescaledHmmLikelihood(
  std::shared_ptr<HmmStateAlphabet> hiddenAlphabet,
  std::shared_ptr<HmmTransitionMatrix> transitionMatrix,
  std::shared_ptr<HmmEmissionProbabilities> emissionProbabilities,
  const std::string& prefix,
  size_t maxSize) :
  AbstractHmmLikelihood(),
  AbstractParametrizable(prefix),
  hiddenAlphabet_(hiddenAlphabet),
  transitionMatrix_(transitionMatrix),
  emissionProbabilities_(emissionProbabilities),
  likelihood1_(),
  likelihood2_(),
  logLik_(),
  maxSize_(maxSize),
  breakPoints_(),
  nbStates_(),
  nbSites_()
{
  if (!hiddenAlphabet)
    throw Exception("LowMemoryRescaledHmmLikelihood: null pointer passed for HmmStateAlphabet.");
  if (!transitionMatrix)
    throw Exception("LowMemoryRescaledHmmLikelihood: null pointer passed for HmmTransitionMatrix.");
  if (!emissionProbabilities)
    throw Exception("LowMemoryRescaledHmmLikelihood: null pointer passed for HmmEmissionProbabilities.");
  if (!hiddenAlphabet_->worksWith(transitionMatrix->hmmStateAlphabet()))
    throw Exception("LowMemoryRescaledHmmLikelihood: HmmTransitionMatrix and HmmEmissionProbabilities should point toward the same HmmStateAlphabet object.");
  if (!hiddenAlphabet_->worksWith(emissionProbabilities->hmmStateAlphabet()))
    throw Exception("LowMemoryRescaledHmmLikelihood: HmmTransitionMatrix and HmmEmissionProbabilities should point toward the same HmmStateAlphabet object.");
  nbStates_ = hiddenAlphabet_->getNumberOfStates();
  nbSites_ = emissionProbabilities_->getNumberOfPositions();

  // Manage parameters:
  addParameters_(hiddenAlphabet_->getParameters());
  addParameters_(transitionMatrix_->getParameters());
  addParameters_(emissionProbabilities_->getParameters());

  // Init arrays:
  likelihood1_.resize(nbStates_);
  likelihood2_.resize(nbStates_);

  // Compute:
  computeForward_();
}

void LowMemoryRescaledHmmLikelihood::setNamespace(const std::string& nameSpace)
{
  AbstractParametrizable::setNamespace(nameSpace);

  hiddenAlphabet_->setNamespace(nameSpace);
  transitionMatrix_->setNamespace(nameSpace);
  emissionProbabilities_->setNamespace(nameSpace);
}

void LowMemoryRescaledHmmLikelihood::fireParameterChanged(const ParameterList& pl)
{
  bool alphabetChanged    = hiddenAlphabet_->matchParametersValues(pl);
  bool transitionsChanged = transitionMatrix_->matchParametersValues(pl);
  bool emissionChanged    = emissionProbabilities_->matchParametersValues(pl);
  // these lines are necessary because the transitions and emissions can depend on the alphabet.
  // we could use a StateChangeEvent, but this would result in computing some calculations twice in some cases
  // (when both the alphabet and other parameter changed).
  if (alphabetChanged && !transitionsChanged)
    transitionMatrix_->setParametersValues(transitionMatrix_->getParameters());
  if (alphabetChanged && !emissionChanged)
    emissionProbabilities_->setParametersValues(emissionProbabilities_->getParameters());

  computeForward_();
}

/***************************************************************************************************************************/

void LowMemoryRescaledHmmLikelihood::computeForward_()
{
  double x;
  vector<double> tmp(nbStates_);
  vector<double> lScales(min(maxSize_, nbSites_));
  vector<double> trans(nbStates_ * nbStates_);

  // Transition probabilities:
  for (size_t i = 0; i < nbStates_; i++)
  {
    size_t ii = i * nbStates_;
    for (size_t j = 0; j < nbStates_; j++)
    {
      trans[ii + j] = transitionMatrix_->Pij(j, i);
    }
  }

  // Initialisation:
  double scale = 0;
  const vector<double>* emissions = &(*emissionProbabilities_)(0);
  for (size_t j = 0; j < nbStates_; j++)
  {
    size_t jj = j * nbStates_;
    x = 0;
    for (size_t k = 0; k < nbStates_; k++)
    {
      x += trans[k + jj] * transitionMatrix_->getEquilibriumFrequencies()[k];
    }
    tmp[j] = (*emissions)[j] * x;
    scale += tmp[j];
  }
  for (size_t j = 0; j < nbStates_; j++)
  {
    likelihood1_[j] = tmp[j] / scale;
  }
  lScales[0] = log(scale);

  vector<double>* previousLikelihood = &likelihood2_, * currentLikelihood = &likelihood1_, * tmpLikelihood;

  // Recursion:
  size_t nextBrkPt = nbSites_; // next break point
  vector<size_t>::const_iterator bpIt = breakPoints_.begin();
  if (bpIt != breakPoints_.end())
    nextBrkPt = *bpIt;

  double a;
  logLik_ = 0;
  size_t offset = 0;
  greater<double> cmp;
  for (size_t i = 1; i < nbSites_; i++)
  {
    // Swap pointers:
    tmpLikelihood = previousLikelihood;
    previousLikelihood = currentLikelihood;
    currentLikelihood = tmpLikelihood;

    scale = 0;
    emissions = &(*emissionProbabilities_)(i);
    if (i < nextBrkPt)
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = 0;
        for (size_t k = 0; k < nbStates_; k++)
        {
          a = trans[jj + k] * (*previousLikelihood)[k];
          if (a < 0)
          {
            // *ApplicationTools::warning << "Negative value for likelihood at " << i << ", state " << j << ": " << _likelihood[i-1][k] << ", Pij = " << _hiddenModel->Pij(k, j) << endl;
            a = 0;
          }
          x += a;
        }
        tmp[j] = (*emissions)[j] * x;
        if (tmp[j] < 0)
        {
          // *ApplicationTools::warning << "Negative emission probability at " << i << ", state " << j << ": " << _emissions[i][j] << endl;
          tmp[j] = 0;
        }
        scale += tmp[j];
      }
    }
    else // Reset markov chain:
    {
      for (size_t j = 0; j < nbStates_; j++)
      {
        size_t jj = j * nbStates_;
        x = 0;
        for (size_t k = 0; k < nbStates_; k++)
        {
          a = trans[jj + k] * transitionMatrix_->getEquilibriumFrequencies()[k];
          if (a < 0)
          {
            // *ApplicationTools::warning << "Negative value for likelihood at " << i << ", state " << j << ": " << _likelihood[i-1][k] << ", Pij = " << _hiddenModel->Pij(k, j) << endl;
            a = 0;
          }
          x += a;
        }
        tmp[j] = (*emissions)[j] * x;
        if (tmp[j] < 0)
        {
          // *ApplicationTools::warning << "Negative emission probability at " << i << ", state " << j << ": " << _emissions[i][j] << endl;
          tmp[j] = 0;
        }
        scale += tmp[j];
      }
      bpIt++;
      if (bpIt != breakPoints_.end())
        nextBrkPt = *bpIt;
      else
        nextBrkPt = nbSites_;
    }

    for (size_t j = 0; j < nbStates_; j++)
    {
      if (scale > 0)
        (*currentLikelihood)[j] = tmp[j] / scale;
      else
        (*currentLikelihood)[j] = 0;
    }
    lScales[i - offset] = log(scale);

    if (i - offset == maxSize_ - 1)
    {
      // We make partial calculations and reset the arrays:
      double partialLogLik = 0;
      sort(lScales.begin(), lScales.end(), cmp);
      for (size_t j = 0; j < maxSize_; ++j)
      {
        partialLogLik += lScales[j];
      }
      logLik_ += partialLogLik;
      offset += maxSize_;
    }
  }
  sort(lScales.begin(), lScales.begin() + static_cast<ptrdiff_t>(nbSites_ - offset), cmp);
  double partialLogLik = 0;
  for (size_t i = 0; i < nbSites_ - offset; ++i)
  {
    partialLogLik += lScales[i];
  }
  logLik_ += partialLogLik;
}

/***************************************************************************************************************************/

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Matrix/MatrixTools.h"
#include "../Random/RandomTools.h"
#include "../VectorTools.h"
#include "AbstractHmmTransitionMatrix.h"

using namespace bpp;
using namespace std;

AbstractHmmTransitionMatrix::AbstractHmmTransitionMatrix(std::shared_ptr<const HmmStateAlphabet> alph, const string& prefix) :
  alph_(alph),
  pij_(static_cast<size_t>(alph->getNumberOfStates()), static_cast<size_t>(alph->getNumberOfStates())),
  tmpmat_(static_cast<size_t>(alph->getNumberOfStates()), static_cast<size_t>(alph->getNumberOfStates())),
  eqFreq_(static_cast<size_t>(alph->getNumberOfStates())),
  upToDate_(false)
{}

AbstractHmmTransitionMatrix::AbstractHmmTransitionMatrix(const AbstractHmmTransitionMatrix& hptm) :
  alph_(hptm.alph_),
  pij_(hptm.pij_),
  tmpmat_(hptm.tmpmat_),
  eqFreq_(hptm.eqFreq_),
  upToDate_(hptm.upToDate_)
{}

AbstractHmmTransitionMatrix& AbstractHmmTransitionMatrix::operator=(const AbstractHmmTransitionMatrix& hptm)
{
  alph_ = hptm.alph_;
  pij_ = hptm.pij_;
  tmpmat_ = hptm.tmpmat_;
  eqFreq_ = hptm.eqFreq_;
  upToDate_ = hptm.upToDate_;

  return *this;
}

void AbstractHmmTransitionMatrix::setHmmStateAlphabet(std::shared_ptr<const HmmStateAlphabet> stateAlphabet)
{
  if (stateAlphabet == nullptr)
    throw HmmUnvalidAlphabetException("Null alphabet in AbstractHmmTransitionMatrix::setHmmStateAlphabet");

  alph_ = stateAlphabet;
}

vector<size_t> AbstractHmmTransitionMatrix::sample(size_t size) const
{
  vector<size_t> vres;
  if (size == 0)
    return vres;

  size_t nbStates = hmmStateAlphabet().getNumberOfStates();

  // update pij_
  getPij();

  size_t sta = 0, stb;
  double prob = RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);

  for (size_t i = 0; i < nbStates; ++i)
  {
    prob -= eqFreq_[i];
    if (prob < 0)
    {
      sta = i;
      break;
    }
  }

  vres.push_back(sta);

  for (size_t pos = 1; pos < size; pos++)
  {
    prob = RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);

    const vector<double>& row = pij_.getRow(sta);

    for (size_t i = 0; i < nbStates; ++i)
    {
      prob -= row[i];
      if (prob < 0)
      {
        stb = i;
        break;
      }
    }
    vres.push_back(stb);
    sta = stb;
  }
  return vres;
}

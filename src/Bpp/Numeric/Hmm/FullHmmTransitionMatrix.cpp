// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Text/TextTools.h"
#include "../Matrix/MatrixTools.h"
#include "../VectorTools.h"
#include "FullHmmTransitionMatrix.h"

using namespace bpp;
using namespace std;

FullHmmTransitionMatrix::FullHmmTransitionMatrix(std::shared_ptr<const HmmStateAlphabet> alph, const string& prefix) :
  AbstractHmmTransitionMatrix(alph),
  AbstractParametrizable(prefix),
  vSimplex_()
{
  size_t size = static_cast<size_t>(getNumberOfStates());

  for (size_t i = 0; i < size; ++i)
  {
    vSimplex_.push_back(Simplex(size, 1, false, prefix + TextTools::toString(i + 1) + "."));
    addParameters_(vSimplex_[i].getParameters());
  }
}

FullHmmTransitionMatrix::FullHmmTransitionMatrix(const FullHmmTransitionMatrix& hptm) :
  AbstractHmmTransitionMatrix(hptm),
  AbstractParametrizable(hptm),
  vSimplex_(hptm.vSimplex_)
{}

FullHmmTransitionMatrix& FullHmmTransitionMatrix::operator=(const FullHmmTransitionMatrix& hptm)
{
  AbstractHmmTransitionMatrix::operator=(hptm);
  AbstractParametrizable::operator=(hptm);

  return *this;
}

void FullHmmTransitionMatrix::setTransitionProbabilities(const Matrix<double>& mat)
{
  if (mat.getNumberOfRows() != vSimplex_.size())
    throw BadSizeException("FullHmmTransitionMatrix::setTransitionProbabilities: Wrong number of rows in given Matrix", mat.getNumberOfRows(), vSimplex_.size());

  ParameterList pl;

  for (size_t i = 0; i < mat.getNumberOfRows(); ++i)
  {
    vSimplex_[i].setFrequencies(mat.row(i));
    ParameterList pls = vSimplex_[i].getParameters();
    for (size_t j = 0; j < pls.size(); ++j)
    {
      Parameter* p = pls[j].clone();
      p->setName(TextTools::toString(i + 1) + "." + p->getName());
      pl.addParameter(p);
    }
  }

  matchParametersValues(pl);
}


const Matrix<double>& FullHmmTransitionMatrix::getPij() const
{
  if (!upToDate_)
  {
    for (size_t i = 0; i < vSimplex_.size(); ++i)
    {
      for (size_t j = 0; j < vSimplex_[i].dimension(); ++j)
      {
        pij_(i, j) = vSimplex_[i].prob(j);
      }
    }
    upToDate_ = true;
  }

  return pij_;
}

const std::vector<double>& FullHmmTransitionMatrix::getEquilibriumFrequencies() const
{
  size_t salph = getNumberOfStates();

  if (!upToDate_)
  {
    pij_ = getPij();

    MatrixTools::pow(pij_, 256, tmpmat_);

    for (size_t i = 0; i < salph; ++i)
    {
      eqFreq_[i] = tmpmat_(0, i);
    }

    upToDate_ = true;
  }

  return eqFreq_;
}

void FullHmmTransitionMatrix::fireParameterChanged(const ParameterList& parameters)
{
  size_t salph = getNumberOfStates();

  for (size_t i = 0; i < salph; ++i)
  {
    vSimplex_[i].matchParametersValues(parameters);
  }

  upToDate_ = false;
}

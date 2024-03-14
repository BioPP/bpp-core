// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Text/TextTools.h"
#include "../Matrix/MatrixTools.h"
#include "../VectorTools.h"
#include "AutoCorrelationTransitionMatrix.h"

using namespace bpp;
using namespace std;

AutoCorrelationTransitionMatrix::AutoCorrelationTransitionMatrix(std::shared_ptr<const HmmStateAlphabet> alph, const string& prefix) :
  AbstractHmmTransitionMatrix(alph),
  AbstractParametrizable(prefix),
  vAutocorrel_()
{
  size_t size = static_cast<size_t>(getNumberOfStates());
  double p = 1. / static_cast<double>(size);

  for (size_t i = 0; i < size; ++i)
  {
    vAutocorrel_.push_back(1. / static_cast<double>(size));
    addParameter_(new Parameter(prefix + "lambda" + TextTools::toString(i + 1), p, Parameter::PROP_CONSTRAINT_EX));
  }

  for (size_t i = 0; i < size; ++i)
  {
    eqFreq_[i] = p;
  }
}


AutoCorrelationTransitionMatrix::AutoCorrelationTransitionMatrix(const AutoCorrelationTransitionMatrix& aptm) :
  AbstractHmmTransitionMatrix(aptm),
  AbstractParametrizable(aptm),
  vAutocorrel_(aptm.vAutocorrel_)
{}

AutoCorrelationTransitionMatrix& AutoCorrelationTransitionMatrix::operator=(const AutoCorrelationTransitionMatrix& aptm)
{
  AbstractHmmTransitionMatrix::operator=(aptm);
  AbstractParametrizable::operator=(aptm);

  vAutocorrel_ = aptm.vAutocorrel_;

  return *this;
}

const Matrix<double>& AutoCorrelationTransitionMatrix::getPij() const
{
  if (!upToDate_)
  {
    for (size_t i = 0; i < vAutocorrel_.size(); ++i)
    {
      for (size_t j = 0; j < vAutocorrel_.size(); ++j)
      {
        pij_(i, j) = (i == j) ? vAutocorrel_[i] : (1 - vAutocorrel_[i]) / static_cast<double>(getNumberOfStates() - 1);
      }
    }

    upToDate_ = true;
  }

  return pij_;
}

const std::vector<double>& AutoCorrelationTransitionMatrix::getEquilibriumFrequencies() const
{
  return eqFreq_;
}

void AutoCorrelationTransitionMatrix::fireParameterChanged(const ParameterList& parameters)
{
  size_t salph = getNumberOfStates();

  for (size_t i = 0; i < salph; i++)
  {
    vAutocorrel_[i] = getParameterValue("lambda" + TextTools::toString(i + 1));
  }

  upToDate_ = false;
}

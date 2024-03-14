// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "HmmLikelihood.h"

using namespace bpp;
using namespace std;

AbstractHmmLikelihood::AbstractHmmLikelihood() :
  dLogLik_(0),
  dVariable_(""),
  d2LogLik_(0),
  d2Variable_("") {}

AbstractHmmLikelihood::AbstractHmmLikelihood(const AbstractHmmLikelihood& adhlik) :
  dLogLik_(adhlik.dLogLik_),
  dVariable_(adhlik.dVariable_),
  d2LogLik_(adhlik.d2LogLik_),
  d2Variable_(adhlik.d2Variable_)
{}

AbstractHmmLikelihood& AbstractHmmLikelihood::operator=(const AbstractHmmLikelihood& adhlik)
{
  dLogLik_ = adhlik.dLogLik_;
  dVariable_ = adhlik.dVariable_;
  d2LogLik_ = adhlik.d2LogLik_;
  d2Variable_ = adhlik.d2Variable_;

  return *this;
}

double AbstractHmmLikelihood::getFirstOrderDerivative(const std::string& variable) const
{
  if (variable != dVariable_)
  {
    dVariable_ = variable;

    hmmEmissionProbabilities().computeDEmissionProbabilities(dVariable_);
    computeDLikelihood_();
  }
  return -dLogLik_;
}

double AbstractHmmLikelihood::getSecondOrderDerivative(const std::string& variable) const
{
  if (variable != d2Variable_)
  {
    d2Variable_ = variable;

    hmmEmissionProbabilities().computeD2EmissionProbabilities(d2Variable_);
    computeD2Likelihood_();
  }
  return -d2LogLik_;
}

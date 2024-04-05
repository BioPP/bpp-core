// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../NumConstants.h"
#include "../VectorTools.h"
#include "Simplex.h"

using namespace bpp;
using namespace std;

Simplex::Simplex(const std::vector<double>& probas, unsigned short method, bool allowNull, const std::string& name) : AbstractParameterAliasable(name),
  dim_(probas.size()),
  method_(method),
  vProb_(),
  valpha_()
{
  if  (dim_ == 0)
    return;

  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > NumConstants::SMALL())
    throw Exception("Simplex. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  std::shared_ptr<ConstraintInterface> pc = (allowNull ? Parameter::PROP_CONSTRAINT_IN : Parameter::PROP_CONSTRAINT_EX);

  for (auto& p : probas)
  {
    vProb_.push_back(p);
  }

  double y = 1;
  switch (method_)
  {
  case 1:
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), vProb_[i] / y, pc));
      y -= vProb_[i];
    }
    break;
  case 2:
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), vProb_[i] / (vProb_[i] + vProb_[i + 1]), pc));
    }
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      valpha_.push_back(vProb_[i + 1] / vProb_[i]);
    }
    break;
  case 3:
    for (size_t i = 1; i < dim_; i++)
    {
      size_t o = i;
      size_t li2 = 0; // rank of the strongest bit
      while (o)
      {
        li2++;
        o = o >> 1;
      }

      double i1 = 0, i0 = 0;
      size_t j = 0;
      size_t pi = i &  ~(1 << (li2 - 1));
      while (j < dim_)
      {
        size_t t = (j << li2) + pi;
        if (t >= dim_)
          break;
        else
          i0 += vProb_[t];
        t += (1 << (li2 - 1));
        if (t < dim_)
          i1 += vProb_[t];
        j++;
      }
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i), i1 / (i0 + i1), pc));
    }
    break;
  }
}

Simplex::Simplex(size_t dim, unsigned short method, bool allowNull, const std::string& name) :
  AbstractParameterAliasable(name),
  dim_(dim),
  method_(method),
  vProb_(),
  valpha_()
{
  if  (dim_ == 0)
    return;

  for (size_t i = 0; i < dim_; i++)
  {
    vProb_.push_back(1. / static_cast<double>(dim_));
  }

  auto pc = (allowNull ? Parameter::PROP_CONSTRAINT_IN : Parameter::PROP_CONSTRAINT_EX);

  double y = 1;
  switch (method_)
  {
  case 1:
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), vProb_[i] / y, pc));
      y -= vProb_[i];
    }
    break;
  case 2:
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), 0.5, pc));
    }
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      valpha_.push_back(1.);
    }
    break;
  case 3:
    for (size_t i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), 0.5, pc));
    }
    setFrequencies(vProb_);

    break;
  }
}

void Simplex::fireParameterChanged(const ParameterList& parameters)
{
  if  (dim_ == 0)
    return;

  double x = 1.0;
  switch (method_)
  {
  case 1:
    double th;
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      th = getParameterValue("theta" + TextTools::toString(i + 1));
      vProb_[i] = th * x;
      x *= 1 - th;
    }
    vProb_[dim_ - 1] = x;
    break;
  case 2:
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      th = getParameterValue("theta" + TextTools::toString(i + 1));
      valpha_[i] = (1 - th) / th;
    }
    th = 1;
    vProb_[0] = 1;
    x = 1.0;
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      th *= valpha_[i];
      vProb_[i + 1] = th;
      x += vProb_[i + 1];
    }

    if (x > NumConstants::TINY()) // avoid rounding pb
      for (auto& vp : vProb_)
      {
        vp /= x;
      }
    else
      for (auto& vp : vProb_)
      {
        vp = 1.0 / double(dim_);
      }

    break;
  case 3:
    size_t o = dim_;
    size_t ld2 = 0; // rank of the strongest bit
    while (o)
    {
      ld2++;
      o = o >> 1;
    }
    for (size_t i = 0; i < dim_; i++)
    {
      x = 1;
      size_t ld = ld2;
      size_t k = i;
      while (ld)
      {
        if (k >> (ld - 1))
          x *= getParameterValue("theta" + TextTools::toString(k));
        else
        {
          if ((k + (1 << (ld - 1))) < dim_)
            x *= 1 - getParameterValue("theta" + TextTools::toString(k + (1 << (ld - 1))));
        }
        k &= ~(1 << (--ld));
      }
      vProb_[i] = x;
    }
    break;
  }
}


void Simplex::setFrequencies(const std::vector<double>& probas)
{
  if  (dim_ == 0)
    return;

  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > NumConstants::SMALL())
    throw Exception("Simplex::setFrequencies. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  double y = 1;

  ParameterList pl;
  switch (method_)
  {
  case 1:
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      pl.addParameter(Parameter(getNamespace() + "theta" + TextTools::toString(i + 1), probas[i] / y));
      y -= probas[i];
    }
    break;
  case 2:
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      pl.addParameter(Parameter(getNamespace() + "theta" + TextTools::toString(i + 1), probas[i] / (probas[i] + probas[i + 1])));
      valpha_[i] = probas[i + 1] / probas[i];
    }
    break;
  case 3:
    for (size_t i = 1; i < dim_; i++)
    {
      size_t o = i;
      size_t li2 = 0; // rank of the strongest bit
      while (o)
      {
        li2++;
        o = o >> 1;
      }

      double i1 = 0, i0 = 0;
      size_t j = 0;
      size_t pi = i &  ~(1 << (li2 - 1));
      while (j < dim_)
      {
        size_t t = (j << li2) + pi;
        if (t >= dim_)
          break;
        else
          i0 += probas[t];
        t += (1 << (li2 - 1));
        if (t < dim_)
          i1 += probas[t];
        j++;
      }
      pl.addParameter(Parameter(getNamespace() + "theta" + TextTools::toString(i), i1 / (i0 + i1)));
    }
    break;
  }

  matchParametersValues(pl);
}


/*****************************/

OrderedSimplex::OrderedSimplex(const std::vector<double>& probas, unsigned short method, bool allowNull, const std::string& name) :
  Simplex(probas.size(), method, allowNull, name),
  vValues_(probas)
{
  OrderedSimplex::setFrequencies(probas);
}

void OrderedSimplex::fireParameterChanged(const ParameterList& pl)
{
  Simplex::fireParameterChanged(pl);
  const auto& probs = Simplex::getFrequencies();

  auto dim = probs.size();

  double x = 0;
  for (auto i = dim; i > 0; i--)
  {
    x += probs[i - 1] / (int)i;
    vValues_[i - 1] = x;
  }
}

void OrderedSimplex::setFrequencies(const std::vector<double>& vValues)
{
  vValues_ = vValues;

  auto dim = vValues.size();
  Vdouble vprob(dim);

  for (size_t i = 0; i < dim - 1; ++i)
  {
    vprob[i] = static_cast<double>(i + 1) * (vValues[i] - vValues[i + 1]);
  }

  vprob[dim - 1] = static_cast<double>(dim) * vValues[dim - 1];
  Simplex::setFrequencies(vprob);
}

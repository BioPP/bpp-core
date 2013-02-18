//
// File: Simplex.cpp
// Created by: Laurent Guéguen
// Created on: mardi 31 mai 2011, à 13h 16
//

/*
   Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

   This software is a computer program whose purpose is to provide classes
   for numerical calculus.

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

#include "Simplex.h"
#include "../NumConstants.h"

#include "../VectorTools.h"

using namespace bpp;
using namespace std;

Simplex::Simplex(const std::vector<double>& probas, unsigned short method, const std::string& name) : AbstractParameterAliasable(name),
  dim_(probas.size()),
  method_(method),
  vProb_(),
  valpha_()
{
  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > NumConstants::SMALL())
    throw Exception("Simplex. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  for (unsigned int i = 0; i < dim_; i++)
  {
    vProb_.push_back(probas[i]);
  }

  switch (method_)
  {
  case 2:
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), vProb_[i] / (vProb_[i] + vProb_[i + 1]), &Parameter::PROP_CONSTRAINT_IN));
    }
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      valpha_.push_back(vProb_[i + 1] / vProb_[i]);
    }
    break;
  case 1:
    double y = 1;
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name + "theta" + TextTools::toString(i + 1), vProb_[i] / y, &Parameter::PROP_CONSTRAINT_IN));
      y -= vProb_[i];
    }
    break;
  }
}

Simplex::Simplex(size_t dim, unsigned short method, const std::string& name) :
  AbstractParameterAliasable(name),
  dim_(dim),
  method_(method),
  vProb_(),
  valpha_()
{
  for (size_t i = 0; i < dim_; i++)
  {
    vProb_.push_back(1. / static_cast<double>(dim_));
  }

  switch (method_)
  {
  case 2:
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name+ "theta" + TextTools::toString(i + 1), 0.5, &Parameter::PROP_CONSTRAINT_IN));
    }
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      valpha_.push_back(1.);
    }
    break;
  case 1:
    double y = 1;
    for (unsigned int i = 0; i < dim_ - 1; i++)
    {
      addParameter_(new Parameter(name+"theta" + TextTools::toString(i + 1), vProb_[i] / y, &Parameter::PROP_CONSTRAINT_IN));
      y -= vProb_[i];
    }
    break;
  }
}

void Simplex::fireParameterChanged(const ParameterList& parameters)
{
  AbstractParameterAliasable::fireParameterChanged(parameters);

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
    for (unsigned int i = 0; i < dim_; i++)
      vProb_[i] /= x;

    break;
  }
}


void Simplex::setFrequencies(const std::vector<double>& probas)
{
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
          pl.addParameter(Parameter(getNamespace()+"theta" + TextTools::toString(i + 1), probas[i] / y));
          y -= probas[i];
        }
      break;
    case 2:
      for (unsigned int i = 0; i < dim_ - 1; i++)
        {
          pl.addParameter(Parameter(getNamespace()+"theta" + TextTools::toString(i + 1), probas[i] / (probas[i] + probas[i + 1])));
          valpha_[i]=probas[i + 1] / probas[i];
        }
      break;
    }

  matchParametersValues(pl);
}


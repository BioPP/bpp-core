//
// File: TruncatedExponentialDiscreteDistribution.cpp
// Created by: Julien Dutheil
// Created on: Fri Jan 25 15:24 2008
//

/*
Copyright or © or Copr. CNRS, (November 17, 2004)

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

#include "TruncatedExponentialDiscreteDistribution.h"
#include "../RandomTools.h"
#include "../../Utils/MapTools.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;
  
/** Constructor: **************************************************************/

TruncatedExponentialDiscreteDistribution::TruncatedExponentialDiscreteDistribution(unsigned int n, double lambda, double truncationPoint, bool median) : AbstractDiscreteDistribution("TruncExponential."), bounds_(), useMedian_(median)
{
  addParameter_(Parameter("TruncExponential.lambda", lambda, new IncludingPositiveReal(0.000001), true));
  addParameter_(Parameter("TruncExponential.tp", truncationPoint, &Parameter::R_PLUS));
  applyParameters(n);
}

TruncatedExponentialDiscreteDistribution::~TruncatedExponentialDiscreteDistribution() {}

/******************************************************************************/

void TruncatedExponentialDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  applyParameters(getNumberOfCategories());  
}

/******************************************************************************/

Domain TruncatedExponentialDiscreteDistribution::getDomain() const
{
  return Domain(bounds_, MapTools::getKeys<double, double, AbstractDiscreteDistribution::Order>(distribution_));
}

/******************************************************************************/

void TruncatedExponentialDiscreteDistribution::applyParameters(unsigned int numberOfCategories)
{
  discretize(numberOfCategories, getParameter_(0).getValue(), getParameter_(1).getValue(), useMedian_);
}

/******************************************************************************/

void TruncatedExponentialDiscreteDistribution::discretize(unsigned int numberOfCategories, double lambda, double tp, bool median)
{
  if (numberOfCategories == 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in ExponentialDiscreteDistribution::applyParameters()." << endl;
  distribution_.clear();
  bounds_.resize(numberOfCategories + 1);
  if (numberOfCategories == 1)
  {
    double value = median ? 
      (log(2.) - log(1. + exp(-lambda * tp)))/ lambda :
      (1. / lambda) - tp  / (exp(lambda * tp) - 1.);
    distribution_[value] = 1.0;
    bounds_[0] = 0; bounds_[1] = tp;
    return;
  }
  else if(numberOfCategories > 1)
  {
    distribution_.clear();

    bounds_[0] = 0;
    vector<double> values(numberOfCategories);

    //The probabilty without the truncated part:
    double totalProb = 1 - exp(-lambda * tp);

    for (unsigned int i = 1; i <= numberOfCategories; i++)
    {
      double a = bounds_[i-1];
      double b;
      if (i == numberOfCategories)
        b = tp;
      else
        b = (1. / lambda) * log(static_cast<double>(numberOfCategories) / (static_cast<double>(numberOfCategories) - static_cast<double>(i) * totalProb));
      bounds_[i] = b;
      if(median)
        values[i-1] = (1. / lambda) * log(2 * static_cast<double>(numberOfCategories) / (static_cast<double>(2 * numberOfCategories) - static_cast<double>(2 * i - 1) * totalProb)); 
      else
        values[i-1] = 1. / lambda + (a*exp(-a*lambda) - b*exp(-b*lambda)) / (exp(-a*lambda) - exp(-b*lambda));
    }
    bounds_[numberOfCategories] = tp;

    double p = 1. / static_cast<double>(numberOfCategories);
    for (unsigned int i = 0; i < numberOfCategories; i++)
    {
      distribution_[values[i]] += p;
    }
    if (getNumberOfCategories() != numberOfCategories)
    {
      cout << "WARNING!!! Couldn't create " << numberOfCategories << " distinct categories." << endl;
      for(unsigned int i = 0; i < getNumberOfCategories(); i++)
      {
        cout << i << "\t" << values[i] << endl;
      }
      cout << "Lambda\t" << lambda << endl; 
      cout << "Tp\t"     << tp << endl; 
      cout << "WARNING!!! This may occure if you specified a too big lambda parameter or a two small upper bound." << endl;
    }
    return ;
  }
  else
  {
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in ExponentialDiscreteDistribution::applyParameters()." << endl;
  }
}

bool TruncatedExponentialDiscreteDistribution::adaptToConstraint(const Constraint& c, bool f)
{
  const Interval* pi=dynamic_cast<const Interval*>(&c);

  if (pi==NULL)
    return false;

  if (pi->getUpperBound()!=NumConstants::VERY_BIG ||
      (pi->getLowerBound()>=0  && pi->strictLowerBound()))
    return false;
  else {
    if (! pi->isCorrect(getParameterValue("tp")))
      return false;

    if (pi->getLowerBound()>=0 && f)
      getParameter_("tp").setConstraint(new IncludingPositiveReal(pi->getLowerBound()));
    return true;
  }
}

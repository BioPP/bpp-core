//
// File: GaussianDiscreteDistribution.cpp
// Created by: Laurent Guéguen
// Created on: April 2010
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

#include "GaussianDiscreteDistribution.h"
#include "../RandomTools.h"
#include "../NumConstants.h"
#include "../../Utils/MapTools.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

GaussianDiscreteDistribution::GaussianDiscreteDistribution(unsigned int n, double mu, double sigma) :
  AbstractDiscreteDistribution("Gaussian."), bounds_() 
{
  Parameter p1("Gaussian.mu", mu);
  addParameter_(p1);
  Parameter p2("Gaussian.sigma", sigma, &Parameter::R_PLUS_STAR);
  addParameter_(p2);
  applyParameters(n);
}

GaussianDiscreteDistribution::~GaussianDiscreteDistribution() {}

/******************************************************************************/

void GaussianDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  applyParameters(getNumberOfCategories());  
}

/******************************************************************************/

Domain GaussianDiscreteDistribution::getDomain() const
{
  return Domain(bounds_, MapTools::getKeys<double, double, AbstractDiscreteDistribution::Order>(distribution_));
}

/******************************************************************************/

void GaussianDiscreteDistribution::applyParameters(unsigned int numberOfCategories)
{
  if(numberOfCategories <= 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in GaussianDiscreteDistribution::applyParameters()." << endl;
  double mu = getParameterValue("mu");
  double sigma = getParameterValue("sigma");
  discretize(numberOfCategories,mu,sigma, false);
}

/******************************************************************************/

void GaussianDiscreteDistribution::discretize(unsigned int nbClasses, double mu, double sigma, bool median)
{
/* discretization of normal distribution with equal proportions in
   each category */

  if (nbClasses <= 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in GaussianDiscreteDistribution::discretize()." << endl;
  
  distribution_.clear();
  bounds_.resize(nbClasses + 1);

  bounds_[0] = -NumConstants::VERY_BIG;
  for(unsigned int i = 1; i < nbClasses; i++)
    bounds_[i] = RandomTools::qNorm((double)i/(double)nbClasses, mu, sigma);
  bounds_[nbClasses] = NumConstants::VERY_BIG;
  
  unsigned int i;
  double gap05=1.0/(2.0*nbClasses), t;
  vector<double> values(nbClasses);

  if(median)
  {
    for (i=0; i<nbClasses; i++)
      values[i]=RandomTools::qNorm((i*2.0+1)*gap05, mu, sigma);
    for (i=0,t=0; i<nbClasses; i++) t+=values[i];
    for (i=0; i<nbClasses; i++) values[i]*=mu/t*nbClasses;
  }
  else
  {
    if(nbClasses==1)
      values[0] = mu;
    else
    {
      double a,b=bounds_[1];
      values[0]=(sigma/sqrt(2*M_PI)*(-exp(-pow((b-mu)/sigma,2)/2))
                 +mu*RandomTools::pNorm((b-mu)/sigma))*nbClasses;
      
      for (i=1; i<nbClasses-1; i++){
        b=bounds_[i+1];
        a=bounds_[i];
        values[i]=(sigma/sqrt(2*M_PI)*(exp(-pow((a-mu)/sigma,2)/2)-exp(-pow((b-mu)/sigma,2)/2))
                   +mu*(RandomTools::pNorm((b-mu)/sigma)-RandomTools::pNorm((a-mu)/sigma)))*nbClasses;
      }
      a=bounds_[nbClasses-1];
      values[nbClasses-1]=(sigma/sqrt(2*M_PI)*exp(-pow((a-mu)/sigma,2)/2)
                           +mu*(1-RandomTools::pNorm((a-mu)/sigma)))*nbClasses;
    }
  }

  double p=1./static_cast<double>(nbClasses);
  for (i=0;i<nbClasses;i++){
    distribution_[values[i]]+=p;
  }
  
  if(getNumberOfCategories() != nbClasses)
    {
      cout << "WARNING!!! Couldn't create " << nbClasses << " distinct categories." << endl;
      cout << "WARNING!!! This may occure if you specified a too low sigma parameter." << endl;
    }
  return ;
}

bool GaussianDiscreteDistribution::adaptToConstraint(const Constraint& c, bool f)
{
  const Interval* pi=dynamic_cast<const Interval*>(&c);

  if (pi==NULL)
    return false;

  if (pi->getLowerBound()!=-NumConstants::VERY_BIG || pi->getUpperBound()!=NumConstants::VERY_BIG)
    return false;
  else
    return true;
}
    

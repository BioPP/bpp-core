//
// File: BetaDiscreteDistribution.cpp
// Created by: Laurent Guéguen
// Created on: lundi 31 mai 2010, à 11h 15
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

#include "BetaDiscreteDistribution.h"
#include "../RandomTools.h"
#include "../NumConstants.h"
#include "../../Utils/MapTools.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

BetaDiscreteDistribution::BetaDiscreteDistribution(unsigned int n, double alpha, double beta) :
  AbstractDiscreteDistribution("Beta."), bounds_() 
{
  Parameter p1("Beta.alpha", alpha, new IncludingPositiveReal(0.0001), true);
  addParameter_(p1);
  Parameter p2("Beta.beta", beta, new IncludingPositiveReal(0.0001), true);
  addParameter_(p2);
  applyParameters(n);
}

BetaDiscreteDistribution::~BetaDiscreteDistribution() {}

/******************************************************************************/

void BetaDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  applyParameters(getNumberOfCategories());  
}

/******************************************************************************/

Domain BetaDiscreteDistribution::getDomain() const
{
  return Domain(bounds_, MapTools::getKeys<double, double, AbstractDiscreteDistribution::Order>(distribution_));
}

/******************************************************************************/

void BetaDiscreteDistribution::applyParameters(unsigned int numberOfCategories)
{
  if(numberOfCategories <= 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in BetaDiscreteDistribution::applyParameters()." << endl;
  double alpha = getParameterValue("alpha");
  double beta = getParameterValue("beta");
  discretize(numberOfCategories,alpha,beta, false);
}

/******************************************************************************/

void BetaDiscreteDistribution::discretize(unsigned int nbClasses, double alpha, double beta, bool median)
{
/* discretization of beta distribution with equal proportions in each 
   category
*/
  if (nbClasses <= 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in BetaDiscreteDistribution::discretize()." << endl;
  
  distribution_.clear();
  bounds_.resize(nbClasses + 1);

  bounds_[0] = 0;
  for(unsigned int i = 1; i < nbClasses; i++)
    bounds_[i] = RandomTools::qBeta((double)i/(double)nbClasses, alpha, beta);
  bounds_[nbClasses] = 1;
  
  unsigned int i;
  double gap05=1.0/(2.0*nbClasses), t;
  vector<double> values(nbClasses);

  if(median)
    {
      for (i=0; i<nbClasses; i++)
        values[i]=RandomTools::qBeta((i*2.0+1)*gap05, alpha, beta);
      for (i=0,t=0; i<nbClasses; i++)
        t+=values[i];
      for (i=0; i<nbClasses; i++)
        values[i]*=alpha/(alpha+beta)/t*nbClasses;
    }
  else
    {
      if(nbClasses==1)
        values[0] = alpha/(alpha+beta);
      else
        for (i=0; i<nbClasses; i++){
          values[i]=(RandomTools::pBeta(bounds_[i+1],alpha+1,beta)
                     -RandomTools::pBeta(bounds_[i],alpha+1,beta))
            *exp(RandomTools::lnBeta(alpha+1,beta)-RandomTools::lnBeta(alpha,beta))
            *nbClasses;
        }
    }

  if (alpha>1)
    for (i=0; i<nbClasses; i++){
      if (values[i]<NumConstants::VERY_TINY)
        values[i]=NumConstants::VERY_TINY;
    }
    
  if (beta>1)
    for (i=0; i<nbClasses; i++){
      if (values[i]>1-NumConstants::VERY_TINY)
        values[i]=1-NumConstants::VERY_TINY;
    }
  
  double p=1./static_cast<double>(nbClasses);
  for (i=0;i<nbClasses;i++)
    distribution_[values[i]]+=p;
  
  
  if(getNumberOfCategories() != nbClasses)
    {
      cout << alpha << " " << beta << endl;
      cout << "WARNING!!! Couldn't create " << nbClasses << " distinct categories." << endl;
    }
  return ;
}

bool BetaDiscreteDistribution::adaptToConstraint(const Constraint& c, bool f)
{
  const Interval* pi=dynamic_cast<const Interval*>(&c);

  if (pi==NULL)
    return false;

  if (pi->getLowerBound()>0 || pi->getUpperBound()<1)
    return false;

  if ((pi->getLowerBound()==0 && pi->strictLowerBound() && getParameterValue("alpha")<=1)
      || (pi->getUpperBound()==1 && pi->strictUpperBound() &&  getParameterValue("beta")<=1))
    return false;

  if (f){
    if (pi->getLowerBound()==0 && pi->strictLowerBound())
      getParameter_("alpha").setConstraint(new ExcludingPositiveReal(1));
    
    if (pi->getUpperBound()==1 && pi->strictUpperBound())
      getParameter_("beta").setConstraint(new ExcludingPositiveReal(1));
  }
  
  return true;

}
    

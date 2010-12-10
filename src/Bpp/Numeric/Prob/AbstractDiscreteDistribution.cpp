//
// File: AbstractDiscreteDistribution.cpp
// Created by: Julien Dutheil
// Created on: ?
//

/*
Copyright or © or Copr. CNRS, (November 19, 2004)

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

#include "AbstractDiscreteDistribution.h"

#include "../RandomTools.h"
#include "../VectorTools.h"

using namespace bpp;
using namespace std;

/******************************************************************************/
  
unsigned int AbstractDiscreteDistribution::getNumberOfCategories() const
{
  return numberOfCategories_;
}

void AbstractDiscreteDistribution::setNumberOfCategories(unsigned int nbClasses)
{
  if (nbClasses <= 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in AbstractDiscreteDistribution::setNumberOfCategories()." << endl;

  if (numberOfCategories_!=nbClasses){
    numberOfCategories_=nbClasses;
    discretize();
  }
}


/******************************************************************************/

double AbstractDiscreteDistribution::getCategory(unsigned int categoryIndex) const
{ 
  map<double, double>::const_iterator it = distribution_.begin();
  for(unsigned int i = 0; i < categoryIndex; i++) it++;
  return it->first;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getProbability(unsigned int categoryIndex) const
{
  map<double, double>::const_iterator it = distribution_.begin();
  for(unsigned int i = 0; i < categoryIndex; i++) it++;
  return it->second;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getProbability(double category) const 
{
  return distribution_.find(category)->second;
}

/******************************************************************************/

Vdouble AbstractDiscreteDistribution::getCategories() const
{
  Vdouble result(distribution_.size());
  unsigned int i = 0;
  for(map<double, double>::const_iterator it = distribution_.begin();
    it != distribution_.end();
    it++)
  {
    result[i] = it->first;
    i++;
  }
  return result;
}

/******************************************************************************/

Vdouble AbstractDiscreteDistribution::getProbabilities() const
{
  Vdouble result(distribution_.size());
  int i = 0;
  for(map<double, double>::const_iterator it = distribution_.begin();
    it != distribution_.end();
    it++) 
  {
    result[i] = it->second;
    i++;
  }
  return result;
}

/******************************************************************************/

void AbstractDiscreteDistribution::set(double category, double probability)
{
  distribution_[category] = probability;
}

/******************************************************************************/

void AbstractDiscreteDistribution::add(double category, double probability)
{
  if(distribution_.find(category) == distribution_.end())
  {
    //new category
    distribution_[category] = probability;
  }
  else
  {
    //existing category
    distribution_[category] += probability;
  }
}

/******************************************************************************/

double AbstractDiscreteDistribution::rand() const 
{
  double r = RandomTools::giveRandomNumberBetweenZeroAndEntry(1);
  double cumprob = 0;
  for(map<double,double>::const_iterator i = distribution_.begin(); 
    i != distribution_.end();
    i++)
  {
    cumprob += i->second;
    if(r <= cumprob) return i->first;
  }
  // This line can't be reached:
  return -1.;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getInfCumulativeProbability(double category) const 
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  for(map<double, double>::const_iterator i = distribution_.begin();
    i != it;
    i++) prob += i->second;
  return prob;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getIInfCumulativeProbability(double category) const
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  if(it == distribution_.end()) return 0;
  for(map<double, double>::const_iterator i = ++it;
    i != distribution_.end();
    i++) prob += i->second;
  return 1. - prob;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getSupCumulativeProbability(double category) const 
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  if(it == distribution_.end()) return 0;
  for(map<double, double>::const_iterator i = ++it;
    i != distribution_.end();
    i++) prob += i->second;
  return prob;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getSSupCumulativeProbability(double category) const
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  for(map<double, double>::const_iterator i = distribution_.begin(); 
    i != it;
    i++) prob += i->second;
  return 1. - prob;
}

/******************************************************************************/

void AbstractDiscreteDistribution::print(OutputStream& out) const
{
  for (map<double, double>::const_iterator i = distribution_.begin(); i != distribution_.end(); i++)
  {
    (out << "Pr(" << (i->first) << ") = " << (i->second)).endLine();
  }
}

/******************************************************************************/

double AbstractDiscreteDistribution::getValueCategory(double value) const throw (OutOfRangeException)
{
  return getDomain().getNearestValue(value);
}

/***********************************************************************/

double AbstractDiscreteDistribution::getLowerBound() const
{
  return bounds_[0];
}

double AbstractDiscreteDistribution::getUpperBound() const
{
  return bounds_[getNumberOfCategories()];
}

void AbstractDiscreteDistribution::discretize()
{
  /* discretization of distribution with equal proportions in each
     category
  */

  distribution_.clear();  
  bounds_.resize(numberOfCategories_ + 1);
  bounds_[0] = intMinMax_.getLowerBound()+(intMinMax_.strictLowerBound()?NumConstants::VERY_TINY:0);
  bounds_[numberOfCategories_] = intMinMax_.getUpperBound()-(intMinMax_.strictUpperBound()?NumConstants::VERY_TINY:0);

  double minX=pProb(bounds_[0]);
  double maxX=pProb(bounds_[numberOfCategories_]);

  double ec;
  unsigned int i;
  vector<double> values(numberOfCategories_);

  // if maxX==minX, uniform discretization of the range
  if (maxX!=minX){
    ec=(maxX-minX)/numberOfCategories_;
    
    for( i = 1; i < numberOfCategories_; i++)
      bounds_[i] = qProb(minX+i*ec);
    
  
    if(median_)
      {
        double t;
        for (i=0; i<numberOfCategories_; i++)
          values[i]=qProb(minX+(i+0.5)*ec);
        
        for (i=0,t=0; i<numberOfCategories_; i++)
          t+=values[i];
        double mean=Expectation(bounds_[numberOfCategories_])-Expectation(bounds_[0]);
        for (i=0; i<numberOfCategories_; i++)
          values[i]*=mean/t*numberOfCategories_/(maxX-minX);
      }
    else
      {
        if(numberOfCategories_==1)
          values[0] = Expectation(bounds_[1])-Expectation(bounds_[0]);
        else{
          double a=Expectation(bounds_[0]), b;
          for (i=0; i<numberOfCategories_; i++){
            b=Expectation(bounds_[i+1]);
            values[i]=(b-a)*numberOfCategories_/(maxX-minX);
            a=b;
          }
        }
      }
  }
  else {
    ec=(bounds_[numberOfCategories_]-bounds_[0])/numberOfCategories_;
    for( i = 1; i < numberOfCategories_; i++)
      bounds_[i] = bounds_[0]+i*ec;

    for( i = 0; i < numberOfCategories_; i++)
      values[i] = (bounds_[i]+bounds_[i+1])/2;
  }

  // useful? //
  
  if (intMinMax_.strictLowerBound())
    for (i=0; i<numberOfCategories_; i++){
      if (values[i]<intMinMax_.getLowerBound()+NumConstants::VERY_TINY)
        values[i]=intMinMax_.getLowerBound()+NumConstants::VERY_TINY;
    }
    
  if (intMinMax_.strictUpperBound())
    for (i=0; i<numberOfCategories_; i++){
      if (values[i]>intMinMax_.getUpperBound()-NumConstants::VERY_TINY)
        values[i]=intMinMax_.getUpperBound()-NumConstants::VERY_TINY;
    }
  
  double p=1./static_cast<double>(numberOfCategories_);
  for (i=0;i<numberOfCategories_;i++)
    if (distribution_.find(values[i])!=distribution_.end()){
      unsigned int j=1;
      int f=((values[i]+NumConstants::TINY)>=intMinMax_.getUpperBound())?-1:1;
      while (distribution_.find(values[i]+f*j*NumConstants::TINY)!=distribution_.end()){
        j++;
        f=((values[i]+f*j*NumConstants::TINY)>=intMinMax_.getUpperBound())?-1:1;
      }
      distribution_[values[i]+f*j*NumConstants::TINY]=p;
    }
    else
      distribution_[values[i]]=p;
    
  return ;
}


void AbstractDiscreteDistribution::restrictToConstraint(const Constraint& c)
{
  const Interval* pi=dynamic_cast<const Interval*>(&c);

  if (pi==NULL)
    throw Exception("AbstractDiscreteDistribution::restrictToConstraint: the constraint is not an interval");

  if (!(intMinMax_<=(*pi))){
    intMinMax_&=c;
    discretize();
  }
}

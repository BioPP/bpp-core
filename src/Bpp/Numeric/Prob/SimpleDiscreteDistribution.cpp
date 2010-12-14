//
// File: SimpleDiscreteDistribution.cpp
// Created by: Julien Dutheil
// Created on: ?
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

#include "SimpleDiscreteDistribution.h"
#include "../NumConstants.h"
#include "../../Utils/MapTools.h"
#include "../../Text/TextTools.h"

using namespace bpp;
using namespace std;

SimpleDiscreteDistribution::SimpleDiscreteDistribution(
                                                       const map<double, double>& distribution) throw(Exception) :
  AbstractDiscreteDistribution(distribution.size(),"Simple.")
{
  double sum=0;
  for(map<double, double>::const_iterator i = distribution.begin(); i != distribution.end(); i++) {
    distribution_[i->first] = i->second;
    sum += i->second;
  }
  if (fabs(1. - sum) > NumConstants::SMALL)
    throw Exception("SimpleDiscreteDistribution. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  intMinMax_.setLowerBound(distribution_.begin()->first,false);
  intMinMax_.setUpperBound(distribution_.rbegin()->first,false);
}

SimpleDiscreteDistribution::SimpleDiscreteDistribution(
                                                       const vector<double>& values,
                                                       const vector<double>& probas,
                                                       bool fixed
                                                       ) throw(Exception)  : AbstractDiscreteDistribution(values.size(),"Simple.")
{
  if (values.size() != probas.size()) {
    throw Exception("SimpleDiscreteDistribution. Values and probabilities vectors must have the same size (" + TextTools::toString(values.size()) + " != " + TextTools::toString(probas.size()) + ").");
  }
  unsigned int size = values.size();
  
  for(unsigned int i = 0; i < size; i++){
    if (distribution_.find(values[i])!=distribution_.end())
      throw Exception("SimpleDiscreteDistribution: two given values are equal");
    else 
      distribution_[values[i]] = probas[i];
  }
  
  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > NumConstants::SMALL)
    throw Exception("SimpleDiscreteDistribution. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");
  
  if (! fixed){
    double y=1;
    for (unsigned int i=0;i<size-1;i++){
      addParameter_(Parameter("Simple.V"+TextTools::toString(i+1),values[i]));
      addParameter_(Parameter("Simple.theta"+TextTools::toString(i+1), probas[i]/y,&Parameter::PROP_CONSTRAINT_IN));
      y-=probas[i];
    }
    addParameter_(Parameter("Simple.V"+TextTools::toString(size),values[size-1]));

  }

}

void SimpleDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  if (getNumberOfParameters() != 0) {
    AbstractDiscreteDistribution::fireParameterChanged(parameters);
    unsigned int size = distribution_.size();
    
    distribution_.clear();
    double x = 1.0;
    double v;
    for (unsigned int i = 0; i < size-1; i++) {
      v=getParameterValue("V"+TextTools::toString(i+1));
      if (distribution_.find(v)!=distribution_.end()){
        unsigned int j=1;
        int f=((v+NumConstants::TINY)>=intMinMax_.getUpperBound())?-1:1;
        while (distribution_.find(v+f*j*NumConstants::TINY)!=distribution_.end()){
          j++;
          f=((v+f*j*NumConstants::TINY)>=intMinMax_.getUpperBound())?-1:1;
        }
        v+=f*j*NumConstants::TINY;
        // approximation to avoid useless computings:
        // setParameterValue("V"+TextTools::toString(i+1),v);
      }
      distribution_[v]=getParameterValue("theta"+TextTools::toString(i+1))*x;
      x *= 1 - getParameterValue("theta"+TextTools::toString(i+1));
    }

    v=getParameterValue("V" + TextTools::toString(size));
    if (distribution_.find(v)!=distribution_.end()){
      unsigned int j=1;
      int f=((v+NumConstants::TINY)>=intMinMax_.getUpperBound())?-1:1;
      while (distribution_.find(v+f*j*NumConstants::TINY)!=distribution_.end()){
        j++;
        f=((v+f*j*NumConstants::TINY)>=intMinMax_.getUpperBound())?-1:1;
      }
      v+=f*j*NumConstants::TINY;
      // approximation to avoid useless computings:
      // setParameterValue("V"+TextTools::toString(size),v);
    }
    distribution_[v]=x;
  }
}

double SimpleDiscreteDistribution::qProb(double x) const{
  double s=-NumConstants::VERY_BIG;
  double x2=x;
  for (map<double,double>::const_iterator it=distribution_.begin();it!=distribution_.end();it++){
    x2-=it->second;
    if (x2<0)
      return s;
    else
      s=it->second;
  }

  return s;
}
     
double SimpleDiscreteDistribution::pProb(double x) const{
  double s=0;
  for (map<double,double>::const_iterator it=distribution_.begin();it!=distribution_.end();it++)
    if (it->first>=x)
      s+=it->second;
    else
      break;
  
  return s;
}
                                
double SimpleDiscreteDistribution::Expectation(double a) const{
  double s=0;
  for (map<double,double>::const_iterator it=distribution_.begin();it!=distribution_.end();it++)
    if (it->first>=a)
      s+=it->second;
    else
      break;
  
  return s;
}


Domain SimpleDiscreteDistribution::getDomain() const
{
  // Compute a new arbitray bounderi:
  vector<double> values = MapTools::getKeys<double, double, AbstractDiscreteDistribution::Order>(distribution_);
  unsigned int n = values.size(); 
  vector<double> bounderi(n + 1);
	
  // Fill from 1 to n-1 with midpoints:
  for (unsigned int i = 1; i <= n - 1; i++)
    bounderi[i] = (values[i] + values[i - 1]) / 2.;
	
  // Fill 0 with the values[0] - (midpoint[0] - values[0]):
  bounderi[0] = 2 * values[0] - bounderi[1];
	
  // Fill n with values[n - 1] + (values[n - 1] - midpoint[n - 1]):
  bounderi[n] = 2 * values[n - 1] - bounderi[n - 1];
	
  // Build a domain and return it
  return Domain(bounderi, values);
}

void SimpleDiscreteDistribution::restrictToConstraint(const Constraint& c)
{
  if (getNumberOfParameters() == 0)
    return;
  
  const Interval* pi = dynamic_cast<const Interval*>(&c);

  if (!pi)
    throw Exception("SimpleDiscreteDistribution::restrictToConstraint: Non-interval exception");

  map<double, double>::const_iterator it;

  for (it=distribution_.begin(); it!= distribution_.end(); it++)
    if (! pi->isCorrect(it->first))
      throw Exception("Impossible to restrict to Constraint  value" + TextTools::toString(it->first));

  AbstractDiscreteDistribution::restrictToConstraint(c);

  unsigned int size=distribution_.size();
  for (unsigned int i = 0; i< size; i++)
    getParameter_("V" + TextTools::toString(i + 1)).setConstraint(&intMinMax_);
}


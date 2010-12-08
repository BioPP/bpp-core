//
// File: MixtureOfDiscreteDistributions
// Created by: Laurent Guéguen
// Created on: mercredi 9 juin 2010, à 23h 09
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

#include "MixtureOfDiscreteDistributions.h"
#include "../NumConstants.h"
#include "../../Utils/MapTools.h"
#include "../../Text/TextTools.h"

using namespace bpp;
using namespace std;

MixtureOfDiscreteDistributions::MixtureOfDiscreteDistributions(const vector<DiscreteDistribution*>& distributions,
                                                               const vector<double>& probas ) : AbstractDiscreteDistribution(1,"Mixture."), vdd_(), probas_()
{
  if (distributions.size() != probas.size()) {
    throw Exception("MixtureOfDiscreteDistributions. Distributions and probabilities vectors must have the same size (" + TextTools::toString(distributions.size()) + " != " + TextTools::toString(probas.size()) + ").");
  }
  
  unsigned int size = distributions.size();
  for (unsigned int i=0;i<size;i++)
    if (distributions[i]==NULL)
      throw Exception("MixtureOfDiscreteDistributions. Null DiscreteDistribution* in argument vector at index " + TextTools::toString(i));

  for (unsigned int i=0;i<size;i++)
    probas_.push_back(probas[i]);
  
  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > NumConstants::SMALL)
    throw Exception("MixtureOfDiscreteDistributions. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  double y=1;
  for (unsigned int i=0;i<size-1;i++){
    addParameter_(Parameter("Mixture.theta"+TextTools::toString(i+1), probas[i]/y,&Parameter::PROP_CONSTRAINT_IN));
    y-=probas[i];
  }

  
  for (unsigned int i = 0; i < size; i++)
    vdd_.push_back(distributions[i]->clone());

  //  Parameters

  for (unsigned int i=0;i<size;i++){
    ParameterList pl=vdd_[i]->getParameters();
    for (unsigned int j=0;j<pl.size();j++)
      if (pl[j].hasConstraint())
        addParameter_(Parameter("Mixture."+TextTools::toString(i+1)+"_"+pl[j].getName(),pl[j].getValue(),pl[j].getConstraint()->clone(),true));
      else
        addParameter_(Parameter("Mixture."+TextTools::toString(i+1)+"_"+pl[j].getName(),pl[j].getValue()));
  }

  updateDistribution();
}

void MixtureOfDiscreteDistributions::setNumberOfCategories(unsigned int nbClasses)
{
  for (unsigned int i=0;i<vdd_.size();i++)
    vdd_[i]->setNumberOfCategories(nbClasses);
  
  updateDistribution();
}
    

MixtureOfDiscreteDistributions::MixtureOfDiscreteDistributions(const MixtureOfDiscreteDistributions& mdd) : AbstractDiscreteDistribution(mdd), vdd_(), probas_()
{
  for (unsigned int i=0;i<mdd.vdd_.size();i++){
    probas_.push_back(mdd.probas_[i]);
    vdd_.push_back(mdd.vdd_[i]->clone());
  }
}
      
MixtureOfDiscreteDistributions::~MixtureOfDiscreteDistributions()
{
  for (unsigned int i=0;i<vdd_.size();i++)
    delete vdd_[i];

  vdd_.clear();
}

void MixtureOfDiscreteDistributions::fireParameterChanged(const ParameterList& parameters)
{
  unsigned int size = vdd_.size();
  double x = 1.0;
  for (unsigned int i = 0; i < size-1; i++) {
    probas_[i]=getParameterValue("theta"+TextTools::toString(i+1))*x;
    x *= 1 - getParameterValue("theta"+TextTools::toString(i+1));
  }
  
  probas_[size-1]=x;


  // fireParameterChanged on members Distributions

  ParameterList pl;
  unsigned int pos;
  vector<string> v=getParameters().getParameterNames();
  vector<string> v2;
  for (unsigned int i=0;i<size;i++){
    pl.reset();
    for (unsigned int j=0;j<v.size();j++){
      pos=v[j].find("Mixture."+TextTools::toString(i+1));
      if (pos!=string::npos)
        pl.addParameter(Parameter(v[j].substr(v[j].find("_",pos)+1),
                                  getParameterValue(getParameterNameWithoutNamespace(v[j]))));
    }
    vdd_[i]->matchParametersValues(pl);
  }
  
  updateDistribution();
}

void MixtureOfDiscreteDistributions::updateDistribution()
{
  unsigned int size=vdd_.size();
  distribution_.clear();
  // calculation of distribution

  for (unsigned int i=0;i<size;i++){
    vector<double> values=vdd_[i]->getCategories();
    for (unsigned int j=0;j<values.size();j++)
      distribution_[values[j]] = 0;
  }

  for (unsigned int i=0;i<size;i++){
    vector<double> values=vdd_[i]->getCategories();
    vector<double> probas2=vdd_[i]->getProbabilities();
    for (unsigned int j=0;j<values.size();j++)
      distribution_[values[j]] += probas2[j]*probas_[i];
  }

  numberOfCategories_=distribution_.size();
  
  // intMinMax_

  double uB, lB;
  uB=-NumConstants::VERY_BIG;
  lB=NumConstants::VERY_BIG;

  for (unsigned int i=0;i<size;i++){
    if (vdd_[i]->getLowerBound()<lB)
      lB=vdd_[i]->getLowerBound();
    if (vdd_[i]->getUpperBound()>uB)
      uB=vdd_[i]->getUpperBound();
  }

  intMinMax_.setLowerBound(lB,false);
  intMinMax_.setUpperBound(uB,false);
}

Domain MixtureOfDiscreteDistributions::getDomain() const
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

void MixtureOfDiscreteDistributions::setMedian(bool median)
{
  if (median_!=median){
    median_=median; 
    for (unsigned int i=0;i < vdd_.size();i++)
      vdd_[i]->setMedian(median);
    updateDistribution();
  }
}
void MixtureOfDiscreteDistributions::discretize()
{
  for (unsigned int i=0;i < vdd_.size();i++)
    vdd_[i]->discretize();

  updateDistribution();
}

double MixtureOfDiscreteDistributions::pProb(double x) const
{
  double s=0;
  for (unsigned int i=0;i < vdd_.size();i++)
    s+=probas_[i]*vdd_[i]->pProb(x);
  return s;
}

double MixtureOfDiscreteDistributions::qProb(double x) const
{
  throw Exception("MixtureOfDiscreteDistributions::qProb to difficult to compute: not implemented");
  return 0;
}

double MixtureOfDiscreteDistributions::Expectation(double a) const
{
  double s=0;
  for (unsigned int i=0;i < vdd_.size();i++)
    s+=probas_[i]*vdd_[i]->Expectation(a);
  return s;
}

void MixtureOfDiscreteDistributions::restrictToConstraint(const Constraint& c)
{
  for (unsigned int i=0;i < vdd_.size();i++)
    vdd_[i]->restrictToConstraint(c);

  updateDistribution();
}


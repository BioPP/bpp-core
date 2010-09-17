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
                                                               const vector<double>& probas ) : AbstractDiscreteDistribution("Mixture."), vdd_()
{
  if (distributions.size() != probas.size()) {
    throw Exception("MixtureOfDiscreteDistributions. Distributions and probabilities vectors must have the same size (" + TextTools::toString(distributions.size()) + " != " + TextTools::toString(probas.size()) + ").");
  }
  unsigned int size = distributions.size();
  for (unsigned int i=0;i<size-1;i++)
    if (distributions[i]==NULL)
      throw Exception("MixtureOfDiscreteDistributions. Null DiscreteDistribution* in argument vector at index " + TextTools::toString(i));


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

  for (unsigned int i=0;i<size;i++){
    vector<double> values=vdd_[i]->getCategories();
    for (unsigned int j=0;j<values.size();j++)
      distribution_[values[j]] = 0;
  }
  
  for (unsigned int i=0;i<size;i++){
    vector<double> values=vdd_[i]->getCategories();
    vector<double> probas2=vdd_[i]->getProbabilities();
    for (unsigned int j=0;j<values.size();j++)
      distribution_[values[j]] += probas2[j]*probas[i];
  }

  //  Parameters

  for (unsigned int i=0;i<size;i++){
    ParameterList pl=vdd_[i]->getParameters();
    for (unsigned int j=0;j<pl.size();j++)
      if (pl[j].hasConstraint())
        addParameter_(Parameter("Mixture."+TextTools::toString(i+1)+"_"+pl[j].getName(),pl[j].getValue(),pl[j].getConstraint()->clone(),true));
      else
        addParameter_(Parameter("Mixture."+TextTools::toString(i+1)+"_"+pl[j].getName(),pl[j].getValue()));
  }
}

MixtureOfDiscreteDistributions::MixtureOfDiscreteDistributions(const MixtureOfDiscreteDistributions& mdd) : AbstractDiscreteDistribution(mdd), vdd_()
{
  for (unsigned int i=0;i<mdd.vdd_.size();i++)
    vdd_.push_back(mdd.vdd_[i]->clone());
}
      
MixtureOfDiscreteDistributions::~MixtureOfDiscreteDistributions(){
  for (unsigned int i=0;i<vdd_.size();i++)
    delete vdd_[i];

  vdd_.clear();
}

void MixtureOfDiscreteDistributions::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  unsigned int size = vdd_.size();

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

  // calculation of distribution

  distribution_.clear();
  vector<double> probas;
  double x = 1.0;
  for (unsigned int i = 0; i < size-1; i++) {
    probas.push_back(getParameterValue("theta"+TextTools::toString(i+1))*x);
    x *= 1 - getParameterValue("theta"+TextTools::toString(i+1));
  }

  probas.push_back(x);

  for (unsigned int i=0;i<size;i++){
    vector<double> values=vdd_[i]->getCategories();
    for (unsigned int j=0;j<values.size();j++)
      distribution_[values[j]] = 0;
  }
  
  for (unsigned int i=0;i<size;i++){
    vector<double> values=vdd_[i]->getCategories();
    vector<double> probas2=vdd_[i]->getProbabilities();
    for (unsigned int j=0;j<values.size();j++)
      distribution_[values[j]] += probas2[j]*probas[i];
  }
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

double MixtureOfDiscreteDistributions::getLowerBound() const
{
  
  double m = vdd_.size()==0?NumConstants::VERY_BIG:vdd_[0]->getLowerBound();

  double x;
  for (unsigned int i=1;i<vdd_.size();i++){
    x=vdd_[i]->getLowerBound();
    if (m > x)
      m = x;
  }
  return m;
}

double MixtureOfDiscreteDistributions::getUpperBound() const
{
  double m = vdd_.size()==0?-NumConstants::VERY_BIG:vdd_[0]->getUpperBound();

  double x;
  for (unsigned int i=1;i<vdd_.size();i++){
    x=vdd_[i]->getUpperBound();
    if (m < x)
      m = x;
  }
  return m;
}

bool MixtureOfDiscreteDistributions::adaptToConstraint(const Constraint& c, bool f)
{
  if (getNumberOfParameters() == 0)
    return true;

  bool f2=true;
  for (unsigned int i=0;i < vdd_.size();i++){
    if (!vdd_[i]->adaptToConstraint(c,false))
        f2= false;
    }
         
  if (!f2)
    return false;
  else
    if (f){
      for (unsigned int i=0;i < vdd_.size();i++)
        vdd_[i]->adaptToConstraint(c);
      
      //// adaptation of the Parameters

      for (unsigned int i=0;i<vdd_.size();i++){
        ParameterList pl=vdd_[i]->getParameters();
        for (unsigned int j=0;j<pl.size();j++)
          if (pl[j].hasConstraint())
            getParameter_(TextTools::toString(i+1)+"_"+pl[j].getName()).setConstraint(pl[j].getConstraint()->clone());
      }
    }
  
  return true;
}


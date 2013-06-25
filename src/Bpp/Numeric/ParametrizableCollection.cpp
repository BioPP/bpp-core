//
// File: ParametrizableCollection.cpp
// Created by: Laurent Guéguen
// Created on: mercredi 12 juin 2013, à 15h 15
//

/*
  Copyright or <A9> or Copr. CNRS, (November 16, 2004)

  This software is a computer program whose purpose is to provide classes
  for phylogenetic data analysis.

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

#include "ParametrizableCollection.h"

using namespace bpp;
using namespace std;

template<class N>
ParametrizableCollection<N>::ParametrizableCollection(const ParametrizableCollection<N>& set) :
  AbstractParameterAliasable(set),
  objectsSet_(),
  parametersSet_(set.parametersSet_)
{
  // Duplicate all objects:
  typename std::map<unsigned int, std::auto_ptr<N> >::const_iterator it;
  for (it=set.objectsSet_.begin(); it!=set.objectsSet_.end(); it++)
    objectsSet_.at(it->first).reset(it->second->clone());
}

template<class N>
ParametrizableCollection<N>& ParametrizableCollection<N>::operator=(const ParametrizableCollection<N>& set)
{
  clear();

  AbstractParameterAliasable::operator=(set);
  parametersSet_     = set.parametersSet_;

  // Duplicate all objects:
  typename std::map<unsigned int, std::auto_ptr<N> >::const_iterator it;
  for (it=set.objectsSet_.begin(); it!=set.objectsSet_.end(); it++)
    objectsSet_.at(it->first).reset(it->second->clone());

  return *this;
}

template<class N>
void ParametrizableCollection<N>::clear()
{
  resetParameters_();
  objectsSet_.clear();
}

template<class N>
void ParametrizableCollection<N>::addObject(N* object, unsigned int objectIndex)
{
  typename std::map<unsigned int, std::auto_ptr<N> >::iterator it=objectsSet_.find(objectIndex);
  if (it==objectsSet_.end())
    throw BadIntegerException("ParametrizableCollection<N>::addObject. Object objectIndex already used", objectIndex);

  it->second.reset(object);

  // Associate parameters:
  string pname;

  vector<string> nplm=object->getParameters().getParameterNames();
  
  parametersSet_[objectIndex]=*object->getParameters().clone();
  
  for (size_t i  = 0; i < nplm.size(); i++)
    {
      pname = nplm[i];
      Parameter* p = new Parameter(object->getParameters().getParameter(pname));
      p->setName(pname + "_" + TextTools::toString(objectIndex));
      addParameter_(p);
    }
}

template<class N>
N* ParametrizableCollection<N>::removeObject(unsigned int objectIndex)
{
  if (objectsSet_.find(objectIndex)==objectsSet_.end())
    throw BadIntegerException("ParametrizableCollection<N>::removeObject. None Object at this objectIndex", objectIndex);

  N* pm=objectsSet_.at(objectIndex).release();
  objectsSet_.erase(objectIndex);
    
  // Erase all parameter references to this object and translate other indices...

  ParameterList pl=getParameters();

  for (size_t i = pl.size(); i>0; i--)
    {
      string pn=pl[i-1].getName();

      size_t pu=pn.rfind("_");
      int nm=TextTools::toInt(pn.substr(pu,string::npos));
      if (nm==(int)objectIndex){
        vector<string> alpn=getAlias(pn);
        for (unsigned j=0; j<alpn.size(); j++)
          try {
            unaliasParameters(alpn[j],pn);
          }
          catch (Exception& e)
            {
              continue;
            }
        deleteParameter_(i-1);
      }
    }

  parametersSet_[objectIndex].reset();
  return pm;
}

template<class N>
N* ParametrizableCollection<N>::replaceObject(N* object, unsigned int objectIndex)
{
  N* pm=removeObject(objectIndex);
  addObject(object, objectIndex);
  return pm;
}


template<class N>
void ParametrizableCollection<N>::fireParameterChanged(const ParameterList& parameters)
{
  typename std::map<unsigned int, std::auto_ptr<N> >::iterator it;
  // Then we update all objects in the set:
  for (it=objectsSet_.begin(); it!=objectsSet_.end(); it++)
    {
      unsigned int i=it->first;
      for (size_t np = 0 ; np< parametersSet_[i].size() ; np++)
        {
          parametersSet_[i][np].setValue(getParameterValue(parametersSet_[i][np].getName()+"_"+TextTools::toString(i)));
        }
      it->second->matchParametersValues(parametersSet_[i]);
    }
}

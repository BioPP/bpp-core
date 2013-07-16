//
// File: ParametrizableCollection.h
// Created by: Laurent Guéguen
// Created on: mercredi 12 juin 2013, à 14h 24
//

/*
  Copyright or (c) or Copr. Bio++ Development Team, (November 16, 2004)

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

#ifndef _PARAMETRIZABLECOLLECTION_H_
#define _PARAMETRIZABLECOLLECTION_H_

#include "Parametrizable.h"
#include "AbstractParameterAliasable.h"

// From the STL:
#include <map>
#include <memory>

namespace bpp
{
  /**
   * @brief Plain collection of  parametrizable objects
   *
   * The collection is a map from unsigned integers to Parametrizables.
   *
   * The access to the parameters of the objects will be done through
   * this collection.
   *
   */
  
  template <class N>
  class ParametrizableCollection :
    public AbstractParameterAliasable
  {
  protected:
    /**
     * @brief Contains all objects used.
     */
    
    std::map<size_t, N* > objectsSet_;

    /**
     * @brief Parameters for each object in the set.
     *
     */
  
    std::map<size_t, ParameterList> parametersSet_;

  public:
    /**
     * @brief Create an empty object set.
     *
     */
  
    ParametrizableCollection():
      AbstractParameterAliasable(""),
      objectsSet_(),
      parametersSet_()
    {
    }

    ParametrizableCollection(const ParametrizableCollection<N>& set) :
      AbstractParameterAliasable(set),
      objectsSet_(),
      parametersSet_(set.parametersSet_)
    {
      // Duplicate all objects:
      typename std::map<size_t, N* >::const_iterator it;
      for (it=set.objectsSet_.begin(); it!=set.objectsSet_.end(); it++)
        objectsSet_[it->first]=it->second->clone();
    }

    ParametrizableCollection<N>& operator=(const ParametrizableCollection<N>& set)
    {
      clear();

      AbstractParameterAliasable::operator=(set);
      parametersSet_     = set.parametersSet_;

      // Duplicate all objects:
      typename std::map<size_t, N* >::const_iterator it;
      for (it=set.objectsSet_.begin(); it!=set.objectsSet_.end(); it++)
        objectsSet_[it->first]=it->second->clone();

      return *this;
    }

    /**
     * @brief Resets all the information contained in this object.
     *
     */
   
    void clear()
    {
      resetParameters_();
      
      typename std::map<size_t, N*>::const_iterator it;
      for (it=objectsSet_.begin(); it!=objectsSet_.end(); it++)
        delete it->second;
      objectsSet_.clear();
    }

    ~ParametrizableCollection()
    {
      clear();
    }

    ParametrizableCollection<N>* clone() const { return new ParametrizableCollection<N>(*this); }

  public:
    /**
     * To be called when a parameter has changed.
     * Depending on parameters, this will actualize the corresponding objects in the set.
     * @param parameters The modified parameters.
     */
  
    void fireParameterChanged(const ParameterList& parameters)
    {
      typename std::map<size_t, N*>::iterator it;
      // Then we update all objects in the set:
      for (it=objectsSet_.begin(); it!=objectsSet_.end(); it++)
        {
          size_t i=it->first;
          for (size_t np = 0 ; np< parametersSet_[i].size() ; np++)
            {
              parametersSet_[i][np].setValue(getParameterValue(parametersSet_[i][np].getName()+"_"+TextTools::toString(i)));
            }
          it->second->matchParametersValues(parametersSet_[i]);
        }
    }


    /**
     * @return The current number of distinct discrete objects in this set.
     */
  
    size_t getNumberOfObjects() const { return objectsSet_.size(); }

    /**
     * @brief Says if there is a object with a given index.
     *
     * @param objectIndex Index of the object in the set.
     * @return true or false.
     */
  
    const bool hasObject(size_t objectIndex) const
    {
      return (objectsSet_.find(objectIndex)!=objectsSet_.end());
    }

    /**
     * @brief Returns the keys of the set.
     *
     */
  
    const std::vector<size_t> keys() const
    {
      std::vector<size_t> vkeys;
      typename std::map<size_t, N*>::const_iterator it;

      for (it=objectsSet_.begin(); it!=objectsSet_.end(); it++)
        vkeys.push_back(it->first);
            
      return vkeys;
    }

  
    /**
     * @brief Get one object from the set knowing its index.
     *
     * @param objectIndex Index of the object in the set.
     * @return A pointer toward the corresponding object.
     */
  
    const N* getObject(size_t objectIndex) const
    {
      typename std::map<size_t, N*>::const_iterator it=objectsSet_.find(objectIndex);
      if (it==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection::getObject().", (int)objectIndex);
    
      return dynamic_cast<const N*>(it->second);
    }

    N* getObject(size_t objectIndex)
    {
      typename std::map<size_t, N*>::iterator it=objectsSet_.find(objectIndex);
      if (it==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection::getObject().", (int)objectIndex);
    
      return it->second;
    }
  
    /**
     * @brief Add a new object to the set with a given number.
     *
     * @throw Exception if the number is already used. See replace function instead.
     *
     * @param object A pointer toward an object, that will added to the set.
     * 
     * WARNING! The set will now be the owner of the pointer, and will destroy it if needed!
     * Copy the object first if you don't want it to be lost!
     * @param number The number of the object in the Collection
     * 
     */

    void addObject(N* object, size_t objectIndex)
    {
      typename std::map<size_t, N*>::iterator it=objectsSet_.find(objectIndex);
      if (it!=objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection<N>::addObject. Object objectIndex already used", (int)objectIndex);

      objectsSet_[objectIndex]=object;

      // Associate parameters:
      std::string pname;
      std::vector<std::string> nplm=object->getParameters().getParameterNames();
  
      parametersSet_[objectIndex]=*object->getParameters().clone();
  
      for (size_t i  = 0; i < nplm.size(); i++)
        {
          pname = nplm[i];
          Parameter* p = new Parameter(object->getParameters().getParameter(pname));
          p->setName(pname + "_" + TextTools::toString(objectIndex));
          addParameter_(p);
        }
    }

    /**
     * @brief Remove a object from the set, and all corresponding
     * parameters. 
     *
     * @param objectIndex The index of the object in the set.
     * @return the removed N*. 
     */
  
    N* removeObject(size_t objectIndex)
    {
      if (objectsSet_.find(objectIndex)==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection<N>::removeObject. None Object at this objectIndex", (int)objectIndex);

      N* pm=objectsSet_[objectIndex];
      objectsSet_.erase(objectIndex);
      
      // Erase all parameter references to this object and translate other indices...
      
      ParameterList pl=getParameters();
      
      for (size_t i = pl.size(); i>0; i--)
        {
          std::string pn=pl[i-1].getName();
          
          size_t pu=pn.rfind("_");
          int nm=TextTools::toInt(pn.substr(pu,std::string::npos));
          if (nm==(int)objectIndex){
            std::vector<std::string> alpn=getAlias(pn);
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

    /**
     * @brief Replace a object in the set, and returns the replaced one.
     *
     * @param objectIndex The index of the object to be replaced in the set.
     * @param object the replacing N
     * @return the replaced N*. 
     */

    N* replaceObject(N* object, size_t objectIndex)
    {
      N* pm=removeObject(objectIndex);
      addObject(object, objectIndex);
      return pm;
    }


    /**
     * @brief Get the parameters attached to a Object.
     *
     * @param objectIndex the index of the object in the set
     *
     * @return The parameters attached to the object.
     */

    ParameterList getObjectParameters(size_t objectIndex) const
    {
      return parametersSet_.at(objectIndex);
    }
  
  };
} // end of namespace bpp.

#endif // _PARAMETRIZABLECOLLECTION_H_


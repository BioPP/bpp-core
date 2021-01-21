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
    
    std::map<size_t, std::shared_ptr<N> > objectsSet_;

    /**
     * @brief A vector of the numbers of objects that have changed
     * during the last fireParameterChanged.
     *
     */

    std::vector<size_t> vChanged_;
    
  public:
    /**
     * @brief Create an empty object set.
     *
     */
  
    ParametrizableCollection():
      AbstractParameterAliasable(""),
      objectsSet_(),
      vChanged_()
    {
    }

    ParametrizableCollection(const ParametrizableCollection<N>& set) :
      AbstractParameterAliasable(set),
      objectsSet_(),
      vChanged_(set.vChanged_)
    {
      // Duplicate all objects:

      for (const auto& it:set.objectsSet_)
        objectsSet_[it.first]=std::shared_ptr<N>(it.second->clone());
    }

    ParametrizableCollection<N>& operator=(const ParametrizableCollection<N>& set)
    {
      clear();

      AbstractParameterAliasable::operator=(set);
      vChanged_ = set.vChanged_;
      
      // Duplicate all objects:

      for (const auto& it:set.objectsSet_)
        objectsSet_[it.first]=std::shared_ptr<N>(it.second->clone());

      return *this;
    }

    /**
     * @brief Resets all the information contained in this object.
     *
     */
   
    void clear()
    {
      resetParameters_();
      
      objectsSet_.clear();
      vChanged_.empty();
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
      vChanged_.clear();
      
      std::vector<size_t> vCh;

      std::map<size_t, ParameterList > mNumPl;
      
      for (size_t i=0; i<parameters.size(); i++)
      {
        std::string n=parameters[i].getName();
        size_t t=n.rfind("_");
        if (t==std::string::npos)
          continue;
        size_t num=(size_t)atoi(n.substr(t+1).c_str());
        mNumPl[num].addParameter(Parameter(n.substr(0,t),parameters[i].getValue()));
      }

      std::map<size_t, ParameterList >::iterator it;
      
      // Then we update all objects in the set:
      for (it=mNumPl.begin(); it!=mNumPl.end(); it++){
        if (hasObject(it->first) && objectsSet_[it->first]->matchParametersValues(it->second))
          vChanged_.push_back(it->first);
      }
    }

    std::vector<size_t> hasChanged() const
    {
      return vChanged_;
    }

    void clearChanged()
    {
      vChanged_.clear();
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
  
    bool hasObject(size_t objectIndex) const
    {
      return (objectsSet_.find(objectIndex)!=objectsSet_.end());
    }

    /**
     * @brief Says if there is an object in the map.
     *
     * @param object Object in the set.
     * @return true or false.
     */
  
    bool hasObject(std::shared_ptr<N> object) const
    {
      
      for (const auto key : objectsSet_)
        if (key.second==object)
          return true;
      return false;
    }

    /**
     * @brief Return the first key mapping an object in the map.
     *
     * @param object Object in the set.
     * @return the key of the object.
     *
     * Throw Exception if object not found.
     */
  
    size_t getFirstKey(std::shared_ptr<N> object) const
    {
      for (const auto key:objectsSet_)
        if (key.second==object)
          return key.first;

      throw Exception("ParametrizableCollection::getFirstKey: Unknown object");
      return 0;
    }

    /**
     * @brief Returns the keys of the set.
     *
     */
  
    const std::vector<size_t> keys() const
    {
      std::vector<size_t> vkeys;

      for (const auto& it:objectsSet_)
        vkeys.push_back(it.first);
            
      return vkeys;
    }
  
    /**
     * @brief Get one object from the set knowing its index.
     *
     * @param objectIndex Index of the object in the set.
     * @return A pointer toward the corresponding object.
     */
  
    std::shared_ptr<const N> operator[](size_t objectIndex) const
    {
      const auto it=objectsSet_.find(objectIndex);
      if (it==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection::getObject().", (int)objectIndex);
    
      return std::dynamic_pointer_cast<const N>(it->second);
    }

    std::shared_ptr<N> operator[](size_t objectIndex)
    {
      auto it=objectsSet_.find(objectIndex);
      if (it==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection::getObject().", (int)objectIndex);
    
      return it->second;
    }
  
    /**
     * @brief Get the paramters of the Collection corresponding to an
     * object from the set knowing its index.
     *
     * @param objectIndex Index of the object in the set.
     * @return A ParameterList
     */

    ParameterList getParametersForObject(size_t objectIndex) const
    {
      ParameterList pl;
      const auto it=objectsSet_.find(objectIndex);

      if (it!=objectsSet_.end())
      {
        if (std::dynamic_pointer_cast<const ParameterAliasable>(it->second)!=NULL)
          pl = std::dynamic_pointer_cast<const ParameterAliasable>(it->second)->getIndependentParameters();
        else
          pl = it->second->getParameters();
        
        for (size_t i=0; i<pl.size();i++)
          pl[i].setName(pl[i].getName()+"_"+TextTools::toString(objectIndex));
      }
      return pl;
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
     *
     * @param objectIndex The number of the object in the Collection
     * 
     */

    void addObject(std::shared_ptr<N> object, size_t objectIndex)
    {
      auto it=objectsSet_.find(objectIndex);
      if (it!=objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection<N>::addObject. Object objectIndex already used", (int)objectIndex);

      objectsSet_[objectIndex]=object;

      // Associate parameters:
      std::vector<std::string> nplm;
      nplm=object->getParameters().getParameterNames();

      for (const auto& pname:nplm)
      {
        Parameter* p = new Parameter(object->getParameters().getParameter(pname));
        p->setName(pname + "_" + TextTools::toString(objectIndex));
        addParameter_(p);
      }

      if (std::dynamic_pointer_cast<ParameterAliasable>(object)){
        auto ppa=std::dynamic_pointer_cast<ParameterAliasable>(object);
        for (const auto& name:nplm)
        {
          std::vector<std::string> va=ppa->getAlias(name);
          for (const auto& alias:va)
            aliasParameters(name + "_" + TextTools::toString(objectIndex), alias + "_" + TextTools::toString(objectIndex));
        }
      }
    }

    /**
     * @brief Remove a object from the set, and all corresponding
     * parameters. 
     *
     * @param objectIndex The index of the object in the set.
     * @return the removed N*. 
     */
  
    std::shared_ptr<N> removeObject(size_t objectIndex)
    {
      if (objectsSet_.find(objectIndex)==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection<N>::removeObject. None Object at this objectIndex", (int)objectIndex);

      auto pm=objectsSet_[objectIndex];
      objectsSet_.erase(objectIndex);
      
      // Erase all parameter references to this object and translate other indices...
      
      ParameterList pl=getParameters();
      
      for (size_t i = pl.size(); i>0; i--)
      {
        std::string pn=pl[i-1].getName();
          
        size_t pu=pn.rfind("_");
        int nm=atoi(pn.substr(pu+1).c_str());
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
      
      return pm;
    }

    /**
     * @brief Replace a object in the set, and returns the replaced one.
     *
     * @param objectIndex The index of the object to be replaced in the set.
     * @param object the replacing N
     * @return the replaced N*. 
     */

    std::shared_ptr<N> replaceObject(std::shared_ptr<N> object, size_t objectIndex)
    {
      auto pm=removeObject(objectIndex);
      addObject(object, objectIndex);
      return pm;
    }


  };
} // end of namespace bpp.

#endif // _PARAMETRIZABLECOLLECTION_H_


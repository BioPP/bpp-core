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
    
    std::map<unsigned int, std::auto_ptr<N> > objectsSet_;

    /**
     * @brief Parameters for each object in the set.
     *
     */
  
    std::map<unsigned int, ParameterList> parametersSet_;

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

    /**
     * @brief Resets all the information contained in this object.
     *
     */
   
    void clear();
  
    ParametrizableCollection(const ParametrizableCollection<N>& set);

    ParametrizableCollection<N>& operator=(const ParametrizableCollection<N>& set);

    virtual ~ParametrizableCollection()
    {
      clear();
    }

#ifndef NO_VIRTUAL_COV
    ParametrizableCollection<N>*
#else
    Clonable*
#endif
    clone() const { return new ParametrizableCollection<N>(*this); }

  public:
    /**
     * To be called when a parameter has changed.
     * Depending on parameters, this will actualize the corresponding objects in the set.
     * @param parameters The modified parameters.
     */
  
    virtual void fireParameterChanged(const ParameterList& parameters);

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
  
    const bool hasObject(unsigned int objectIndex) const
    {
      return (objectsSet_.find(objectIndex)!=objectsSet_.end());
    }

    /**
     * @brief Returns the keys of the set.
     *
     */
  
    const std::vector<unsigned int> keys() const
    {
      std::vector<unsigned int> vkeys;
      typename std::map<unsigned int, std::auto_ptr<N> >::const_iterator it;

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
  
    const N* getObject(unsigned int objectIndex) const
    {
      typename std::map<unsigned int, std::auto_ptr<N> >::const_iterator it=objectsSet_.find(objectIndex);
      if (it==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection::getObject().", objectIndex);
    
      return dynamic_cast<const N*>(it->second.get());
    }

    N* getObject(unsigned int objectIndex)
    {
      if (objectsSet_.find(objectIndex)==objectsSet_.end())
        throw BadIntegerException("ParametrizableCollection::getObject().", objectIndex);
      return objectsSet_.at(objectIndex).get();
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

    void addObject(N* object, unsigned int objectIndex);

    /**
     * @brief Remove a object from the set, and all corresponding
     * parameters. 
     *
     * @param objectIndex The index of the object in the set.
     * @return the removed N*. 
     */
  
    N* removeObject(unsigned int objectIndex);

    /**
     * @brief Replace a object in the set, and returns the replaced one.
     *
     * @param objectIndex The index of the object to be replaced in the set.
     * @param object the replacing N
     * @return the replaced N*. 
     */

    N* replaceObject(N* object, unsigned int objectIndex);

    /**
     * @brief Get the parameters attached to a Object.
     *
     * @param objectIndex the index of the object in the set
     *
     * @return The parameters attached to the object.
     */

    ParameterList getObjectParameters(unsigned int objectIndex) const;
  
  };
} // end of namespace bpp.

#endif // _PARAMETRIZABLECOLLECTION_H_


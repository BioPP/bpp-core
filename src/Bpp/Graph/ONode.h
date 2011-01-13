// 
// File:    ONode.h
// Author:  Sylvain Gaillard
// Created: 12/01/2011 08:36:47
// 

/*
Copyright or © or Copr. CNRS, (January 12, 2011)

This software is a computer program whose purpose is to provide utilitary
classes. This file belongs to the Bio++ Project.

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

#ifndef _BPP_GRAPH_ONODE_H_
#define _BPP_GRAPH_ONODE_H_

#include "UNode.h"

namespace bpp {
  /**
   * @brief Oriented Node interface
   *
   * ONode is an interface for oriented nodes aimed to build oriented graphs.
   *
   * @author Sylvain Gaillard
   */
  class ONode: public virtual UNode {
    public:
      /**
       * @name Neighbors
       *
       * @{
       */

      /**
       * @brief Get all node's neighbors in const environment.
       */
      virtual std::vector< const ONode * > getNeighbors() const = 0;

      /**
       * @brief Get all node's neighbors.
       */
      virtual std::vector< ONode * > getNeighbors() = 0;

      /** @} */

      /**
       * @name The Clonable interface.
       *
       * @{
       */
#ifndef NO_VIRTUAL_COV      
      ONode * clone() const = 0;
#endif
      /** @} */

      /**
       * @name Fathers
       *
       * @{
       */

      /**
       * @brief Get all the fathers in const environment.
       */
      virtual std::vector< const ONode * > getFathers() const = 0;

      /**
       * @brief Get all the fathers.
       */
      virtual std::vector< ONode * > getFathers() = 0;

      /**
       * @brief Get a particular father in const environment.
       */
      virtual const ONode * getFather(size_t pos) const = 0;

      /**
       * @brief Get a particular father.
       */
      virtual ONode * getFather(size_t pos) = 0;

      /**
       * @brief Tell if this node has one or more father nodes.
       */
      virtual bool hasFathers() const = 0;

      /**
       * @brief Give the number of father nodes for this node.
       */
      virtual unsigned int getNumberOfFathers() const = 0;

      /** @} */

      /**
       * @name Sons
       *
       * @{
       */

      /**
       * @brief Get all the sons in const environment.
       */
      virtual std::vector< const ONode * > getSons() const = 0;

      /**
       * @brief Get all the sons.
       */
      virtual std::vector< ONode * > getSons() = 0;

      /**
       * @brief Get a particular son in const environment.
       */
      virtual const ONode * getSon(size_t pos) const = 0;

      /**
       * @brief Get a particular son.
       */
      virtual ONode * getSon(size_t pos) = 0;

      /**
       * @brief Tell if this node has one or more son nodes.
       */
      virtual bool hasSons() const = 0;

      /**
       * @brief Give the number of son nodes for this node.
       */
      virtual unsigned int getNumberOfSons() const = 0;

      /** @} */

      /**
       * @name Operators
       *
       * @{
       */

      const ONode * operator[] (size_t i) const = 0;
      ONode * operator[] (size_t i) = 0;

      /** @} */
  };
}

#endif //_BPP_GRAPH_ONODE_H_

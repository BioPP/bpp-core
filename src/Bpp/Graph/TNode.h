// 
// File:    TNode.h
// Author:  Sylvain Gaillard
// Created: 12/01/2011 08:43:15
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

#ifndef _BPP_GRAPH_TNODE_H_
#define _BPP_GRAPH_TNODE_H_

#include "ONode.h"

namespace bpp {
  /**
   * @brief Tree Node interface
   *
   * TNode is an interface for tree nodes (i.e. oriented nodes with only one
   * father). It is aimed to build trees.
   *
   * @author Sylvain Gaillard
   */
  class TNode: public virtual ONode {
    public:
      /**
       * @brief Get all node's neighbors in const environment.
       */
      virtual std::vector< const TNode * > getNeighbors() const = 0;

      /**
       * @brief Get all node's neighbors.
       */
      virtual std::vector< TNode * > getNeighbors() = 0;

      /**
       * @name The Clonable interface.
       *
       * @{
       */
#ifndef NO_VIRTUAL_COV      
      TNode * clone() const = 0;
#endif
      /** @} */
  };
}

#endif //_BPP_GRAPH_TNODE_H_

//
// File AssociationTreeGraphImplObserver.h
// Created by: Thomas Bigot
// Last modification : vendredi 4 novembre 2016, à 10h 21
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#ifndef _ASSOCIATION_TREEGRAPH_IMPL_OBSERVER_HPP_
#define _ASSOCIATION_TREEGRAPH_IMPL_OBSERVER_HPP_

#include "TreeGraphImpl.h"
#include "AssociationTreeGraphObserver.h"
#include "AssociationGraphImplObserver.h"

#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include <memory>

namespace bpp
{
  template<class N, class E, class TreeGraphImpl>
  class AssociationTreeGraphImplObserver :
    public AssociationTreeGraphObserver<N, E>,
    public AssociationGraphImplObserver<N, E, TreeGraphImpl>
  {
  public:
    typedef typename AssociationGraphObserver<N, E>::NodeIndex NodeIndex;
    typedef typename AssociationGraphObserver<N, E>::EdgeIndex EdgeIndex;

    typedef typename Graph::NodeId NodeGraphid;
    typedef typename Graph::EdgeId EdgeGraphid;

  public:
    /**
     * Constructor
     * @param directed is the graph directed
     */
    AssociationTreeGraphImplObserver(bool rooted = false);

    /**
     * Constructor
     * @param subjectGraph the graph which is observed
     */
    AssociationTreeGraphImplObserver(std::shared_ptr<TreeGraphImpl> subjectTreeGraph = 00);

    /**
     * Copy Constructor
     * @param graphObserver the treeGraphObserver to be copied
     */
    AssociationTreeGraphImplObserver(bpp::AssociationTreeGraphImplObserver<N, E, TreeGraphImpl> const& treeGraphObserver);

    /**
     * Copy Constructor
     * @param treeGraphObserver the treeGraphObserver to be copied
     */

    template<class N2, class E2>
    AssociationTreeGraphImplObserver(bpp::AssociationTreeGraphImplObserver<N2, E2, TreeGraphImpl> const& treeGraphObserver);

    /**
     * = Operator
     * @param graphObserver the treeGraphObserver we want to copy the values
     */

    AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>& operator=(bpp::AssociationTreeGraphImplObserver<N, E, TreeGraphImpl> const& treeGraphObserver);

    /**
     * Destructor
     * @param graphObserver the treeGraphObserver to be copied
     */
    ~AssociationTreeGraphImplObserver();


    /**
     * clone function
     * @param graphObserver the graphObserver to be copied
     */
    AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>* clone() const { return new AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>(*this); }


    /**
     * Is the graph a tree? A tree must be acyclic and with no isolated node.
     * @return true if valid tree
     */
    bool isValid() const;

    /**
     * Return, in a rooted tree, the branch leading to the father
     * @param nodeObject the concerned node
     * @return an Edge which is the branch to the father
     */
    std::shared_ptr<E>  getEdgeToFather(const std::shared_ptr<N>  nodeObject) const;
    std::shared_ptr<E>  getEdgeToFather(const NodeIndex  nodeIndex) const;

    /**
     * @brief Sets the root and make the tree directed from root to leaves
     *
     */

    void rootAt(const std::shared_ptr<N> root);

    /*
     * @brief check if rooted, ie directed 
     *
     */
    
    bool isRooted() const
    {
      return this->getGraph()->isRooted();
    }


    /**
     * Return, in a rooted tree, the father node
     * @param nodeObject the concerned node
     * @return the father
     */
    std::shared_ptr<N>  getFather(const std::shared_ptr<N>  nodeObject) const;

    /**
     * Has the node a father?
     */
    bool hasFather(const std::shared_ptr<N>  nodeObject) const;
    bool hasFather(const NodeIndex  node) const;

    /**
     * Return, in a rooted tree, the sons of a node 
     * @param nodeObject the concerned node
     * @return a vector of son Nodes
     */
  
    std::vector<std::shared_ptr<N> > getSons(const std::shared_ptr<N>  node) const;
    std::vector<NodeIndex> getSons(const NodeIndex node) const;

    /**
     * Return, in a rooted tree, the branches to the sons of a node 
     * @param nodeObject the concerned node
     * @return a vector of branch Nodes
     */
    
    std::vector<std::shared_ptr<E> > getBranches(const std::shared_ptr<N>  node) const;
    
    std::vector<EdgeIndex> getBranches(const NodeIndex node) const;

    /**
     * Return, in a rooted tree, the son of an edge
     * @param nodeObject the concerned node
     * @return the son Node
     */
  
    std::shared_ptr<N> getSon(const std::shared_ptr<E>  edge) const;
    NodeIndex getSon(const EdgeIndex edge) const;

    /**
     * Return, in a rooted tree, the father of an edge
     * @param nodeObject the concerned node
     * @return the father Node
     */
  
    std::shared_ptr<N> getFather(const std::shared_ptr<E>  edge) const;
    NodeIndex getFather(const EdgeIndex edge) const;

    /**
     * Return, in a rooted tree, the number of sons
     * @param nodeObject the concerned node
     * @return the number of sons
     */
    size_t getNumberOfSons(const std::shared_ptr<N>  node) const;

    /**
     * Get the leaves of a tree under a peculiar node.
     *
     * @param node the starting node
     * @return a vector containing the leaves
     */

    std::vector<std::shared_ptr<N> > getLeavesUnderNode(std::shared_ptr<N>  node) const;

    /**
     * Remove the sons of a node
     * @return a vector containing the removed nodes
     */
    std::vector<std::shared_ptr<N> > removeSons(const std::shared_ptr<N>  node);

    /**
     * Remove a son of a node
     */

    void removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son);
  
    /**
     * Change / set the father of a node
     * @param nodeObject the concerned node
     * @param fatherNodeObject the node to be the father
     */

    void setFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N> fatherNodeObject, const std::shared_ptr<E> edgeObject = 0);
    
    /**
     * Add a son to a node
     * @param nodeObject the concerned node
     * @param sonNodeObject the node to be added as a son to the father
     */
    
    void addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N> sonNodeObject, const std::shared_ptr<E> edgeObject = 0);

    /**
     * Iterators
     *
     */

    /*
     * @brief builds iterator on the sons of a Node
     *
     */

    std::unique_ptr<typename AssociationTreeGraphObserver<N,E>::NodeIterator> sonsIterator(std::shared_ptr<N> node);

    /*
     * @brief builds iterator on the branches to sons of a Node
     *
     */

    std::unique_ptr<typename AssociationTreeGraphObserver<N,E>::EdgeIterator> branchesIterator(std::shared_ptr<N> node);

    /**
     * @brief Get a vector of ancestor nodes between to nodes.
     *
     * @param nodeId1 first node.
     * @param nodeId2 second node.
     * @param includeAncestor Tell if the common ancestor must be included in the vector.
     * @return A vector of ancestor nodes ids.
     * @throw PhyloNodeNotFoundException If a node is not found.
     */
    
    std::vector<std::shared_ptr<N> > getNodePathBetweenTwoNodes(const std::shared_ptr<N>  nodeObjectA, const std::shared_ptr<N>  nodeObjectB, bool includeAncestor = true) const;

    std::vector<std::shared_ptr<E> > getEdgePathBetweenTwoNodes(const std::shared_ptr<N>  nodeObjectA, const std::shared_ptr<N>  nodeObjectB) const;

    std::vector<std::shared_ptr<N> > getSubtreeNodes(const std::shared_ptr<N> localRoot);
    std::vector<std::shared_ptr<E> > getSubtreeEdges(const std::shared_ptr<N> localRoot);
  };

  /********************/

  template<class N, class E>
  using AssociationTreeGlobalGraphObserver =  AssociationTreeGraphImplObserver<N, E, TreeGlobalGraph> ;

  /********************/


  template<class N, class E, class TreeGraphImpl>
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::AssociationTreeGraphImplObserver(bool rooted_p) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>::AssociationGraphImplObserver(rooted_p)
  {
  }


  template<class N, class E, class TreeGraphImpl>
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::AssociationTreeGraphImplObserver(AssociationTreeGraphImplObserver<N, E, TreeGraphImpl> const& treeGraphObserver) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>::AssociationGraphImplObserver(treeGraphObserver)
  {
  }

  /**
   * Copy Constructor
   * @param graphObserver the treeGraphObserver to be copied
   */
  
  template<class N, class E, class TreeGraphImpl> template<class N2, class E2>
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::AssociationTreeGraphImplObserver(bpp::AssociationTreeGraphImplObserver<N2, E2, TreeGraphImpl> const& treeGraphObserver) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>::AssociationGraphImplObserver(treeGraphObserver)
  {
  } 

  
  template<class N, class E, class TreeGraphImpl>
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>& AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::operator=(AssociationTreeGraphImplObserver<N, E, TreeGraphImpl> const& treeGraphObserver)
  {
    AssociationGraphImplObserver<N, E, TreeGraphImpl>::operator=(treeGraphObserver);
    return *this;
  }

  template<class N, class E, class TreeGraphImpl>
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::~AssociationTreeGraphImplObserver()
  {
  }


  template<class N, class E, class TreeGraphImpl>
  bool AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::isValid() const
  {
    return this->getGraph()->isValid();
  }

  template<class N, class E, class TreeGraphImpl>
  bool AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::hasFather(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getGraph()->hasFather(this->getNodeGraphid(nodeObject));
  }

  template<class N, class E, class TreeGraphImpl>
  bool AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::hasFather(const AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex index) const
  {
    return this->getGraph()->hasFather(this->getNodeGraphid(this->getNode(index)));
  }


  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<N>  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getFather(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getFather(this->getNodeGraphid(nodeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<E>  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getEdgeToFather(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getEdgeFromGraphid(this->getGraph()->getEdgeToFather(this->getNodeGraphid(nodeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<E>  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getEdgeToFather(const AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex index) const
  {
    return this->getEdgeFromGraphid(this->getGraph()->getEdgeToFather(this->getNodeGraphid(this->getNode(index))));
  }

  
  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<N> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getSons(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getSons(this->getNodeGraphid(nodeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<typename AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex> AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getSons(AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex node) const
  {
    return this->getNodeIndexes(this->getNodesFromGraphid(this->getGraph()->getSons(this->getNodeGraphid(this->getNode(node)))));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<E> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getBranches(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getEdgesFromGraphid(this->getGraph()->getBranches(this->getNodeGraphid(nodeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<typename AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::EdgeIndex> AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getBranches(AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex node) const
  {
    return this->getEdgeIndexes(this->getEdgesFromGraphid(this->getGraph()->getBranches(this->getNodeGraphid(this->getNode(node)))));
  }

  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<N> AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getSon(const std::shared_ptr<E>  edgeObject) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getBottom(this->getEdgeGraphid(edgeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  typename AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getSon(AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::EdgeIndex edge) const
  {
    return this->getNodeIndex(this->getNode(this->getGraph()->getBottom(this->getEdgeGraphid(this->getEdge(edge)))));
  }

  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<N> AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getFather(const std::shared_ptr<E>  edgeObject) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getTop(this->getEdgeGraphid(edgeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  typename AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::NodeIndex AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getFather(AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::EdgeIndex edge) const
  {
    return this->getNodeIndex(this->getNode(this->getGraph()->getTop(this->getEdgeGraphid(this->getEdge(edge)))));
  }

  template<class N, class E, class TreeGraphImpl>
  size_t AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getNumberOfSons(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getGraph()->getNumberOfSons(this->getNodeGraphid(nodeObject));
  }

  template<class N, class E, class TreeGraphImpl>
  void AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::setFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  fatherNodeObject, const std::shared_ptr<E> edgeObject)
  {
    if (edgeObject)
      this->getGraph()->setFather(this->getNodeGraphid(nodeObject), this->getNodeGraphid(fatherNodeObject), this->getEdgeGraphid(edgeObject));
    else
      this->getGraph()->setFather(this->getNodeGraphid(nodeObject), this->getNodeGraphid(fatherNodeObject));
  }

  template<class N, class E, class TreeGraphImpl>
  void AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  sonNodeObject, const std::shared_ptr<E> edgeObject)
  {
    if (edgeObject)
      this->getGraph()->addSon(this->getNodeGraphid(nodeObject), this->getNodeGraphid(sonNodeObject), this->getEdgeGraphid(edgeObject));
    else
      this->getGraph()->addSon(this->getNodeGraphid(nodeObject), this->getNodeGraphid(sonNodeObject));
  }


  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<N> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getLeavesUnderNode(const std::shared_ptr<N> node) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getLeavesUnderNode(this->getNodeGraphid(node)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<N> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::removeSons(const std::shared_ptr<N> node)
  {
    return this->getNodesFromGraphid(this->getGraph()->removeSons(this->getNodeGraphid(node)));
  }

  
  template<class N, class E, class TreeGraphImpl>
  void AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son)
  {
    this->getGraph()->removeSon(this->getNodeGraphid(node), this->getNodeGraphid(son));
  }

  template<class N, class E, class TreeGraphImpl>
  void AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::rootAt(const std::shared_ptr<N> root)
  {
    this->getGraph()->rootAt(this->getNodeGraphid(root));
  }

  template<class N, class E, class TreeGraphImpl>
  std::unique_ptr<typename AssociationTreeGraphObserver<N,E>::NodeIterator> AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::sonsIterator(std::shared_ptr<N> node)
  {
    return this->outgoingNeighborNodesIterator(node);
  }
  
  template<class N, class E, class TreeGraphImpl>
  std::unique_ptr<typename AssociationTreeGraphObserver<N,E>::EdgeIterator> AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::branchesIterator(std::shared_ptr<N> node)
  {
    return this->outgoingEdgesIterator(node);
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<N> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getNodePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB, bool includeAncestor) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getNodePathBetweenTwoNodes(this->getNodeGraphid(nodeA), this->getNodeGraphid(nodeB), includeAncestor));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<E> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getEdgePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB) const
  {
    return this->getEdgesFromGraphid(this->getGraph()->getEdgePathBetweenTwoNodes(this->getNodeGraphid(nodeA), this->getNodeGraphid(nodeB)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<N> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getSubtreeNodes(const std::shared_ptr<N> localRoot)
  {
    return this->getNodesFromGraphid(this->getGraph()->getSubtreeNodes(this->getNodeGraphid(localRoot)));
  }

  template<class N, class E, class TreeGraphImpl>
  std::vector<std::shared_ptr<E> > AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>::getSubtreeEdges(const std::shared_ptr<N>  localRoot)
  {
    return AssociationGraphImplObserver<N, E, TreeGraphImpl>::getEdgesFromGraphid(this->getGraph()->getSubtreeEdges(this->getNodeGraphid(localRoot)));
  }
}


#endif

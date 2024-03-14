// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPH_ASSOCIATIONTREEGRAPHIMPLOBSERVER_H
#define BPP_GRAPH_ASSOCIATIONTREEGRAPHIMPLOBSERVER_H

#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "AssociationGraphImplObserver.h"
#include "AssociationTreeGraphObserver.h"
#include "TreeGraphImpl.h"

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
   * @param rooted if the graph rooted
   */
  AssociationTreeGraphImplObserver(bool rooted = false) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>(rooted)
  {}

  /**
   * Constructor
   * @param subjectTreeGraph the graph which is observed
   */

  AssociationTreeGraphImplObserver(std::shared_ptr<TreeGraphImpl> subjectTreeGraph) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>(subjectTreeGraph)
  {}

  /**
   * Copy Constructor
   * @param treeGraphObserver the treeGraphObserver to be copied
   */

  AssociationTreeGraphImplObserver(bpp::AssociationTreeGraphImplObserver<N, E, TreeGraphImpl> const& treeGraphObserver) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>(treeGraphObserver)
  {}

  /**
   * Copy Constructor
   * @param treeGraphObserver the treeGraphObserver to be copied
   */

  template<class N2, class E2>
  AssociationTreeGraphImplObserver(bpp::AssociationTreeGraphImplObserver<N2, E2, TreeGraphImpl> const& treeGraphObserver) :
    AssociationGraphImplObserver<N, E, TreeGraphImpl>::AssociationGraphImplObserver(treeGraphObserver)
  {}


  /**
   * = Operator
   * @param treeGraphObserver the treeGraphObserver we want to copy the values
   */
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>& operator=(bpp::AssociationTreeGraphImplObserver<N, E, TreeGraphImpl> const& treeGraphObserver)
  {
    AssociationGraphImplObserver<N, E, TreeGraphImpl>::operator=(treeGraphObserver);
    return *this;
  }


  /**
   * Destructor
   */
  ~AssociationTreeGraphImplObserver()
  {}


  /**
   * clone function
   */
  AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>* clone() const { return new AssociationTreeGraphImplObserver<N, E, TreeGraphImpl>(*this); }


  /**
   * Is the graph a tree? A tree must be acyclic and with no isolated node.
   * @return true if valid tree
   */
  bool isValid() const
  {
    return this->getGraph()->isValid();
  }

  /**
   * Return, in a rooted tree, the branch leading to the father
   * @param nodeObject the concerned node
   * @return an Edge which is the branch to the father
   */
  std::shared_ptr<E>  getEdgeToFather(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getEdgeFromGraphid(this->getGraph()->getEdgeToFather(this->getNodeGraphid(nodeObject)));
  }

  std::shared_ptr<E>  getEdgeToFather(const NodeIndex index) const
  {
    return this->getEdgeFromGraphid(this->getGraph()->getEdgeToFather(this->getNodeGraphid(this->getNode(index))));
  }


  /**
   * @brief Sets the root and make the tree directed from root to leaves
   *
   */
  void rootAt(const std::shared_ptr<N> root)
  {
    this->getGraph()->rootAt(this->getNodeGraphid(root));
  }

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
  std::shared_ptr<N>  getFatherOfNode(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getFatherOfNode(this->getNodeGraphid(nodeObject)));
  }

  /**
   * Has the node a father?
   */
  bool hasFather(const std::shared_ptr<N>  nodeObject) const
  {
    return this->getGraph()->hasFather(this->getNodeGraphid(nodeObject));
  }

  bool hasFather(const NodeIndex index) const
  {
    return this->getGraph()->hasFather(this->getNodeGraphid(this->getNode(index)));
  }

  /**
   * Return, in a rooted tree, the sons of a node
   * @param node the concerned node
   * @return a vector of son Nodes
   */
  std::vector<std::shared_ptr<N> > getSons(const std::shared_ptr<N>  node) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getSons(this->getNodeGraphid(node)));
  }

  std::vector<NodeIndex> getSons(const NodeIndex node) const
  {
    return this->getNodeIndexes(this->getNodesFromGraphid(this->getGraph()->getSons(this->getNodeGraphid(this->getNode(node)))));
  }

  /**
   * Return, in a rooted tree, the branches to the sons of a node
   * @param node the concerned node
   * @return a vector of branch Nodes
   */
  std::vector<std::shared_ptr<E> > getBranches(const std::shared_ptr<N>  node) const
  {
    return this->getEdgesFromGraphid(this->getGraph()->getBranches(this->getNodeGraphid(node)));
  }

  std::vector<EdgeIndex> getBranches(const NodeIndex node) const
  {
    return this->getEdgeIndexes(this->getEdgesFromGraphid(this->getGraph()->getBranches(this->getNodeGraphid(this->getNode(node)))));
  }

  /**
   * Return, in a rooted tree, the son of an edge
   * @param edge the concerned edge
   * @return the son Node
   */
  std::shared_ptr<N> getSon(const std::shared_ptr<E>  edge) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getBottom(this->getEdgeGraphid(edge)));
  }

  NodeIndex getSon(const EdgeIndex edge) const
  {
    return this->getNodeIndex(this->getNodeFromGraphid(this->getGraph()->getBottom(this->getEdgeGraphid(this->getEdge(edge)))));
  }

  /**
   * Return, in a rooted tree, the father of an edge
   * @param edge the concerned edge
   * @return the father Node
   */
  std::shared_ptr<N> getFatherOfEdge(const std::shared_ptr<E>  edge) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getTop(this->getEdgeGraphid(edge)));
  }

  NodeIndex getFatherOfEdge(const EdgeIndex edge) const
  {
    return this->getNodeIndex(this->getNodeFromGraphid(this->getGraph()->getTop(this->getEdgeGraphid(this->getEdge(edge)))));
  }


  /**
   * Return, in a rooted tree, the number of sons
   * @param node the concerned node
   * @return the number of sons
   */
  size_t getNumberOfSons(const std::shared_ptr<N>  node) const
  {
    return this->getGraph()->getNumberOfSons(this->getNodeGraphid(node));
  }

  /**
   * Get the leaves of a tree under a peculiar node.
   *
   * @param node the starting node
   * @return a vector containing the leaves
   */
  std::vector<std::shared_ptr<N> > getLeavesUnderNode(std::shared_ptr<N>  node) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getLeavesUnderNode(this->getNodeGraphid(node)));
  }

  std::vector<NodeIndex> getLeavesUnderNode(NodeIndex node) const
  {
    return this->getNodeIndexes(getLeavesUnderNode(this->getNode(node)));
  }

  /**
   * Remove the sons of a node
   * @return a vector containing the removed nodes
   */
  std::vector<std::shared_ptr<N> > removeSons(const std::shared_ptr<N>  node)
  {
    return this->getNodesFromGraphid(this->getGraph()->removeSons(this->getNodeGraphid(node)));
  }

  /**
   * Remove a son of a node
   */
  void removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son)
  {
    this->getGraph()->removeSon(this->getNodeGraphid(node), this->getNodeGraphid(son));
  }

  /**
   * Change / set the father of a node
   * @param nodeObject the concerned node
   * @param fatherNodeObject the node to be the father
   * @param edgeObject the optional edge  between the nodes (default
   * = 00)
   */
  void setFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N> fatherNodeObject, const std::shared_ptr<E> edgeObject = 0)
  {
    if (edgeObject)
      this->getGraph()->setFather(this->getNodeGraphid(nodeObject), this->getNodeGraphid(fatherNodeObject), this->getEdgeGraphid(edgeObject));
    else
      this->getGraph()->setFather(this->getNodeGraphid(nodeObject), this->getNodeGraphid(fatherNodeObject));
  }


  /**
   * Add a son to a node
   * @param nodeObject the concerned node
   * @param sonNodeObject the node to be added as a son to the father
   * @param edgeObject the optional edge  between the nodes (default
   * = 00)
   */
  void addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N> sonNodeObject, const std::shared_ptr<E> edgeObject = 0)
  {
    if (edgeObject)
      this->getGraph()->addSon(this->getNodeGraphid(nodeObject), this->getNodeGraphid(sonNodeObject), this->getEdgeGraphid(edgeObject));
    else
      this->getGraph()->addSon(this->getNodeGraphid(nodeObject), this->getNodeGraphid(sonNodeObject));
  }

  /**
   * Iterators
   *
   */

  /*
   * @brief builds iterator on the sons of a Node
   *
   */
  std::unique_ptr<typename AssociationTreeGraphObserver<N, E>::NodeIterator> sonsIterator(std::shared_ptr<N> node)
  {
    return this->outgoingNeighborNodesIterator(node);
  }

  std::unique_ptr<typename AssociationTreeGraphObserver<N, E>::NodeIterator> sonsIterator(std::shared_ptr<N> node) const
  {
    return this->outgoingNeighborNodesIterator(node);
  }

  /*
   * @brief builds iterator on the branches to sons of a Node
   *
   */
  std::unique_ptr<typename AssociationTreeGraphObserver<N, E>::EdgeIterator> branchesIterator(std::shared_ptr<N> node)
  {
    return this->outgoingEdgesIterator(node);
  }

  std::unique_ptr<typename AssociationTreeGraphObserver<N, E>::EdgeIterator> branchesIterator(std::shared_ptr<N> node) const
  {
    return this->outgoingEdgesIterator(node);
  }

  /**
   * @brief Get a vector of ancestor nodes between to nodes.
   *
   * @param nodeA first node.
   * @param nodeB second node.
   * @param includeAncestor Tell if the common ancestor must be included in the vector.
   * @return A vector of ancestor nodes ids.
   * @throw PhyloNodeNotFoundException If a node is not found.
   */
  std::vector<std::shared_ptr<N> > getNodePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB, bool includeAncestor = true) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getNodePathBetweenTwoNodes(this->getNodeGraphid(nodeA), this->getNodeGraphid(nodeB), includeAncestor));
  }

  std::vector<std::shared_ptr<E> > getEdgePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB) const
  {
    return this->getEdgesFromGraphid(this->getGraph()->getEdgePathBetweenTwoNodes(this->getNodeGraphid(nodeA), this->getNodeGraphid(nodeB)));
  }

  std::vector<NodeIndex> getNodePathBetweenTwoNodes(const NodeIndex nodeA, const NodeIndex nodeB, bool includeAncestor = true) const
  {
    return getNodeIndexes(getNodePathBetweenTwoNodes(this->getNode(nodeA), this->getNode(nodeB), includeAncestor));
  }

  std::vector<EdgeIndex> getEdgePathBetweenTwoNodes(const NodeIndex nodeA, const NodeIndex nodeB, bool includeAncestor = true) const
  {
    return this->getEdgeIndexes(getEdgePathBetweenTwoNodes(this->getNode(nodeA), this->getNode(nodeB), includeAncestor));
  }

  std::vector<std::shared_ptr<N> > getSubtreeNodes(const std::shared_ptr<N> localRoot) const
  {
    return this->getNodesFromGraphid(this->getGraph()->getSubtreeNodes(this->getNodeGraphid(localRoot)));
  }

  std::vector<std::shared_ptr<E> > getSubtreeEdges(const std::shared_ptr<N> localRoot) const
  {
    return AssociationGraphImplObserver<N, E, TreeGraphImpl>::getEdgesFromGraphid(this->getGraph()->getSubtreeEdges(this->getNodeGraphid(localRoot)));
  }

  std::vector<NodeIndex> getSubtreeNodes(const NodeIndex localRoot) const
  {
    return getNodeIndexes(getSubtreeNodes(this->getNode(localRoot)));
  }

  std::vector<EdgeIndex> getSubtreeEdges(const NodeIndex localRoot) const
  {
    return this->getEdgeIndexes(getSubtreeEdges(this->getNode(localRoot)));
  }

  /**
   * Return, in a rooted tree, the MRCA node of a vector of nodes
   * @param vNodeObject the vector of concerned nodes
   * @return the MRCA
   */
  std::shared_ptr<N>  MRCA(const std::vector<std::shared_ptr<N> >  vNodeObject) const
  {
    std::vector<Graph::NodeId> vNid(vNodeObject.size());

    std::transform(vNodeObject.begin(), vNodeObject.end(), vNid.begin(), [this](const std::shared_ptr<N>& nodeObject){return this->getNodeGraphid(nodeObject);});

    return this->getNodeFromGraphid(this->getGraph()->MRCA(vNid));
  }
};

/********************/

template<class N, class E>
using AssociationTreeGlobalGraphObserver =  AssociationTreeGraphImplObserver<N, E, TreeGlobalGraph>;
}
#endif // BPP_GRAPH_ASSOCIATIONTREEGRAPHIMPLOBSERVER_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPH_ASSOCIATIONTREEGRAPHOBSERVER_H
#define BPP_GRAPH_ASSOCIATIONTREEGRAPHOBSERVER_H

#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "AssociationGraphObserver.h"
#include "TreeGraph.h"

namespace bpp
{
/**
 * @brief Defines a Tree Graph Associator. It is a template which follows
 * (subscribed to) a Graph.
 *
 * @author Thomas Bigot, Laurent Guéguen
 */

template<class N, class E>
class AssociationTreeGraphObserver :
  public virtual AssociationGraphObserver<N, E>
{
public:
  typedef typename AssociationTreeGraphObserver<N, E>::NodeIndex NodeIndex;
  typedef typename AssociationTreeGraphObserver<N, E>::EdgeIndex EdgeIndex;

  typedef typename TreeGraph::NodeId NodeGraphid;
  typedef typename TreeGraph::EdgeId EdgeGraphid;

public:
  /**
   * Is the graph a tree? A tree must be acyclic and with no isolated node.
   * @return true if valid tree
   */

  virtual bool isValid() const = 0;

  /**
   * Return, in a rooted tree, the branch leading to the father
   * @param nodeObject the concerned node
   * @return an Edge which is the branch to the father
   */

  virtual std::shared_ptr<E>  getEdgeToFather(const std::shared_ptr<N>  nodeObject) const = 0;

  virtual std::shared_ptr<E>  getEdgeToFather(const NodeIndex nodeIndex) const = 0;

  /**
   * @brief Sets the root and make the tree directed from root to leaves
   *
   */

  virtual void rootAt(const std::shared_ptr<N> root) = 0;

  /*
   * @brief check if rooted, ie directed
   *
   */

  virtual bool isRooted() const = 0;

  /**
   * Return, in a rooted tree, the father node
   * @param nodeObject the concerned node
   * @return the father
   */

  virtual std::shared_ptr<N>  getFatherOfNode(const std::shared_ptr<N>  nodeObject) const = 0;

  /**
   * Has the node a father?
   */

  virtual bool hasFather(const std::shared_ptr<N>  nodeObject) const = 0;
  virtual bool hasFather(const NodeIndex node) const = 0;

  /**
   * Return, in a rooted tree, the sons of a node
   * @param node the concerned node
   * @return a vector of son Nodes
   */

  virtual std::vector<std::shared_ptr<N>> getSons(const std::shared_ptr<N>  node) const = 0;

  virtual std::vector<NodeIndex> getSons(const NodeIndex node) const = 0;

  /**
   * Return the branches to the sons of a node
   * @param node the concerned node
   * @return a vector of branch Nodes
   */

  virtual std::vector<std::shared_ptr<E>> getBranches(const std::shared_ptr<N>  node) const = 0;

  virtual std::vector<EdgeIndex> getBranches(const NodeIndex node) const = 0;

  /**
   * Return, in a rooted tree, the son of an edge
   * @param edge the concerned edge
   * @return the son Node
   */

  virtual std::shared_ptr<N> getSon(const std::shared_ptr<E>  edge) const = 0;
  virtual NodeIndex getSon(const EdgeIndex edge) const = 0;

  /**
   * Return, in a rooted tree, the father of an edge
   * @param edge the concerned edge
   * @return the father Node
   */

  virtual std::shared_ptr<N> getFatherOfEdge(const std::shared_ptr<E>  edge) const = 0;
  virtual NodeIndex getFatherOfEdge(const EdgeIndex edge) const = 0;

  /**
   * Return, in a rooted tree, the number of sons
   * @param node the concerned node
   * @return the number of sons
   */

  virtual size_t getNumberOfSons(const std::shared_ptr<N>  node) const = 0;

  /**
   * Get the leaves of a tree under a peculiar node.
   *
   * @param node the starting node
   * @return a vector containing the leaves
   */

  virtual std::vector<std::shared_ptr<N>> getLeavesUnderNode(std::shared_ptr<N>  node) const = 0;

  /**
   * Remove the sons of a node
   * @return a vector containing the removed nodes
   */

  virtual std::vector<std::shared_ptr<N>> removeSons(const std::shared_ptr<N>  node) = 0;

  /**
   * Remove a son of a node
   */

  virtual void removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son) = 0;

  /**
   * Change / set the father of a node
   * @param nodeObject the concerned node
   * @param fatherNodeObject the node to be the father
   * @param edgeObject the optional edge  between the nodes (default
   * = 00)
   */

  virtual void setFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  fatherNodeObject, const std::shared_ptr<E> edgeObject = 0) = 0;

  /**
   * Add a son to a node
   * @param nodeObject the concerned node
   * @param sonNodeObject the node to be added as a son to the father
   * @param edgeObject the optional edge  between the nodes (default
   * = 00)
   */

  virtual void addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  sonNodeObject, const std::shared_ptr<E> edgeObject = 0) = 0;

  /**
   * Iterators
   *
   */

  /*
   * @brief builds iterator on the neighbor nodes of a Node
   *
   */

  typedef typename AssociationGraphObserver<N, E>::NodeIterator NodeIterator;
  typedef typename AssociationGraphObserver<N, E>::EdgeIterator EdgeIterator;


  /*
   * @brief builds iterator on the sons of a Node
   *
   */

  virtual std::unique_ptr<NodeIterator> sonsIterator(std::shared_ptr<N> node) = 0;

  virtual std::unique_ptr<NodeIterator> sonsIterator(std::shared_ptr<N> node) const = 0;

  /*
   * @brief builds iterator on the branches to sons of a Node
   *
   */

  virtual std::unique_ptr<EdgeIterator> branchesIterator(std::shared_ptr<N> node) = 0;

  virtual std::unique_ptr<EdgeIterator> branchesIterator(std::shared_ptr<N> node) const = 0;

  /**
   * @brief Get a vector of ancestor nodes between to nodes.
   *
   * @param nodeA first node.
   * @param nodeB second node.
   * @param includeAncestor Tell if the common ancestor must be included in the vector.
   * @return A vector of ancestor nodes ids.
   * @throw PhyloNodeNotFoundException If a node is not found.
   */

  virtual std::vector<std::shared_ptr<N>> getNodePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB, bool includeAncestor = true) const = 0;

  virtual std::vector<std::shared_ptr<E>> getEdgePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB) const = 0;

  virtual std::vector<std::shared_ptr<N>> getSubtreeNodes(const std::shared_ptr<N> localRoot) const = 0;

  virtual std::vector<std::shared_ptr<E>> getSubtreeEdges(const std::shared_ptr<N> localRoot) const = 0;
};
}
#endif // BPP_GRAPH_ASSOCIATIONTREEGRAPHOBSERVER_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPH_ASSOCIATIONDAGRAPHOBSERVER_H
#define BPP_GRAPH_ASSOCIATIONDAGRAPHOBSERVER_H

#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <vector>

#include "AssociationGraphObserver.h"
#include "DAGraph.h"

namespace bpp
{
/**
 * @brief Defines a DA Graph Associator. It is a template which
 * follows (subscribed to) a Graph.
 *
 * @author Laurent Guéguen
 */

template<class N, class E>
class AssociationDAGraphObserver :
  public virtual AssociationGraphObserver<N, E>
{
public:
  typedef typename AssociationDAGraphObserver<N, E>::NodeIndex NodeIndex;
  typedef typename AssociationDAGraphObserver<N, E>::EdgeIndex EdgeIndex;

  typedef typename DAGraph::NodeId NodeGraphid;
  typedef typename DAGraph::EdgeId EdgeGraphid;

public:
  /**
   * Is the graph a DAG? A DAG must be acyclic.
   * @return true if valid DAG
   */

  virtual bool isValid() const = 0;

  /**
   * Is the DAG rooted? Ie with at most one node with no father.
   * @return true if rooted DAG
   */

  virtual bool isRooted() const = 0;

  /**
   * @brief Sets the root and make the DAG directed from root to
   * leaves
   *
   */

  virtual void rootAt(const std::shared_ptr<N> root) = 0;


  /**
   * Return, the fathers of a node
   * @param node the concerned node
   * @return the father
   */

  virtual std::vector<std::shared_ptr<N>> getFathers(const std::shared_ptr<N>  node) const = 0;

  virtual std::vector<NodeIndex> getFathers(const NodeIndex node) const = 0;

  /**
   * Has the node a father?
   */

  virtual bool hasFather(const std::shared_ptr<N>  nodeObject) const = 0;
  virtual bool hasFather(const NodeIndex node) const = 0;

  /**
   * Return the sons of a node
   * @param node the concerned node
   * @return a vector of son Nodes
   */

  virtual std::vector<std::shared_ptr<N>> getSons(const std::shared_ptr<N>  node) const = 0;

  virtual std::vector<NodeIndex> getSons(const NodeIndex node) const = 0;

  /**
   * Return the son of an edge
   * @param edge the concerned edge
   * @return the son Node
   */

  virtual std::shared_ptr<N> getSon(const std::shared_ptr<E>  edge) const = 0;
  virtual NodeIndex getSon(const EdgeIndex edge) const = 0;

  /**
   * Return the father of an edge
   * @param edge the concerned edge
   * @return the father Node
   */

  virtual std::shared_ptr<N> getFatherOfEdge(const std::shared_ptr<E>  edge) const = 0;
  virtual NodeIndex getFatherOfEdge(const EdgeIndex edge) const = 0;

  /**
   * Return the number of sons
   * @param node the concerned node
   * @return the number of sons
   */

  virtual size_t getNumberOfSons(const std::shared_ptr<N>  node) const = 0;

  /**
   * Return the number of fathers
   * @param node the concerned node
   * @return the number of fathers
   */

  virtual size_t getNumberOfFathers(const std::shared_ptr<N>  node) const = 0;

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
   * @param node the concerned node
   * @param son the node to be removed
   */

  virtual void removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son) = 0;

  /**
   * Add a son to a node
   * @param nodeObject the concerned node
   * @param sonNodeObject the node to be added as a son to the father
   * @param edgeObject the optional edge  between the nodes (default
   * = 0)
   */

  virtual void addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  sonNodeObject, const std::shared_ptr<E> edgeObject = 0) = 0;

  /**
   * Remove the fathers of a node
   * @return a vector containing the removed fathers
   */

  virtual std::vector<std::shared_ptr<N>> removeFathers(const std::shared_ptr<N> node) = 0;

  /**
   * Remove a father of a node
   */

  virtual void removeFather(const std::shared_ptr<N> node, const std::shared_ptr<N> father) = 0;

  /**
   * Add a father to a node
   * @param nodeObject the concerned node
   * @param fatherNodeObject the node to be added as a father to the node
   * @param edgeObject the optional edge  between the nodes (default
   * = 00)
   */

  virtual void addFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  fatherNodeObject, const std::shared_ptr<E> edgeObject = 0) = 0;

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
   * @brief builds iterator on the fathers of a Node
   *
   */

  virtual std::unique_ptr<NodeIterator> fathersIterator(std::shared_ptr<N> node) = 0;

  virtual std::unique_ptr<NodeIterator> fathersIterator(std::shared_ptr<N> node) const = 0;

  /**
   * @brief Get Below Objects.
   *
   * @param localRoot of the subgraph.
   * @return A vector of ancestor nodes ids.
   *
   */

  virtual std::vector<std::shared_ptr<N>> getBelowNodes(const std::shared_ptr<N> localRoot) = 0;

  virtual std::vector<std::shared_ptr<E>> getBelowEdges(const std::shared_ptr<N> localRoot) = 0;
};
}
#endif // BPP_GRAPH_ASSOCIATIONDAGRAPHOBSERVER_H

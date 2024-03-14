// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPH_DAGRAPH_H
#define BPP_GRAPH_DAGRAPH_H

#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../Exceptions.h"
#include "../Numeric/VectorTools.h"
#include "Graph.h"

namespace bpp
{
class DAGraph :
  public virtual Graph
{
public:
  /**
   * Is the graph a directed acyclic?
   * @return true if valid DAG
   */
  virtual bool isValid() const = 0;

  /**
   * Is the DAG rooted?
   *
   * @return true if rooted, ie has only one node with no father.
   */

  virtual bool isRooted() const = 0;

  /**
   * Check if node has a father
   */

  virtual bool hasFather(Graph::NodeId node) const = 0;

  /**
   * Get the fathers node of a node
   */

  virtual std::vector<Graph::NodeId> getFathers(Graph::NodeId node) const = 0;

  /**
   * @brief Get the number of fathers nodes
   */

  virtual size_t getNumberOfFathers(Graph::NodeId node) const = 0;

  /**
   * Add a father to a node
   */

  virtual void addFather(Graph::NodeId node, Graph::NodeId father) = 0;

  virtual void addFather(Graph::NodeId node, Graph::NodeId father, Graph::EdgeId edgeId) = 0;

  /**
   * Remove one father
   */

  virtual void removeFather(Graph::NodeId node, Graph::NodeId father) = 0;

  /**
   * Remove all the fathers
   */

  virtual std::vector<Graph::NodeId> removeFathers(Graph::NodeId node) = 0;

  /**
   * Get the leaves under a node
   * @param node the starting node
   * @return a vector containing the leaves
   */

  virtual std::vector<Graph::NodeId> getLeavesUnderNode(Graph::NodeId node) const = 0;

  /**
   * Get the sons node of a node
   */

  virtual std::vector<Graph::NodeId> getSons(Graph::NodeId node) const = 0;

  /**
   * @brief Get the number of sons node
   */

  virtual size_t getNumberOfSons(Graph::NodeId node) const = 0;

  /**
   * Add a son to a node
   */

  void addSon(Graph::NodeId node, Graph::NodeId sonNode);

  void addSon(Graph::NodeId node, Graph::NodeId sonNode, Graph::EdgeId edge);

  /**
   * Remove all the sons
   */

  virtual std::vector<Graph::NodeId> removeSons(Graph::NodeId node) = 0;

  /**
   * Remove one son
   */

  virtual void removeSon(Graph::NodeId node, Graph::NodeId son) = 0;

  /**
   * Re-root the DA with the new root
   */

  virtual void rootAt(Graph::NodeId newRoot) = 0;

  /**
   * Get all the nodes below a node
   */

  virtual std::vector<Graph::NodeId> getBelowNodes(Graph::NodeId localRoot) const = 0;

  /**
   * Get all the branches below a node
   */

  virtual std::vector<Graph::EdgeId> getBelowEdges(Graph::NodeId localRoot) const = 0;
};
}
#endif // BPP_GRAPH_DAGRAPH_H

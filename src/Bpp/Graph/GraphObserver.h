// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPH_GRAPHOBSERVER_H
#define BPP_GRAPH_GRAPHOBSERVER_H

#include <iostream>
#include <map>
#include <ostream>
#include <vector>

#include "../Clonable.h"
#include "../Exceptions.h"
#include "Graph.h"

namespace bpp
{
/**
 * @brief Defines a Graph Observer. It is a template which follows
 * (subscribed to) a Graph.
 * The graph and the graph observer communicate to keep them up-to-date
 * each other. The observer is also an actor, since it can change
 * the structure of the observed Graph.
 *
 * @author Thomas Bigot
 */

// interface

class GraphObserver :
  public virtual Clonable
{
public:
  /** @name Function called by the subjectGraph
   *  Methods called by the subject graph to make this observer so fit the subject graph
   */
  // /@{

  /**
   * Delete unused object edges, since they have been deleted in the graph
   * @param edgesToDelete a vector of Edges to delete
   */
  virtual void deletedEdgesUpdate(const std::vector<unsigned int>& edgesToDelete) = 0;

  /**
   * Delete unused object nodes, since they have been deleted in the graph
   * @param nodesToDelete a vector of N to delete
   */
  virtual void deletedNodesUpdate(const std::vector<unsigned int>& nodesToDelete) = 0;

  // /@}
};
}

#else

namespace bpp
{class GraphObserver; }
#endif // BPP_GRAPH_GRAPHOBSERVER_H

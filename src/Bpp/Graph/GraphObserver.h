#ifndef _GRAPHOBSERVER_HPP_
#define _GRAPHOBSERVER_HPP_

#include "Graph.h"

#include <vector>
#include <map>


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
    
    class GraphObserverI:
    public bpp::Clonable
    {
    
    public:
      
      /** @name Function called by the subjectGraph
      *  Methods called by the subject graph to make this observer so fit the subject graph
      */
      ///@{
      
      /**
      * Delete unused object edges, since they have been deleted in the graph
      * @param edgesToDelete a vector of Edges to delete
      */
      virtual void update_removeDeletedEdges(std::vector< bpp::Graph::Edge > edgesToDelete) = 0;
      
      /**
      * Delete unused object nodes, since they have been deleted in the graph
      * @param nodesToDelete a vector of N to delete
      */
      virtual void update_removeDeletedNodes(std::vector< bpp::Graph::Node > nodesToDelete) = 0;
      
      ///@}
      
    };

  
    template<class N, class E>
    class GraphObserver:
    public GraphObserverI
    {
    private:
      
      /**
      * List of edges, stored at the same ID than the corresponding edges
      * in the bserved graph.
      */
      std::vector<E*> edgesToObjects;
      
      /**
      * List of nodes, stored at the same ID than the corresponding nodes
      * in the observed graph.
      */
      std::vector<N*> nodesToObjects;
      
      /**
      * Can find an Edge with the corresponding object.
      */
      std::map<E*,Graph::Edge> objectsToEdges;
      
      /**
      * Can find a Node with the corresponding object.
      */
      std::map<N*,Graph::Node> objectsToNodes;
      
      
      
      
      /**
      * The observed Graph. Anytime this graph is observed,
      * the current object will be warned to take changes into account.
      */
      bpp::Graph* subjectGraph;
      
      /**
      * Set the observed Graph
      * @param subjectGraph the graph which is observed
      */
      void observe(bpp::Graph subjectGraph);
      
      
        
    public:
      
      /**
      * Constructor
      * @param directed is the graph directed
      */
      GraphObserver(bool directed = false);
      
      /**
      * Constructor
      * @param subjectGraph the graph which is observed
      */
      GraphObserver(bpp::Graph* subjectGraph = 00);
      
      /**
      * Copy Constructor
      * @param graphObserver the graphObserver to be copied
      */
      GraphObserver(bpp::GraphObserver<N,E> const& graphObserver);
      
      /**
      * clone function
      * @param graphObserver the graphObserver to be copied
      */
      GraphObserver<N,E>* clone();
      
      
      /**
      * This function is called to tell the observer that the subject
      * has changed and hence the observer has to take the changes
      * into account.
      */
      void update();
      
      
      
      /** @name Graph Relations Management
      *  Modificating the structure of the graph.
      */
      
      /**
      * Creates an orphaned node from a NodeClass object.
      * @param objectNewNode the N object associated to the node in the graph.
      * 
      */
      void createNode(N& objectNewNode);
      
      /**
      * Creates an node linked to an existing node.
      * @param objectNewNode the N object associated to the node in the graph.
      * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
      */
      void createNode(N& objectOriginNode, N& objectNewNode);
      
      /**
      * Creates a link between two existing nodes.
      * If directed graph: nodeA -> nodeB.
      * @param nodeA source node (or first node if undirected)
      * @param nodeB target node (or second node if undirected)
      * @return the new edge
      */
      void link(N& nodeObjectA, N& nodeObjectB, E& edgeObject);
      
      /**
      * Creates a link between two existing nodes.
      * If directed graph: nodeA -> nodeB.
      * @param nodeA source node (or first node if undirected)
      * @param nodeB target node (or second node if undirected)
      */
      void unlink(N& nodeObjectA, N& nodeObjectB);
      
      /**
      * Deletes a node
      * @param node node to be deleted
      */
      void deleteNode(N& node);
      
      
      ///@}
      
      /** @name Object Association
      *  Associate or dissociate N and E objects to pre-existing Graph Nodes and Graph Edges
      */
      
      /**
      * Associate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to associate
      * @param node/edge existing node/edge to be associated
      */
      void associate(N& nodeObject, Graph::Node node);
      void associate(E& nodeObject, Graph::Edge edge);
      
      /**
      * Dissociate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to dissociate
      */
      void forget(N& nodeObject);
      void forget(E& edgeObject);
      
      
      
      
      ///@}      
      
      /** @name Nodes Functions
      *  These methodes of the graph concern the node management.
      */
      ///@{
      /**
      * Get all the neighbors of a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the neighbors
      */
      const std::vector<N&> getNeighbors(N& node);
      /**
      * In an directed graph, get all the neighbors which
      * are leaving a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the outgoing neighbors
      */
      const std::vector<N&> getOutgoingNeighbors(N& node);
      /**
      * In an directed graph, get all the neighbors which
      * are coming to a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the incoming neighbors
      */
      const std::vector<N&> getIncomingNeighbors(N& node);
      /**
      * Get the leaves of a graph, ie, nodes with only one neighbor,
      * starting from a peculiar node.
      * @param node the starting node
      * @param maxDepth the maximum number of allowed depth, 0 means no max.
      * @return a vector containing the leaves
      */
      const std::vector<N&> getLeavesFromNode(N& node, unsigned int maxDepth);
      /**
      * Get all the leaves of a graph, ie, nodes with only one neighbor,
      * @return a vector containing the leaves
      */
      const std::vector<N&> getLeaves();
      ///@}
      
      
      /** @name Edge Functions
      *  These methodes of the graph concern the edges.
      */
      ///@{
      /**
      * Returns the Edge between two nodes
      * @param nodes a pair of implied nodes (if directed
      * graph nodeA then nodeB)
      * @return the edge between these two nodes
      */
      const E& getEdge(N& nodeA, N& nodeB);
      /**
      * Returns the Edge between two nodes
      * @param nodes a pair of implied nodes
      * (if directed graph nodeA then nodeB)
      * @return the edge between these two nodes
      */
      const std::vector<E&> getEdges(N& node);
      ///@}
      
      
      /** @name Function called by the subjectGraph
      *  These methodes are called by the subject graph to make this observer so fit the subject graph
      */
      ///@{
      
      /**
      * Delete unused object edges, since they have been deleted in the graph
      * @param edgesToDelete a vector of Edges to delete
      */
      void update_removeDeletedEdges(std::vector< bpp::Graph::Edge > edgesToDelete);
      
      /**
      * Delete unused object nodes, since they have been deleted in the graph
      * @param nodesToDelete a vector of N to delete
      */
      void update_removeDeletedNodes(std::vector< bpp::Graph::Node > nodesToDelete);
      
      ///@}
      
    };
  
}

#else

namespace bpp {class GraphObserver;}

#endif
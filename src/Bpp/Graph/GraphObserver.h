#ifndef _GRAPHOBSERVER_HPP_
#define _GRAPHOBSERVER_HPP_

#include "Graph.h"

#include "../Exceptions.h"


#include <vector>
#include <map>
#include <iostream>


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
    public virtual bpp::Clonable
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
      virtual void update_removeDeletedEdges(std::vector< bpp::Graph::Edge >& edgesToDelete) = 0;
      
      /**
      * Delete unused object nodes, since they have been deleted in the graph
      * @param nodesToDelete a vector of N to delete
      */
      virtual void update_removeDeletedNodes(std::vector< bpp::Graph::Node >& nodesToDelete) = 0;
      
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
      
      //is the graph directed
      bool directed;
      
      
        
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
      * Destructor
      * @param graphObserver the graphObserver to be copied
      */
      ~GraphObserver();
      
      
      
      /**
      * clone function
      * @param graphObserver the graphObserver to be copied
      */
      #ifdef NO_VIRTUAL_COV
        Clonable*
      #else
        GraphObserver<N,E>*
      #endif
      clone() const { return new GraphObserver<N,E>(*this); };
      
      
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
      void createNode(N* objectNewNode);
      
      /**
      * Creates an node linked to an existing node.
      * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
      * @param objectNewNode the N object associated to the node in the graph.
      */
      void createNode(N* objectNewNode, N* objectOriginNode);
      
      /**
      * Creates a link between two existing nodes.
      * If directed graph: nodeA -> nodeB.
      * @param nodeA source node (or first node if undirected)
      * @param nodeB target node (or second node if undirected)
      * @return the new edge
      */
      void link(N* nodeObjectA, N* nodeObjectB, E* edgeObject = 00);
      
      /**
      * Creates a link between two existing nodes.
      * If directed graph: nodeA -> nodeB.
      * @param nodeA source node (or first node if undirected)
      * @param nodeB target node (or second node if undirected)
      */
      void unlink(N* nodeObjectA, N* nodeObjectB);
      
      /**
      * Deletes a node
      * @param node node to be deleted
      */
      void deleteNode(N* node);
      
      
      ///@}
      
      /** @name Object Association
      *  Associate or dissociate N and E objects to pre-existing Graph Nodes and Graph Edges
      */
      
      /**
      * Associate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to associate
      * @param node/edge existing node/edge to be associated
      */
      void associate(N* nodeObject, Graph::Node node);
      void associate(E* edgeObject, Graph::Edge edge);
      
      /**
      * Dissociate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to dissociate
      */
      void forget(N* nodeObject);
      void forget(E* edgeObject);
      
      
      
      
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
      const std::vector<N*> getNeighbors(N* node);
      /**
      * In an directed graph, get all the neighbors which
      * are leaving a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the outgoing neighbors
      */
      const std::vector<N*> getOutgoingNeighbors(N* node);
      /**
      * In an directed graph, get all the neighbors which
      * are coming to a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the incoming neighbors
      */
      const std::vector<N*> getIncomingNeighbors(N* node);
      /**
      * Get the leaves of a graph, ie, nodes with only one neighbor,
      * starting from a peculiar node.
      * @param node the starting node
      * @param maxDepth the maximum number of allowed depth, 0 means no max.
      * @return a vector containing the leaves
      */
      const std::vector<N*> getLeavesFromNode(N* node, unsigned int maxDepth);
      /**
      * Get all the leaves objects of a graph, ie, nodes with only one neighbor,
      * @return a vector containing the leaves
      */
      const std::vector<N*> getLeaves();
      
      /**
      * Get all the defined nodes of a graphO,
      * @return a vector containing the nodesObjects
      */
      const std::vector<N*> getNodes();
      
      
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
      const E* getEdge(N* nodeA, N* nodeB);
      /**
      * Returns the Edge between two nodes
      * @param nodes a pair of implied nodes
      * (if directed graph nodeA then nodeB)
      * @return the edge between these two nodes
      */
      const std::vector<E*> getEdges(N* node);
      ///@}
      
      
      /** @name Function called by the subjectGraph
      *  These methodes are called by the subject graph to make this observer so fit the subject graph
      */
      ///@{
      
      /**
      * Delete unused object edges, since they have been deleted in the graph
      * @param edgesToDelete a vector of Edges to delete
      */
      void update_removeDeletedEdges(std::vector< bpp::Graph::Edge >& edgesToDelete);
      
      /**
      * Delete unused object nodes, since they have been deleted in the graph
      * @param nodesToDelete a vector of N to delete
      */
      void update_removeDeletedNodes(std::vector< bpp::Graph::Node >& nodesToDelete);
      
      ///@}
      
       /** @name General Info
      *  General information about the graph
      */
      ///@{
      /**
      * Return the number of defined nodes, ie nodes that have a corresponding object
      * in this GraphObserver
      * @return the number of nodes
      */
      size_t getNumberOfNodes();
      
      /**
      * Return the number of defined leaves, ie leaves that have a corresponding object
      * in this GraphObserver
      * @return the number of leaves
      */
      size_t getNumberOfLeaves();
      
      
      ///@}
      
      
    };
    
    
template <class N, class E>
void GraphObserver<N,E>::createNode(N* nodeObject)
{
  Graph::Node newGraphNode = subjectGraph->createNode();
  
  associate(nodeObject, newGraphNode);
  
}

template <class N, class E>
GraphObserver<N,E>::GraphObserver(bool directed_p)
{
  this->directed = directed_p;
  this->subjectGraph = new Graph(directed);
  this->subjectGraph->attach(this);
}

template <class N, class E>
GraphObserver<N,E>::~GraphObserver()
{
  this->subjectGraph->detach(this);
}

template <class N, class E>
GraphObserver<N,E>::GraphObserver(GraphObserver<N,E> const& graphObserver)
{
  this->directed = graphObserver.directed;
  this->edgesToObjects = graphObserver.edgesToObjects;
  this->nodesToObjects = graphObserver.nodesToObjects;
  this->objectsToEdges = graphObserver.objectsToEdges;
  this->objectsToNodes = graphObserver.objectsToNodes;
  this->subjectGraph = graphObserver.subjectGraph;
}


template <class N, class E>
void GraphObserver<N,E>::createNode(N* objectNewNode,N* objectOriginNode)
{
  createNode(objectNewNode);
  link(objectOriginNode,objectNewNode);
}

template <class N, class E>
void GraphObserver<N,E>::link(N* nodeObjectA, N* nodeObjectB, E* edgeObject)
{
  // checking the nodes
  typename std::map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectsToNodes.find(nodeObjectA);
  foundNodeB = objectsToNodes.find(nodeObjectB);
  if(foundNodeA == objectsToNodes.end() || foundNodeB == objectsToNodes.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  //checking if the edge is not already in the GraphObserver
  if(edgeObject != 00 && objectsToEdges.find(edgeObject) != objectsToEdges.end())
    throw Exception("The given edge is already associated to a relation in the subjectGraph.");
  
  std::cout << "Trying to link node " << foundNodeA->second << " -> " << foundNodeB->second << std::endl;
  Graph::Edge newGraphEdge = subjectGraph->link(foundNodeA->second,foundNodeB->second);
  
  if(edgesToObjects.size() < newGraphEdge+1)
    edgesToObjects.resize(newGraphEdge+1);
  edgesToObjects.at(newGraphEdge) = edgeObject;
  
  objectsToEdges[edgeObject] = newGraphEdge;
}

template <class N, class E>
void GraphObserver<N,E>::unlink(N* nodeObjectA, N* nodeObjectB)
{
  //checking the nodes
  typename std::map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectsToNodes.find(nodeObjectA);
  foundNodeB = objectsToNodes.find(nodeObjectB);
  if(foundNodeA == objectsToNodes.end() || foundNodeB == objectsToNodes.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  subjectGraph->unlink(foundNodeA->second,foundNodeB->second);
}

template <class N, class E>
void GraphObserver<N,E>::update_removeDeletedEdges(std::vector<Graph::Edge>& edgesToDelete)
{
  for(std::vector<Graph::Edge>::iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++){
    E* edgeObject = edgesToObjects.at(*currEdge);
    edgesToObjects.at(*currEdge) = 00;
    
    objectsToEdges.erase(edgeObject);
    
  }
}

template <class N, class E>
void GraphObserver<N,E>::update_removeDeletedNodes(std::vector<Graph::Node>& nodesToDelete){
  for(std::vector<Graph::Edge>::iterator currNode = nodesToDelete.begin(); currNode != nodesToDelete.end(); currNode++){
    N* nodeObject = nodesToObjects.at(*currNode);
    nodesToObjects.at(*currNode) = 00;
    
    objectsToNodes.erase(nodeObject);
    
  }
}

template <class N, class E>
const std::vector<N*> GraphObserver<N,E>::getOutgoingNeighbors(N* nodeObject)
{
  Graph::Node graphNode = objectsToNodes.at(nodeObject);
  std::vector<Graph::Node> graphNodes = subjectGraph->getOutgoingNeighbors(graphNode);
  std::vector<N*> result;
  for(std::vector<Graph::Node>::iterator currGraphNode = graphNodes.begin(); currGraphNode != graphNodes.end(); currGraphNode++)
  {
    result.push_back(nodesToObjects.at(*currGraphNode));
  }
  return result;
}

template <class N, class E>
void GraphObserver<N,E>::associate(N* nodeObject, Graph::Node graphNode)
{
  
  // nodes vector must be the right size. Eg: to store a node with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  nodesToObjects.resize(subjectGraph->getHighestNodeID()+1);

  // now storing the node
  nodesToObjects.at(graphNode) = nodeObject;
  objectsToNodes[nodeObject] = graphNode;
}

template <class N, class E>
void GraphObserver<N,E>::associate(E* edgeObject, Graph::Edge graphEdge)
{
  
  // nodes vector must be the right size. Eg: to store an edge with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  edgesToObjects.resize(subjectGraph->getHighestEdgeID()+1);
  
  // now storing the edge
  edgesToObjects.at(graphEdge) = edgeObject;
  objectsToNodes[edgeObject] = graphEdge;
}

template <class N, class E>
void GraphObserver<N,E>::forget(N* nodeObject)
{
  typename std::map<N*,Graph::Node>::iterator nodeToForget = objectsToNodes.find(nodeObject);
  nodesToObjects.erase(nodeToForget->second);
  objectsToNodes.erase(nodeToForget);
}

template <class N, class E>
void GraphObserver<N,E>::forget(E* edgeObject)
{
  typename std::map<E*,Graph::Edge>::iterator edgeToForget = objectsToEdges.find(edgeObject);
  edgesToObjects.erase(edgeToForget->second);
  objectsToEdges.erase(edgeToForget);
}


template <class N, class E>
const std::vector<N*> GraphObserver<N,E>::getLeaves()
{
  std::vector<N*> leavesToReturn;
  // fetching all the graph Leaves
  std::vector<Graph::Node> graphLeaves = subjectGraph->getLeaves();
  // testing if they are defined in this observer
  for(std::vector<Graph::Node>::iterator currGraphLeave = graphLeaves.begin(); currGraphLeave != graphLeaves.end(); currGraphLeave++)
  {
    N* foundLeafObject = nodesToObjects.at(*currGraphLeave);
    if(foundLeafObject != 00)
      leavesToReturn.push_back(foundLeafObject);
  }
  return leavesToReturn;
}

template <class N, class E>
const std::vector<N*> GraphObserver<N,E>::getNodes()
{
  std::vector<N*> nodesToReturn;
  for(typename std::vector<N*>::iterator currNodeObject = nodesToObjects.begin(); currNodeObject != nodesToObjects.end(); currNodeObject++)
  {
    if(*currNodeObject != 00)
    {
      nodesToReturn.push_back(*currNodeObject);
    }
  }
  return nodesToReturn;
}

template <class N, class E>
size_t GraphObserver<N,E>::getNumberOfNodes(){
  return objectsToNodes.size();
}
 
template <class N, class E>
size_t GraphObserver<N,E>::getNumberOfLeaves(){
  return getLeaves().size();
}
 
}

#else

namespace bpp {class GraphObserver;}

#endif
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
      virtual void deletedEdgesUpdate(std::vector< bpp::Graph::Edge >& edgesToDelete) = 0;
      
      /**
      * Delete unused object nodes, since they have been deleted in the graph
      * @param nodesToDelete a vector of N to delete
      */
      virtual void deletedNodesUpdate(std::vector< bpp::Graph::Node >& nodesToDelete) = 0;
      
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
      std::vector<E*> edgesToObjects_;
      
      /**
      * List of nodes, stored at the same ID than the corresponding nodes
      * in the observed graph.
      */
      std::vector<N*> nodesToObjects_;
      
      /**
      * Can find an Edge with the corresponding object.
      */
      std::map<E*,Graph::Edge> objectsToEdges_;
      
      /**
      * Can find a Node with the corresponding object.
      */
      std::map<N*,Graph::Node> objectsToNodes_;
      
      
      
      
      /**
      * The observed Graph. Anytime this graph is observed,
      * the current object will be warned to take changes into account.
      */
      bpp::Graph* subjectGraph_;
      
      /**
      * Set the observed Graph
      * @param subjectGraph the graph which is observed
      */
      void observe_(bpp::Graph subjectGraph);
      
      //is the graph directed
      bool directed_;
      
      
        
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
      void associateNode(N* nodeObject, Graph::Node node);
      void associateEdge(E* edgeObject, Graph::Edge edge);
      
      /**
      * Dissociate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to dissociate
      */
      void forgetNode(N* nodeObject);
      void forgetEdge(E* edgeObject);
      
      
      
      
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
      void deletedEdgesUpdate(std::vector< bpp::Graph::Edge >& edgesToDelete);
      
      /**
      * Delete unused object nodes, since they have been deleted in the graph
      * @param nodesToDelete a vector of N to delete
      */
      void deletedNodesUpdate(std::vector< bpp::Graph::Node >& nodesToDelete);
      
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
  Graph::Node newGraphNode = subjectGraph_->createNode();
  
  associateNode(nodeObject, newGraphNode);
  
}

template <class N, class E>
GraphObserver<N,E>::GraphObserver(bool directed_p)
{
  this->directed_ = directed_p;
  this->subjectGraph_ = new Graph(directed_);
  this->subjectGraph_->addObserver(this);
}

template <class N, class E>
GraphObserver<N,E>::~GraphObserver()
{
  this->subjectGraph_->removeObserver(this);
}

template <class N, class E>
GraphObserver<N,E>::GraphObserver(GraphObserver<N,E> const& graphObserver)
{
  this->directed_ = graphObserver.directed_;
  this->edgesToObjects_ = graphObserver.edgesToObjects_;
  this->nodesToObjects_ = graphObserver.nodesToObjects_;
  this->objectsToEdges_ = graphObserver.objectsToEdges_;
  this->objectsToNodes_ = graphObserver.objectsToNodes_;
  this->subjectGraph_ = graphObserver.subjectGraph_;
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
  foundNodeA = objectsToNodes_.find(nodeObjectA);
  foundNodeB = objectsToNodes_.find(nodeObjectB);
  if(foundNodeA == objectsToNodes_.end() || foundNodeB == objectsToNodes_.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  //checking if the edge is not already in the GraphObserver
  if(edgeObject != 00 && objectsToEdges_.find(edgeObject) != objectsToEdges_.end())
    throw Exception("The given edge is already associated to a relation in the subjectGraph.");
  
  std::cout << "Trying to link node " << foundNodeA->second << " -> " << foundNodeB->second << std::endl;
  Graph::Edge newGraphEdge = subjectGraph_->link(foundNodeA->second,foundNodeB->second);
  
  if(edgesToObjects_.size() < newGraphEdge+1)
    edgesToObjects_.resize(newGraphEdge+1);
  edgesToObjects_.at(newGraphEdge) = edgeObject;
  
  objectsToEdges_[edgeObject] = newGraphEdge;
}

template <class N, class E>
void GraphObserver<N,E>::unlink(N* nodeObjectA, N* nodeObjectB)
{
  //checking the nodes
  typename std::map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectsToNodes_.find(nodeObjectA);
  foundNodeB = objectsToNodes_.find(nodeObjectB);
  if(foundNodeA == objectsToNodes_.end() || foundNodeB == objectsToNodes_.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  subjectGraph_->unlink(foundNodeA->second,foundNodeB->second);
}

template <class N, class E>
void GraphObserver<N,E>::deletedEdgesUpdate(std::vector<Graph::Edge>& edgesToDelete)
{
  for(std::vector<Graph::Edge>::iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++){
    E* edgeObject = edgesToObjects_.at(*currEdge);
    edgesToObjects_.at(*currEdge) = 00;
    
    objectsToEdges_.erase(edgeObject);
    
  }
}

template <class N, class E>
void GraphObserver<N,E>::deletedNodesUpdate(std::vector<Graph::Node>& nodesToDelete){
  for(std::vector<Graph::Edge>::iterator currNode = nodesToDelete.begin(); currNode != nodesToDelete.end(); currNode++){
    N* nodeObject = nodesToObjects_.at(*currNode);
    nodesToObjects_.at(*currNode) = 00;
    
    objectsToNodes_.erase(nodeObject);
    
  }
}

template <class N, class E>
const std::vector<N*> GraphObserver<N,E>::getOutgoingNeighbors(N* nodeObject)
{
  Graph::Node graphNode = objectsToNodes_.at(nodeObject);
  std::vector<Graph::Node> graphNodes = subjectGraph_->getOutgoingNeighbors(graphNode);
  std::vector<N*> result;
  for(std::vector<Graph::Node>::iterator currGraphNode = graphNodes.begin(); currGraphNode != graphNodes.end(); currGraphNode++)
  {
    result.push_back(nodesToObjects_.at(*currGraphNode));
  }
  return result;
}

template <class N, class E>
void GraphObserver<N,E>::associateNode(N* nodeObject, Graph::Node graphNode)
{
  
  // nodes vector must be the right size. Eg: to store a node with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  nodesToObjects_.resize(subjectGraph_->getHighestNodeID()+1);

  // now storing the node
  nodesToObjects_.at(graphNode) = nodeObject;
  objectsToNodes_[nodeObject] = graphNode;
}

template <class N, class E>
void GraphObserver<N,E>::associateEdge(E* edgeObject, Graph::Edge graphEdge)
{
  
  // nodes vector must be the right size. Eg: to store an edge with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  edgesToObjects_.resize(subjectGraph_->getHighestEdgeID()+1);
  
  // now storing the edge
  edgesToObjects_.at(graphEdge) = edgeObject;
  objectsToNodes_[edgeObject] = graphEdge;
}

template <class N, class E>
void GraphObserver<N,E>::forgetNode(N* nodeObject)
{
  typename std::map<N*,Graph::Node>::iterator nodeToForget = objectsToNodes_.find(nodeObject);
  nodesToObjects_.erase(nodeToForget->second);
  objectsToNodes_.erase(nodeToForget);
}

template <class N, class E>
void GraphObserver<N,E>::forgetEdge(E* edgeObject)
{
  typename std::map<E*,Graph::Edge>::iterator edgeToForget = objectsToEdges_.find(edgeObject);
  edgesToObjects_.erase(edgeToForget->second);
  objectsToEdges_.erase(edgeToForget);
}


template <class N, class E>
const std::vector<N*> GraphObserver<N,E>::getLeaves()
{
  std::vector<N*> leavesToReturn;
  // fetching all the graph Leaves
  std::vector<Graph::Node> graphLeaves = subjectGraph_->getLeaves();
  // testing if they are defined in this observer
  for(std::vector<Graph::Node>::iterator currGraphLeave = graphLeaves.begin(); currGraphLeave != graphLeaves.end(); currGraphLeave++)
  {
    N* foundLeafObject = nodesToObjects_.at(*currGraphLeave);
    if(foundLeafObject != 00)
      leavesToReturn.push_back(foundLeafObject);
  }
  return leavesToReturn;
}

template <class N, class E>
const std::vector<N*> GraphObserver<N,E>::getNodes()
{
  std::vector<N*> nodesToReturn;
  for(typename std::vector<N*>::iterator currNodeObject = nodesToObjects_.begin(); currNodeObject != nodesToObjects_.end(); currNodeObject++)
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
  return objectsToNodes_.size();
}
 
template <class N, class E>
size_t GraphObserver<N,E>::getNumberOfLeaves(){
  return getLeaves().size();
}
 
}

#else

namespace bpp {class GraphObserver;}

#endif
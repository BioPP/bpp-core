#ifndef _GRAPHOBSERVER_HPP_
#define _GRAPHOBSERVER_HPP_

#include "Graph.h"
#include "GraphObserver.h"

#include "../Exceptions.h"
#include "../Clonable.h"

#include <vector>
#include <map>
#include <iostream>
#include <ostream>


namespace bpp
{
    
    /**
   * @brief Defines a Graph Associator. It is a template which follows
   * (subscribed to) a Graph.
   *
   * @author Thomas Bigot
   */
  
    // interface
    
    template <class N, class E>
    class AssociationGraphObserver:
    public virtual GraphObserver
    {
      
            
    typedef unsigned int NodeIndex;
    typedef unsigned int EdgeIndex;
    
    public:
      
      /** @name Graph Relations Management
      *  Modificating the structure of the graph.
      */
      ///@{
      
      /**
      * Creates an orphaned node from a NodeClass object.
      * @param newNodeObject the N object associated to the node in the graph.
      * 
      */
      virtual void createNode(N* newNodeObject) = 0;
      
      
      /**
      * Creates an node linked to an existing node. Order of parameters match
      * the link method.
      * @param newNodeObject the N object associated to the node in the graph.
      * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
      */
      virtual void createNode(N* objectOriginNode, N* newNodeObject) = 0;
      
      /**
      * Creates a link between two existing nodes.
      * If directed graph: nodeA -> nodeB.
      * @param nodeA source node (or first node if undirected)
      * @param nodeB target node (or second node if undirected)
      * @return the new edge
      */
      virtual void link(N* nodeObjectA, N* nodeObjectB, E* edgeObject = 00) = 0;
      
      /**
      * Creates a link between two existing nodes.
      * If directed graph: nodeA -> nodeB.
      * @param nodeA source node (or first node if undirected)
      * @param nodeB target node (or second node if undirected)
      */
      virtual void unlink(N* nodeObjectA, N* nodeObjectB) = 0;
      
      /**
      * Deletes a node
      * @param node node to be deleted
      */
      virtual void deleteNode(N* node) = 0;
      
      ///@}
      
      /** @name Object Association
      *  Associate or dissociate N and E objects to pre-existing Graph Nodes and Graph Edges
      */
      ///@{
      
      /**
      * Associate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to associate
      * @param node/edge existing node/edge to be associated
      */
      virtual void associateNode(N* nodeObject, Graph::Node node) = 0;
      virtual void associateEdge(E* edgeObject, Graph::Edge edge) = 0;
      
      /**
      * Dissociate a N or E object to a node or an edge in the graph.
      * @param nodeObject object to dissociate
      */
      virtual void forgetNode(N* nodeObject) = 0;
      virtual void forgetEdge(E* edgeObject) = 0;
      
      
      /**
      * Return the associated Node ID
      * @param nodeObject object which to return the node ID
      * @return a node ID
      */
      virtual Graph::Node getNodeId(N* nodeObject) = 0;
      
      /**
      * Return the associated Node ID
      * @param nodeObject object which to return the node ID
      * @return a node ID
      */
      virtual Graph::Edge getEdgeId(E* edgeObject) = 0;
      
      ///@}
      
      
       /** @name Object Indexation
      *  Get or set indexes to nodes and edges
      */
      ///@{
      
     /**
      * Return the associated Node index
      * @param nodeObject object which to return the node index
      * @return a node index
      */
      virtual AssociationGraphObserver<N,E>::NodeIndex getNodeIndex(N* nodeObject) = 0;
      
            
      /**
      * Return the associated Node index
      * @param edgeObject object which to return the node index
      * @return a node index
      */
      virtual AssociationGraphObserver<N,E>::EdgeIndex getEdgeIndex(E* edgeObject) = 0;
     
      /**
      * Set an index associated to a node
      * @param nodeObject object to which one want to set the index
      * @param index intex to be given, 0 to get the first free index
      * @return the given index
      */
      virtual unsigned int setNodeIndex(E* nodeObject, unsigned int index = 0) = 0;
      
      /**
      * Set an index associated to an edge
      * @param edgeObject object to which one want to set the index
      * @param index intex to be given, 0 to get the first free index
      * @return the given index
      */
      virtual unsigned int setEdgeIndex(E* edgeObject, unsigned int index = 0) = 0;
      
      ///@}    
      
      /** @name Topology exploration
      *  These methodes of the graph concern the topology exploration.
      */
      ///@{
      
      /**
      * Get all the neighbors of a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the neighbors
      */
      virtual const std::vector<N*> getNeighbors(N* objectNode) = 0;
      
      /**
      * In an directed graph, get all the neighbors which
      * are leaving a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the outgoing neighbors
      */
      virtual const std::vector<N*> getOutgoingNeighbors(N* node) = 0;
      
      /**
      * In an directed graph, get all the neighbors which
      * are coming to a node in the graph.
      * @param node the node one wants to get its neighbors
      * @return a vector containing the incoming neighbors
      */
      virtual const std::vector<N*> getIncomingNeighbors(N* node) = 0;
      
      /**
      * Get the leaves of a graph, ie, nodes with only one neighbor,
      * starting from a peculiar node.
      * @param node the starting node
      * @param maxDepth the maximum number of allowed depth, 0 means no max.
      * @return a vector containing the leaves
      */
      virtual const std::vector<N*> getLeavesFromNode(N* node, unsigned int maxDepth) = 0;
      
      /**
      * Get all the leaves objects of a graph, ie, nodes with only one neighbor,
      * @return a vector containing the leaves
      */
      virtual const std::vector<N*> getLeaves() = 0;
      
      /**
      * Get all the defined nodes of a graphO,
      * @return a vector containing the nodesObjects
      */
      virtual const std::vector<N*> getNodes() = 0;
      
     /**
      * Returns the Edge between two nodes nodeA -> nodeB
      * @param nodeA source node (if directed)
      * @param nodeB destination node (if directed)
      * @return the edge between these two nodes
      */
      virtual const E* getEdgeBetweenTwoNodes(N* nodeA, N* nodeB) = 0;
      
      
      ///@}
      
    };
    
    
    template <class N, class E, class I>
    class SimpleAssociationGraphObserver: public virtual AssociationGraphObserver<N,E>
    {

    private:
      //is the graph directed
      bool directed_;
      
      //index management
      typename AssociationGraphObserver<N,E>::NodeIndex highestNodeIndex_;
      typename AssociationGraphObserver<N,E>::EdgeIndex highestEdgeIndex_;
      
      /**
      * List of nodes, stored at the same ID than the corresponding nodes
      * in the observed graph.
      */
      std::vector<N*> nodeToObject_;
      
      /**
      * List of edges, stored at the same ID than the corresponding edges
      * in the bserved graph.
      */
      std::vector<E*> edgeToObject_;
      
      /**
      * Can find a Node with the corresponding object.
      */
      std::map<N*,Graph::Node> objectToNode_;
      
      /**
      * Can find an Edge with the corresponding object.
      */
      std::map<E*,Graph::Edge> objectToEdge_;
      
      
      /**
      * List of edges, stored at a given index.
      */
      std::vector<E*> indexToNodeObject_;
      
      /**
      * List of nodes, stored at a given index.
      */
      std::vector<N*> indexToEdgeObject_;
      
      /**
      * Can find a Node index with the corresponding object.
      */
      std::map<N*,typename AssociationGraphObserver<N,E>::NodeIndex> nodeObjectToIndex_;
      
      /**
      * Can find an Edge index with the corresponding object.
      */
      std::map<E*,typename AssociationGraphObserver<N,E>::EdgeIndex> edgeObjectToIndex_;
      
      /**
       * defines a type of neighbors : incoming and/or outgoing
       */
      enum neighborType {INCOMING,OUTGOING,BOTH};
      
      /**
      * Get incoming / outgoing neighbors according to the enum type
      */
      std::vector<N*> getNeighbors_(N* objectNode, neighborType type);
      
      
      /**
      * The observed Graph. Anytime this graph is observed,
      * the current object will be warned to take changes into account.
      */
      bpp::SimpleGraph* subjectGraph_;
      
      /**
      * Set the observed Graph
      * @param subjectGraph the graph which is observed
      */
      void observe_(bpp::SimpleGraph subjectGraph);
      

      /**
       * Transforms an (a list of) id(s) into an (a list of) object(s)
       */
      N* getNodeObject_(Graph::Node);
      std::vector<N*> getNodeObjects_(std::vector<Graph::Node>);
      E* getEdgeObject_(Graph::Edge);
      std::vector<E*> getEdgeObjects_(std::vector<Graph::Edge>);
      
        
    public:
      
      /**
      * Constructor
      * @param directed is the graph directed
      */
      SimpleAssociationGraphObserver(bool directed = false);
      
      /**
      * Constructor
      * @param subjectGraph the graph which is observed
      */
      SimpleAssociationGraphObserver(bpp::SimpleGraph* subjectGraph = 00);
      
      /**
      * Copy Constructor
      * @param graphObserver the graphObserver to be copied
      */
      SimpleAssociationGraphObserver(bpp::SimpleAssociationGraphObserver<N,E> const& graphObserver);
      
      /**
      * = Operator
      * @param graphObserver the graphObserver we want to copy the values
      */
      SimpleAssociationGraphObserver<N,E> operator=(bpp::SimpleAssociationGraphObserver<N,E> const& graphObserver);
      
      
      
      /**
      * Destructor
      * @param graphObserver the graphObserver to be copied
      */
      ~SimpleAssociationGraphObserver();
      
      
      
      /**
      * clone function
      * @param graphObserver the graphObserver to be copied
      */
      #ifdef NO_VIRTUAL_COV
        Clonable*
      #else
        SimpleAssociationGraphObserver<N,E>*
      #endif
      clone() const { return new SimpleAssociationGraphObserver<N,E>(*this); };
      
      
      SimpleGraph* getGraph();
      
      /**
      * This function is called to tell the observer that the subject
      * has changed and hence the observer has to take the changes
      * into account.
      */
      void update();
      
      
      
      /** @name Graph Relations Management
      *  Modificating the structure of the graph.
      */
      ///@{
      
      /**
      * Creates an orphaned node from a NodeClass object.
      * @param newNodeObject the N object associated to the node in the graph.
      * 
      */
      void createNode(N* newNodeObject);
      
      /**
      * Creates an node linked to an existing node. Order of parameters match
      * the link method.
      * @param newNodeObject the N object associated to the node in the graph.
      * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
      */
      void createNode(N* objectOriginNode, N* newNodeObject);
      
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
      ///@{
      
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
      
      
      /**
      * Return the associated Node ID
      * @param nodeObject object which to return the node ID
      * @return a node ID
      */
      Graph::Node getNodeId(N* nodeObject);

      /**
      * Return the associated Node ID
      * @param nodeObject object which to return the node ID
      * @return a node ID
      */
      Graph::Edge getEdgeId(E* edgeObject);
      
      
      ///@}   
      
      /** @name Object Indexation
      *  Get or set indexes to nodes and edges
      */
      ///@{
      
     /**
      * Return the associated Node index
      * @param nodeObject object which to return the node index
      * @return a node index
      */
      typename AssociationGraphObserver<N,E>::NodeIndex getNodeIndex(N* nodeObject);
      
            
      /**
      * Return the associated Node index
      * @param edgeObject object which to return the node index
      * @return a node index
      */
      typename AssociationGraphObserver<N,E>::EdgeIndex getEdgeIndex(E* edgeObject);
      
      /**
      * Set an index associated to a node
      * @param nodeObject object to which one want to set the index
      * @param index intex to be given, 0 to get the first free index
      * @return the given index
      */
      unsigned int setNodeIndex(E* nodeObject, typename AssociationGraphObserver<N,E>::NodeIndex index = 0);
      
      /**
      * Set an index associated to an edge
      * @param edgeObject object to which one want to set the index
      * @param index intex to be given, 0 to get the first free index
      * @return the given index
      */
      unsigned int setEdgeIndex(E* edgeObject, typename AssociationGraphObserver<N,E>::EdgeIndex index = 0);
      
      ///@}    
      
      /** @name Topology exploration
      *  These methodes of the graph concern the topology exploration.
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
      * Get all the defined nodes of a graph,
      * @return a vector containing the nodesObjects
      */
      const std::vector<N*> getNodes();
      
            
      /**
      * Returns the Edge between two nodes nodeA -> nodeB
      * @param nodeA source node (if directed)
      * @param nodeB destination node (if directed)
      * @return the edge between these two nodes
      */
      const E* getEdgeBetweenTwoNodes(N* nodeA, N* nodeB);
      
      
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
    
    class GraphObserverTools
    {
      static void outputToDot(SimpleAssociationGraphObserver<std::string,void>,std::ostream &out);
    };
    
    
template <class N, class E, class I>
N* SimpleAssociationGraphObserver<N,E>::getNodeObject_(Graph::Node node)
{
  return nodeToObject_.at(node); 
}

template <class N, class E, class I>
E* SimpleAssociationGraphObserver<N,E>::getEdgeObject_(Graph::Edge edge)
{
  return edgeToObject_.at(edge); 
}

template <class N, class E, class I>
std::vector<N*> SimpleAssociationGraphObserver<N,E>::getNodeObjects_(std::vector<Graph::Node> nodes)
{
  std::vector<N*> nodeObjects;
  for(std::vector<Graph::Node>::iterator currNode = nodes.begin(); currNode != nodes.end(); currNode++)
  {
    nodeObjects.push_back(nodeToObject_(*currNode));
  }
  return nodeObjects;
}

template <class N, class E, class I>
std::vector<E*> SimpleAssociationGraphObserver<N,E>::getEdgeObjects_(std::vector<Graph::Edge> edges)
{
  std::vector<N*> edgeObjects;
  for(std::vector<Graph::Node>::iterator currEdge = edges.begin(); currEdge != edges.end(); currEdge++)
  {
    edgeObjects.push_back(edgeToObject_(*currEdge));
  }
  return edgeObjects;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::createNode(N* nodeObject)
{
  Graph::Node newGraphNode = subjectGraph_->createNode();
  
  associateNode(nodeObject, newGraphNode);
  
}

template <class N, class E, class I>
SimpleAssociationGraphObserver<N,E>::SimpleAssociationGraphObserver(bool directed_p):
  directed_(directed_p),
  highestNodeIndex_(0),
  highestEdgeIndex_(0),
  nodeToObject_(std::vector<N*>()),
  edgeToObject_(std::vector<E*>()),
  objectToNode_(std::map<N*,Graph::Node>()),
  objectToEdge_(std::map<E*,Graph::Node>()),
  indexToNodeObject_(std::vector<E*>()),
  indexToEdgeObject_(std::vector<N*>()),
  nodeObjectToIndex_(std::map<N*,typename AssociationGraphObserver<N,E>::NodeIndex>()),
  edgeObjectToIndex_(std::map<E*,typename AssociationGraphObserver<N,E>::EdgeIndex>()),
  subjectGraph_(new SimpleGraph(directed_p))
{
  this->subjectGraph_->registerObserver(this);
}

template <class N, class E, class I>
SimpleAssociationGraphObserver<N,E>::~SimpleAssociationGraphObserver()
{
  this->subjectGraph_->unregisterObserver(this);
}

template <class N, class E, class I>
SimpleAssociationGraphObserver<N,E>::SimpleAssociationGraphObserver(SimpleAssociationGraphObserver<N,E> const& graphObserver):
  directed_(graphObserver.directed_),
  highestNodeIndex_(0),
  highestEdgeIndex_(0),
  edgeToObject_(graphObserver.edgeToObject_),
  nodeToObject_(graphObserver.nodeToObject_),
  objectToEdge_(graphObserver.objectToEdge_),
  objectToNode_(graphObserver.objectToNode_),
  indexToNodeObject_(std::vector<E*>()),
  indexToEdgeObject_(std::vector<N*>()),
  nodeObjectToIndex_(std::map<N*,typename AssociationGraphObserver<N,E>::NodeIndex>()),
  edgeObjectToIndex_(std::map<E*,typename AssociationGraphObserver<N,E>::EdgeIndex>()),
  subjectGraph_(graphObserver.subjectGraph_)
{
}


template <class N, class E, class I>
SimpleAssociationGraphObserver<N,E> SimpleAssociationGraphObserver<N,E>::operator=(SimpleAssociationGraphObserver<N,E> const& graphObserver)
{
  this->directed_ = graphObserver.directed_;
  this->highestEdgeIndex_ = graphObserver.highestEdgeIndex_;
  this->edgeToObject_ = graphObserver.edgeToObject_;
  this->nodeToObject_ = graphObserver.nodeToObject_;
  this->objectToEdge_ = graphObserver.objectToEdge_;
  this->objectToNode_ = graphObserver.objectToNode_;
  this->indexToNodeObject_ = graphObserver.indexToNodeObject_;
  this->indexToEdgeObject_ = graphObserver.indexToEdgeObject_;
  this->nodeObjectToIndex_ = graphObserver.nodeObjectToIndex_;
  this->edgeObjectToIndex_ = graphObserver.edgeObjectToIndex_;
  this->subjectGraph_ = graphObserver.subjectGraph_;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::createNode(N* objectOriginNode,N* newNodeObject)
{
  createNode(newNodeObject);
  link(objectOriginNode,newNodeObject);
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::link(N* nodeObjectA, N* nodeObjectB, E* edgeObject)
{
  // checking the nodes
  typename std::map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectToNode_.find(nodeObjectA);
  foundNodeB = objectToNode_.find(nodeObjectB);
  if(foundNodeA == objectToNode_.end() || foundNodeB == objectToNode_.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  //checking if the edge is not already in the GraphObserver
  if(edgeObject != 00 && objectToEdge_.find(edgeObject) != objectToEdge_.end())
    throw Exception("The given edge is already associated to a relation in the subjectGraph.");
  
  std::cout << "Trying to link node " << foundNodeA->second << " -> " << foundNodeB->second << std::endl;
  Graph::Edge newGraphEdge = subjectGraph_->link(foundNodeA->second,foundNodeB->second);
  
  if(edgeToObject_.size() < newGraphEdge+1)
    edgeToObject_.resize(newGraphEdge+1);
  edgeToObject_.at(newGraphEdge) = edgeObject;
  
  objectToEdge_[edgeObject] = newGraphEdge;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::unlink(N* nodeObjectA, N* nodeObjectB)
{
  //checking the nodes
  typename std::map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectToNode_.find(nodeObjectA);
  foundNodeB = objectToNode_.find(nodeObjectB);
  if(foundNodeA == objectToNode_.end() || foundNodeB == objectToNode_.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  subjectGraph_->unlink(foundNodeA->second,foundNodeB->second);
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::deletedEdgesUpdate(std::vector<Graph::Edge>& edgesToDelete)
{
  for(std::vector<Graph::Edge>::iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++){
    E* edgeObject = edgeToObject_.at(*currEdge);
    edgeToObject_.at(*currEdge) = 00;
    
    objectToEdge_.erase(edgeObject);
    
  }
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::deletedNodesUpdate(std::vector<Graph::Node>& nodesToDelete){
  for(std::vector<Graph::Edge>::iterator currNode = nodesToDelete.begin(); currNode != nodesToDelete.end(); currNode++){
    N* nodeObject = nodeToObject_.at(*currNode);
    nodeToObject_.at(*currNode) = 00;
    
    objectToNode_.erase(nodeObject);
    
  }
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::associateNode(N* nodeObject, Graph::Node graphNode)
{
  // nodes vector must be the right size. Eg: to store a node with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  nodeToObject_.resize(subjectGraph_->getHighestNodeID()+1);

  // now storing the node
  nodeToObject_.at(graphNode) = nodeObject;
  objectToNode_[nodeObject] = graphNode;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::associateEdge(E* edgeObject, Graph::Edge graphEdge)
{
  // nodes vector must be the right size. Eg: to store an edge with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  edgeToObject_.resize(subjectGraph_->getHighestEdgeID()+1);
  
  // now storing the edge
  edgeToObject_.at(graphEdge) = edgeObject;
  objectToEdge_[edgeObject] = graphEdge;
}

template <class N, class E, class I>
unsigned int SimpleAssociationGraphObserver<N,E>::setEdgeIndex(E* edgeObject, typename AssociationGraphObserver<N,E>::EdgeIndex index)
{
  //TODO: check if this object has already an index?
  if(index == 0)
    index == ++highestEdgeIndex_;
  // nodes vector must be the right size. Eg: to store an edge with
  // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if(index > highestEdgeIndex_){
    highestEdgeIndex_ = index;
  }
  indexToEdgeObject_.resize(highestEdgeIndex_+1);
  
  // now storing the edge
  indexToEdgeObject_.at(index) = edgeObject;
  edgeObjectToIndex_[edgeObject] = index;
  return index;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::forgetNode(N* nodeObject)
{
  typename std::map<N*,Graph::Node>::iterator nodeToForget = objectToNode_.find(nodeObject);
  nodeToObject_.at(nodeToForget->second) = 00;
  objectToNode_.erase(nodeToForget);
}

template <class N, class E, class I>
unsigned int SimpleAssociationGraphObserver<N,E>::setNodeIndex(E* nodeObject, typename AssociationGraphObserver<N,E>::NodeIndex index)
{
  //TODO: check if this object has already an index?
  if(index == 0)
    index == ++highestNodeIndex_;
  // nodes vector must be the right size. Eg: to store a node with
  // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if(index > highestNodeIndex_){
    highestNodeIndex_ = index;
  }
  indexToNodeObject_.resize(highestNodeIndex_+1);
  
  // now storing the node
  indexToNodeObject_.at(index) = nodeObject;
  nodeObjectToIndex_[nodeObject] = index;
  return index;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::forgetEdge(E* edgeObject)
{
  typename std::map<E*,Graph::Edge>::iterator edgeToForget = objectToEdge_.find(edgeObject);
  edgeToObject_.at(edgeToForget->second) = 00;
  objectToEdge_.erase(edgeToForget);
}


template <class N, class E, class I>
const std::vector<N*> SimpleAssociationGraphObserver<N,E>::getLeaves()
{
  std::vector<N*> leavesToReturn;
  // fetching all the graph Leaves
  std::vector<Graph::Node> graphLeaves = subjectGraph_->getLeaves();
  // testing if they are defined in this observer
  for(std::vector<Graph::Node>::iterator currGraphLeave = graphLeaves.begin(); currGraphLeave != graphLeaves.end(); currGraphLeave++)
  {
    N* foundLeafObject = nodeToObject_.at(*currGraphLeave);
    if(foundLeafObject != 00)
      leavesToReturn.push_back(foundLeafObject);
  }
  return leavesToReturn;
}

template <class N, class E, class I>
const std::vector<N*> SimpleAssociationGraphObserver<N,E>::getNodes()
{
  std::vector<N*> nodesToReturn;
  for(typename std::vector<N*>::iterator currNodeObject = nodeToObject_.begin(); currNodeObject != nodeToObject_.end(); currNodeObject++)
  {
    if(*currNodeObject != 00)
    {
      nodesToReturn.push_back(*currNodeObject);
    }
  }
  return nodesToReturn;
}

template <class N, class E, class I>
size_t SimpleAssociationGraphObserver<N,E>::getNumberOfNodes()
{
  return objectToNode_.size();
}
 
template <class N, class E, class I>
size_t SimpleAssociationGraphObserver<N,E>::getNumberOfLeaves()
{
  return getLeaves().size();
}
 
template <class N, class E, class I>
SimpleGraph* SimpleAssociationGraphObserver<N,E>::getGraph()
{
  return subjectGraph_;
}

template <class N, class E, class I>
void SimpleAssociationGraphObserver<N,E>::deleteNode(N* node)
{
  // first deleting the node in the graph
  subjectGraph_->deleteNode(getNodeId(node));
  // then forgetting
  forgetNode(node);
}

template <class N, class E, class I>
Graph::Node SimpleAssociationGraphObserver<N,E>::getNodeId(N* nodeObject)
{
  typename std::map<N*,Graph::Node>::iterator found = objectToNode_.find(nodeObject);
  if(found == objectToNode_.end())
    throw Exception("Unexisting node object.");
  return found->second;
}

template <class N, class E, class I>
typename AssociationGraphObserver<N,E>::NodeIndex SimpleAssociationGraphObserver<N,E>::getNodeIndex(N* nodeObject)
{
  typename std::map<N*,typename AssociationGraphObserver<N,E>::NodeIndex>::iterator found = nodeObjectToIndex_.find(nodeObject);
  if(found == nodeObjectToIndex_.end())
    throw Exception("Unexisting node object.");
  return found->second;
}

template <class N, class E, class I>
Graph::Edge SimpleAssociationGraphObserver<N,E>::getEdgeId(E* edgeObject)
{
  typename std::map<E*,Graph::Edge>::iterator found = objectToEdge_.find(edgeObject);
  if(found == objectToEdge_.end())
    throw Exception("Unexisting edge object.");
  return found->second;
}

template <class N, class E, class I>
typename AssociationGraphObserver<N,E>::EdgeIndex SimpleAssociationGraphObserver<N,E>::getEdgeIndex(E* edgeObject)
{
  typename std::map<E*,typename AssociationGraphObserver<N,E>::EdgeIndex>::iterator found = edgeObjectToIndex_.find(edgeObject);
  if(found == edgeObjectToIndex_.end())
    throw Exception("Unexisting edge object.");
  return found->second;
}

template <class N, class E, class I>
std::vector< N* > SimpleAssociationGraphObserver<N,E>::getNeighbors_(N* objectNode, neighborType type)
{
  Graph::Node node = getNodeId(objectNode);
  
  // PHASE 1: getting the right neighbors
  std::vector<Graph::Node> neighbors;
  switch(type){
    case OUTGOING:
      neighbors = subjectGraph_->getOutgoingNeighbors(node);
      break;
    case INCOMING:
      neighbors = subjectGraph_->getIncomingNeighbors(node);
      break;
    case BOTH:
      neighbors = subjectGraph_->getNeighbors(node);
  }
  
  // PHASE 2: transforming into nodeObjects
  std::vector<N*> objectNeighbors;
  for(std::vector<Graph::Node>::iterator currNeighbor = neighbors.begin(); currNeighbor != neighbors.end(); currNeighbor++){
    objectNeighbors.push_back(nodeToObject_.at(*currNeighbor));
  }
  return objectNeighbors;
}

template <class N, class E, class I>
const std::vector< N* > SimpleAssociationGraphObserver<N,E>::getIncomingNeighbors(N* node)
{
  return(getNeighbors_(node,INCOMING));
}

template <class N, class E, class I>
const std::vector< N* > SimpleAssociationGraphObserver<N,E>::getOutgoingNeighbors(N* node)
{
  return(getNeighbors_(node,OUTGOING));
}

template <class N, class E, class I>
const std::vector< N* > SimpleAssociationGraphObserver<N,E>::getNeighbors(N* node)
{
  return(getNeighbors_(node,BOTH));
}


 
}

#else

namespace bpp {class GraphObserver;}

#endif
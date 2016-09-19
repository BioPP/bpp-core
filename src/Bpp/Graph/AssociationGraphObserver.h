#ifndef _ASSOCIATIONGRAPHOBSERVER_HPP_
#define _ASSOCIATIONGRAPHOBSERVER_HPP_

#include "GraphObserver.h"

#include "../Exceptions.h"
#include "../Clonable.h"

#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include <memory>

namespace bpp
{
/**
 * @brief Defines a Graph Associator. It is a template which follows
 * (subscribed to) a Graph.
 *
 * @author Thomas Bigot
 */

// interface

template<class N, class E>
class AssociationGraphObserver :
  public virtual GraphObserver
{
private:
  typedef Graph::Node NodeGraphid;
  typedef Graph::Edge EdgeGraphid;

public:
  typedef unsigned int NodeIndex;
  typedef unsigned int EdgeIndex;


  /** @name Graph Relations Management
   *  Modificating the structure of the graph.
   */
  // /@{

  /**
   * Creates an orphaned node from a NodeClass object.
   * @param newNodeObject the N object associated to the node in the graph.
   *
   */
  virtual void createNode(std::shared_ptr<N>  newNodeObject) = 0;


  /**
   * Creates an node linked to an existing node. Order of parameters match
   * the link method.
   * @param newNodeObject the N object associated to the node in the graph.
   * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
   */
  virtual void createNode(std::shared_ptr<N>  objectOriginNode, std::shared_ptr<N>  newNodeObject, std::shared_ptr<E>  newEdgeObject = 00) = 0;

  /**
   * Creates a link between two existing nodes.
   * If directed graph: nodeA -> nodeB.
   * @param nodeObjectA source node (or first node if undirected)
   * @param nodeObjectB target node (or second node if undirected)
   */
  virtual void link(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB, std::shared_ptr<E>  edgeObject = 00) = 0;

  /**
   * Detroys a link between two existing nodes in the graph.
   * If directed graph: nodeA -> nodeB.
   * @param nodeObjectA source node (or first node if undirected)
   * @param nodeObjectB target node (or second node if undirected)
   */
  
  virtual void unlink(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB) = 0;

  /**
   * Deletes a node
   * @param nodeObject node to be deleted
   */
  virtual void deleteNode(std::shared_ptr<N>  nodeObject) = 0;

  // /@}

  /** @name Object Association
   *  Associate or dissociate N and E objects to pre-existing Graph Nodes and Graph Edges
   */
  // /@{

  /**
   * Associate a N or E object to a node or an edge in the graph.
   * @param nodeObject object to associate
   * @param node/edge existing node/edge to be associated
   */
  virtual void associateNode(std::shared_ptr<N>  nodeObject, NodeGraphid node) = 0;
  virtual void associateEdge(std::shared_ptr<E>  edgeObject, EdgeGraphid edge) = 0;

  /**
   * Dissociate a N or E object to a node or an edge in the graph.
   * Graph is not changed.
   *
   * @param nodeObject object to dissociate
   */
  virtual void dissociateNode(std::shared_ptr<N>  nodeObject) = 0;
  virtual void dissociateEdge(std::shared_ptr<E>  edgeObject) = 0;


  /**
   * Return the associated Node ID
   * @param nodeObject object which to return the node ID
   * @return a node ID
   */
  virtual NodeGraphid getNodeGraphid(const std::shared_ptr<N>  nodeObject) const = 0;

  /**
   * Return the associated Node ID
   * @param edgeObject object which to return the node ID
   * @return a node ID
   */
  virtual EdgeGraphid getEdgeGraphid(const std::shared_ptr<E>  edgeObject) const = 0;

  /**
  * Transforms an (a list of) id(s) into an (a list of) object(s)
  */
  virtual const std::shared_ptr<N>  getNodeFromGraphid(NodeGraphid) const = 0;
  virtual std::shared_ptr<N>  getNodeFromGraphid(NodeGraphid) = 0;
  
  virtual  std::vector<std::shared_ptr<N> > getNodesFromGraphid(std::vector<NodeGraphid> ) const = 0;
  virtual  std::shared_ptr<E>  getEdgeFromGraphid(EdgeGraphid) = 0;
  virtual  const std::shared_ptr<E>  getEdgeFromGraphid(EdgeGraphid) const = 0;
  virtual std::vector<std::shared_ptr<E> > getEdgesFromGraphid(std::vector<EdgeGraphid> ) const = 0;

  
  /**
   * Set a new root
   * @param newRoot the new root node
   */
  virtual void setRoot(const std::shared_ptr<N>  newRoot) = 0;
  
  
  // /@}


  /** @name Object Indexation
   *  Get or set indexes to nodes and edges
   */
  // /@{

  /**
   * @brief return if the object has an index.
   */

  virtual bool hasIndex(const std::shared_ptr<N> nodeObject) const = 0;
  virtual bool hasIndex(const std::shared_ptr<E> edgeObject) const = 0;
  
  /**
   * Return the associated Node index
   * @param nodeObject object which to return the node index
   * @return a node index
   */
  
  virtual NodeIndex getNodeIndex(const std::shared_ptr<N>  nodeObject) const = 0;
  virtual std::vector<NodeIndex> getNodeIndexes(std::vector<std::shared_ptr<N> > nodeObjects) const = 0;


  /**
   * Return the associated Node index
   * @param edgeObject object which to return the node index
   * @return a node index
   */
  virtual EdgeIndex getEdgeIndex(const std::shared_ptr<E>  edgeObject) const = 0;
  virtual std::vector<EdgeIndex> getEdgeIndexes(std::vector<std::shared_ptr<E> > edgeObjects) const = 0;

  /**
   * Set an index associated to a node
   * @param nodeObject object to which one want to set the index
   * @param index intex to be given, 0 to get the first free index
   * @return the given index
   */
  virtual NodeIndex setNodeIndex(const std::shared_ptr<N>  nodeObject, NodeIndex index) = 0;

  /**
   * Set an index associated to an edge
   * @param edgeObject object to which one want to set the index
   * @param index intex to be given, 0 to get the first free index
   * @return the given index
   */
  virtual EdgeIndex setEdgeIndex(const std::shared_ptr<E>  edgeObject, EdgeIndex index) = 0;

  /**
   * Return the associated Node, querying with an index
   * @param nodeIndex the index of the wanted node
   * @return N, a node object
   */
  virtual std::shared_ptr<N>  getNode(NodeIndex nodeIndex) const = 0;


  /**
   * Return the associated Node index
   * @param edgeIndex the index of the wanted edge
   * @return E, an edge object
   */
  virtual std::shared_ptr<E>  getEdge(EdgeIndex edgeIndex) const = 0;

  // /@}

  /** @name Topology exploration
   *  These methodes of the graph concern the topology exploration.
   */
  // /@{

  /**
   * Get all the neighbors of a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the neighbors
   */
  virtual std::vector<std::shared_ptr<N> > getNeighbors(const std::shared_ptr<N>  node) const = 0;
  virtual std::vector<NodeIndex> getNeighbors(NodeIndex node) const = 0;

  /**
   * Get all the edges from/to a node in the graph.
   * @param node the node one wants to get its neighbor edges
   * @return a vector containing the edges
   */
  virtual std::vector<std::shared_ptr<E> > getEdges(const std::shared_ptr<N>  node) const = 0;
  virtual std::vector<EdgeIndex> getEdges(NodeIndex node) const = 0;

  /**
   * In an directed graph, get all the neighbors which
   * are leaving a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the outgoing neighbors
   */
  virtual std::vector<std::shared_ptr<N> > getOutgoingNeighbors(const std::shared_ptr<N>  node) const = 0;
  virtual std::vector<NodeIndex> getOutgoingNeighbors(NodeIndex node) const = 0;

  /**
   * In an directed graph, get all the edges which
   * are leaving a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the outgoing edges
   */
  virtual std::vector<std::shared_ptr<E> > getOutgoingEdges(const std::shared_ptr<N>  node) const = 0;
  virtual std::vector<EdgeIndex> getOutgoingEdges(NodeIndex node) const = 0;


  /**
   * In an directed graph, get all the neighbors which
   * are coming to a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the incoming neighbors
   */
  virtual std::vector<std::shared_ptr<N> > getIncomingNeighbors(const std::shared_ptr<N>  node) const = 0;
  virtual std::vector<NodeIndex> getIncomingNeighbors(NodeIndex node) const = 0;

  /**
   * In an directed graph, get all the edges which
   * are coming to a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the incoming edges
   */
  virtual std::vector<std::shared_ptr<E> > getIncomingEdges(const std::shared_ptr<N>  node) const = 0;
  virtual std::vector<EdgeIndex> getIncomingEdges(NodeIndex node) const = 0;


  /**
   * Get the leaves of a graph, ie, nodes with only one neighbor,
   * starting from a peculiar node.
   * @param node the starting node
   * @param maxDepth the maximum number of allowed depth, 0 means no max.
   * @return a vector containing the leaves
   */
  virtual std::vector<std::shared_ptr<N> > getLeavesFromNode(std::shared_ptr<N>  node, unsigned int maxDepth) const = 0;

  /**
   * Get all the leaves objects of a graph, ie, nodes with only one neighbor,
   * @return a vector containing the leaves
   */
  virtual std::vector<std::shared_ptr<N> > getAllLeaves() const = 0;

  /**
   * Get all the defined nodes of a graphO,
   * @return a vector containing the nodesObjects
   */
  virtual std::vector<std::shared_ptr<N> > getAllNodes() const = 0;

  /**
   * Get all the defined edges of a graphO,
   * @return a vector containing the branchesObjects
   */
  
  virtual std::vector<std::shared_ptr<E> > getAllEdges() const = 0;

  /**
   * Returns the Edge between two nodes nodeA -> nodeB
   * @param nodeA source node (if directed)
   * @param nodeB destination node (if directed)
   * @return the edge between these two nodes
   */
  virtual std::shared_ptr<E>  getEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB) const = 0;

  /**
   * Sets the Edge between two nodes nodeA -> nodeB
   * @param nodeA source node (if directed)
   * @param nodeB destination node (if directed)
   * @param edge the edge between these two nodes
   */
  virtual void setEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB, std::shared_ptr<E>  edge) = 0;
  
  // /@}
};


template<class N, class E, class GraphImpl>
class SimpleAssociationGraphObserver : public virtual AssociationGraphObserver<N, E>
{
public:
  typedef typename AssociationGraphObserver<N, E>::NodeIndex NodeIndex;
  typedef typename AssociationGraphObserver<N, E>::EdgeIndex EdgeIndex;

  typedef typename Graph::Node NodeGraphid;
  typedef typename Graph::Edge EdgeGraphid;

protected:
  /**
   * The observed Graph. Anytime this graph is observed,
   * the current object will be warned to take changes into account.
   */

  std::shared_ptr<GraphImpl> subjectGraph_;

private:
  /**
   * Registration with Graph Ids.
   *
   * @{
   */
  
  /**
   * List of nodes, stored at the same ID than the corresponding nodes
   * in the observed graph.
   */
  std::vector<std::shared_ptr<N> > graphidToN_;

  /**
   * List of edges, stored at the same ID than the corresponding edges
   * in the observed graph.
   */
  std::vector<std::shared_ptr<E> > graphidToE_;

  /**
   * Can find a Node with the corresponding object.
   */
  std::map<std::shared_ptr<N>, NodeGraphid> NToGraphid_;

  /**
   * Can find an Edge with the corresponding object.
   */
  std::map<std::shared_ptr<E>, EdgeGraphid> EToGraphid_;

  /*
   * @}
   */

  /**
   * Registration with own Ids.
   *
   * @{
   */

  /**
   * List of nodes, stored at a given index.
   */
  std::vector<std::shared_ptr<N> > indexToN_;

  /**
   * List of edges, stored at a given index.
   */
  std::vector<std::shared_ptr<E> > indexToE_;

  /**
   * Can find a Node index with the corresponding object.
   */
  std::map<std::shared_ptr<N> , NodeIndex> NToIndex_;

  /**
   * Can find an Edge index with the corresponding object.
   */
  std::map<std::shared_ptr<E> , EdgeIndex> EToIndex_;

  /*
   * @}
   */

  
  /**
   * defines a type of neighbors : incoming and/or outgoing
   */
  enum neighborType {INCOMING, OUTGOING, BOTH};

  /**
   * Get incoming / outgoing neighbors according to the enum type
   */
  std::vector<std::shared_ptr<N> > getNeighbors_(const std::shared_ptr<N>  nodeObject, neighborType type) const;

  /**
   * Get incoming / outgoing edges according to the enum type
   */
  std::vector<std::shared_ptr<E> > getEdges_(const std::shared_ptr<N>  nodeObject, neighborType type) const;

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
  
  SimpleAssociationGraphObserver(std::shared_ptr<SimpleGraph> subjectGraph = 00);

  /**
   * Copy Constructor
   * @param graphObserver the graphObserver to be copied
   */

  template<class N2, class E2>
  SimpleAssociationGraphObserver(SimpleAssociationGraphObserver<N2, E2, GraphImpl> const& graphObserver);
  
  SimpleAssociationGraphObserver(SimpleAssociationGraphObserver<N, E, GraphImpl> const& graphObserver);

  /**
   * = Operator
   * @param graphObserver the graphObserver we want to copy the values
   */
  
  SimpleAssociationGraphObserver<N, E, GraphImpl>& operator=(bpp::SimpleAssociationGraphObserver<N, E, GraphImpl> const& graphObserver);

  /**
   * Destructor
   * @param graphObserver the graphObserver to be copied
   */
  ~SimpleAssociationGraphObserver();


  /**
   * clone function
   * @param graphObserver the graphObserver to be copied
   */
  SimpleAssociationGraphObserver<N, E, GraphImpl>* clone() const { return new SimpleAssociationGraphObserver<N, E, GraphImpl>(*this); }

  const std::shared_ptr<GraphImpl> getGraph() const;

  std::shared_ptr<GraphImpl> getGraph();

  /**
   * This function is called to tell the observer that the subject
   * has changed and hence the observer has to take the changes
   * into account.
   */
  void update();


  /** @name Graph Relations Management
   *  Modificating the structure of the graph.
   */
  // /@{

  /**
   * Creates an orphaned node from a NodeClass object.
   * @param newNodeObject the N object associated to the node in the graph.
   *
   */
  void createNode(std::shared_ptr<N>  newNodeObject);

  /**
   * Creates an node linked to an existing node. Order of parameters match
   * the link method.
   * @param newNodeObject the N object associated to the node in the graph.
   * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
   */
  void createNode(std::shared_ptr<N>  objectOriginNode, std::shared_ptr<N>  newNodeObject, std::shared_ptr<E>  newEdgeObject = 00);

  /**
   * Creates a link between two existing nodes.
   * If directed graph: nodeA -> nodeB.
   * @param nodeObjectA source node (or first node if undirected)
   * @param nodeObjectB target node (or second node if undirected)
   */
  void link(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB, std::shared_ptr<E>  edgeObject = 00);

  /**
   * Creates a link between two existing nodes in the graph.
   * If directed graph: nodeA -> nodeB.
   * @param nodeObjectA source node (or first node if undirected)
   * @param nodeObjectB target node (or second node if undirected)
   */
  void unlink(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB);

  /**
   * Deletes a node
   * @param nodeObject node to be deleted. The N node object given in argument is not deleted.
   */
  void deleteNode(std::shared_ptr<N>  nodeObject);


  // /@}

  /** @name Object Association
   *  Associate or dissociate N and E objects to pre-existing Graph Nodes and Graph Edges
   */
  // /@{

  /**
   * Associate a N or E object to a node or an edge in the graph.
   * @param nodeObject object to associate
   * @param node/edge existing node/edge to be associated
   */
  void associateNode(std::shared_ptr<N>  nodeObject, NodeGraphid graphNode);
  void associateEdge(std::shared_ptr<E>  edgeObject, EdgeGraphid graphEdge);

  /**
   * Dissociate a N or E object to a node or an edge in the graph.
   * @param nodeObject object to dissociate
   */
  void dissociateNode(std::shared_ptr<N>  nodeObject);
  void dissociateEdge(std::shared_ptr<E>  edgeObject);


  /**
   * Return the associated Node ID
   * @param nodeObject object which to return the node ID
   * @return a node ID
   */
  NodeGraphid getNodeGraphid(const std::shared_ptr<N>  nodeObject) const;

  /**
   * Return the associated Edge ID
   * @param edgeObject object which to return the node ID
   * @return a edge ID
   */
  EdgeGraphid getEdgeGraphid(const std::shared_ptr<E>  edgeObject) const;


  /**
   * Transforms an (a list of) id(s) into an (a list of) object(s)
   */
  std::shared_ptr<N>  getNodeFromGraphid(NodeGraphid);
  const std::shared_ptr<N>  getNodeFromGraphid(NodeGraphid) const;
  
  std::vector<std::shared_ptr<N> > getNodesFromGraphid(std::vector<NodeGraphid> ) const;
  std::shared_ptr<E> getEdgeFromGraphid(EdgeGraphid);
  const std::shared_ptr<E> getEdgeFromGraphid(EdgeGraphid) const;
  std::vector<std::shared_ptr<E> > getEdgesFromGraphid(std::vector<EdgeGraphid> ) const;


  /**
   * Set a new root
   * @param newRoot the new root node
   */
  void setRoot(const std::shared_ptr<N>  newRoot);
  
  // /@}

  /** @name Object Indexation
   *  Get or set indexes to nodes and edges
   */
  // /@{

  /*
   * @brief Return if object has an index.
   *
   */
  
  bool hasIndex(const std::shared_ptr<N> nodeObject) const;
  bool hasIndex(const std::shared_ptr<E> edgeObject) const;

  /**
   * Return the associated Node index
   * @param nodeObject object which to return the node index
   * @return a node index
   */
  NodeIndex getNodeIndex(const std::shared_ptr<N>  nodeObject) const;
  std::vector<NodeIndex> getNodeIndexes(std::vector<std::shared_ptr<N> > nodeObjects) const;

  /**
   * Return the associated Node index
   * @param edgeObject object which to return the node index
   * @return a node index
   */
  EdgeIndex getEdgeIndex(const std::shared_ptr<E>  edgeObject) const;
  std::vector<EdgeIndex> getEdgeIndexes(std::vector<std::shared_ptr<E> > edgeObjects) const;

  /**
   * Set an index associated to a node
   * @param nodeObject object to which one want to set the index
   * @param index intex to be given, 0 to get the first free index
   * @return the given index
   */
  NodeIndex setNodeIndex(const std::shared_ptr<N>  nodeObject, NodeIndex index);

  /**
   * Set an index associated to an edge
   * @param edgeObject object to which one want to set the index
   * @param index intex to be given, 0 to get the first free index
   * @return the given index
   */
  EdgeIndex setEdgeIndex(const std::shared_ptr<E>  edgeObject, EdgeIndex index);


  /**
   * Return the associated Node index
   * @param nodeObject object which to return the node index
   * @return a node index
   */
  virtual std::shared_ptr<N>  getNode(NodeIndex nodeIndex) const;


  /**
   * Return the associated Node index
   * @param edgeObject object which to return the node index
   * @return a node index
   */
  virtual std::shared_ptr<E>  getEdge(EdgeIndex edgeIndex) const;
  // /@}

  /** @name Topology exploration
   *  These methodes of the graph concern the topology exploration.
   */
  // /@{
  /**
   * Get all the neighbors of a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the neighbors
   */
  std::vector<std::shared_ptr<N> > getNeighbors(const std::shared_ptr<N>  node) const;
  std::vector<NodeIndex> getNeighbors(NodeIndex node) const;

  /**
   * Get all the edges of a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the edges
   */
  std::vector<std::shared_ptr<E> > getEdges(const std::shared_ptr<N>  node) const;
  std::vector<EdgeIndex> getEdges(NodeIndex node) const;

  /**
   * In an directed graph, get all the neighbors which
   * are leaving a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the outgoing neighbors
   */
  std::vector<std::shared_ptr<N> > getOutgoingNeighbors(const std::shared_ptr<N>  node) const;
  std::vector<NodeIndex> getOutgoingNeighbors(NodeIndex node) const;

  /**
   * In an directed graph, get all the edges which
   * are leaving a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the outgoing edges
   */
  std::vector<std::shared_ptr<E> > getOutgoingEdges(const std::shared_ptr<N>  node) const;
  std::vector<EdgeIndex> getOutgoingEdges(NodeIndex node) const;

  /**
   * In an directed graph, get all the neighbors which
   * are coming to a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the incoming neighbors
   */
  std::vector<std::shared_ptr<N> > getIncomingNeighbors(const std::shared_ptr<N>  node) const;
  std::vector<NodeIndex> getIncomingNeighbors(NodeIndex node) const;

/**
   * In an directed graph, get all the edges which
   * are coming to a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the incoming edges
   */
  std::vector<std::shared_ptr<E> > getIncomingEdges(const std::shared_ptr<N>  node) const;
  std::vector<EdgeIndex> getIncomingEdges(NodeIndex node) const;

/**
   * Get the leaves of a graph, ie, nodes with only one neighbor,
   * starting from a peculiar node.
   * @param node the starting node
   * @param maxDepth the maximum number of allowed depth, 0 means no max.
   * @return a vector containing the leaves
   */
  std::vector<std::shared_ptr<N> > getLeavesFromNode(std::shared_ptr<N>  node, unsigned int maxDepth) const;
  /**
   * Get all the leaves objects of a graph, ie, nodes with only one neighbor,
   * @return a vector containing the leaves
   */
  std::vector<std::shared_ptr<N> > getAllLeaves() const;

  /**
   * Get all the defined nodes of a graph,
   * @return a vector containing the nodesObjects
   */
  std::vector<std::shared_ptr<N> > getAllNodes() const;

  /**
   * Get all the defined branches of a graph,
   * @return a vector containing the branchesObjects
   */
  std::vector<std::shared_ptr<E> > getAllEdges() const;

  /**
   * @brief Is  the node a leaf?
   */
  
  bool isLeaf(const std::shared_ptr<N>  node) const;

  /**
   * Get nodes located at the extremities of an edge
   * @param edge an edge
   * @return a pair of the Nodes at each extremity of the edge
   *        example : N1--E1-->N2; getNodes(E1) will return (N1,N2);
   */
  std::pair<std::shared_ptr<N> , std::shared_ptr<N> > getNodes(std::shared_ptr<E>  edge) const;


  /**
   * Returns the Edge between two nodes nodeA -> nodeB
   * @param nodeA source node (if directed)
   * @param nodeB destination node (if directed)
   * @return the edge between these two nodes
   */
  std::shared_ptr<E>  getEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB) const;

  /**
   * Sets the Edge between two nodes nodeA -> nodeB
   * @param nodeA source node (if directed)
   * @param nodeB destination node (if directed)
   * @param edge the edge between these two nodes
   */
  void setEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB, std::shared_ptr<E>  edge);
  
  
  // /@}


  /** @name Function called by the subjectGraph
   *  These methodes are called by the subject graph to make this observer so fit the subject graph
   */
  // /@{

  /**
   * Delete unused object edges, since they have been deleted in the graph
   * @param edgesToDelete a vector of Edges to delete
   */
  void deletedEdgesUpdate(std::vector< EdgeGraphid >& edgesToDelete);

  /**
   * Delete unused object nodes, since they have been deleted in the graph
   * @param nodesToDelete a vector of N to delete
   */
  void deletedNodesUpdate(std::vector< NodeGraphid >& nodesToDelete);

  // /@}

  /** @name General Info
   *  General information about the graph
   */
  // /@{

  /**
   * Return the number of defined nodes, ie nodes that have a corresponding object
   * in this GraphObserver
   * @return the number of nodes
   */
//  size_t getNumberOfNodeIndexes() const;

  /**
   * Return the number of defined nodes, ie nodes that have a corresponding object
   * in this GraphObserver
   * @return the number of nodes
   */
  size_t getNumberOfNodes() const;

  /**
   * Return the number of defined leaves, ie leaves that have a corresponding object
   * in this GraphObserver
   * @return the number of leaves
   */
  size_t getNumberOfLeaves() const;

  /**
   * Return the number of neighbors of a node
   * @param nodeObject the concerned node
   * @return the number of neighbors
   */
  
  size_t getDegree(const std::shared_ptr<N>  node) const;
  
  // /@}

  template<typename N2, typename E2, typename G2> friend class SimpleAssociationGraphObserver;
  
};

//     class GraphObserverTools
//     {
//       static void outputToDot(SimpleAssociationGraphObserver<std::string,void>,std::ostream &out);
//     };
//

  /**
   * Constructor
   * @param subjectGraph the graph which is observed
   */
  
  template<class N, class E, class GraphImpl>
  SimpleAssociationGraphObserver<N, E, GraphImpl>::SimpleAssociationGraphObserver(std::shared_ptr<SimpleGraph> subjectGraph) :
    subjectGraph_(subjectGraph?subjectGraph:new GraphImpl()),
    graphidToN_(),
    graphidToE_(),
    NToGraphid_(),
    EToGraphid_(),
    indexToN_(),
    indexToE_(),
    NToIndex_(),
    EToIndex_()
  {
    getGraph()->registerObserver(this);
  }
    

template<class N, class E, class GraphImpl>
SimpleAssociationGraphObserver<N, E, GraphImpl>::SimpleAssociationGraphObserver(bool directed_p) :
  subjectGraph_(new GraphImpl(directed_p)),
  graphidToN_(),
  graphidToE_(),
  NToGraphid_(),
  EToGraphid_(),
  indexToN_(),
  indexToE_(),
  NToIndex_(),
  EToIndex_()
{
  getGraph()->registerObserver(this);
}

template<class N, class E, class GraphImpl>
SimpleAssociationGraphObserver<N, E, GraphImpl>::SimpleAssociationGraphObserver(SimpleAssociationGraphObserver<N, E, GraphImpl> const& graphObserver) :
  subjectGraph_(graphObserver.subjectGraph_),
  graphidToN_(graphObserver.graphidToN_.size()),
  graphidToE_(graphObserver.graphidToE_.size()),
  NToGraphid_(),
  EToGraphid_(),
  indexToN_(graphObserver.indexToN_.size()),
  indexToE_(graphObserver.indexToE_.size()),
  NToIndex_(),
  EToIndex_()
{
  for (typename std::map<std::shared_ptr<N>, NodeGraphid >::const_iterator itN=graphObserver.NToGraphid_.begin();
       itN != graphObserver.NToGraphid_.end(); itN++)
  {  

    std::shared_ptr<N> node(new N(*itN->first));
    
    NToGraphid_[node]=itN->second;
    graphidToN_[itN->second]=node;

    typename std::map<std::shared_ptr<N>, typename AssociationGraphObserver<N, E>::NodeIndex>::const_iterator indN=graphObserver.NToIndex_.find(itN->first);

    if (indN!=graphObserver.NToIndex_.end())
    {
      NToIndex_[node]=indN->second;
      indexToN_[indN->second]=node;
    }
  }

  for (typename std::map<std::shared_ptr<E>, EdgeGraphid >::const_iterator itE=graphObserver.EToGraphid_.begin();
       itE != graphObserver.EToGraphid_.end(); itE++)
  {  
    std::shared_ptr<E> edge(new E(*itE->first));

    EToGraphid_[edge]=itE->second;
    graphidToE_[itE->second]=edge;

    typename std::map<std::shared_ptr<E> , typename AssociationGraphObserver<N, E>::EdgeIndex>::const_iterator indE=graphObserver.EToIndex_.find(itE->first);
    
    if (indE!=graphObserver.EToIndex_.end())
    {    
      EToIndex_[edge]=indE->second;
      indexToE_[indE->second]=edge;
    }
  }

  getGraph()->registerObserver(this);
}

  template<class N, class E, class GraphImpl> template<class N2, class E2>
  SimpleAssociationGraphObserver<N, E, GraphImpl>::SimpleAssociationGraphObserver(SimpleAssociationGraphObserver<N2, E2, GraphImpl> const& graphObserver) :
    subjectGraph_(graphObserver.getGraph()),
    graphidToN_(graphObserver.graphidToN_.size()),
    graphidToE_(graphObserver.graphidToE_.size()),
    NToGraphid_(),
    EToGraphid_(),
    indexToN_(graphObserver.indexToN_.size()),
    indexToE_(graphObserver.indexToE_.size()),
    NToIndex_(),
    EToIndex_()
  {
    for (typename std::map<std::shared_ptr<N2>, NodeGraphid >::const_iterator itN=graphObserver.NToGraphid_.begin();
         itN != graphObserver.NToGraphid_.end(); itN++)
    {  

      std::shared_ptr<N> node(new N(*itN->first));
    
      NToGraphid_[node]=itN->second;
      graphidToN_[itN->second]=node;

      typename std::map<std::shared_ptr<N2>, typename AssociationGraphObserver<N2, E2>::NodeIndex>::const_iterator indN=graphObserver.NToIndex_.find(itN->first);

      if (indN!=graphObserver.NToIndex_.end())
      {
        NToIndex_[node]=indN->second;
        indexToN_[indN->second]=node;
      }
    }

    for (typename std::map<std::shared_ptr<E2>, EdgeGraphid >::const_iterator itE=graphObserver.EToGraphid_.begin();
         itE != graphObserver.EToGraphid_.end(); itE++)
    {  
      std::shared_ptr<E> edge(new E(*itE->first));

      EToGraphid_[edge]=itE->second;
      graphidToE_[itE->second]=edge;

      typename std::map<std::shared_ptr<E2> , typename AssociationGraphObserver<N2, E2>::EdgeIndex>::const_iterator indE=graphObserver.EToIndex_.find(itE->first);
    
      if (indE!=graphObserver.EToIndex_.end())
      {    
        EToIndex_[edge]=indE->second;
        indexToE_[indE->second]=edge;
      }
    }

    getGraph()->registerObserver(this);
  }


  
template<class N, class E, class GraphImpl>
SimpleAssociationGraphObserver<N, E, GraphImpl>::~SimpleAssociationGraphObserver()
{
  getGraph()->unregisterObserver(this);
}


template<class N, class E, class GraphImpl>
SimpleAssociationGraphObserver<N, E, GraphImpl>& SimpleAssociationGraphObserver<N, E, GraphImpl>::operator=(SimpleAssociationGraphObserver<N, E, GraphImpl> const& graphObserver)
{
  this->graphidToN_.resize(graphObserver.graphidToN_.size());
  this->graphidToE_.resize(graphObserver.graphidToE_.size());
  this->indexToN_.resize(graphObserver.indexToN_.size());
  this->indexToE_.resize(graphObserver.indexToE_.size());
  
  for (typename std::map<std::shared_ptr<N>, NodeGraphid >::const_iterator itN=graphObserver.NToGraphid_.begin();
       itN != graphObserver.NToGraphid_.end(); itN++)
  {  
    std::shared_ptr<N> node(new N(*itN->first));
    
    NToGraphid_[node]=itN->second;
    graphidToN_[itN->second]=node;

    typename std::map<std::shared_ptr<N>, typename AssociationGraphObserver<N, E>::NodeIndex>::const_iterator indN=graphObserver.NToIndex_.find(itN->first);

    if (indN!=graphObserver.NToIndex_.end())
    {
      NToIndex_[node]=indN->second;
      indexToN_[indN->second]=node;
    }
    
  }

  for (typename std::map<std::shared_ptr<E>, EdgeGraphid >::const_iterator itE=graphObserver.EToGraphid_.begin(); itE != graphObserver.EToGraphid_.end(); itE++)
  {  
    std::shared_ptr<E> edge(new E(*itE->first));
    
    EToGraphid_[edge]=itE->second;
    graphidToE_[itE->second]=edge;

    typename std::map<std::shared_ptr<E> , typename AssociationGraphObserver<N, E>::EdgeIndex>::const_iterator indE=graphObserver.EToIndex_.find(itE->first);
    
    if (indE!=graphObserver.EToIndex_.end())
    {
      EToIndex_[edge]=indE->second;
      indexToE_[indE->second]=edge;
    }
  }

  this->subjectGraph_ = graphObserver.getGraph();

  this->getGraph()->registerObserver(this);

  return *this;
}

  template<class N, class E, class GraphImpl>
  const std::shared_ptr<N>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodeFromGraphid(NodeGraphid node) const
  {
    return graphidToN_.at(node);
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<N>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodeFromGraphid(NodeGraphid node)
  {
    return graphidToN_.at(node);
  }

  template<class N, class E, class GraphImpl>
  const std::shared_ptr<E>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgeFromGraphid(EdgeGraphid edge) const
  {
    return graphidToE_.at(edge);
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<E>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgeFromGraphid(EdgeGraphid edge)
  {
    return graphidToE_.at(edge);
  }

  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<N> > SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodesFromGraphid(std::vector<NodeGraphid> nodes) const
  {
    std::vector<std::shared_ptr<N> > nodeObjects;
    for (typename std::vector<NodeGraphid>::iterator currNode = nodes.begin(); currNode != nodes.end(); currNode++)
    {
      if (*currNode > graphidToN_.size())
        continue;
      std::shared_ptr<N>  foundNodeObject = graphidToN_.at(*currNode);
      if (!foundNodeObject)
        continue;
      nodeObjects.push_back(foundNodeObject);
    }
    return nodeObjects;
  }

  template<class N, class E, class GraphImpl>
  std::vector<typename AssociationGraphObserver<N, E>::NodeIndex> SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodeIndexes(std::vector<std::shared_ptr<N> > nodes) const
  {
    std::vector<NodeIndex> nodeIndexes;
    for (typename std::vector<std::shared_ptr<N> >::iterator currNode = nodes.begin(); currNode != nodes.end(); currNode++)
    {
      nodeIndexes.push_back(getNodeIndex(*currNode));
    }
    return nodeIndexes;
  }

  template<class N, class E, class GraphImpl>
  std::vector<typename AssociationGraphObserver<N, E>::EdgeIndex> SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgeIndexes(std::vector<std::shared_ptr<E> > edges) const
  {
    std::vector<EdgeIndex> edgeIndexes;
    for (typename std::vector<std::shared_ptr<E> >::iterator currEdge = edges.begin(); currEdge != edges.end(); currEdge++)
    {
      edgeIndexes.push_back(getEdgeIndex(*currEdge));
    }
    return edgeIndexes;
  }
  
  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<E> > SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgesFromGraphid(std::vector<EdgeGraphid> edges) const
  {
    std::vector<std::shared_ptr<E> > edgeObjects;
    for (typename std::vector<EdgeGraphid>::iterator currEdge = edges.begin(); currEdge != edges.end(); currEdge++)
    {
      if (*currEdge > graphidToE_.size())
        continue;
      std::shared_ptr<E>  foundEdgeObject = graphidToE_.at(*currEdge);
      if (!foundEdgeObject)
        continue;
      edgeObjects.push_back(foundEdgeObject);
    }
    return edgeObjects;
  }

  template<class N, class E, class GraphImpl>
  void SimpleAssociationGraphObserver<N, E, GraphImpl>::createNode(std::shared_ptr<N>  nodeObject)
  {
    NodeGraphid newGraphNode = getGraph()->createNode();
    associateNode(nodeObject, newGraphNode);
  }

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::createNode(std::shared_ptr<N>  objectOriginNode, std::shared_ptr<N>  newNodeObject, std::shared_ptr<E>  newEdgeObject)
{
  createNode(newNodeObject);
  link(objectOriginNode, newNodeObject, newEdgeObject);
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::link(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB, std::shared_ptr<E>  edgeObject)
{
  // checking the nodes
  typename std::map<std::shared_ptr<N> , NodeGraphid>::iterator foundNodeA, foundNodeB;
  foundNodeA = NToGraphid_.find(nodeObjectA);
  foundNodeB = NToGraphid_.find(nodeObjectB);
  if (foundNodeA == NToGraphid_.end() || foundNodeB == NToGraphid_.end())
    throw Exception("One of the nodes is not in the graph observer.");

  // checking if the edge is not already in the GraphObserver
  if (edgeObject != 00 && EToGraphid_.find(edgeObject) != EToGraphid_.end())
    throw Exception("The given edge is already associated to a relation in the subjectGraph.");

//  std::cout << "Trying to link node " << foundNodeA->second << " -> " << foundNodeB->second << std::endl;
  EdgeGraphid newGraphEdge = getGraph()->link(foundNodeA->second, foundNodeB->second);

  if(edgeObject != 00)
  {
    if (graphidToE_.size() < newGraphEdge + 1)
      graphidToE_.resize(newGraphEdge + 1);
    graphidToE_.at(newGraphEdge) = edgeObject;
    EToGraphid_[edgeObject] = newGraphEdge;
  }
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::unlink(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB)
{
  // checking the nodes
  typename std::map<std::shared_ptr<N> , NodeGraphid>::iterator foundNodeA, foundNodeB;
  foundNodeA = NToGraphid_.find(nodeObjectA);
  foundNodeB = NToGraphid_.find(nodeObjectB);
  if (foundNodeA == NToGraphid_.end() || foundNodeB == NToGraphid_.end())
    throw Exception("One of the nodes is not in the graph observer.");

  getGraph()->unlink(foundNodeA->second, foundNodeB->second);
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::deletedEdgesUpdate(std::vector<EdgeGraphid>& edgesToDelete)
{
  for (typename std::vector<EdgeGraphid>::iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++)
  {
    if(graphidToE_.size() > *currEdge){
      std::shared_ptr<E>  edgeObject = graphidToE_.at(*currEdge);
      graphidToE_.at(*currEdge) = 00;

      EToGraphid_.erase(edgeObject);
    }
  }
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::deletedNodesUpdate(std::vector<NodeGraphid>& nodesToDelete)
{
  for (typename std::vector<EdgeGraphid>::iterator currNode = nodesToDelete.begin(); currNode != nodesToDelete.end(); currNode++)
  {
    if(graphidToN_.size() > *currNode){
      std::shared_ptr<N>  nodeObject = graphidToN_.at(*currNode);
      graphidToN_.at(*currNode) = 00;

      NToGraphid_.erase(nodeObject);
    }
  }
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::associateNode(std::shared_ptr<N>  nodeObject, NodeGraphid graphNode)
{
  // nodes vector must be the right size. Eg: to store a node with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if (graphidToN_.size() < graphNode + 1)
    graphidToN_.resize(graphNode + 1);

  // now storing the node
  graphidToN_.at(graphNode) = nodeObject;
  NToGraphid_[nodeObject] = graphNode;
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::associateEdge(std::shared_ptr<E>  edgeObject, EdgeGraphid graphEdge)
{
  // nodes vector must be the right size. Eg: to store an edge with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if (graphidToE_.size() < graphEdge + 1)
    graphidToE_.resize(graphEdge + 1);

  // now storing the edge
  graphidToE_.at(graphEdge) = edgeObject;
  EToGraphid_[edgeObject] = graphEdge;
}

template<class N, class E, class GraphImpl>
typename AssociationGraphObserver<N, E>::EdgeIndex SimpleAssociationGraphObserver<N, E, GraphImpl>::setEdgeIndex(const std::shared_ptr<E>  edgeObject_p, typename AssociationGraphObserver<N, E>::EdgeIndex index)
{
  // TODO: check if this object has already an index?

  // nodes vector must be the right size. Eg: to store an edge with
  // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if (index >= indexToE_.size())
    indexToE_.resize(index + 1);

  // now storing the edge
  indexToE_.at(index) = edgeObject_p;
  EToIndex_[edgeObject_p] = index;
  return index;
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::dissociateNode(std::shared_ptr<N>  nodeObject)
{
  typename std::map<std::shared_ptr<N> , NodeGraphid>::iterator nodeToForget = NToGraphid_.find(nodeObject);
  graphidToN_.at(nodeToForget->second) = 00;
  NToGraphid_.erase(nodeToForget);
}

template<class N, class E, class GraphImpl>
typename AssociationGraphObserver<N, E>::NodeIndex SimpleAssociationGraphObserver<N, E, GraphImpl>::setNodeIndex(const std::shared_ptr<N>  nodeObject_p, typename AssociationGraphObserver<N, E>::NodeIndex index)
{  
  // TODO: check if this object has already an index?

  // nodes vector must be the right size. Eg: to store a node with
  // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if (index >= indexToN_.size())
    indexToN_.resize(index + 1);

  // now storing the node
  indexToN_.at(index) = nodeObject_p;
  NToIndex_[nodeObject_p] = index;
  return index;
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::dissociateEdge(std::shared_ptr<E>  edgeObject)
{
  typename std::map<std::shared_ptr<E> , EdgeGraphid>::iterator edgeToForget = EToGraphid_.find(edgeObject);
  graphidToE_.at(edgeToForget->second) = 00;
  EToGraphid_.erase(edgeToForget);
}


template<class N, class E, class GraphImpl>
std::vector<std::shared_ptr<N> > SimpleAssociationGraphObserver<N, E, GraphImpl>::getAllLeaves() const
{
  std::vector<std::shared_ptr<N> > leavesToReturn;
  // fetching all the graph Leaves
  std::vector<NodeGraphid> graphLeaves = getGraph()->getAllLeaves();
  // testing if they are defined in this observer
  for (typename std::vector<NodeGraphid>::iterator currGraphLeave = graphLeaves.begin(); currGraphLeave != graphLeaves.end(); currGraphLeave++)
  {
    std::shared_ptr<N>  foundLeafObject = graphidToN_.at(*currGraphLeave);
    if (foundLeafObject != 00)
      leavesToReturn.push_back(foundLeafObject);
  }
  return leavesToReturn;
}

  template<class N, class E, class GraphImpl>
  bool SimpleAssociationGraphObserver<N, E, GraphImpl>::isLeaf(const std::shared_ptr<N>  node) const
  {
    return getGraph()->isLeaf(this->getNodeGraphid(node));
  }


template<class N, class E, class GraphImpl>
std::vector<std::shared_ptr<N> > SimpleAssociationGraphObserver<N, E, GraphImpl>::getAllNodes() const
{
  std::vector<std::shared_ptr<N> > nodesToReturn;
  for (typename std::vector<std::shared_ptr<N> >::const_iterator currNodeObject = graphidToN_.begin(); currNodeObject != graphidToN_.end(); currNodeObject++)
  {
    if (*currNodeObject != 00)
    {
      nodesToReturn.push_back(*currNodeObject);
    }
  }
  return nodesToReturn;
}

  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<E> > SimpleAssociationGraphObserver<N, E, GraphImpl>::getAllEdges() const
  {
    std::vector<std::shared_ptr<E> > branchesToReturn;
    for (typename std::vector<std::shared_ptr<E> >::const_iterator currBrObject = graphidToE_.begin(); currBrObject != graphidToE_.end(); currBrObject++)
    {
      if (*currBrObject != 00)
      {
        branchesToReturn.push_back(*currBrObject);
      }
    }
    return branchesToReturn;
  }


  template<class N, class E, class GraphImpl>
size_t SimpleAssociationGraphObserver<N, E, GraphImpl>::getNumberOfNodes() const
{
  return NToGraphid_.size();
}

template<class N, class E, class GraphImpl>
size_t SimpleAssociationGraphObserver<N, E, GraphImpl>::getNumberOfLeaves() const
{
  size_t nb=0;
  for (typename std::vector<std::shared_ptr<N> >::const_iterator currNodeObject = graphidToN_.begin(); currNodeObject != graphidToN_.end(); currNodeObject++)
  {
    if ((*currNodeObject != 00) && (isLeaf(*currNodeObject)))
      nb++;
  }
  
  return nb;
}

template<class N, class E, class GraphImpl>
size_t SimpleAssociationGraphObserver<N, E, GraphImpl>::getDegree(const std::shared_ptr<N>  nodeObject) const
{
  return getGraph()->getDegree(getNodeGraphid(nodeObject));
}

template<class N, class E, class GraphImpl>
const std::shared_ptr<GraphImpl> SimpleAssociationGraphObserver<N, E, GraphImpl>::getGraph() const
{
  return subjectGraph_;
}

  template<class N, class E, class GraphImpl>
  std::shared_ptr<GraphImpl> SimpleAssociationGraphObserver<N, E, GraphImpl>::getGraph() 
  {
    return subjectGraph_;
  }

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::deleteNode(std::shared_ptr<N>  nodeObject)
{
  // first deleting the node in the graph
  getGraph()->deleteNode(getNodeGraphid(nodeObject));
  // then forgetting
  dissociateNode(nodeObject);
}

template<class N, class E, class GraphImpl>
typename SimpleAssociationGraphObserver<N, E, GraphImpl>::NodeGraphid SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodeGraphid(const std::shared_ptr<N>  nodeObject) const
{
  typename std::map<std::shared_ptr<N> , NodeGraphid>::const_iterator found = NToGraphid_.find(nodeObject);
  if (found == NToGraphid_.end())
    throw Exception("Unexisting node object.");
  return found->second;
}

  template<class N, class E, class GraphImpl>
  bool SimpleAssociationGraphObserver<N, E, GraphImpl>::hasIndex(const std::shared_ptr<N> nodeObject) const
  {
    typename std::map<std::shared_ptr<N> , typename AssociationGraphObserver<N, E>::NodeIndex>::const_iterator found = NToIndex_.find(nodeObject);
    return  (found != NToIndex_.end());
  }

  
  template<class N, class E, class GraphImpl>
  bool SimpleAssociationGraphObserver<N, E, GraphImpl>::hasIndex(const std::shared_ptr<E> edgeObject) const
  {
    typename std::map<std::shared_ptr<E> , typename AssociationGraphObserver<N, E>::EdgeIndex>::const_iterator found = EToIndex_.find(edgeObject);
    return (found != EToIndex_.end());
  }
  
template<class N, class E, class GraphImpl>
typename AssociationGraphObserver<N, E>::NodeIndex SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodeIndex(const std::shared_ptr<N>  nodeObject) const
{
  typename std::map<std::shared_ptr<N> , typename AssociationGraphObserver<N, E>::NodeIndex>::const_iterator found = NToIndex_.find(nodeObject);
  if (found == NToIndex_.end())
    throw Exception("Node object with graph id " + TextTools::toString(getNodeGraphid(nodeObject)) + " has no index.");

  return found->second;
}

template<class N, class E, class GraphImpl>
typename SimpleAssociationGraphObserver<N, E, GraphImpl>::EdgeGraphid SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgeGraphid(const std::shared_ptr<E>  edgeObject) const
{
  typename std::map<std::shared_ptr<E> , EdgeGraphid>::const_iterator found = EToGraphid_.find(edgeObject);
  if (found == EToGraphid_.end())
    throw Exception("Unexisting edge object.");
  return found->second;
}

template<class N, class E, class GraphImpl>
typename AssociationGraphObserver<N, E>::EdgeIndex SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgeIndex(const std::shared_ptr<E>  edgeObject) const
{
  typename std::map<std::shared_ptr<E> , typename AssociationGraphObserver<N, E>::EdgeIndex>::const_iterator found = EToIndex_.find(edgeObject);
  if (found == EToIndex_.end())
    throw Exception("Edge object with graph id " + TextTools::toString(getEdgeGraphid(edgeObject)) + " has no index.");
  return found->second;
}

template<class N, class E, class GraphImpl>
std::vector< std::shared_ptr<N>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getNeighbors_(const std::shared_ptr<N>  nodeObject, neighborType type) const
{
  NodeGraphid node = getNodeGraphid(nodeObject);

  // PHASE 1: getting the right neighbors
  std::vector<NodeGraphid> neighbors;
  switch (type)
  {
  case OUTGOING:
    neighbors = getGraph()->getOutgoingNeighbors(node);
    break;
  case INCOMING:
    neighbors = getGraph()->getIncomingNeighbors(node);
    break;
  case BOTH:
    neighbors = getGraph()->getNeighbors(node);
  }
  return getNodesFromGraphid(neighbors);
}

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdges_(const std::shared_ptr<N>  nodeObject, neighborType type) const
  {
    NodeGraphid node = getNodeGraphid(nodeObject);

    // PHASE 1: getting the right neighbors
    std::vector<EdgeGraphid> edges;
    switch (type)
    {
    case OUTGOING:
      edges = getGraph()->getOutgoingEdges(node);
      break;
    case INCOMING:
      edges = getGraph()->getIncomingEdges(node);
      break;
    case BOTH:
      edges = getGraph()->getEdges(node);
    }
    return getEdgesFromGraphid(edges);
  }

template<class N, class E, class GraphImpl>
std::vector< std::shared_ptr<N>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getIncomingNeighbors(const std::shared_ptr<N>  node) const
{
  return getNeighbors_(node, INCOMING);
}

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getIncomingEdges(const std::shared_ptr<N>  node) const
  {
    return getEdges_(node, INCOMING);
  }

template<class N, class E, class GraphImpl>
std::vector<typename AssociationGraphObserver<N, E>::NodeIndex > SimpleAssociationGraphObserver<N, E, GraphImpl>::getIncomingNeighbors(NodeIndex node) const
{
  return getNodeIndexes(getIncomingNeighbors(getNode(node)));
}

  template<class N, class E, class GraphImpl>
  std::vector<typename AssociationGraphObserver<N, E>::EdgeIndex > SimpleAssociationGraphObserver<N, E, GraphImpl>::getIncomingEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getIncomingEdges(getNode(node)));
  }

template<class N, class E, class GraphImpl>
std::vector< std::shared_ptr<N>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getOutgoingNeighbors(const std::shared_ptr<N>  node) const
{
  return getNeighbors_(node, OUTGOING);
}

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getOutgoingEdges(const std::shared_ptr<N>  node) const
  {
    return getEdges_(node, OUTGOING);
  }

template<class N, class E, class GraphImpl>
std::vector< typename AssociationGraphObserver<N, E>::NodeIndex > SimpleAssociationGraphObserver<N, E, GraphImpl>::getOutgoingNeighbors(NodeIndex node) const
{
  return getNodeIndexes(getOutgoingNeighbors(getNode(node)));
}

  template<class N, class E, class GraphImpl>
  std::vector< typename AssociationGraphObserver<N, E>::EdgeIndex > SimpleAssociationGraphObserver<N, E, GraphImpl>::getOutgoingEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getOutgoingEdges(getNode(node)));
  }

template<class N, class E, class GraphImpl>
std::vector< std::shared_ptr<N>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getNeighbors(const std::shared_ptr<N>  node) const
{
  return getNeighbors_(node, BOTH);
}

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdges(const std::shared_ptr<N>  node) const
  {
    return getEdges_(node, BOTH);
  }

template<class N, class E, class GraphImpl>
std::vector< typename AssociationGraphObserver<N, E>::NodeIndex > SimpleAssociationGraphObserver<N, E, GraphImpl>::getNeighbors(NodeIndex node) const
{
  return getNodeIndexes(getNeighbors(getNode(node)));
}

  template<class N, class E, class GraphImpl>
  std::vector< typename AssociationGraphObserver<N, E>::EdgeIndex > SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getEdges(getNode(node)));
  }


template<class N, class E, class GraphImpl>
std::vector< std::shared_ptr<N>  > SimpleAssociationGraphObserver<N, E, GraphImpl>::getLeavesFromNode(std::shared_ptr<N>  node, unsigned int maxDepth) const
{
  return getNodesFromGraphid(getGraph()->getLeavesFromNode(getNodeGraphid(node)));
}

template<class N, class E, class GraphImpl>
std::shared_ptr<E>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB) const
{
  return getEdgeFromGraphid(getGraph()->getEdge(getNodeGraphid(nodeA), getNodeGraphid(nodeB)));
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::setEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB, std::shared_ptr<E>  edge)
{
  associateEdge(edge,getGraph()->getEdge(getNodeGraphid(nodeA), getNodeGraphid(nodeB)));
}

template<class N, class E, class GraphImpl>
std::shared_ptr<N>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getNode(typename AssociationGraphObserver<N, E>::NodeIndex node) const
{
  return indexToN_.at(node);
}

template<class N, class E, class GraphImpl>
std::shared_ptr<E>  SimpleAssociationGraphObserver<N, E, GraphImpl>::getEdge(typename AssociationGraphObserver<N, E>::EdgeIndex edge) const
{
  return indexToE_.at(edge);
}

template<class N, class E, class GraphImpl>
void SimpleAssociationGraphObserver<N, E, GraphImpl>::setRoot(const std::shared_ptr<N>  newRoot)
{
  return getGraph()->setRoot(getNodeGraphid(newRoot));
}

template<class N, class E, class GraphImpl>
std::pair<std::shared_ptr<N> , std::shared_ptr<N> > SimpleAssociationGraphObserver<N, E, GraphImpl>::getNodes(std::shared_ptr<E> edge) const
{
  std::pair<NodeGraphid, NodeGraphid> nodes = getGraph()->getNodes(getEdgeGraphid(edge));
  return std::pair<std::shared_ptr<N> , std::shared_ptr<N> >(getNodeFromGraphid(nodes.first), getNodeFromGraphid(nodes.second));
}
}

#endif

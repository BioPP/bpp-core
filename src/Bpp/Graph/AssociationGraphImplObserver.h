//
// File AssociationGraphImplObserver.h
// Created by: Thomas Bigot
// Last modification : vendredi 4 novembre 2016, à 10h 21
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide utilitary
classes. This file belongs to the Bio++ Project.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _ASSOCIATION_GRAPH_IMPL_OBSERVER_HPP_
#define _ASSOCIATION_GRAPH_IMPL_OBSERVER_HPP_

#include "AssociationGraphObserver.h"
#include "GlobalGraph.h"

#include "../Exceptions.h"
#include "../Clonable.h"

#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include <memory>
#include <type_traits>

namespace bpp
{
  template<class N, class E, class GraphImpl>
  class AssociationGraphImplObserver : public virtual AssociationGraphObserver<N, E>
  {
  public:
    typedef typename AssociationGraphObserver<N, E>::NodeIndex NodeIndex;
    typedef typename AssociationGraphObserver<N, E>::EdgeIndex EdgeIndex;

    typedef Graph::NodeId NodeGraphid;
    typedef Graph::EdgeId EdgeGraphid;

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
    AssociationGraphImplObserver(bool directed = false);

    /**
     * Constructor
     * @param subjectGraph the graph which is observed
     */
  
    AssociationGraphImplObserver(std::shared_ptr<GraphImpl> subjectGraph = 00);

    /**
     * Copy Constructor
     * @param graphObserver the graphObserver to be copied
     */

    template<class N2, class E2>
    AssociationGraphImplObserver(AssociationGraphImplObserver<N2, E2, GraphImpl> const& graphObserver);
  
    AssociationGraphImplObserver(AssociationGraphImplObserver<N, E, GraphImpl> const& graphObserver);

    /**
     * = Operator
     * @param graphObserver the graphObserver we want to copy the values
     */
  
    AssociationGraphImplObserver<N, E, GraphImpl>& operator=(bpp::AssociationGraphImplObserver<N, E, GraphImpl> const& graphObserver);

    /**
     * Destructor
     * @param graphObserver the graphObserver to be copied
     */

    ~AssociationGraphImplObserver();

    /**
     * clone function
     * @param graphObserver the graphObserver to be copied
     */
    AssociationGraphImplObserver<N, E, GraphImpl>* clone() const { return new AssociationGraphImplObserver<N, E, GraphImpl>(*this); }

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

  public:
    /**
     * Creates a link between two existing nodes.
     * If directed graph: nodeA -> nodeB.
     * @param nodeObjectA source node (or first node if undirected)
     * @param nodeObjectB target node (or second node if undirected)
     */
    void link(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB, std::shared_ptr<E>  edgeObject = 00);

    /**
     * Deletes the link between two existing nodes in the graph.
     */
    
    void unlink(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB);

  public:
    
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
     * @set the root (but no checking, prefer rootAt)
     */

    void setRoot(const std::shared_ptr<N> newRoot);
    

    /**
     * @return the root
     */

    std::shared_ptr<N> getRoot() const;
    
    NodeIndex getRootIndex() const;

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
    
    std::shared_ptr<N>  getNode(NodeIndex nodeIndex) const;


    /**
     * Return the associated Node index
     * @param edgeObject object which to return the node index
     * @return a node index
     */

    std::shared_ptr<E>  getEdge(EdgeIndex edgeIndex) const;
    // /@}

    /** @name Topology exploration
     *  These methodes of the graph concern the topology exploration.
     */
    // /@{

    /**
     * @name Iterators on Nodes
     *
     */

    template<class GraphIterator, bool is_const>
    class NodeIteratorClass :
      virtual public AssociationGraphObserver<N,E>::NodeIterator
    {
    private:
      NodesIteratorClass<GraphIterator, is_const> it_;
      const AssociationGraphImplObserver<N,E,GraphImpl>& agio_;
      
    public:

      NodeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N,E,GraphImpl>& agio) : it_(*agio.getGraph()), agio_(agio) { start();};

      NodeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N,E,GraphImpl>& agio) : it_(*agio.getGraph()), agio_(agio) { start();};

      NodeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N,E,GraphImpl>& agio, std::shared_ptr<N> n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)), agio_(agio) {start();};

      NodeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N,E,GraphImpl>& agio, std::shared_ptr<N> n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)), agio_(agio) {start();};

      ~NodeIteratorClass<GraphIterator, is_const>() {};
      
      void next() {it_.next(); while (!it_.end() && agio_.getNodeFromGraphid(*it_)==0) it_.next();}
      
      bool end() const {return it_.end();}
      
      void start() { it_.start(); while (!it_.end() && (agio_.getNodeFromGraphid(*it_)==0)) it_.next();}
      
      std::shared_ptr<N> operator*() {return agio_.getNodeFromGraphid(*it_);}
    };

    
    std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> allNodesIterator();

    std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> allNodesIterator() const;

    /*
     * @brief builds iterator on the neighbor nodes of a Node
     *
     */
    
    std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> outgoingNeighborNodesIterator(std::shared_ptr<N> node);

    std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> outgoingNeighborNodesIterator(std::shared_ptr<N> node) const;

    /*
     * @brief builds iterator on the neighbor nodes of a Node
     *
     */

    std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> incomingNeighborNodesIterator(std::shared_ptr<N> node);
    
    std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> incomingNeighborNodesIterator(std::shared_ptr<N> node) const;
    
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
     * starting from a peculiar node, with maxDepth recursion depths.
     *
     * @param node the starting node
     * @maxDepth the max recursion depth
     * @return a vector containing the leaves
     */
    
    std::vector<std::shared_ptr<N> > getLeavesFromNode(std::shared_ptr<N>  node, unsigned int maxDepth) const;

    /**
     * Get all the leaves objects of a graph, ie, nodes with only one neighbor,
     * @return a vector containing the leaves
     */

    std::vector<std::shared_ptr<N> > getAllLeaves() const;

    /**
     * Get all the inner nodes of a graph, ie, nodes with degree >= 2.
     * @return a vector containing the inner nodes.
     */    

    virtual std::vector<std::shared_ptr<N> > getAllInnerNodes() const;

    /**
     * Get all the defined nodes of a graph,
     * @return a vector containing the nodesObjects
     */

    std::vector<std::shared_ptr<N> > getAllNodes() const;
    std::vector<NodeIndex> getAllNodesIndexes() const;


    template<class GraphIterator, bool is_const>
    class EdgeIteratorClass :
      public AssociationGraphObserver<N,E>::EdgeIterator
    {
    private:
      EdgesIteratorClass<GraphIterator, is_const> it_;
      const AssociationGraphImplObserver<N,E,GraphImpl>& agio_;
      
    public:

      EdgeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N,E,GraphImpl>& agio) : it_(*agio.getGraph()), agio_(agio) { start();};
      
      EdgeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N,E,GraphImpl>& agio, std::shared_ptr<N> n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)), agio_(agio) { start();};

      EdgeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N,E,GraphImpl>& agio) : it_(*agio.getGraph()), agio_(agio) { start();};
      
      EdgeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N,E,GraphImpl>& agio, std::shared_ptr<N> n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)), agio_(agio) { start();};

      ~EdgeIteratorClass<GraphIterator, is_const>() {};
      
      void next() {it_.next(); while (!it_.end() && (agio_.getEdgeFromGraphid(*it_)==0)) it_.next();}
      
      bool end() const {return it_.end();}
      
      void start() { it_.start(); while (!it_.end() && (agio_.getEdgeFromGraphid(*it_)==0)) it_.next();}
      
      std::shared_ptr<E> operator*() {return agio_.getEdgeFromGraphid(*it_);}
    };

    
    std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> allEdgesIterator();

    std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> allEdgesIterator() const;

    /*
     * @brief builds iterator on the outgoing neighbor nodes of a Edge
     *
     */
    
    std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> outgoingEdgesIterator(std::shared_ptr<N> node);

    std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> outgoingEdgesIterator(std::shared_ptr<N> node) const;

    /*
     * @brief builds iterator on the incoming neighbor nodes of a Edge
     *
     */
     
    std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> incomingEdgesIterator(std::shared_ptr<N> node);
    
    std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> incomingEdgesIterator(std::shared_ptr<N> node) const;
    


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
    void deletedEdgesUpdate(const std::vector< unsigned int >& edgesToDelete);

    /**
     * Delete unused object nodes, since they have been deleted in the graph
     * @param nodesToDelete a vector of N to delete
     */
    void deletedNodesUpdate(const std::vector< unsigned int >& nodesToDelete);

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
     * Return the number of defined edges, ie edges that have a corresponding object
     * in this GraphObserver
     * @return the number of edges
     */
    
    size_t getNumberOfEdges() const;


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

    template<typename N2, typename E2, typename G2> friend class AssociationGraphImplObserver;
    
  };

  /***************/

  template<class N, class E>
  using AssociationGlobalGraphObserver = AssociationGraphImplObserver<N, E, GlobalGraph> ;

  /*******************/
  
  /**
   * Constructor
   * @param subjectGraph the graph which is observed
   */
  
  template<class N, class E, class GraphImpl>
  AssociationGraphImplObserver<N, E, GraphImpl>::AssociationGraphImplObserver(std::shared_ptr<GraphImpl> subjectGraph) :
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
  AssociationGraphImplObserver<N, E, GraphImpl>::AssociationGraphImplObserver(bool directed_p) :
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
  AssociationGraphImplObserver<N, E, GraphImpl>::AssociationGraphImplObserver(AssociationGraphImplObserver<N, E, GraphImpl> const& graphObserver) :
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
      std::shared_ptr<N> node(AssociationGraphObserver<N,E>::template copy<N,N>(*itN->first));
    
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
      std::shared_ptr<E> edge(AssociationGraphObserver<N,E>::template copy<E,E>(*itE->first));

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
  AssociationGraphImplObserver<N, E, GraphImpl>::AssociationGraphImplObserver(AssociationGraphImplObserver<N2, E2, GraphImpl> const& graphObserver) :
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
      std::shared_ptr<N> node(AssociationGraphObserver<N,E>::template copy<N2,N>(*itN->first));

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
      std::shared_ptr<E> edge(AssociationGraphObserver<N,E>::template copy<E2,E>(*itE->first));
      
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
  AssociationGraphImplObserver<N, E, GraphImpl>::~AssociationGraphImplObserver()
  {
    getGraph()->unregisterObserver(this);
  }


  template<class N, class E, class GraphImpl>
  AssociationGraphImplObserver<N, E, GraphImpl>& AssociationGraphImplObserver<N, E, GraphImpl>::operator=(AssociationGraphImplObserver<N, E, GraphImpl> const& graphObserver)
  {
    this->graphidToN_.resize(graphObserver.graphidToN_.size());
    this->graphidToE_.resize(graphObserver.graphidToE_.size());
    this->indexToN_.resize(graphObserver.indexToN_.size());
    this->indexToE_.resize(graphObserver.indexToE_.size());
  
    for (typename std::map<std::shared_ptr<N>, NodeGraphid >::const_iterator itN=graphObserver.NToGraphid_.begin();
         itN != graphObserver.NToGraphid_.end(); itN++)
    {
      std::shared_ptr<N> node(AssociationGraphObserver<N,E>::template copy<N,N>(*itN->first));
        
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
      std::shared_ptr<E> edge(AssociationGraphObserver<N,E>::template copy<E,E>(*itE->first));
    
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
  const std::shared_ptr<N>  AssociationGraphImplObserver<N, E, GraphImpl>::getNodeFromGraphid(NodeGraphid node) const
  {
    return graphidToN_.at(node);
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<N>  AssociationGraphImplObserver<N, E, GraphImpl>::getNodeFromGraphid(NodeGraphid node)
  {
    return graphidToN_.at(node);
  }

  template<class N, class E, class GraphImpl>
  const std::shared_ptr<E>  AssociationGraphImplObserver<N, E, GraphImpl>::getEdgeFromGraphid(EdgeGraphid edge) const
  {
    return graphidToE_.at(edge);
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<E>  AssociationGraphImplObserver<N, E, GraphImpl>::getEdgeFromGraphid(EdgeGraphid edge)
  {
    return graphidToE_.at(edge);
  }

  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<N> > AssociationGraphImplObserver<N, E, GraphImpl>::getNodesFromGraphid(std::vector<NodeGraphid> nodes) const
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
  std::vector<typename AssociationGraphObserver<N, E>::NodeIndex> AssociationGraphImplObserver<N, E, GraphImpl>::getNodeIndexes(std::vector<std::shared_ptr<N> > nodes) const
  {
    std::vector<NodeIndex> nodeIndexes;
    for (typename std::vector<std::shared_ptr<N> >::iterator currNode = nodes.begin(); currNode != nodes.end(); currNode++)
    {
      nodeIndexes.push_back(getNodeIndex(*currNode));
    }
    return nodeIndexes;
  }

  template<class N, class E, class GraphImpl>
  std::vector<typename AssociationGraphObserver<N, E>::EdgeIndex> AssociationGraphImplObserver<N, E, GraphImpl>::getEdgeIndexes(std::vector<std::shared_ptr<E> > edges) const
  {
    std::vector<EdgeIndex> edgeIndexes;
    for (typename std::vector<std::shared_ptr<E> >::iterator currEdge = edges.begin(); currEdge != edges.end(); currEdge++)
    {
      edgeIndexes.push_back(getEdgeIndex(*currEdge));
    }
    return edgeIndexes;
  }
  
  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<E> > AssociationGraphImplObserver<N, E, GraphImpl>::getEdgesFromGraphid(std::vector<EdgeGraphid> edges) const
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
  void AssociationGraphImplObserver<N, E, GraphImpl>::createNode(std::shared_ptr<N>  nodeObject)
  {
    if (NToGraphid_.find(nodeObject)!=NToGraphid_.end())
      throw Exception("AssociationGraphImplObserver::createNode : node already exists");
    
    unsigned int newGraphNode = getGraph()->createNode();
    associateNode(nodeObject, newGraphNode);
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::createNode(std::shared_ptr<N>  objectOriginNode, std::shared_ptr<N>  newNodeObject, std::shared_ptr<E>  newEdgeObject)
  {
    createNode(newNodeObject);
    link(objectOriginNode, newNodeObject, newEdgeObject);
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::link(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB, std::shared_ptr<E>  edgeObject)
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

    if (graphidToE_.size() < newGraphEdge + 1)
      graphidToE_.resize(newGraphEdge + 1);
    graphidToE_.at(newGraphEdge) = edgeObject;
    EToGraphid_[edgeObject] = newGraphEdge;
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::unlink(std::shared_ptr<N>  nodeObjectA, std::shared_ptr<N>  nodeObjectB)
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
  void AssociationGraphImplObserver<N, E, GraphImpl>::deletedEdgesUpdate(const std::vector<unsigned int>& edgesToDelete)
  {
    for (typename std::vector<EdgeGraphid>::const_iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++)
    {
      if(graphidToE_.size() > *currEdge){
        std::shared_ptr<E>  edgeObject = graphidToE_.at(*currEdge);
        graphidToE_.at(*currEdge) = 00;

        EToGraphid_.erase(edgeObject);
      }
    }
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::deletedNodesUpdate(const std::vector<unsigned int>& nodesToDelete)
  {
    for (typename std::vector<EdgeGraphid>::const_iterator currNode = nodesToDelete.begin(); currNode != nodesToDelete.end(); currNode++)
    {
      if(graphidToN_.size() > *currNode){
        std::shared_ptr<N>  nodeObject = graphidToN_.at(*currNode);
        graphidToN_.at(*currNode) = 00;

        NToGraphid_.erase(nodeObject);
      }
    }
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::associateNode(std::shared_ptr<N>  nodeObject, NodeGraphid graphNode)
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
  void AssociationGraphImplObserver<N, E, GraphImpl>::associateEdge(std::shared_ptr<E>  edgeObject, EdgeGraphid graphEdge)
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
  typename AssociationGraphObserver<N, E>::EdgeIndex AssociationGraphImplObserver<N, E, GraphImpl>::setEdgeIndex(const std::shared_ptr<E>  edgeObject_p, typename AssociationGraphObserver<N, E>::EdgeIndex index)
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
  void AssociationGraphImplObserver<N, E, GraphImpl>::dissociateNode(std::shared_ptr<N>  nodeObject)
  {
    typename std::map<std::shared_ptr<N> , NodeGraphid>::iterator nodeToForget = NToGraphid_.find(nodeObject);
    graphidToN_.at(nodeToForget->second) = 00;
    NToGraphid_.erase(nodeToForget);
  }

  template<class N, class E, class GraphImpl>
  typename AssociationGraphObserver<N, E>::NodeIndex AssociationGraphImplObserver<N, E, GraphImpl>::setNodeIndex(const std::shared_ptr<N>  nodeObject_p, typename AssociationGraphObserver<N, E>::NodeIndex index)
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
  void AssociationGraphImplObserver<N, E, GraphImpl>::dissociateEdge(std::shared_ptr<E>  edgeObject)
  {
    typename std::map<std::shared_ptr<E> , EdgeGraphid>::iterator edgeToForget = EToGraphid_.find(edgeObject);
    graphidToE_.at(edgeToForget->second) = 00;
    EToGraphid_.erase(edgeToForget);
  }


  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<N> > AssociationGraphImplObserver<N, E, GraphImpl>::getAllLeaves() const
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
  std::vector<std::shared_ptr<N> > AssociationGraphImplObserver<N, E, GraphImpl>::getAllInnerNodes() const
  {
    std::vector<std::shared_ptr<N> > nodesToReturn;
    // fetching all the graph Leaves
    std::vector<NodeGraphid> graphNodes = getGraph()->getAllInnerNodes();
    // testing if they are defined in this observer
    for (typename std::vector<NodeGraphid>::iterator currGraphNode = graphNodes.begin(); currGraphNode != graphNodes.end(); currGraphNode++)
    {
      std::shared_ptr<N>  foundNodeObject = graphidToN_.at(*currGraphNode);
      if (foundNodeObject != 00)
        nodesToReturn.push_back(foundNodeObject);
    }
    return nodesToReturn;
  }

  template<class N, class E, class GraphImpl>
  bool AssociationGraphImplObserver<N, E, GraphImpl>::isLeaf(const std::shared_ptr<N>  node) const
  {
    return getGraph()->isLeaf(this->getNodeGraphid(node));
  }


  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<N> > AssociationGraphImplObserver<N, E, GraphImpl>::getAllNodes() const
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
  std::vector<typename AssociationGraphObserver<N, E>::NodeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getAllNodesIndexes() const
  {
    std::vector<typename AssociationGraphObserver<N, E>::NodeIndex > indexesToReturn;
    for (typename std::vector<std::shared_ptr<N> >::const_iterator currNodeObject = graphidToN_.begin(); currNodeObject != graphidToN_.end(); currNodeObject++)
    {
      if (*currNodeObject != 00)
      {
        indexesToReturn.push_back(getNodeIndex(*currNodeObject));
      }
    }
    return indexesToReturn;
  }

  template<class N, class E, class GraphImpl>
  std::vector<std::shared_ptr<E> > AssociationGraphImplObserver<N, E, GraphImpl>::getAllEdges() const
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
  size_t AssociationGraphImplObserver<N, E, GraphImpl>::getNumberOfNodes() const
  {
    return NToGraphid_.size();
  }

  template<class N, class E, class GraphImpl>
  size_t AssociationGraphImplObserver<N, E, GraphImpl>::getNumberOfEdges() const
  {
    return EToGraphid_.size();
  }

  template<class N, class E, class GraphImpl>
  size_t AssociationGraphImplObserver<N, E, GraphImpl>::getNumberOfLeaves() const
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
  size_t AssociationGraphImplObserver<N, E, GraphImpl>::getDegree(const std::shared_ptr<N>  nodeObject) const
  {
    return getGraph()->getDegree(getNodeGraphid(nodeObject));
  }

  template<class N, class E, class GraphImpl>
  const std::shared_ptr<GraphImpl> AssociationGraphImplObserver<N, E, GraphImpl>::getGraph() const
  {
    return subjectGraph_;
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<GraphImpl> AssociationGraphImplObserver<N, E, GraphImpl>::getGraph() 
  {
    return subjectGraph_;
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::deleteNode(std::shared_ptr<N>  nodeObject)
  {
    // first deleting the node in the graph
    getGraph()->deleteNode(getNodeGraphid(nodeObject));
    // then forgetting
    dissociateNode(nodeObject);
  }

  template<class N, class E, class GraphImpl>
  typename AssociationGraphImplObserver<N, E, GraphImpl>::NodeGraphid AssociationGraphImplObserver<N, E, GraphImpl>::getNodeGraphid(const std::shared_ptr<N>  nodeObject) const
  {
    typename std::map<std::shared_ptr<N> , NodeGraphid>::const_iterator found = NToGraphid_.find(nodeObject);
    if (found == NToGraphid_.end())
      throw Exception("Unexisting node object.");
    return found->second;
  }

  template<class N, class E, class GraphImpl>
  bool AssociationGraphImplObserver<N, E, GraphImpl>::hasIndex(const std::shared_ptr<N> nodeObject) const
  {
    typename std::map<std::shared_ptr<N> , typename AssociationGraphObserver<N, E>::NodeIndex>::const_iterator found = NToIndex_.find(nodeObject);
    return  (found != NToIndex_.end());
  }

  
  template<class N, class E, class GraphImpl>
  bool AssociationGraphImplObserver<N, E, GraphImpl>::hasIndex(const std::shared_ptr<E> edgeObject) const
  {
    typename std::map<std::shared_ptr<E> , typename AssociationGraphObserver<N, E>::EdgeIndex>::const_iterator found = EToIndex_.find(edgeObject);
    return (found != EToIndex_.end());
  }
  
  template<class N, class E, class GraphImpl>
  typename AssociationGraphObserver<N, E>::NodeIndex AssociationGraphImplObserver<N, E, GraphImpl>::getNodeIndex(const std::shared_ptr<N>  nodeObject) const
  {
    typename std::map<std::shared_ptr<N> , typename AssociationGraphObserver<N, E>::NodeIndex>::const_iterator found = NToIndex_.find(nodeObject);
    if (found == NToIndex_.end())
      throw Exception("Node object with graph id " + TextTools::toString(getNodeGraphid(nodeObject)) + " has no index.");

    return found->second;
  }

  template<class N, class E, class GraphImpl>
  typename AssociationGraphImplObserver<N, E, GraphImpl>::EdgeGraphid AssociationGraphImplObserver<N, E, GraphImpl>::getEdgeGraphid(const std::shared_ptr<E>  edgeObject) const
  {
    typename std::map<std::shared_ptr<E> , EdgeGraphid>::const_iterator found = EToGraphid_.find(edgeObject);
    if (found == EToGraphid_.end())
      throw Exception("Unexisting edge object.");
    return found->second;
  }

  template<class N, class E, class GraphImpl>
  typename AssociationGraphObserver<N, E>::EdgeIndex AssociationGraphImplObserver<N, E, GraphImpl>::getEdgeIndex(const std::shared_ptr<E>  edgeObject) const
  {
    typename std::map<std::shared_ptr<E> , typename AssociationGraphObserver<N, E>::EdgeIndex>::const_iterator found = EToIndex_.find(edgeObject);
    if (found == EToIndex_.end())
      throw Exception("Edge object with graph id " + TextTools::toString(getEdgeGraphid(edgeObject)) + " has no index.");
    return found->second;
  }

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<N>  > AssociationGraphImplObserver<N, E, GraphImpl>::getNeighbors_(const std::shared_ptr<N>  nodeObject, neighborType type) const
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
  std::vector< std::shared_ptr<E>  > AssociationGraphImplObserver<N, E, GraphImpl>::getEdges_(const std::shared_ptr<N>  nodeObject, neighborType type) const
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
  std::vector< std::shared_ptr<N>  > AssociationGraphImplObserver<N, E, GraphImpl>::getIncomingNeighbors(const std::shared_ptr<N>  node) const
  {
    return getNeighbors_(node, INCOMING);
  }

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > AssociationGraphImplObserver<N, E, GraphImpl>::getIncomingEdges(const std::shared_ptr<N>  node) const
  {
    return getEdges_(node, INCOMING);
  }

  template<class N, class E, class GraphImpl>
  std::vector<typename AssociationGraphObserver<N, E>::NodeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getIncomingNeighbors(NodeIndex node) const
  {
    return getNodeIndexes(getIncomingNeighbors(getNode(node)));
  }

  template<class N, class E, class GraphImpl>
  std::vector<typename AssociationGraphObserver<N, E>::EdgeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getIncomingEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getIncomingEdges(getNode(node)));
  }

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<N>  > AssociationGraphImplObserver<N, E, GraphImpl>::getOutgoingNeighbors(const std::shared_ptr<N>  node) const
  {
    return getNeighbors_(node, OUTGOING);
  }

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > AssociationGraphImplObserver<N, E, GraphImpl>::getOutgoingEdges(const std::shared_ptr<N>  node) const
  {
    return getEdges_(node, OUTGOING);
  }

  template<class N, class E, class GraphImpl>
  std::vector< typename AssociationGraphObserver<N, E>::NodeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getOutgoingNeighbors(NodeIndex node) const
  {
    return getNodeIndexes(getOutgoingNeighbors(getNode(node)));
  }

  template<class N, class E, class GraphImpl>
  std::vector< typename AssociationGraphObserver<N, E>::EdgeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getOutgoingEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getOutgoingEdges(getNode(node)));
  }


  /*
   * @brief builds iterator on all Nodes of the graph
   *
   */

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> AssociationGraphImplObserver<N,E, GraphImpl>::allNodesIterator()
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, false> >(new AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, false>(*this));
  }

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator> AssociationGraphImplObserver<N,E, GraphImpl>::allNodesIterator() const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, true> >(new AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, true>(*this));
  }

  /*
   * @brief builds iterator on the neighbor nodes of a Node
   *
   */
    
  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator>
  AssociationGraphImplObserver<N,E, GraphImpl>::outgoingNeighborNodesIterator(std::shared_ptr<N> node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false> >(new AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>(*this,node));
  }

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator>
  AssociationGraphImplObserver<N,E,GraphImpl>::incomingNeighborNodesIterator(std::shared_ptr<N> node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, false> >(new AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, false>(*this,node));
  }
    
  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator>
  AssociationGraphImplObserver<N,E, GraphImpl>::outgoingNeighborNodesIterator(std::shared_ptr<N> node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true> >(new AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>(*this,node));
  }

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::NodeIterator>
  AssociationGraphImplObserver<N,E,GraphImpl>::incomingNeighborNodesIterator(std::shared_ptr<N> node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, true> >(new AssociationGraphImplObserver<N,E,GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, true>(*this,node));
  }
    

  

  /***********************************/
  
  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<N>  > AssociationGraphImplObserver<N, E, GraphImpl>::getNeighbors(const std::shared_ptr<N>  node) const
  {
    return getNeighbors_(node, BOTH);
  }

  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<E>  > AssociationGraphImplObserver<N, E, GraphImpl>::getEdges(const std::shared_ptr<N>  node) const
  {
    return getEdges_(node, BOTH);
  }

  template<class N, class E, class GraphImpl>
  std::vector< typename AssociationGraphObserver<N, E>::NodeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getNeighbors(NodeIndex node) const
  {
    return getNodeIndexes(getNeighbors(getNode(node)));
  }

  template<class N, class E, class GraphImpl>
  std::vector< typename AssociationGraphObserver<N, E>::EdgeIndex > AssociationGraphImplObserver<N, E, GraphImpl>::getEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getEdges(getNode(node)));
  }


  template<class N, class E, class GraphImpl>
  std::vector< std::shared_ptr<N>  > AssociationGraphImplObserver<N, E, GraphImpl>::getLeavesFromNode(std::shared_ptr<N>  node, unsigned int maxDepth) const
  {
    return getNodesFromGraphid(getGraph()->getLeavesFromNode(getNodeGraphid(node), maxDepth));
  }

  /*
   * @brief builds iterator on all Edges of the graph
   *
   */

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> AssociationGraphImplObserver<N,E,GraphImpl>::allEdgesIterator()
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, false> >(new AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, false>(*this));
  }
  
  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> AssociationGraphImplObserver<N,E,GraphImpl>::allEdgesIterator() const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, true> >(new AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, true>(*this));
  }
  

  /*
   * @brief builds iterator on the neighbor nodes of a Node
   *
   */
    
  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> AssociationGraphImplObserver<N,E,GraphImpl>::outgoingEdgesIterator(std::shared_ptr<N> node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false> >(new AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>(*this,node));
  }

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> AssociationGraphImplObserver<N,E,GraphImpl>::incomingEdgesIterator(std::shared_ptr<N> node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, false> >(new AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, false>(*this,node));
  }
    
  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> AssociationGraphImplObserver<N,E,GraphImpl>::outgoingEdgesIterator(std::shared_ptr<N> node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true> >(new AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>(*this,node));
  }

  template<class N, class E, class GraphImpl>
  std::unique_ptr<typename AssociationGraphObserver<N,E>::EdgeIterator> AssociationGraphImplObserver<N,E,GraphImpl>::incomingEdgesIterator(std::shared_ptr<N> node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, true> >(new AssociationGraphImplObserver<N,E,GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, true>(*this,node));
  }
    

  /**********************************************/
  
  template<class N, class E, class GraphImpl>
  std::shared_ptr<E>  AssociationGraphImplObserver<N, E, GraphImpl>::getEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB) const
  {
    return getEdgeFromGraphid(getGraph()->getEdge(getNodeGraphid(nodeA), getNodeGraphid(nodeB)));
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::setEdgeLinking(std::shared_ptr<N>  nodeA, std::shared_ptr<N>  nodeB, std::shared_ptr<E>  edge)
  {
    associateEdge(edge,getGraph()->getEdge(getNodeGraphid(nodeA), getNodeGraphid(nodeB)));
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<N>  AssociationGraphImplObserver<N, E, GraphImpl>::getNode(typename AssociationGraphObserver<N, E>::NodeIndex node) const
  {
    return indexToN_.at(node);
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<E>  AssociationGraphImplObserver<N, E, GraphImpl>::getEdge(typename AssociationGraphObserver<N, E>::EdgeIndex edge) const
  {
    return indexToE_.at(edge);
  }

  template<class N, class E, class GraphImpl>
  void AssociationGraphImplObserver<N, E, GraphImpl>::setRoot(const std::shared_ptr<N>  newRoot)
  {
    return getGraph()->setRoot(getNodeGraphid(newRoot));
  }

  template<class N, class E, class GraphImpl>
  std::shared_ptr<N>  AssociationGraphImplObserver<N, E, GraphImpl>::getRoot() const
  {
    return this->getNodeFromGraphid(this->getGraph()->getRoot());
  }

  template<class N, class E, class GraphImpl>
  typename AssociationGraphImplObserver<N, E, GraphImpl>::NodeIndex AssociationGraphImplObserver<N, E, GraphImpl>::getRootIndex() const
  {
    return this->getNodeIndex(this->getNodeFromGraphid(this->getGraph()->getRoot()));
  }


  template<class N, class E, class GraphImpl>
  std::pair<std::shared_ptr<N> , std::shared_ptr<N> > AssociationGraphImplObserver<N, E, GraphImpl>::getNodes(std::shared_ptr<E> edge) const
  {
    std::pair<NodeGraphid, NodeGraphid> nodes = getGraph()->getNodes(getEdgeGraphid(edge));
    return std::pair<std::shared_ptr<N> , std::shared_ptr<N> >(getNodeFromGraphid(nodes.first), getNodeFromGraphid(nodes.second));
  }
}

#endif

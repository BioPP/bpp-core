// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_GRAPH_ASSOCIATIONGRAPHIMPLOBSERVER_H
#define BPP_GRAPH_ASSOCIATIONGRAPHIMPLOBSERVER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <type_traits>
#include <vector>

#include "../Clonable.h"
#include "../Exceptions.h"
#include "../Text/TextTools.h"
#include "AssociationGraphObserver.h"
#include "GlobalGraph.h"

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

  using Eref = std::shared_ptr<E>;
  using Nref = std::shared_ptr<N>;

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
  std::vector<Nref > graphidToN_;

  /**
   * List of edges, stored at the same ID than the corresponding edges
   * in the observed graph.
   */
  std::vector<Eref > graphidToE_;

  /**
   * Can find a Node with the corresponding object.
   */
  std::map<Nref, NodeGraphid> NToGraphid_;

  /**
   * Can find an Edge with the corresponding object.
   */
  std::map<Eref, EdgeGraphid> EToGraphid_;

  /*
   * @}
   */

  /**
   * Registration with own Ids (named as index).
   *
   * @{
   */

  /**
   * List of nodes, stored at a given index.
   */
  std::vector<Nref > indexToN_;

  /**
   * List of edges, stored at a given index.
   */
  std::vector<Eref > indexToE_;

  /**
   * Can find a Node index with the corresponding object.
   */
  std::map<Nref, NodeIndex> NToIndex_;

  /**
   * Can find an Edge index with the corresponding object.
   */
  std::map<Eref, EdgeIndex> EToIndex_;

  /*
   * @}
   */

public:
  /**
   * Constructor
   * @param directed is the graph directed
   */

  AssociationGraphImplObserver(bool directed = false) :
    subjectGraph_(std::shared_ptr<GraphImpl>(new GraphImpl(directed))),
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

  /**
   * @brief Constructor
   *
   * @param subjectGraph the graph which is observed. If == 00, a
   * new Graph is built.
   *
   *
   */

  AssociationGraphImplObserver(std::shared_ptr<GraphImpl> subjectGraph = 00) :
    subjectGraph_(subjectGraph ? subjectGraph : std::shared_ptr<GraphImpl>(new GraphImpl(true))),
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

  /**
   * Copy Constructor
   * @param graphObserver the graphObserver to be copied
   */

  template<class N2, class E2>
  AssociationGraphImplObserver(AssociationGraphImplObserver<N2, E2, GraphImpl> const& graphObserver) :
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
    for (const auto& itN:graphObserver.NToGraphid_)
    {
      Nref node(AssociationGraphObserver<N, E>::template copy<N2, N>(*itN.first));

      NToGraphid_[node] = itN.second;
      graphidToN_[itN.second] = node;

      const auto& indN = graphObserver.NToIndex_.find(itN.first);

      if (indN != graphObserver.NToIndex_.end())
      {
        NToIndex_[node] = indN->second;
        indexToN_[indN->second] = node;
      }
    }

    for (const auto& itE:graphObserver.EToGraphid_)
    {
      Eref edge(AssociationGraphObserver<N, E>::template copy<E2, E>(*itE.first));

      EToGraphid_[edge] = itE.second;
      graphidToE_[itE.second] = edge;

      const auto& indE = graphObserver.EToIndex_.find(itE.first);

      if (indE != graphObserver.EToIndex_.end())
      {
        EToIndex_[edge] = indE->second;
        indexToE_[indE->second] = edge;
      }
    }

    getGraph()->registerObserver(this);
  }

  AssociationGraphImplObserver(AssociationGraphImplObserver<N, E, GraphImpl> const& graphObserver) :
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
    for (const auto& itN:graphObserver.NToGraphid_)
    {
      Nref node(AssociationGraphObserver<N, E>::template copy<N, N>(*itN.first));

      NToGraphid_[node] = itN.second;
      graphidToN_[itN.second] = node;

      const auto& indN = graphObserver.NToIndex_.find(itN.first);

      if (indN != graphObserver.NToIndex_.end())
      {
        NToIndex_[node] = indN->second;
        indexToN_[indN->second] = node;
      }
    }

    for (const auto& itE:graphObserver.EToGraphid_)
    {
      Eref edge(AssociationGraphObserver<N, E>::template copy<E, E>(*itE.first));

      EToGraphid_[edge] = itE.second;
      graphidToE_[itE.second] = edge;

      const auto& indE = graphObserver.EToIndex_.find(itE.first);

      if (indE != graphObserver.EToIndex_.end())
      {
        EToIndex_[edge] = indE->second;
        indexToE_[indE->second] = edge;
      }
    }

    getGraph()->registerObserver(this);
  }


  /**
   * = Operator
   * @param graphObserver the graphObserver we want to copy the values
   */
  AssociationGraphImplObserver<N, E, GraphImpl>& operator=(bpp::AssociationGraphImplObserver<N, E, GraphImpl> const& graphObserver)
  {
    this->graphidToN_.resize(graphObserver.graphidToN_.size());
    this->graphidToE_.resize(graphObserver.graphidToE_.size());
    this->indexToN_.resize(graphObserver.indexToN_.size());
    this->indexToE_.resize(graphObserver.indexToE_.size());

    for (const auto& itN:graphObserver.NToGraphid_)
    {
      Nref node(AssociationGraphObserver<N, E>::template copy<N, N>(*itN.first));

      NToGraphid_[node] = itN.second;
      graphidToN_[itN.second] = node;

      const auto& indN = graphObserver.NToIndex_.find(itN.first);

      if (indN != graphObserver.NToIndex_.end())
      {
        NToIndex_[node] = indN->second;
        indexToN_[indN->second] = node;
      }
    }

    for (const auto& itE:graphObserver.EToGraphid_)
    {
      Eref edge(AssociationGraphObserver<N, E>::template copy<E, E>(*itE.first));

      EToGraphid_[edge] = itE.second;
      graphidToE_[itE.second] = edge;

      const auto& indE = graphObserver.EToIndex_.find(itE.first);

      if (indE != graphObserver.EToIndex_.end())
      {
        EToIndex_[edge] = indE->second;
        indexToE_[indE->second] = edge;
      }
    }

    this->subjectGraph_ = graphObserver.getGraph();

    this->getGraph()->registerObserver(this);

    return *this;
  }


  /**
   * Destructor
   */
  ~AssociationGraphImplObserver()
  {
    getGraph()->unregisterObserver(this);
  }

  /**
   * clone function
   */
  AssociationGraphImplObserver<N, E, GraphImpl>* clone() const { return new AssociationGraphImplObserver<N, E, GraphImpl>(*this); }

  const std::shared_ptr<GraphImpl> getGraph() const
  {
    return subjectGraph_;
  }

  std::shared_ptr<GraphImpl> getGraph()
  {
    return subjectGraph_;
  }

public:
/*
 *@brief Check if has node/edge
 *
 */
  bool hasNode(Nref nodeObject) const
  {
    if (nodeObject == 0)
      return false;
    return NToGraphid_.find(nodeObject) != NToGraphid_.end();
  }

  bool hasEdge(Eref edgeObject) const
  {
    if (edgeObject == 0)
      return false;
    return EToGraphid_.find(edgeObject) != EToGraphid_.end();
  }

  /*
   * @brief Text description of objects
   */
  std::string nodeToString(const Nref nodeObject) const
  {
    auto mess = TextTools::toString(nodeObject);

    if (!hasNode(nodeObject))
      return mess;

    mess += ":Id=" + TextTools::toString(NToGraphid_.at(nodeObject));
    if (hasNodeIndex(nodeObject))
      mess += ":Index=" + TextTools::toString(getNodeIndex(nodeObject));

    return mess;
  }

  std::string edgeToString(const Eref edgeObject) const
  {
    auto mess = TextTools::toString(edgeObject);

    if (!hasEdge(edgeObject))
      return mess;

    mess += ":Id=" + TextTools::toString(EToGraphid_.at(edgeObject));
    if (hasEdgeIndex(edgeObject))
      mess += ":Index=" + TextTools::toString(getEdgeIndex(edgeObject));

    return mess;
  }

private:
  /**
   * defines a type of neighbors : incoming and/or outgoing
   */
  enum neighborType {INCOMING, OUTGOING, BOTH};

  /**
   * Get incoming / outgoing neighbors according to the enum type
   */
  std::vector<Nref > getNeighbors_(const Nref nodeObject, neighborType type) const
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

  /**
   * Get incoming / outgoing edges according to the enum type
   */
  std::vector<Eref > getEdges_(const Nref nodeObject, neighborType type) const
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

  /** @name Graph Relations Management
   *  Modificating the structure of the graph.
   */
  // /@{

public:
  /**
   * Creates an orphaned node from a NodeClass object.
   * @param nodeObject the N object associated to the node in the graph.
   *
   */
  void createNode(Nref nodeObject)
  {
    if (hasNode(nodeObject))
      throw Exception("AssociationGraphImplObserver::createNode : node already exists: " + nodeToString(nodeObject));

    unsigned int newGraphNode = getGraph()->createNode();
    associateNode(nodeObject, newGraphNode);
  }

  /**
   * Creates an node linked to an existing node. Order of parameters match
   * the link method.
   * @param objectOriginNode existing node. In a directed graph: origin -> newNode.
   * @param newNodeObject the N object associated to the node in the graph.
   * @param newEdgeObject the optional edge  between the nodes (default
   * = 00)
   */
  void createNode(Nref objectOriginNode, Nref newNodeObject, Eref newEdgeObject = 00)
  {
    createNode(newNodeObject);
    link(objectOriginNode, newNodeObject, newEdgeObject);
  }

public:
  /**
   * Creates a link between two existing nodes.
   * If directed graph: nodeA -> nodeB.
   * @param nodeObjectA source node (or first node if undirected)
   * @param nodeObjectB target node (or second node if undirected)
   * @param edgeObject the optional edge  between the nodes (default
   * = 00)
   */
  void link(Nref nodeObjectA, Nref nodeObjectB, Eref edgeObject = 00)
  {
    // checking the nodes
    if (!hasNode(nodeObjectA))
      throw Exception("First node is not in the graph observer: " + nodeToString(nodeObjectA));
    if (!hasNode(nodeObjectB))
      throw Exception("Second node is not in the graph observer:" + nodeToString(nodeObjectB));

    // checking if the edge is not already in the GraphObserver
    if (edgeObject != 00 && hasEdge(edgeObject))
    {
      auto nodes = getNodes(edgeObject);
      throw Exception("AssociationGraphImplObserver::link: The given edge is already associated to a relation in the subjectGraph: " + edgeToString(edgeObject) + ":" + nodeToString(nodes.first) + " -> " + nodeToString(nodes.second));
    }

    EdgeGraphid newGraphEdge = getGraph()->link(NToGraphid_.at(nodeObjectA), NToGraphid_.at(nodeObjectB));

    if (graphidToE_.size() < newGraphEdge + 1)
      graphidToE_.resize(newGraphEdge + 1);
    graphidToE_.at(newGraphEdge) = edgeObject;
    EToGraphid_[edgeObject] = newGraphEdge;
  }


  /**
   * Deletes the link between two existing nodes in the graph.
   */
  void unlink(Nref nodeObjectA, Nref nodeObjectB)
  {
    // checking the nodes
    if (!hasNode(nodeObjectA))
      throw Exception("First node is not in the graph observer: " + nodeToString(nodeObjectA));
    if (!hasNode(nodeObjectB))
      throw Exception("Second node is not in the graph observer:" + nodeToString(nodeObjectB));

    getGraph()->unlink(NToGraphid_.at(nodeObjectA), NToGraphid_.at(nodeObjectB));
  }

public:
  /**
   * Deletes a node
   * @param nodeObject node to be deleted. The N node object given in argument is not deleted.
   */
  void deleteNode(Nref nodeObject)
  {
    // first deleting the node in the graph
    getGraph()->deleteNode(getNodeGraphid(nodeObject));
    // then forgetting
    dissociateNode(nodeObject);
  }


  // /@}

  /** @name Object Association
   *  Associate or dissociate N and E objects to pre-existing Graph Nodes and Graph Edges
   */
  // /@{

  /**
   * Associate a N  object to a node in the graph.
   * @param nodeObject object to associate
   * @param graphNode existing node to be associated
   */
  void associateNode(Nref nodeObject, NodeGraphid graphNode)
  {
    if (hasNode(nodeObject))
      throw Exception("AssociationGraphImplObserver::associateNode : node already exists: " + nodeToString(nodeObject));

    // nodes vector must be the right size. Eg: to store a node with
    // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
    if (graphidToN_.size() < graphNode + 1)
      graphidToN_.resize(graphNode + 1);

    // now storing the node
    graphidToN_.at(graphNode) = nodeObject;
    NToGraphid_[nodeObject] = graphNode;
  }

  /**
   * Associate a E object to an edge in the graph.
   * @param edgeObject object to associate
   * @param graphEdge existing edge to be associated
   */
  void associateEdge(Eref edgeObject, EdgeGraphid graphEdge)
  {
    if (hasEdge(edgeObject))
      throw Exception("AssociationGraphImplObserver::associateEdge : edge already exists: " + edgeToString(edgeObject));

    // edges vector must be the right size. Eg: to store an edge with
    // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
    if (graphidToE_.size() < graphEdge + 1)
      graphidToE_.resize(graphEdge + 1);

    // now storing the edge
    graphidToE_.at(graphEdge) = edgeObject;
    EToGraphid_[edgeObject] = graphEdge;
  }

  /**
   * Dissociate a N or E object to a node or an edge in the graph.
   * @param nodeObject object to dissociate
   */
  void dissociateNode(Nref nodeObject)
  {
    typename std::map<Nref, NodeGraphid>::iterator nodeToForget = NToGraphid_.find(nodeObject);
    graphidToN_.at(nodeToForget->second) = 00;
    NToGraphid_.erase(nodeToForget);
  }


  void dissociateEdge(Eref edgeObject)
  {
    typename std::map<Eref, EdgeGraphid>::iterator edgeToForget = EToGraphid_.find(edgeObject);
    graphidToE_.at(edgeToForget->second) = 00;
    EToGraphid_.erase(edgeToForget);
  }


  /**
   * Return the associated Node ID
   * @param nodeObject object which to return the node ID
   * @return a node ID
   */
  NodeGraphid getNodeGraphid(const Nref nodeObject) const
  {
    typename std::map<Nref, NodeGraphid>::const_iterator found = NToGraphid_.find(nodeObject);
    if (found == NToGraphid_.end())
      throw Exception("Unexisting node object: " + TextTools::toString(nodeObject));
    return found->second;
  }


  /**
   * Return the associated Edge ID
   * @param edgeObject object which to return the node ID
   * @return a edge ID
   */
  EdgeGraphid getEdgeGraphid(const Eref edgeObject) const
  {
    typename std::map<Eref, EdgeGraphid>::const_iterator found = EToGraphid_.find(edgeObject);
    if (found == EToGraphid_.end())
      throw Exception("Unexisting edge object: " + TextTools::toString(edgeObject));
    return found->second;
  }


  /**
   * Transforms an (a list of) id(s) into an (a list of) object(s)
   */
  Nref getNodeFromGraphid(NodeGraphid node)
  {
    if (node >= graphidToN_.size())
      return 0;
    return graphidToN_.at(node);
  }

  const Nref getNodeFromGraphid(NodeGraphid node) const
  {
    if (node >= graphidToN_.size())
      return 0;
    return graphidToN_.at(node);
  }

  std::vector<Nref > getNodesFromGraphid(std::vector<NodeGraphid> nodes) const
  {
    std::vector<Nref > nodeObjects;
    for (typename std::vector<NodeGraphid>::iterator currNode = nodes.begin(); currNode != nodes.end(); currNode++)
    {
      if (*currNode > graphidToN_.size())
        continue;
      Nref foundNodeObject = graphidToN_.at(*currNode);
      if (!foundNodeObject)
        continue;
      nodeObjects.push_back(foundNodeObject);
    }
    return nodeObjects;
  }

  Eref getEdgeFromGraphid(EdgeGraphid edge)
  {
    if (edge >= graphidToE_.size())
      return 0;

    return graphidToE_.at(edge);
  }

  const Eref getEdgeFromGraphid(EdgeGraphid edge) const
  {
    if (edge >= graphidToE_.size())
      return 0;

    return graphidToE_.at(edge);
  }

  std::vector<Eref > getEdgesFromGraphid(std::vector<EdgeGraphid> edges) const
  {
    std::vector<Eref > edgeObjects;
    for (const auto& currEdge:edges)
    {
      if (currEdge > graphidToE_.size())
        continue;
      Eref foundEdgeObject = graphidToE_.at(currEdge);
      if (!foundEdgeObject)
        continue;
      edgeObjects.push_back(foundEdgeObject);
    }
    return edgeObjects;
  }


  /**
   * @brief set the root (but no checking, to be used at first construction)
   */
  void setRoot(const Nref newRoot)
  {
    return getGraph()->setRoot(getNodeGraphid(newRoot));
  }

  /**
   * @return the root
   */
  Nref getRoot() const
  {
    return this->getNodeFromGraphid(this->getGraph()->getRoot());
  }

  NodeIndex getRootIndex() const
  {
    return this->getNodeIndex(this->getNodeFromGraphid(this->getGraph()->getRoot()));
  }

  // /@}

  /** @name Object Indexation
   *  Get or set indexes to nodes and edges
   */
  // /@{

  /*
   * @brief Return if object has an index.
   *
   */
  bool hasNodeIndex(const Nref nodeObject) const
  {
    return NToIndex_.find(nodeObject) != NToIndex_.end();
  }

  bool hasEdgeIndex(const Eref edgeObject) const
  {
    return EToIndex_.find(edgeObject) != EToIndex_.end();
  }

  /**
   * Return the associated Node index
   * @param nodeObject object which to return the node index
   * @return a node index
   */
  NodeIndex getNodeIndex(const Nref nodeObject) const
  {
    const auto found = NToIndex_.find(nodeObject);
    if (found == NToIndex_.end())
      throw Exception("getNodeIndex: Node object has no index : " + nodeToString(nodeObject));

    return found->second;
  }

  std::vector<NodeIndex> getNodeIndexes(std::vector<Nref > nodes) const
  {
    std::vector<NodeIndex> nodeIndexes(nodes.size());

    std::transform(nodes.begin(), nodes.end(), nodeIndexes.begin(), [this](const Nref& nodeObject){return this->getNodeIndex(nodeObject);});

    return nodeIndexes;
  }

  /**
   * Return the associated Node index
   * @param edgeObject object which to return the node index
   * @return a node index
   */
  EdgeIndex getEdgeIndex(const Eref edgeObject) const
  {
    auto found = EToIndex_.find(edgeObject);
    if (found == EToIndex_.end())
      throw Exception("getEdgeIndex: Edge object has no index: " + edgeToString(edgeObject));
    return found->second;
  }

  std::vector<EdgeIndex> getEdgeIndexes(std::vector<Eref > edges) const
  {
    std::vector<EdgeIndex> edgeIndexes(edges.size());

    std::transform(edges.begin(), edges.end(), edgeIndexes.begin(), [this](const Eref& edgeObject){return this->getEdgeIndex(edgeObject);});

    return edgeIndexes;
  }

  /**
   * Set an index associated to a node
   * @param nodeObject object to which one want to set the index
   * @param index index to be given
   * @return the given index
   */
  NodeIndex setNodeIndex(const Nref nodeObject, NodeIndex index)
  {
    // nodes vector must be the right size. Eg: to store a node with
    // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
    if (hasNode(index))
      throw Exception("AssociationGraphImplObserver::setNodeIndex : index already exists: " + nodeToString(nodeObject));
    if (NToIndex_.find(nodeObject) != NToIndex_.end())
      throw Exception("AssociationGraphImplObserver::setNodeIndex : nodeObject has already an index: " + nodeToString(nodeObject));

    if (index >= indexToN_.size())
      indexToN_.resize(index + 1);

    // now storing the node
    indexToN_.at(index) = nodeObject;
    NToIndex_[nodeObject] = index;
    return index;
  }

  /**
   * Set an index associated to an edge
   * @param edgeObject object to which one want to set the index
   * @param index intex to be given, 0 to get the first free index
   * @return the given index
   */
  EdgeIndex setEdgeIndex(const Eref edgeObject, EdgeIndex index)
  {
    // nodes vector must be the right size. Eg: to store an edge with
    // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
    if (hasEdge(index))
      throw Exception("AssociationGraphImplObserver::setEdgeIndex : index already exists: " + edgeToString(edgeObject));
    if (EToIndex_.find(edgeObject) != EToIndex_.end())
      throw Exception("AssociationGraphImplObserver::setEdgeIndex : edgeObject has already an index: " + edgeToString(edgeObject));

    if (index >= indexToE_.size())
      indexToE_.resize(index + 1);

    // now storing the edge
    indexToE_.at(index) = edgeObject;
    EToIndex_[edgeObject] = index;
    return index;
  }

  /**
   * Set an new index associated to a node
   * @param nodeObject object to which one want to set the index
   * @return the given index
   */
  NodeIndex addNodeIndex(const Nref nodeObject)
  {
    // nodes vector must be the right size. Eg: to store a node with
    // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
    if (NToIndex_.find(nodeObject) != NToIndex_.end())
      throw Exception("AssociationGraphImplObserver::addNodeIndex : nodeObject has already an index: " + nodeToString(nodeObject));

    NodeIndex index = NodeIndex(indexToN_.size());
    for (auto i = 0; i < indexToN_.size(); i++)
    {
      if (!indexToN_.at(size_t(i)))
      {
        index = NodeIndex(i);
        break;
      }
    }

    if (index >= indexToN_.size())
      indexToN_.resize(index + 1);

    // now storing the node
    indexToN_.at(index) = nodeObject;
    NToIndex_[nodeObject] = index;
    return index;
  }

  /**
   * Add an index associated to an edge
   * @param edgeObject object to which one want to set the index
   * @return the given index
   */
  EdgeIndex addEdgeIndex(const Eref edgeObject)
  {
    // nodes vector must be the right size. Eg: to store an edge with
    // the index 3, the vector must be of size 4: {0,1,2,3} (size = 4)
    if (EToIndex_.find(edgeObject) != EToIndex_.end())
      throw Exception("AssociationGraphImplObserver::addEdgeIndex : edgeObject has already an index: " + edgeToString(edgeObject));

    EdgeIndex index = indexToE_.size();
    for (auto i = 0; i < indexToE_.size(); i++)
    {
      if (!indexToE_.at(i))
      {
        index = i;
        break;
      }
    }

    if (index >= indexToE_.size())
      indexToE_.resize(index + 1);

    // now storing the edge
    indexToE_.at(index) = edgeObject;
    EToIndex_[edgeObject] = index;
    return index;
  }

  /**
   * Return if has Node matching NodeIndex
   * @param node NodeIndex index
   */
  bool hasNode(NodeIndex node) const
  {
    return node < indexToN_.size() && indexToN_.at(node);
  }

  /**
   * Return if has Edge matching EdgeIndex
   * @param edge EdgeIndex index
   */
  bool hasEdge(EdgeIndex edge) const
  {
    return edge < indexToE_.size() && indexToE_.at(edge);
  }

  /**
   * Return the associated Node index
   * @param node object which to return the node index
   * @return a node index
   */
  Nref getNode(NodeIndex node) const
  {
    return indexToN_.at(node);
  }

  /**
   * Return the associated Node index
   * @param edge object which to return the node index
   * @return a node index
   */
  Eref getEdge(EdgeIndex edge) const
  {
    return indexToE_.at(edge);
  }

  // /@}

  /** @name Topology exploration
   *  These methodes of the graph concern the topology exploration.
   */
  // /@{

  /*
   * @brief Output graph in a dot format.
   *
   * Nodes are labeled as "n[Index_]Id", and edge are labeled as
   * "e[Index]_Id".
   *
   * Existing edge between nodes are plain, otherwise dotted.
   *
   * @param fname the name of the file where the DOT format will be output
   * @param name a string naming the graph
   *
   */
  void outputToDot(const std::string& fname, const std::string& name) const
  {
    std::ofstream out;
    out.open(fname);
    outputToDot(out, name);
    out.close();
  }

  void outputToDot(std::ostream& out, const std::string& name) const
  {
    out << (getGraph()->isDirected() ? "digraph" : "graph") << " " << name << " {\n   ";
    std::set<std::pair<Nref, Nref>> alreadyFigured;
    auto allNodes = getAllNodes();

    for (const auto& node: allNodes)
    {
      auto children = getOutgoingNeighbors(node);
      for (const auto& currChild : children)
      {
        if (alreadyFigured.find(std::pair<Nref, Nref>(node, currChild)) != alreadyFigured.end() || (getGraph()->isDirected() && alreadyFigured.find(std::pair<Nref, Nref>(currChild, node)) != alreadyFigured.end()))
          continue;

        alreadyFigured.insert(std::pair<Nref, Nref>(node, currChild));

        out << "n";
        if (hasNodeIndex(node))
          out << getNodeIndex(node) << "_";
        out << NToGraphid_.at(node);

        out << (getGraph()->isDirected() ? " -> " : " -- ");

        out << "n";
        if (hasNodeIndex(currChild))
          out << getNodeIndex(currChild) << "_";
        out << NToGraphid_.at(currChild);

        auto edge = getEdgeLinking(node, currChild);
        if (!edge)
          out << " [style = dotted]";
        else
        {
          out << " [label = e";
          if (hasEdgeIndex(edge))
            out << getEdgeIndex(edge) << "_";
          out << EToGraphid_.at(edge) << "]";
        }
        out << ";\n   ";
      }
    }
    out << "}";
  }

  /**
   * @name Iterators on Nodes
   *
   */

  template<class GraphIterator, bool is_const>
  class NodeIteratorClass :
    virtual public AssociationGraphObserver<N, E>::NodeIterator
  {
private:
    NodesIteratorClass<GraphIterator, is_const> it_;
    const AssociationGraphImplObserver<N, E, GraphImpl>& agio_;

public:
    NodeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N, E, GraphImpl>& agio) : it_(*agio.getGraph()),
      agio_(agio) { start(); }

    NodeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N, E, GraphImpl>& agio) : it_(*agio.getGraph()),
      agio_(agio) { start(); }

    NodeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N, E, GraphImpl>& agio, Nref n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)),
      agio_(agio) {start(); }

    NodeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N, E, GraphImpl>& agio, Nref n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)),
      agio_(agio) {start(); }

    ~NodeIteratorClass<GraphIterator, is_const>() {}

    void next() {it_.next(); while (!it_.end() && agio_.getNodeFromGraphid(*it_) == 0) it_.next(); }

    bool end() const {return it_.end(); }

    void start() { it_.start(); while (!it_.end() && (agio_.getNodeFromGraphid(*it_) == 0)) it_.next(); }

    Nref operator*() {return agio_.getNodeFromGraphid(*it_); }
  };


  std::unique_ptr<typename AssociationGraphObserver<N, E>::NodeIterator> allNodesIterator()
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, false>>(new AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, false>(*this));
  }

  std::unique_ptr<typename AssociationGraphObserver<N, E>::NodeIterator> allNodesIterator() const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, true>>(new AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::ALLGRAPHITER, true>(*this));
  }


  /*
   * @brief builds iterator on the neighbor nodes of a Node
   *
   */
  std::unique_ptr<typename AssociationGraphObserver<N, E>::NodeIterator> outgoingNeighborNodesIterator(Nref node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>>(new AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>(*this, node));
  }

  std::unique_ptr<typename AssociationGraphObserver<N, E>::NodeIterator> outgoingNeighborNodesIterator(Nref node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>>(new AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>(*this, node));
  }


  /*
   * @brief builds iterator on the neighbor nodes of a Node
   *
   */
  std::unique_ptr<typename AssociationGraphObserver<N, E>::NodeIterator> incomingNeighborNodesIterator(Nref node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, false>>(new AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, false>(*this, node));
  }

  std::unique_ptr<typename AssociationGraphObserver<N, E>::NodeIterator> incomingNeighborNodesIterator(Nref node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, true>>(new AssociationGraphImplObserver<N, E, GraphImpl>::NodeIteratorClass<Graph::INCOMINGNEIGHBORITER, true>(*this, node));
  }


  /**
   * Get all the neighbors of a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the neighbors
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<Nref > getNeighbors(const Nref node) const
  {
    return getNodesFromGraphid(getGraph()->getNeighbors(getNodeGraphid(node)));
  }

  /**
   * Get the index of all neighbors of a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the neighbors
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<NodeIndex> getNeighbors(NodeIndex node) const
  {
    return getNodeIndexes(getNeighbors(getNode(node)));
  }

  /**
   * Get all the edges of a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the edges
   *
   */
  std::vector<Eref > getEdges(const Nref node) const
  {
    return getEdgesFromGraphid(getGraph()->getEdges(getNodeGraphid(node)));
  }

  std::vector<EdgeIndex> getEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getEdges(getNode(node)));
  }


  /**
   * In an directed graph, get all the neighbors which
   * are leaving a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the outgoing neighbors
   */
  std::vector<Nref > getOutgoingNeighbors(const Nref node) const
  {
    return getNodesFromGraphid(getGraph()->getOutgoingNeighbors(getNodeGraphid(node)));
  }

  std::vector<NodeIndex> getOutgoingNeighbors(NodeIndex node) const
  {
    return getNodeIndexes(getOutgoingNeighbors(getNode(node)));
  }

  /**
   * In an directed graph, get all the edges which
   * are leaving a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the outgoing edges
   */
  std::vector<Eref > getOutgoingEdges(const Nref node) const
  {
    return getEdgesFromGraphid(getGraph()->getOutgoingEdges(getNodeGraphid(node)));
  }

  std::vector<EdgeIndex> getOutgoingEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getOutgoingEdges(getNode(node)));
  }

  /**
   * In an directed graph, get all the neighbors which
   * are coming to a node in the graph.
   * @param node the node one wants to get its neighbors
   * @return a vector containing the incoming neighbors
   */
  std::vector<Nref > getIncomingNeighbors(const Nref node) const
  {
    return getNodesFromGraphid(getGraph()->getIncomingNeighbors(getNodeGraphid(node)));
  }

  std::vector<NodeIndex> getIncomingNeighbors(NodeIndex node) const
  {
    return getNodeIndexes(getIncomingNeighbors(getNode(node)));
  }


  /**
   * In an directed graph, get all the edges which
   * are coming to a node in the graph.
   * @param node the node one wants to get its edges
   * @return a vector containing the incoming edges
   */
  std::vector<Eref > getIncomingEdges(const Nref node) const
  {
    return getEdgesFromGraphid(getGraph()->getIncomingEdges(getNodeGraphid(node)));
  }

  std::vector<EdgeIndex> getIncomingEdges(NodeIndex node) const
  {
    return getEdgeIndexes(getIncomingEdges(getNode(node)));
  }

  /**
   * Get the leaves of a graph, ie, nodes with only one neighbor,
   * starting from a peculiar node, with maxDepth recursion depths.
   *
   * @param node the starting node
   * @param maxDepth the max recursion depth
   * @return a vector containing the leaves
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<Nref > getLeavesFromNode(Nref node, unsigned int maxDepth) const
  {
    return getNodesFromGraphid(getGraph()->getLeavesFromNode(getNodeGraphid(node), maxDepth));
  }

  /**
   * Get all the leaves objects of a graph, ie, nodes with only one neighbor,
   * @return a vector containing the leaves
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<Nref > getAllLeaves() const
  {
    std::vector<Nref > leavesToReturn;
    // fetching all the graph Leaves
    std::vector<NodeGraphid> graphLeaves = getGraph()->getAllLeaves();
    // testing if they are defined in this observer
    for (typename std::vector<NodeGraphid>::iterator currGraphLeave = graphLeaves.begin(); currGraphLeave != graphLeaves.end(); currGraphLeave++)
    {
      Nref foundLeafObject = graphidToN_.at(*currGraphLeave);
      if (foundLeafObject != 00)
        leavesToReturn.push_back(foundLeafObject);
    }
    return leavesToReturn;
  }

  /**
   * Get index of the leaves objects of a graph, ie, nodes with only one neighbor,
   * @return a vector containing the leaves
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<NodeIndex> getAllLeavesIndexes() const
  {
    std::vector<NodeIndex> leavesToReturn;

    // fetching all the graph Leaves
    std::vector<NodeGraphid> graphLeaves = getGraph()->getAllLeaves();
    // testing if they are defined in this observer
    for (typename std::vector<NodeGraphid>::iterator currGraphLeave = graphLeaves.begin(); currGraphLeave != graphLeaves.end(); currGraphLeave++)
    {
      Nref foundLeafObject = graphidToN_.at(*currGraphLeave);
      if (foundLeafObject != 00)
        leavesToReturn.push_back(getNodeIndex(foundLeafObject));
    }
    return leavesToReturn;
  }

  /**
   * Get all the inner nodes of a graph, ie, nodes with degree >= 2.
   * @return a vector containing the inner nodes.
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<Nref > getAllInnerNodes() const
  {
    std::vector<Nref > nodesToReturn;
    // fetching all the graph Leaves
    std::vector<NodeGraphid> graphNodes = getGraph()->getAllInnerNodes();
    // testing if they are defined in this observer
    for (const auto& currGraphNode : graphNodes)
    {
      Nref foundNodeObject = graphidToN_.at(currGraphNode);
      if (foundNodeObject != 00)
        nodesToReturn.push_back(foundNodeObject);
    }
    return nodesToReturn;
  }

  /**
   * Get the indexes of all inner nodes of a graph, ie, nodes with degree >= 2.
   * @return a vector containing the inner nodes.
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<NodeIndex> getAllInnerNodesIndexes() const
  {
    std::vector<NodeIndex> nodesToReturn;
    // fetching all the graph Leaves
    std::vector<NodeGraphid> graphNodes = getGraph()->getAllInnerNodes();
    // testing if they are defined in this observer
    for (const auto& currGraphNode : graphNodes)
    {
      Nref foundNodeObject = graphidToN_.at(currGraphNode);
      if (foundNodeObject != 00)
        nodesToReturn.push_back(getNodeIndex(foundNodeObject));
    }
    return nodesToReturn;
  }

  /**
   * Get all the defined nodes of a graph,
   * @return a vector containing the nodesObjects
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<Nref > getAllNodes() const
  {
    std::vector<Nref > nodesToReturn;
    for (typename std::vector<Nref >::const_iterator currNodeObject = graphidToN_.begin(); currNodeObject != graphidToN_.end(); currNodeObject++)
    {
      if (*currNodeObject != 00)
      {
        nodesToReturn.push_back(*currNodeObject);
      }
    }
    return nodesToReturn;
  }

  /**
   * Get indexes of all defined nodes of a graph,
   * @return a vector containing the nodesObjects
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<NodeIndex> getAllNodesIndexes() const
  {
    std::vector<NodeIndex> indexesToReturn;
    for (const auto& currNodeObject : graphidToN_)
    {
      if (currNodeObject != 00)
      {
        indexesToReturn.push_back(getNodeIndex(currNodeObject));
      }
    }
    return indexesToReturn;
  }

  /**
   * Get indexes of all defined edges of a graph,
   * @return a vector containing the edgesObjects
   *
   * They are in the same order as the graphIds (and not necessary Indexes).
   */
  std::vector<EdgeIndex> getAllEdgesIndexes() const
  {
    std::vector<typename AssociationGraphObserver<N, E>::EdgeIndex > indexesToReturn;
    for (typename std::vector<Eref >::const_iterator currEdgeObject = graphidToE_.begin(); currEdgeObject != graphidToE_.end(); currEdgeObject++)
    {
      if (*currEdgeObject != 00)
      {
        indexesToReturn.push_back(getEdgeIndex(*currEdgeObject));
      }
    }
    return indexesToReturn;
  }


  template<class GraphIterator, bool is_const>
  class EdgeIteratorClass :
    public AssociationGraphObserver<N, E>::EdgeIterator
  {
private:
    EdgesIteratorClass<GraphIterator, is_const> it_;
    const AssociationGraphImplObserver<N, E, GraphImpl>& agio_;

public:
    EdgeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N, E, GraphImpl>& agio) : it_(*agio.getGraph()),
      agio_(agio) { start(); }

    EdgeIteratorClass<GraphIterator, is_const>(AssociationGraphImplObserver<N, E, GraphImpl>& agio, Nref n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)),
      agio_(agio) { start(); }

    EdgeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N, E, GraphImpl>& agio) : it_(*agio.getGraph()),
      agio_(agio) { start(); }

    EdgeIteratorClass<GraphIterator, is_const>(const AssociationGraphImplObserver<N, E, GraphImpl>& agio, Nref n) : it_(*agio.getGraph(), agio.getNodeGraphid(n)),
      agio_(agio) { start(); }

    ~EdgeIteratorClass<GraphIterator, is_const>() {}

    void next() {it_.next(); while (!it_.end() && (agio_.getEdgeFromGraphid(*it_) == 0)) it_.next(); }

    bool end() const {return it_.end(); }

    void start() { it_.start(); while (!it_.end() && (agio_.getEdgeFromGraphid(*it_) == 0)) it_.next(); }

    Eref operator*() {return agio_.getEdgeFromGraphid(*it_); }
  };


  std::unique_ptr<typename AssociationGraphObserver<N, E>::EdgeIterator> allEdgesIterator()
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, false>>(new AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, false>(*this));
  }

  std::unique_ptr<typename AssociationGraphObserver<N, E>::EdgeIterator> allEdgesIterator() const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, true>>(new AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::ALLGRAPHITER, true>(*this));
  }

  /*
   * @brief builds iterator on the outgoing neighbor nodes of a Edge
   *
   */
  std::unique_ptr<typename AssociationGraphObserver<N, E>::EdgeIterator> outgoingEdgesIterator(Nref node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>>(new AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>(*this, node));
  }

  std::unique_ptr<typename AssociationGraphObserver<N, E>::EdgeIterator> outgoingEdgesIterator(Nref node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>>(new AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>(*this, node));
  }

  /*
   * @brief builds iterator on the incoming neighbor nodes of a Edge
   *
   */
  std::unique_ptr<typename AssociationGraphObserver<N, E>::EdgeIterator> incomingEdgesIterator(Nref node)
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, false>>(new AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, false>(*this, node));
  }

  std::unique_ptr<typename AssociationGraphObserver<N, E>::EdgeIterator> incomingEdgesIterator(Nref node) const
  {
    return std::unique_ptr<AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, true>>(new AssociationGraphImplObserver<N, E, GraphImpl>::EdgeIteratorClass<Graph::INCOMINGNEIGHBORITER, true>(*this, node));
  }

  /**
   * Get all the defined branches of a graph,
   * @return a vector containing the branchesObjects
   */
  std::vector<Eref > getAllEdges() const
  {
    std::vector<Eref > branchesToReturn;
    for (typename std::vector<Eref >::const_iterator currBrObject = graphidToE_.begin(); currBrObject != graphidToE_.end(); currBrObject++)
    {
      if (*currBrObject != 00)
      {
        branchesToReturn.push_back(*currBrObject);
      }
    }
    return branchesToReturn;
  }

  /**
   * @brief Is  the node a leaf?
   */
  bool isLeaf(const Nref node) const
  {
    return getGraph()->isLeaf(this->getNodeGraphid(node));
  }

  bool isLeaf(const NodeIndex nodeid) const
  {
    return getGraph()->isLeaf(this->getNodeGraphid(getNode(nodeid)));
  }

  /**
   * Get nodes located at the extremities of an edge
   * @param edge an edge
   * @return a pair of the Nodes at each extremity of the edge
   *        example : N1--E1-->N2; getNodes(E1) will return (N1,N2);
   */
  std::pair<Nref, Nref> getNodes(Eref edge) const
  {
    std::pair<NodeGraphid, NodeGraphid> nodes = getGraph()->getNodes(getEdgeGraphid(edge));
    return std::pair<Nref, Nref >(getNodeFromGraphid(nodes.first), getNodeFromGraphid(nodes.second));
  }

  /**
   * Returns the Edge between two nodes nodeA -> nodeB
   * @param nodeA source node (if directed)
   * @param nodeB destination node (if directed)
   * @return the edge between these two nodes, or 0 if edge does not exist
   */
  Eref getEdgeLinking(Nref nodeA, Nref nodeB) const
  {
    return getEdgeFromGraphid(getGraph()->getEdge(getNodeGraphid(nodeA), getNodeGraphid(nodeB)));
  }

  /**
   * Sets the Edge between two nodes nodeA -> nodeB
   * @param nodeA source node (if directed)
   * @param nodeB destination node (if directed)
   * @param edge the edge between these two nodes
   */
  void setEdgeLinking(Nref nodeA, Nref nodeB, Eref edge)
  {
    associateEdge(edge, getGraph()->getEdge(getNodeGraphid(nodeA), getNodeGraphid(nodeB)));
  }

  // /@}


  /** @name Function called by the subjectGraph
   *  These methodes are called by the subject graph to make this observer so fit the subject graph
   */
  // /@{

  /**
   * Delete unused object edges, since they have been deleted in the graph
   * @param edgesToDelete a vector of Edges to delete
   */
  void deletedEdgesUpdate(const std::vector< unsigned int >& edgesToDelete)
  {
    for (typename std::vector<EdgeGraphid>::const_iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++)
    {
      if (graphidToE_.size() > *currEdge)
      {
        Eref edgeObject = graphidToE_.at(*currEdge);
        graphidToE_.at(*currEdge) = 00;

        EToGraphid_.erase(edgeObject);
      }
    }
  }

  /**
   * Delete unused object nodes, since they have been deleted in the graph
   * @param nodesToDelete a vector of N to delete
   */
  void deletedNodesUpdate(const std::vector< unsigned int >& nodesToDelete)
  {
    for (typename std::vector<EdgeGraphid>::const_iterator currNode = nodesToDelete.begin(); currNode != nodesToDelete.end(); currNode++)
    {
      if (graphidToN_.size() > *currNode)
      {
        Nref nodeObject = graphidToN_.at(*currNode);
        graphidToN_.at(*currNode) = 00;

        NToGraphid_.erase(nodeObject);
      }
    }
  }

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
  size_t getNumberOfNodes() const
  {
    return NToGraphid_.size();
  }

  /**
   * Return the number of defined edges, ie edges that have a corresponding object
   * in this GraphObserver
   * @return the number of edges
   */
  size_t getNumberOfEdges() const
  {
    return EToGraphid_.size();
  }


  /**
   * Return the number of defined leaves, ie leaves that have a corresponding object
   * in this GraphObserver
   * @return the number of leaves
   */
  size_t getNumberOfLeaves() const
  {
    size_t nb = 0;
    for (typename std::vector<Nref >::const_iterator currNodeObject = graphidToN_.begin(); currNodeObject != graphidToN_.end(); currNodeObject++)
    {
      if ((*currNodeObject != 00) && (isLeaf(*currNodeObject)))
        nb++;
    }

    return nb;
  }

  /**
   * Return the number of neighbors of a node
   * @param node the concerned node
   * @return the number of neighbors
   */
  size_t getDegree(const Nref node) const
  {
    return getGraph()->getDegree(getNodeGraphid(node));
  }

  // /@}

  template<typename N2, typename E2, typename G2> friend class AssociationGraphImplObserver;
};

/***************/

template<class N, class E>
using AssociationGlobalGraphObserver = AssociationGraphImplObserver<N, E, GlobalGraph>;
}
#endif // BPP_GRAPH_ASSOCIATIONGRAPHIMPLOBSERVER_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

#include "../Exceptions.h"
#include "../Text/TextTools.h"
#include "GlobalGraph.h"
#include "GraphObserver.h"

using namespace bpp;
using namespace std;

GlobalGraph::GlobalGraph(bool directed_p) :
  directed_(directed_p),
  observers_(set<GraphObserver*>()),
  highestNodeID_(0),
  highestEdgeID_(0),
  nodeStructure_(nodeStructureType()),
  edgeStructure_(edgeStructureType()),
  root_(0)
{}


GlobalGraph::GlobalGraph(const GlobalGraph& gg) :
  directed_(gg.directed_),
  observers_(gg.observers_),
  highestNodeID_(gg.highestNodeID_),
  highestEdgeID_(gg.highestEdgeID_),
  nodeStructure_(gg.nodeStructure_),
  edgeStructure_(gg.edgeStructure_),
  root_(gg.root_)
{}

GlobalGraph& GlobalGraph::operator=(const GlobalGraph& gg)
{
  directed_ = gg.directed_;
  observers_ = gg.observers_;
  highestNodeID_ = gg.highestNodeID_;
  highestEdgeID_ = gg.highestEdgeID_;
  nodeStructure_ = gg.nodeStructure_;
  edgeStructure_ = gg.edgeStructure_;
  root_ = gg.root_;

  return *this;
}


void GlobalGraph::nodeMustExist_(const GlobalGraph::Node& node, string name) const
{
  if (nodeStructure_.find(node) == nodeStructure_.end())
    throw Exception("This node must exist: " + TextTools::toString(node) + " as " + name + ".");
}

void GlobalGraph::edgeMustExist_(const GlobalGraph::Edge& edge, string name) const
{
  if (edgeStructure_.find(edge) == edgeStructure_.end())
    throw Exception("This edge must exist: " + TextTools::toString(edge) + " as " + name + ".");
}


GlobalGraph::Edge GlobalGraph::link(Graph::NodeId nodeA, Graph::NodeId nodeB)
{
  // which ID is available?
  GlobalGraph::Edge edgeID = highestEdgeID_++;

  // writing the new relation to the structure
  linkInNodeStructure_(nodeA, nodeB, edgeID);
  if (!directed_)
  {
    linkInNodeStructure_(nodeB, nodeA, edgeID);
  }
  linkInEdgeStructure_(nodeA, nodeB, edgeID);
  return edgeID;
}

void GlobalGraph::link(Graph::NodeId nodeA, Graph::NodeId nodeB, GlobalGraph::Edge edgeID)
{
  if (edgeStructure_.find(edgeID) != edgeStructure_.end())
    throw Exception("GlobalGraph::link : already existing edgeId " + TextTools::toString(edgeID));

  // writing the new relation to the structure
  linkInNodeStructure_(nodeA, nodeB, edgeID);
  if (!directed_)
  {
    linkInNodeStructure_(nodeB, nodeA, edgeID);
  }
  linkInEdgeStructure_(nodeA, nodeB, edgeID);
}

vector<GlobalGraph::Edge> GlobalGraph::unlink(Graph::NodeId nodeA, Graph::NodeId nodeB)
{
  // unlinking in the structure
  vector<GlobalGraph::Edge> deletedEdges; // what edges ID are affected by this unlinking
  deletedEdges.push_back(unlinkInNodeStructure_(nodeA, nodeB));

  for (auto& currEdgeToDelete : deletedEdges)
  {
    unlinkInEdgeStructure_(currEdgeToDelete);
  }

  // telling the observers
  notifyDeletedEdges(deletedEdges);

  return deletedEdges;
}

void GlobalGraph::switchNodes(Graph::NodeId nodeA, Graph::NodeId nodeB)
{
  Graph::NodeId father, son;

  nodeStructureType::iterator nodeARow = nodeStructure_.find(nodeA);
  nodeStructureType::iterator nodeBRow = nodeStructure_.find(nodeB);
  nodeStructureType::iterator nodeSonRow, nodeFatherRow;

  // Forwards
  map<GlobalGraph::Node, GlobalGraph::Edge>::iterator foundForwardRelation = nodeARow->second.first.find(nodeB);
  if (foundForwardRelation == nodeARow->second.first.end())
  {
    foundForwardRelation = nodeBRow->second.first.find(nodeA);
    if (foundForwardRelation == nodeBRow->second.first.end())
      throw Exception("GlobalGraph::exchangeNodes : no edge between nodes " + TextTools::toString(nodeA) + " and " + TextTools::toString(nodeB));
    father = nodeB;
    son = nodeA;
    nodeFatherRow = nodeBRow;
    nodeSonRow = nodeARow;
  }
  else
  {
    father = nodeA;
    son = nodeB;
    nodeFatherRow = nodeARow;
    nodeSonRow = nodeBRow;
  }

  // Edge
  GlobalGraph::Edge foundEdge = foundForwardRelation->second;

  // Backwards
  map<GlobalGraph::Node, GlobalGraph::Edge>::iterator foundBackwardsRelation = nodeSonRow->second.second.find(father);


  // Exchange
  nodeFatherRow->second.first.erase(foundForwardRelation);
  nodeSonRow->second.second.erase(foundBackwardsRelation);


  nodeSonRow->second.first[father] = foundEdge;

  nodeFatherRow->second.second[son] = foundEdge;


//    std::map<GlobalGraph::Node, std::pair<std::map<GlobalGraph::Node, GlobalGraph::Edge>, std::map<GlobalGraph::Node, GlobalGraph::Edge> > >::iterator ita = nodeStructure_.find(nodeA);

  edgeStructure_[foundEdge] = pair<Node, Node>(son, father);

  this->topologyHasChanged_();
}


void GlobalGraph::unlinkInEdgeStructure_(const GlobalGraph::Edge& edge)
{
  edgeStructureType::iterator foundEdge = edgeStructure_.find(edge);
  if (foundEdge == edgeStructure_.end())
    throw Exception("GlobalGraph::unlinkInEdgeStructure_ : no edge to erase " + TextTools::toString(edge));

  edgeStructure_.erase(foundEdge);
  this->topologyHasChanged_();
}

void GlobalGraph::linkInEdgeStructure_(const GlobalGraph::Node& nodeA, const GlobalGraph::Node& nodeB, const GlobalGraph::Edge& edge)
{
  edgeStructure_[edge] = pair<Node, Node>(nodeA, nodeB);
  this->topologyHasChanged_();
}


unsigned int GlobalGraph::unlinkInNodeStructure_(const GlobalGraph::Node& nodeA, const GlobalGraph::Node& nodeB)
{
  // Forward
  nodeStructureType::iterator nodeARow = nodeStructure_.find(nodeA);
  map<GlobalGraph::Node, GlobalGraph::Edge>::iterator foundForwardRelation = nodeARow->second.first.find(nodeB);
  if (foundForwardRelation == nodeARow->second.first.end())
    throw Exception("GlobalGraph::unlinkInNodeStructure_ : no edge to erase " + TextTools::toString(nodeA) + "->" + TextTools::toString(nodeB));

  GlobalGraph::Edge foundEdge = foundForwardRelation->second;
  nodeARow->second.first.erase(foundForwardRelation);

  // Backwards
  nodeStructureType::iterator nodeBRow = nodeStructure_.find(nodeB);
  map<GlobalGraph::Node, GlobalGraph::Edge>::iterator foundBackwardsRelation = nodeBRow->second.second.find(nodeA);
  if (foundBackwardsRelation == nodeBRow->second.first.end())
    throw Exception("GlobalGraph::unlinkInNodeStructure_ : no edge to erase " + TextTools::toString(nodeB) + "<-" + TextTools::toString(nodeA));

  nodeBRow->second.second.erase(foundBackwardsRelation);

  this->topologyHasChanged_();
  return foundEdge;
}

void GlobalGraph::linkInNodeStructure_(const GlobalGraph::Node& nodeA, const GlobalGraph::Node& nodeB, const GlobalGraph::Edge& edge)
{
  auto ita = nodeStructure_.find(nodeA);
  if (ita != nodeStructure_.end())
    ita->second.first.insert( pair<GlobalGraph::Node, GlobalGraph::Edge>(nodeB, edge));

  auto itb = nodeStructure_.find(nodeB);
  if (itb != nodeStructure_.end())
    nodeStructure_.find(nodeB)->second.second.insert( pair<GlobalGraph::Node, GlobalGraph::Edge>(nodeA, edge));

  this->topologyHasChanged_();
}

Graph::NodeId GlobalGraph::createNode()
{
  GlobalGraph::Node newNode = highestNodeID_++;
  nodeStructure_[newNode] = std::pair<std::map<GlobalGraph::Node, GlobalGraph::Edge>, std::map<GlobalGraph::Node, GlobalGraph::Edge>>();
  this->topologyHasChanged_();

  return newNode;
}

Graph::NodeId GlobalGraph::createNodeFromNode(Graph::NodeId origin)
{
  Graph::NodeId newNode = createNode();
  link(origin, newNode);
  this->topologyHasChanged_();
  return newNode;
}

Graph::NodeId GlobalGraph::createNodeOnEdge(Graph::EdgeId edge)
{
  // origin must be an existing edge
  edgeMustExist_(edge, "");

  Graph::NodeId newNode = createNode();

  // determining the nodes on the border of the edge
  pair<GlobalGraph::Node, GlobalGraph::Node> nodes = edgeStructure_[edge];
  GlobalGraph::Node nodeA = nodes.first;
  GlobalGraph::Node nodeB = nodes.second;

  unlink(nodeA, nodeB);
  link(nodeA, newNode);
  link(newNode, nodeB);
  this->topologyHasChanged_();
  return newNode;
}


Graph::NodeId GlobalGraph::createNodeFromEdge(Graph::NodeId origin)
{
  // origin must be an existing edge
  edgeMustExist_(origin, "origin edge");

  // splitting the edge
  Graph::NodeId anchor = createNodeOnEdge(origin);

  Graph::NodeId newNode = createNodeFromNode(anchor);
  this->topologyHasChanged_();
  return newNode;
}

/*********************************************/

void GlobalGraph::registerObserver(GraphObserver* observer)
{
  if (!observers_.insert(observer).second)
    throw (Exception("This GraphObserver was already an observer of this Graph"));
  ;
}

void GlobalGraph::unregisterObserver(GraphObserver* observer)
{
  if (!observers_.erase(observer))
    throw (Exception("This GraphObserver was not an observer of this Graph"));
}


/**********************************************/

std::vector<GlobalGraph::Node> GlobalGraph::getNeighbors_(const GlobalGraph::Node& node, bool outgoing) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw (Exception("The requested node is not in the structure."));
  const std::map<GlobalGraph::Node, GlobalGraph::Edge>& forOrBack = (outgoing ? foundNode->second.first : foundNode->second.second);
  vector<GlobalGraph::Node> result;
  for (auto& currNeighbor : forOrBack)
  {
    result.push_back(currNeighbor.first);
  }

  return result;
}

std::vector<GlobalGraph::Edge> GlobalGraph::getEdges_(const GlobalGraph::Node& node, bool outgoing) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw (Exception("The requested node is not in the structure."));
  const std::map<GlobalGraph::Node, GlobalGraph::Edge>& forOrBack = (outgoing ? foundNode->second.first : foundNode->second.second);

  vector<GlobalGraph::Edge> result;
  for (const auto& currNeighbor : forOrBack)
  {
    result.push_back(currNeighbor.second);
  }

  return result;
}

vector<Graph::NodeId> GlobalGraph::getIncomingNeighbors(Graph::NodeId node) const
{
  return getNeighbors_(node, false);
}

vector<Graph::EdgeId> GlobalGraph::getIncomingEdges(const Graph::NodeId node) const
{
  return getEdges_(node, false);
}

vector<Graph::NodeId> GlobalGraph::getOutgoingNeighbors(const Graph::NodeId node) const
{
  return getNeighbors_(node, true);
}

vector<Graph::EdgeId> GlobalGraph::getOutgoingEdges(const Graph::NodeId node) const
{
  return getEdges_(node, true);
}

std::unique_ptr<Graph::NodeIterator> GlobalGraph::allNodesIterator()
{
  return std::unique_ptr<Graph::NodeIterator>(new NodesIteratorClass<Graph::ALLGRAPHITER, false>(*this));
}

std::unique_ptr<Graph::NodeIterator> GlobalGraph::allNodesIterator() const
{
  return std::unique_ptr<Graph::NodeIterator>(new NodesIteratorClass<Graph::ALLGRAPHITER, true>(*this));
}


std::unique_ptr<Graph::NodeIterator> GlobalGraph::outgoingNeighborNodesIterator(Graph::NodeId node)
{
  return std::unique_ptr<Graph::NodeIterator>(new NodesIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>(*this, node));
}


std::unique_ptr<Graph::NodeIterator> GlobalGraph::incomingNeighborNodesIterator(Graph::NodeId node)
{
  return std::unique_ptr<Graph::NodeIterator>(new NodesIteratorClass<Graph::INCOMINGNEIGHBORITER, false>(*this, node));
}

std::unique_ptr<Graph::NodeIterator> GlobalGraph::outgoingNeighborNodesIterator(Graph::NodeId node) const
{
  return std::unique_ptr<Graph::NodeIterator>(new NodesIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>(*this, node));
}


std::unique_ptr<Graph::NodeIterator> GlobalGraph::incomingNeighborNodesIterator(Graph::NodeId node) const
{
  return std::unique_ptr<Graph::NodeIterator>(new NodesIteratorClass<Graph::INCOMINGNEIGHBORITER, true>(*this, node));
}


size_t GlobalGraph::getNumberOfNodes() const
{
  return nodeStructure_.size();
}


size_t GlobalGraph::getNumberOfEdges() const
{
  return edgeStructure_.size();
}


size_t GlobalGraph::getDegree(const Graph::NodeId node) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw Exception("GlobalGraph::getDegree : Node " + TextTools::toString(node) + " does not exist.");

  return isDirected() ? foundNode->second.first.size() + foundNode->second.second.size() : foundNode->second.first.size();
}


bool GlobalGraph::isLeaf(const Graph::NodeId node) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw Exception("GlobalGraph::isLeaf : Node " + TextTools::toString(node) + " does not exist.");

  const auto& assoc = foundNode->second;
  return (!isDirected() && (assoc.first.size() <= 1))
         || (isDirected() && (
               (assoc.first.size() + assoc.second.size() <= 1)
               || (assoc.first.size() == 1 &&  assoc.second.size() == 1 &&
                   assoc.first.begin()->first == assoc.second.begin()->first)));
}


size_t GlobalGraph::getNumberOfNeighbors(const Graph::NodeId node) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw (Exception("The requested node is not in the structure."));

  if (isDirected())
    return foundNode->second.first.size() + foundNode->second.second.size();
  else
    return foundNode->second.first.size();
}

size_t GlobalGraph::getNumberOfOutgoingNeighbors(const Graph::NodeId node) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw (Exception("The requested node is not in the structure."));
  return foundNode->second.first.size();
}

size_t GlobalGraph::getNumberOfIncomingNeighbors(const Graph::NodeId node) const
{
  nodeStructureType::const_iterator foundNode = nodeStructure_.find(node);
  if (foundNode == nodeStructure_.end())
    throw (Exception("The requested node is not in the structure."));
  return foundNode->second.second.size();
}

vector<Graph::NodeId> GlobalGraph::getNeighbors(const Graph::NodeId node) const
{
  vector<Graph::NodeId> result;
  vector<Graph::NodeId> neighborsToInsert;
  neighborsToInsert = getNeighbors_(node, false);
  result.insert(result.end(), neighborsToInsert.begin(), neighborsToInsert.end());
  neighborsToInsert = getNeighbors_(node, true);
  result.insert(result.end(), neighborsToInsert.begin(), neighborsToInsert.end());
  return result;
}

std::pair<Graph::NodeId, Graph::NodeId> GlobalGraph::getNodes(Graph::EdgeId edge) const
{
  edgeMustExist_(edge);
  edgeStructureType::const_iterator found = edgeStructure_.find(edge);
  return found->second;
}

Graph::NodeId GlobalGraph::getTop(Graph::EdgeId edge) const
{
  return std::get<0>(getNodes(edge));
}

Graph::NodeId GlobalGraph::getBottom(Graph::EdgeId edge) const
{
  return std::get<1>(getNodes(edge));
}

void GlobalGraph::deleteNode(Graph::NodeId node)
{
  // checking the node
  nodeMustExist_(node, "node to delete");
  isolate_(node);

  nodeStructureType::iterator found = nodeStructure_.find(node);
  if (found == nodeStructure_.end())
    throw Exception("GlobalGraph::deleteNode : no node to erase " + TextTools::toString(node));

  nodeStructure_.erase(found);

  this->topologyHasChanged_();
}

void GlobalGraph::isolate_(GlobalGraph::Node& node)
{
  vector<Graph::NodeId> oneighbors = getOutgoingNeighbors(node);
  for (auto& currNeighbor : oneighbors)
  {
    unlink(node, currNeighbor);
  }

  vector<Graph::NodeId> ineighbors = getIncomingNeighbors(node);
  for (auto& currNeighbor : ineighbors)
  {
    unlink(currNeighbor, node);
  }
}

vector<Graph::EdgeId> GlobalGraph::getAllEdges() const
{
  vector<Graph::EdgeId> listOfEdges;
  for (const auto& it : edgeStructure_)
  {
    listOfEdges.push_back(it.first);
  }

  return listOfEdges;
}

Graph::EdgeId GlobalGraph::getAnyEdge(Graph::NodeId nodeA, Graph::NodeId nodeB) const
{
  try
  {
    // trying in the given order A->B
    return getEdge(nodeA, nodeB);
  }
  catch (Exception& e)
  {
    // didn’t work, hence trying in the opposite order B->A
    return getEdge(nodeB, nodeA);
  }
}

vector<Graph::NodeId> GlobalGraph::getAllLeaves() const
{
  vector<Graph::NodeId> listOfLeaves;
  for (const auto& it : nodeStructure_)
  {
    if (this->isLeaf(it.first))
      listOfLeaves.push_back(it.first);
  }

  return listOfLeaves;
}

set<Graph::NodeId> GlobalGraph::getSetOfAllLeaves() const
{
  set<Graph::NodeId> listOfLeaves;
  for (const auto& it : nodeStructure_)
  {
    if (this->isLeaf(it.first))
      listOfLeaves.insert(it.first);
  }

  return listOfLeaves;
}

vector<Graph::NodeId> GlobalGraph::getAllNodes() const
{
  vector<Graph::NodeId> listOfNodes;
  for (const auto& it : nodeStructure_)
  {
    listOfNodes.push_back(it.first);
  }

  return listOfNodes;
}

vector<Graph::NodeId> GlobalGraph::getAllInnerNodes() const
{
  vector<Graph::NodeId> listOfInNodes;
  for (const auto& it : nodeStructure_)
  {
    if (this->getNumberOfOutgoingNeighbors(it.first) >= 1)
      listOfInNodes.push_back(it.first);
  }
  return listOfInNodes;
}


void GlobalGraph::fillListOfLeaves_(const GlobalGraph::Node& startingNode, vector<GlobalGraph::Node>& foundLeaves, const GlobalGraph::Node& originNode, unsigned int maxRecursions) const
{
  const vector<Graph::NodeId> neighbors = getNeighbors(startingNode);
  if (neighbors.size() > 1)
  {
    if (maxRecursions > 0)
      for (const auto& currNeighbor : neighbors)
      {
        if (currNeighbor != originNode)
          fillListOfLeaves_(currNeighbor, foundLeaves, startingNode, maxRecursions - 1);
      }
  }
  else
    foundLeaves.push_back(startingNode);
}


std::vector<Graph::NodeId> GlobalGraph::getLeavesFromNode(const Graph::NodeId node, unsigned int maxDepth) const
{
  vector<Graph::NodeId> listOfLeaves;
  fillListOfLeaves_(node, listOfLeaves, node, maxDepth);
  return listOfLeaves;
}

void GlobalGraph::nodeToDot_(const GlobalGraph::Node& node, ostream& out,  std::set<std::pair<Node, Node>>& alreadyFigured) const
{
  out << node;
  const std::map<Node, Edge>& children = nodeStructure_.at(node).first;
  bool flag(false);
  for (const auto& currChild : children)
  {
    if (alreadyFigured.find(pair<Node, Node>(node, currChild.first)) != alreadyFigured.end() || (!directed_ && alreadyFigured.find(pair<Node, Node>(currChild.first, node)) != alreadyFigured.end()))
      continue;
    alreadyFigured.insert(pair<Node, Node>(node, currChild.first));
    if (flag)
      out << node;
    out << (directed_ ? " -> " : " -- ");
    nodeToDot_(currChild.first, out, alreadyFigured);
    flag = true;
  }
  if (!flag)
    out << ";\n   ";
}

bool GlobalGraph::isTree() const
{
  set<GlobalGraph::Node> metNodes;
  bool nodesAreMetOnlyOnce = nodesAreMetOnlyOnce_(root_, metNodes, root_);

  if (!nodesAreMetOnlyOnce)
    return false;

  // now they have only been met at most once, they have to be met at least once
  for (const auto& currNode:nodeStructure_)
  {
    if (metNodes.find(currNode.first) == metNodes.end())
      return false;
  }

  return true;
}


bool GlobalGraph::nodesAreMetOnlyOnce_(const GlobalGraph::Node& node, set<GlobalGraph::Node>& metNodes, const GlobalGraph::Node& originNode) const
{
  // insert().second <=> not yet in the set
  if (!metNodes.insert(node).second)
    return false;

  vector<Graph::NodeId> neighbors = getOutgoingNeighbors(node);
  for (auto currNeighbor:neighbors)
  {
    if (currNeighbor == originNode)
      continue;
    if (!nodesAreMetOnlyOnce_(currNeighbor, metNodes, node))
      return false;
  }
  return true;
}

bool GlobalGraph::isDA() const
{
  GlobalGraph gg(*this);

  gg.observers_.clear();
  // Algo: remove recursively all nodes with no sons from graph

  std::vector<Graph::NodeId> vL;

  std::unique_ptr<Graph::NodeIterator> it = gg.allNodesIterator();
  for ( ; !it->end(); it->next())
  {
    if (gg.getNumberOfOutgoingNeighbors(**it) == 0)
      vL.push_back(**it);
  }

  while (vL.size() != 0)
  {
    for (auto& it2 : vL)
    {
      gg.deleteNode(it2);
    }

    if (gg.getNumberOfNodes() == 0)
      return true;

    vL.clear();

    it = gg.allNodesIterator();
    for ( ; !it->end(); it->next())
    {
      if (gg.getNumberOfOutgoingNeighbors(**it) == 0)
        vL.push_back(**it);
    }
  }

  return false;
}


void GlobalGraph::orientate()
{
  if (!isDirected())
    makeDirected();

  GlobalGraph gg(*this);
  gg.observers_.clear();

  // Algo: remove recursively all nodes from graph, starting with
  // root_

  Graph::NodeId node = root_;
  std::set<Graph::NodeId> nextNodes;
  nextNodes.insert(node);

  while (gg.getNumberOfNodes() != 0)
  {
    // look for the next node to be treated
    Graph::NodeId nbgg = 0;

    // first node with one neighbor (ie no choice on orientation)

    std::set<Graph::NodeId>::iterator it = nextNodes.begin();
    for ( ; it != nextNodes.end(); it++)
    {
      if (gg.getNumberOfNeighbors(*it) <= 1)
        break;
    }

    // if none, look for node wih minimum number of fathers
    if (it == nextNodes.end())
    {
      size_t nbF = numeric_limits<size_t>::infinity();
      it = nextNodes.begin();

      for ( ; it != nextNodes.end(); it++)
      {
        size_t nbFi = gg.getNumberOfIncomingNeighbors(*it);
        if (nbF == 0)
        {
          nbgg = *it;
          break;
        }
        else
        {
          if (nbFi < nbF)
          {
            nbgg = *it;
            nbF = nbFi;
          }
        }
      }
    }
    else
      nbgg = *it;

    // next orient edges from this node and catch neighbors
    std::vector<Graph::NodeId> vL = gg.getIncomingNeighbors(nbgg);
    for (auto& it2:vL)
    {
      switchNodes(nbgg, it2);
      nextNodes.insert(it2);
    }

    vL = gg.getOutgoingNeighbors(nbgg);
    for (auto&  it2:vL)
    {
      nextNodes.insert(it2);
    }

    gg.deleteNode(nbgg);
    nextNodes.erase(nbgg);
  }
}

void GlobalGraph::setRoot(Graph::NodeId newRoot)
{
  nodeMustExist_(newRoot, "new root");
  root_ = newRoot;
}

Graph::NodeId GlobalGraph::getRoot() const
{
  return root_;
}

bool GlobalGraph::isDirected() const
{
  return directed_;
}

void GlobalGraph::makeDirected()
{
  if (directed_)
    return;
  // save and clean the undirectedStructure
  nodeStructureType undirectedStructure = nodeStructure_;
  for (auto& it : nodeStructure_)
  {
    it.second = std::pair<std::map<Node, Edge>, std::map<Node, Edge>>();
  }

  // copy each relation once, without the reciprocal link
  // (first met, first kept)
  // eg: A - B in undirected is represented as A->B and B->A
  //     in directed, becomes A->B only
  std::set<pair<Node, Node>> alreadyConvertedRelations;
  for (auto& currNodeRow : undirectedStructure)
  {
    Node nodeA = currNodeRow.first;

    for (auto& currRelation : currNodeRow.second.first)
    {
      Node nodeB = currRelation.first;
      Edge edge = currRelation.second;
      if (alreadyConvertedRelations.insert(pair<Node, Node>(min(nodeA, nodeB), max(nodeA, nodeB))).second)
        linkInNodeStructure_(nodeA, nodeB, edge);
    }
  }
  directed_ = true;
  this->topologyHasChanged_();
}

void GlobalGraph::makeUndirected()
{
  if (!directed_)
    return;
  if (containsReciprocalRelations())
    throw Exception("Cannot make an undirected graph from a directed one containing reciprocal relations.");
  // save and clean the undirectedStructure
  nodeStructureType directedStructure = nodeStructure_;
  for (auto& it : nodeStructure_)
  {
    it.second = std::pair<std::map<Node, Edge>, std::map<Node, Edge>>();
  }

  // copy each relation twice, making the reciprocal link
  // eg: A - B in directed is represented as A->B
  //     in undirected, becomes A->B and B->A
  for (auto& currNodeRow : directedStructure)
  {
    Node nodeA = currNodeRow.first;
    for (auto currRelation : currNodeRow.second.first)
    {
      Node nodeB = currRelation.first;
      Edge edge = currRelation.second;
      linkInNodeStructure_(nodeA, nodeB, edge);
      linkInNodeStructure_(nodeB, nodeA, edge);
    }
  }
  directed_ = false;
  this->topologyHasChanged_();
}

bool GlobalGraph::containsReciprocalRelations() const
{
  if (!directed_)
    throw Exception("Cannot state reciprocal link in an undirected graph.");
  std::set<pair<Node, Node>> alreadyMetRelations;
  for (const auto& currNodeRow : nodeStructure_)
  {
    Node nodeA = currNodeRow.first;
    for (const auto& currRelation : currNodeRow.second.first)
    {
      Node nodeB = currRelation.first;
      if (!alreadyMetRelations.insert(pair<Node, Node>(min(nodeA, nodeB), max(nodeA, nodeB))).second)
        return true;
    }
  }
  return false;
}

std::unique_ptr<Graph::EdgeIterator> GlobalGraph::allEdgesIterator()
{
  return std::unique_ptr<Graph::EdgeIterator>(new EdgesIteratorClass<Graph::ALLGRAPHITER, false>(*this));
}

std::unique_ptr<Graph::EdgeIterator> GlobalGraph::outgoingEdgesIterator(Graph::NodeId node)
{
  return std::unique_ptr<Graph::EdgeIterator>(new EdgesIteratorClass<Graph::OUTGOINGNEIGHBORITER, false>(*this, node));
}

std::unique_ptr<Graph::EdgeIterator> GlobalGraph::incomingEdgesIterator(Graph::NodeId node)
{
  return std::unique_ptr<Graph::EdgeIterator>(new EdgesIteratorClass<Graph::INCOMINGNEIGHBORITER, false>(*this, node));
}

std::unique_ptr<Graph::EdgeIterator> GlobalGraph::allEdgesIterator() const
{
  return std::unique_ptr<Graph::EdgeIterator>(new EdgesIteratorClass<Graph::ALLGRAPHITER, true>(*this));
}

std::unique_ptr<Graph::EdgeIterator> GlobalGraph::outgoingEdgesIterator(Graph::NodeId node) const
{
  return std::unique_ptr<Graph::EdgeIterator>(new EdgesIteratorClass<Graph::OUTGOINGNEIGHBORITER, true>(*this, node));
}

std::unique_ptr<Graph::EdgeIterator> GlobalGraph::incomingEdgesIterator(Graph::NodeId node) const
{
  return std::unique_ptr<Graph::EdgeIterator>(new EdgesIteratorClass<Graph::INCOMINGNEIGHBORITER, true>(*this, node));
}

Graph::EdgeId GlobalGraph::getEdge(Graph::NodeId nodeA, Graph::NodeId nodeB) const
{
  nodeStructureType::const_iterator firstNodeFound = nodeStructure_.find(nodeA);
  if (firstNodeFound == nodeStructure_.end())
    throw (Exception("The fist node was not the origin of an edge."));
  map<Node, Edge>::const_iterator secondNodeFound = firstNodeFound->second.first.find(nodeB);
  if (secondNodeFound == firstNodeFound->second.first.end())
    throw (Exception("The second node was not in a relation with the first one."));
  return secondNodeFound->second;
}

vector<Graph::EdgeId> GlobalGraph::getEdges(Graph::NodeId node) const
{
  vector<Graph::EdgeId> result;
  vector<Graph::EdgeId> edgesToInsert;
  edgesToInsert = getEdges_(node, false);
  result.insert(result.end(), edgesToInsert.begin(), edgesToInsert.end());
  edgesToInsert = getEdges_(node, true);
  result.insert(result.end(), edgesToInsert.begin(), edgesToInsert.end());
  return result;
}

void GlobalGraph::outputToDot(ostream& out, const std::string& name) const
{
  out << (directed_ ? "digraph" : "graph") << " " << name << " {\n   ";
  set<pair<Node, Node>> alreadyFigured;
  nodeToDot_(root_, out, alreadyFigured);
  for (const auto& node: nodeStructure_)
  {
    if (node.first != root_)
      nodeToDot_(node.first, out, alreadyFigured);
  }
  out << "\r}" << endl;
}

void GlobalGraph::notifyDeletedEdges(const vector<Graph::EdgeId>& edgesToDelete) const
{
  for (auto& currObserver : observers_)
  {
    currObserver->deletedEdgesUpdate(edgesToDelete);
  }
}

void GlobalGraph::notifyDeletedNodes(const vector<Graph::NodeId>& nodesToDelete) const
{
  for (auto& currObserver : observers_)
  {
    currObserver->deletedNodesUpdate(nodesToDelete);
  }
}

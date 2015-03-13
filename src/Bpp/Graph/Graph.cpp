
#include <string>
#include <vector>
#include <sstream>

#include "GraphObserver.h"
#include "Graph.h"
#include "../Exceptions.h"

using namespace bpp;
using namespace std;

SimpleGraph::SimpleGraph(bool directed_p):
  directed_(directed_p),
  observers_(set<GraphObserver*>()),
  numberOfNodes_(0),
  highestNodeID_(0),
  highestEdgeID_(0),
  nodeStructure_(nodeStructureType()),
  edgeStructure_(edgeStructureType()),
  
  root_(0)
{
 
}


void SimpleGraph::registerObserver(GraphObserver* observer)
{
  if(!observers_.insert(observer).second)
    throw(Exception("This GraphObserver was already an observer of this Graph"));;
}

void SimpleGraph::unregisterObserver(GraphObserver* observer)
{
  if(!observers_.erase(observer))
    throw(Exception("This GraphObserver was not an observer of this Graph"));
}

const SimpleGraph::Edge SimpleGraph::getEdge(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB)
{
   nodeStructureType::iterator firstNodeFound = nodeStructure_.find(nodeA);
   if(firstNodeFound == nodeStructure_.end())
     throw(Exception("The fist node was not the origin of an edge."));
   map<Node,Edge>::iterator secondNodeFound = firstNodeFound->second.first.find(nodeB);
   if(secondNodeFound == firstNodeFound->second.first.end())
     throw(Exception("The second node was not in a relation with the first one."));
   return(secondNodeFound->second);
}

const SimpleGraph::Edge SimpleGraph::link(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB)
{
  // the nodes must exist
  checkNodeExistence_(nodeA, "first node");
  checkNodeExistence_(nodeB, "second node");
  
  // which ID is available?
  unsigned int edgeID = ++highestEdgeID_;
  
  // writing the new relation to the structure
  linkInNodeStructure_(nodeA, nodeB, edgeID, false);
  linkInNodeStructure_(nodeB, nodeA, edgeID, directed_);
  linkInEdgeStructure_(nodeA, nodeB, edgeID);

  return edgeID;
}

void SimpleGraph::checkNodeExistence_(Node node, string name)
{
  if(nodeStructure_.find(node) == nodeStructure_.end())
  {
    ostringstream errMessage;
    errMessage << "This node must exist: " << node << " as " << name << ".";
    throw(Exception(errMessage.str()));
  }
}

void SimpleGraph::checkEdgeExistence_(SimpleGraph::Edge edge, string name)
{
  if(edgeStructure_.find(edge) != edgeStructure_.end())
  {
    ostringstream errMessage;
    errMessage << "This edge must exist: " << edge << " as " << name << ".";
    throw(Exception(errMessage.str()));
  }
}


const std::vector<SimpleGraph::Edge> SimpleGraph::unlink(const Node nodeA, const Node nodeB)
{
  // the nodes must exist
  checkNodeExistence_(nodeA, "first node");
  checkNodeExistence_(nodeB, "second node");
  
  // unlinking in the structure
  vector<Edge> deletedEdges; //what edges ID are affected by this unlinking
  deletedEdges.push_back(unlinkInNodeStructure_(nodeA, nodeB, false));
  deletedEdges.push_back(unlinkInNodeStructure_(nodeB, nodeA, directed_));
  for(vector<Edge>::iterator currEdgeToDelete = deletedEdges.begin(); currEdgeToDelete != deletedEdges.end();currEdgeToDelete++)
  {
    unlinkInEdgeStructure_(*currEdgeToDelete);
  }
  
  // telling the observers
  notifyDeletedEdges(deletedEdges);
  
  return deletedEdges;
}

void SimpleGraph::unlinkInEdgeStructure_(Edge edge)
{
  edgeStructureType::iterator foundEdge = edgeStructure_.find(edge);
  edgeStructure_.erase(foundEdge);
}

void SimpleGraph::linkInEdgeStructure_(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB, SimpleGraph::Edge edge)
{
  edgeStructure_[edge] = pair<Node,Node>(nodeA,nodeB);
}



SimpleGraph::Edge SimpleGraph::unlinkInNodeStructure_(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB, bool toBackwards)
{
  nodeStructureType::iterator foundNodeA = nodeStructure_.find(nodeA);
  std::map<Node,Edge> &forOrBack = (toBackwards?foundNodeA->second.second:foundNodeA->second.first);
  map<Node,Edge>::iterator foundRelation = forOrBack.find(nodeB);
  Edge foundEdge = foundRelation->second;
  forOrBack.erase(foundRelation);
  return foundEdge;
}

void SimpleGraph::linkInNodeStructure_(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB, SimpleGraph::Edge edge, bool toBackwards)
{
  nodeStructureType::iterator foundNodeArrow = nodeStructure_.find(nodeA);
  std::map<Node,Edge> &forOrBack = (toBackwards?foundNodeArrow->second.second:foundNodeArrow->second.first);
  forOrBack.insert(pair<SimpleGraph::Node,SimpleGraph::Edge>(nodeB,edge));
}

const SimpleGraph::Node SimpleGraph::createNode()
{
  Node newNode = highestNodeID_++;
  nodeStructure_[newNode];
  numberOfNodes_++;
  return newNode;
}

const SimpleGraph::Node SimpleGraph::createNodeFromNode(SimpleGraph::Node origin)
{
  //origin must be an existing node
  checkNodeExistence_(origin,"origin node");
  
  Node newNode = createNode();
  link(origin,newNode);
  return newNode;
}

const SimpleGraph::Node SimpleGraph::createNodeOnEdge(SimpleGraph::Edge edge)
{
  //origin must be an existing edge
  checkEdgeExistence_(edge,"");
  
  Node newNode = createNode();
  
  // determining the nodes on the border of the edge
  pair<Node,Node> nodes = edgeStructure_[edge];
  Node nodeA = nodes.first;
  Node nodeB = nodes.second;
  
  unlink(nodeA,nodeB);
  link(nodeA,newNode);
  link(newNode,nodeB);
  
  return newNode;
}



const SimpleGraph::Node SimpleGraph::createNodeFromEdge(SimpleGraph::Edge origin)
{
  //origin must be an existing edge
  checkEdgeExistence_(origin,"origin edge");
  
  // splitting the edge
  Node anchor = createNodeOnEdge(origin);
  
  Node newNode = createNodeFromNode(anchor);
  
  return newNode;
}


void SimpleGraph::notifyDeletedEdges(vector< SimpleGraph::Edge > edgesToDelete)
{
  for(set<GraphObserver*>::iterator currObserver = observers_.begin(); currObserver != observers_.end(); currObserver++)
  {
    (*currObserver)->deletedEdgesUpdate(edgesToDelete);
  }
}

void SimpleGraph::notifyDeletedNodes(vector< SimpleGraph::Node > nodesToDelete)
{
  for(set<GraphObserver*>::iterator currObserver = observers_.begin(); currObserver != observers_.end(); currObserver++)
  {
    (*currObserver)->deletedNodesUpdate(nodesToDelete);
  }
}

const std::vector< SimpleGraph::Node > SimpleGraph::getInOrOutGoingNeighbors_(SimpleGraph::Node node, bool outgoing)
{
  checkNodeExistence_(node,"");
  nodeStructureType::iterator foundNode = nodeStructure_.find(node);
  if(foundNode == nodeStructure_.end())
    throw(Exception("The requested node is not in the structure."));
  std::map<Node,Edge> &forOrBack = (outgoing?foundNode->second.first:foundNode->second.second);
  vector<Node> result;
  for(map<Node,Edge>::iterator currNeighbor = forOrBack.begin(); currNeighbor!= forOrBack.end(); currNeighbor++)
  {
    result.push_back(currNeighbor->first);
  }
  
  return result;
}

const vector< SimpleGraph::Node > SimpleGraph::getIncomingNeighbors(SimpleGraph::Node node)
{
  return getInOrOutGoingNeighbors_(node,false);
}

const vector< SimpleGraph::Node > SimpleGraph::getOutgoingNeighbors(SimpleGraph::Node node)
{
  return getInOrOutGoingNeighbors_(node,true);
}

const vector< SimpleGraph::Node > SimpleGraph::getNeighbors(SimpleGraph::Node node)
{
  vector<SimpleGraph::Node> result;
  vector<SimpleGraph::Node> neighborsToInsert;
  neighborsToInsert = getInOrOutGoingNeighbors_(node,false);
  result.insert(result.end(),neighborsToInsert.begin(),neighborsToInsert.end());
  neighborsToInsert = getInOrOutGoingNeighbors_(node,true);
  result.insert(result.end(),neighborsToInsert.begin(),neighborsToInsert.end());
  return(result);
}

void SimpleGraph::deleteNode(SimpleGraph::Node node)
{
  //checking the node
  checkNodeExistence_(node,"node to delete");
  isolate_(node);
  nodeStructureType::iterator found = nodeStructure_.find(node);
  nodeStructure_.erase(found);
  numberOfNodes_--;
}

void SimpleGraph::isolate_(SimpleGraph::Node node)
{
  vector<Node> neighbors = getNeighbors(node);
  for(vector<Node>::iterator currNeighbor = neighbors.begin(); currNeighbor != neighbors.end(); currNeighbor++){
    unlink(node,*currNeighbor);
  }
}

unsigned int SimpleGraph::getHighestNodeID()
{
  return highestNodeID_;
}


unsigned int SimpleGraph::getHighestEdgeID()
{
  return highestEdgeID_;
}

const vector<SimpleGraph::Node> SimpleGraph::getLeaves()
{
  vector<Node> listOfLeaves;
  fillListOfLeaves_(root_,listOfLeaves,root_);
  return listOfLeaves;
}

void SimpleGraph::fillListOfLeaves_(Node startingNode, vector<Node>& foundLeaves, Node originNode, bool limitedRecursions, unsigned int maxRecursions)
{
  const vector<Node> neighbors = getNeighbors(startingNode);
  if (neighbors.size() > 1)
  {
    if(!limitedRecursions || maxRecursions > 0)
      for(vector<Node>::const_iterator currNeighbor = neighbors.begin(); currNeighbor != neighbors.end(); currNeighbor++)
      {
        if (*currNeighbor != originNode)
          fillListOfLeaves_(*currNeighbor, foundLeaves, startingNode, limitedRecursions, maxRecursions-1);
      }
  }
  else
  {
    foundLeaves.push_back(startingNode);
  }
}


const std::vector<SimpleGraph::Node> SimpleGraph::getLeavesFromNode(SimpleGraph::Node node,unsigned int maxDepth)
{
  vector<Node> listOfLeaves;
  fillListOfLeaves_(node,listOfLeaves,node,(maxDepth!=0),maxDepth);
  return listOfLeaves;
}

void SimpleGraph::nodeToDot_(SimpleGraph::Node node, ostream& out,  std::set<Node> &alreadyExplored)
{
  std::map<Node,Edge> &children = nodeStructure_[node].first;
  out << node;
  map<Node,Edge>::iterator lastChild = children.end();
  lastChild--;
  for(map<Node,Edge>::iterator currChild = children.begin();currChild != children.end();currChild++)
  {
    if(alreadyExplored.find(currChild->first) == alreadyExplored.end())
      nodeToDot_(currChild->first,out,alreadyExplored);
    if(currChild != lastChild)
      out << (directed_? "->":"--");
  }
  alreadyExplored.insert(node);
  out << endl;
}

void SimpleGraph::outputToDot(ostream& out)
{
  set<Node> alreadyExplored;
  nodeToDot_(root_,out,alreadyExplored);
}

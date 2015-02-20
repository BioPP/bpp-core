
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
  observers_(set<GraphObserverI*>()),
  numberOfNodes_(0),
  highestNodeID_(0),
  highestEdgeID_(0),
  nodes_(set<SimpleGraph::Node>()),
  structure_(structure_type()),
  backwardsStructure_(structure_type()),
  
  root_(0)
{
 
}


void SimpleGraph::registerObserver(GraphObserverI* observer)
{
  if(!observers_.insert(observer).second)
    throw(Exception("This GraphObserver was already an observer of this Graph"));;
}

void SimpleGraph::unregisterObserver(GraphObserverI* observer)
{
  if(!observers_.erase(observer))
    throw(Exception("This GraphObserver was not an observer of this Graph"));
}

const SimpleGraph::Edge SimpleGraph::getEdge(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB)
{
   structure_type::iterator firstNodeFound = structure_.find(nodeA);
   if(firstNodeFound == structure_.end())
     throw(Exception("The fist node was not the origin of an edge."));
   map<Node,Edge>::iterator secondNodeFound = firstNodeFound->second.find(nodeB);
   if(secondNodeFound == firstNodeFound->second.end())
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
  link_(nodeA, nodeB, edgeID, false);
  link_(nodeB, nodeA, edgeID, directed_);

  return edgeID;
}

void SimpleGraph::checkNodeExistence_(Node node, string name)
{
  if(nodes_.find(node) == nodes_.end()){
    ostringstream errMessage;
    errMessage << "This node must exist: " << node << " as " << name << ".";
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
  deletedEdges.push_back(unlink_(nodeA, nodeB, false));
  deletedEdges.push_back(unlink_(nodeB, nodeA, directed_));
  
  // telling the observers
  notifyDeletedEdges(deletedEdges);
  
  return deletedEdges;
}

SimpleGraph::Edge SimpleGraph::unlink_(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB, bool toBackwards)
{
  structure_type &currStruct = (toBackwards? backwardsStructure_: structure_);
  structure_type::iterator foundNodeA = currStruct.find(nodeA);
  map<Node,Edge>::iterator foundRelation = foundNodeA->second.find(nodeB);
  Edge foundEdge = foundRelation->second;
  foundNodeA->second.erase(foundRelation);
  return foundEdge;
}

void SimpleGraph::link_(SimpleGraph::Node nodeA, SimpleGraph::Node nodeB, SimpleGraph::Edge edge, bool toBackwards)
{
  structure_type &currStruct = (toBackwards? backwardsStructure_: structure_);
  structure_type::iterator foundNodeArow = currStruct.find(nodeA);
  foundNodeArow->second.insert(pair<SimpleGraph::Node,SimpleGraph::Edge>(nodeB,edge));
}

const SimpleGraph::Node SimpleGraph::createNode()
{
  Node newNode = highestNodeID_++;
  nodes_.insert(newNode);
  structure_[newNode];
  if(directed_)
    backwardsStructure_[newNode];
  numberOfNodes_++;
  return newNode;
}

const SimpleGraph::Node SimpleGraph::createNode(SimpleGraph::Node origin)
{
  //origin must be an existing node
  checkNodeExistence_(origin,"origin node");
  
  Node newNode = createNode();
  link(origin,newNode);
  return newNode;
}

void SimpleGraph::notifyDeletedEdges(vector< SimpleGraph::Edge > edgesToDelete)
{
  for(set<GraphObserverI*>::iterator currObserver = observers_.begin(); currObserver != observers_.end(); currObserver++)
  {
    (*currObserver)->deletedEdgesUpdate(edgesToDelete);
  }
}

void SimpleGraph::notifyDeletedNodes(vector< SimpleGraph::Node > nodesToDelete)
{
  for(set<GraphObserverI*>::iterator currObserver = observers_.begin(); currObserver != observers_.end(); currObserver++)
  {
    (*currObserver)->deletedNodesUpdate(nodesToDelete);
  }
}

const std::vector< SimpleGraph::Node > SimpleGraph::getInOrOutGoingNeighbors_(SimpleGraph::Node node, bool outgoing)
{
  checkNodeExistence_(node,"");
  structure_type &currStruct = (outgoing? structure_ : backwardsStructure_);
  structure_type::iterator foundNode = currStruct.find(node);
  if(foundNode == currStruct.end())
    throw(Exception("The requested node is not in the structure."));
  
  vector<Node> result;
  map<Node,Edge> neighborsOfInterest = foundNode->second;
  for(map<Node,Edge>::iterator currNeighbor = neighborsOfInterest.begin(); currNeighbor!= neighborsOfInterest.end(); currNeighbor++)
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
  nodes_.erase(node);
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

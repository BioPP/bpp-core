
#include <string>
#include <vector>
#include <sstream>

#include "GraphObserver.h"
#include "Graph.h"
#include "Bpp/Exceptions.h"

using namespace bpp;
using namespace std;

Graph::Graph(bool directed_p):
  directed(directed_p),
  numberOfNodes(0),
  highestNodeID(0),
  highestEdgeID(0),
  root(0)
{
 
}


Graph::Graph(const Graph& graph):
  directed(graph.directed),
  numberOfNodes(graph.numberOfNodes),
  highestNodeID(graph.highestNodeID),
  highestEdgeID(graph.highestEdgeID),
  nodes(graph.nodes),
  structure(graph.structure),
  backwardsStructure(graph.backwardsStructure),
  root(graph.root)
{
 
}

#ifdef NO_VIRTUAL_COV
  Clonable*
#else
  Graph*
#endif
Graph::clone() const
{
  return new Graph(*this);
}

void Graph::attach(GraphObserverI* observer)
{
  if(!observers.insert(observer).second)
    throw(Exception("This GraphObserver was already an observer of this Graph"));;
}

void Graph::detach(GraphObserverI* observer)
{
  if(!observers.erase(observer))
    throw(Exception("This GraphObserver was not an observer of this Graph"));
}

const Graph::Edge Graph::getEdge(Graph::Node nodeA, Graph::Node nodeB)
{
   structure_type::iterator firstNodeFound = structure.find(nodeA);
   if(firstNodeFound == structure.end())
     throw(Exception("The fist node was not the origin of an edge."));
   map<Node,Edge>::iterator secondNodeFound = firstNodeFound->second.find(nodeB);
   if(secondNodeFound == firstNodeFound->second.end())
     throw(Exception("The second node was not in a relation with the first one."));
   return(secondNodeFound->second);
}

const Graph::Edge Graph::link(Graph::Node nodeA, Graph::Node nodeB)
{
  // the nodes must exist
  checkNodeExistence(nodeA, "first node");
  checkNodeExistence(nodeB, "second node");
  
  // which ID is available?
  unsigned int edgeID = ++highestEdgeID;
  
  // writing the new relation to the structure
  rawLink(nodeA, nodeB, edgeID, false);
  rawLink(nodeB, nodeA, edgeID, directed);

  return edgeID;
}

void Graph::checkNodeExistence(Node node, string name)
{
  if(nodes.find(node) == nodes.end()){
    ostringstream errMessage;
    errMessage << "This node must exist: " << node << " as " << name << ".";
    throw(Exception(errMessage.str()));
  }
}

const std::vector<Graph::Edge> Graph::unlink(const Node nodeA, const Node nodeB)
{
  // the nodes must exist
  checkNodeExistence(nodeA, "first node");
  checkNodeExistence(nodeB, "second node");
  
  // unlinking in the structure
  vector<Edge> deletedEdges; //what edges ID are affected by this unlinking
  deletedEdges.push_back(rawUnLink(nodeA, nodeB, false));
  deletedEdges.push_back(rawUnLink(nodeB, nodeA, directed));
  
  // telling the observers
  tellObservers_removeDeletedEdges(deletedEdges);
  
  return deletedEdges;
}

Graph::Edge Graph::rawUnLink(Graph::Node nodeA, Graph::Node nodeB, bool toBackwards)
{
  structure_type &currStruct = (toBackwards? backwardsStructure: structure);
  structure_type::iterator foundNodeA = currStruct.find(nodeA);
  map<Node,Edge>::iterator foundRelation = foundNodeA->second.find(nodeB);
  Edge foundEdge = foundRelation->second;
  foundNodeA->second.erase(foundRelation);
  return foundEdge;
}

void Graph::rawLink(Graph::Node nodeA, Graph::Node nodeB, Graph::Edge edge, bool toBackwards)
{
  structure_type &currStruct = (toBackwards? backwardsStructure: structure);
  structure_type::iterator foundNodeArow = currStruct.find(nodeA);
  foundNodeArow->second.insert(pair<Graph::Node,Graph::Edge>(nodeB,edge));
}

const Graph::Node Graph::createNode()
{
  Node newNode = highestNodeID++;
  nodes.insert(newNode);
  structure[newNode];
  if(directed)
    backwardsStructure[newNode];
  numberOfNodes++;
  return newNode;
}

const Graph::Node Graph::createNode(Graph::Node origin)
{
  //origin must be an existing node
  checkNodeExistence(origin,"origin node");
  
  Node newNode = createNode();
  link(origin,newNode);
  return newNode;
}

void Graph::tellObservers_removeDeletedEdges(vector< Graph::Edge > edgesToDelete)
{
  for(set<GraphObserverI*>::iterator currObserver = observers.begin(); currObserver != observers.end(); currObserver++)
  {
    (*currObserver)->update_removeDeletedEdges(edgesToDelete);
  }
}

void Graph::tellObservers_removeDeletedNodes(vector< Graph::Node > nodesToDelete)
{
  for(set<GraphObserverI*>::iterator currObserver = observers.begin(); currObserver != observers.end(); currObserver++)
  {
    (*currObserver)->update_removeDeletedNodes(nodesToDelete);
  }
}

const std::vector< Graph::Node > Graph::getInOrOutGoingNeighbors(Graph::Node node, bool outgoing)
{
  checkNodeExistence(node,"");
  structure_type &currStruct = (outgoing? structure : backwardsStructure);
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

const vector< Graph::Node > Graph::getIncomingNeighbors(Graph::Node node)
{
  return getInOrOutGoingNeighbors(node,false);
}

const vector< Graph::Node > Graph::getOutgoingNeighbors(Graph::Node node)
{
  return getInOrOutGoingNeighbors(node,true);
}

const vector< Graph::Node > Graph::getNeighbors(Graph::Node node)
{
  vector<Graph::Node> result;
  vector<Graph::Node> neighborsToInsert;
  neighborsToInsert = getInOrOutGoingNeighbors(node,false);
  result.insert(result.end(),neighborsToInsert.begin(),neighborsToInsert.end());
  neighborsToInsert = getInOrOutGoingNeighbors(node,true);
  result.insert(result.end(),neighborsToInsert.begin(),neighborsToInsert.end());
  return(result);
}

void Graph::deleteNode(Graph::Node node)
{
  //checking the node
  checkNodeExistence(node,"node to delete");
  isolate(node);
  nodes.erase(node);
  numberOfNodes--;
}

void Graph::isolate(Graph::Node node)
{
  vector<Node> neighbors = getNeighbors(node);
  for(vector<Node>::iterator currNeighbor = neighbors.begin(); currNeighbor != neighbors.end(); currNeighbor++){
    unlink(node,*currNeighbor);
  }
}

unsigned int Graph::getHighestNodeID()
{
  return highestNodeID;
}


unsigned int Graph::getHighestEdgeID()
{
  return highestEdgeID;
}

const vector<Graph::Node> Graph::getLeaves()
{
  vector<Node> listOfLeaves;
  fillListOfLeaves(root,listOfLeaves,root);
  return listOfLeaves;
}

void Graph::fillListOfLeaves(Node startingNode, vector<Node>& foundLeaves, Node originNode, bool limitedRecursions, unsigned int maxRecursions)
{
  const vector<Node> neighbors = getNeighbors(startingNode);
  if (neighbors.size() > 1)
  {
    if(!limitedRecursions || maxRecursions > 0)
      for(vector<Node>::const_iterator currNeighbor = neighbors.begin(); currNeighbor != neighbors.end(); currNeighbor++)
      {
        if (*currNeighbor != originNode)
          fillListOfLeaves(*currNeighbor, foundLeaves, startingNode, limitedRecursions, maxRecursions-1);
      }
  }
  else
  {
    foundLeaves.push_back(startingNode);
  }
}


const std::vector<Graph::Node> Graph::getLeavesFromNode(Graph::Node node,unsigned int maxDepth)
{
  vector<Node> listOfLeaves;
  fillListOfLeaves(node,listOfLeaves,node,(maxDepth!=0),maxDepth);
  return listOfLeaves;
}

#include "GraphObserver.h"
#include "Graph.h"

#include <Bpp/Exceptions.h>

#include <vector>

using namespace bpp;
using namespace std;


template <class N, class E>
void GraphObserver<N,E>::createNode(N& objectNewNode)
{
  Graph::Node newGraphNode = subjectGraph->createNode();
  
  associate(objectNewNode, newGraphNode);
  
}

template <class N, class E>
void GraphObserver<N,E>::createNode(N& objectOriginNode, N& objectNewNode)
{
  createNode(objectNewNode);
  link(objectOriginNode,objectNewNode);
}

template <class N, class E>
void GraphObserver<N,E>::link(N& nodeObjectA, N& nodeObjectB, E& edgeObject)
{
  // checking the nodes
  typename map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectsToNodes.find(&nodeObjectA);
  foundNodeB = objectsToNodes.find(&nodeObjectB);
  if(foundNodeA == objectsToNodes.end() || foundNodeB == objectsToNodes.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  //checking if the edge is not already in the GraphObserver
  if(edgesToObjects.find(&edgeObject) != edgesToObjects.end())
    throw Exception("The given edge is already associated to a relation in the subjectGraph.");
  
  Graph::Edge newGraphEdge = subjectGraph->link(foundNodeA->second,foundNodeB->second);
  
  if(edgesToObjects.size() < newGraphEdge+1)
    edgesToObjects.resize(newGraphEdge+1);
  edgesToObjects.at(newGraphEdge) = edgeObject;
  
  objectsToEdges[edgeObject] = newGraphEdge;
}

template <class N, class E>
void GraphObserver<N,E>::unlink(N& nodeObjectA, N& nodeObjectB)
{
  //checking the nodes
  typename map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = objectsToNodes.find(&nodeObjectA);
  foundNodeB = objectsToNodes.find(&nodeObjectB);
  if(foundNodeA == objectsToNodes.end() || foundNodeB == objectsToNodes.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  subjectGraph->unlink(foundNodeA->second,foundNodeB->second);
}

template <class N, class E>
void GraphObserver<N,E>::update_removeDeletedEdges(std::vector<Graph::Edge> edgesToDelete)
{
  for(vector<Graph::Edge>::iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++){
    E edgeObject = edgesToObjects.at(*currEdge);
    edgesToObjects.at(*currEdge) = 00;
    
    objectsToEdges.erase(edgeObject);
    
  }
}

template <class N, class E>
void GraphObserver<N,E>::update_removeDeletedNodes(std::vector<Graph::Node> nodesToDelete){
  for(vector<Graph::Edge>::iterator currEdge = nodesToDelete.begin(); currEdge != nodesToDelete.end(); currEdge++){
    E nodeObject = nodesToObjects.at(*currEdge);
    nodesToObjects.at(*currEdge) = 00;
    
    objectsToEdges.erase(nodeObject);
    
  }
}

template <class N, class E>
const vector<N&> GraphObserver<N,E>::getOutgoingNeighbors(N& node)
{
  Graph::Node graphNode = objectsToNodes.at(&node);
  vector<Graph::Node> graphNodes = subjectGraph->getOutgoingNeighbors(graphNode);
  vector<N*> result;
  for(vector<Graph::Node>::iterator currGraphNode = graphNodes.begin(); currGraphNode != graphNodes.end(); currGraphNode++)
  {
    result.push_back(nodesToObjects.at(*currGraphNode));
  }
  return result;
}

template <class N, class E>
void GraphObserver<N,E>::associate(N& nodeObject, Graph::Node node)
{
  
  // nodes vector must be the right size. Eg: to store a node with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  nodesToObjects.resize(subjectGraph->getHighestNodeID()+1);

  // now storing the node
  nodesToObjects.at(node) = &nodeObject;
  objectsToNodes[&nodeObject] = node;
}

template <class N, class E>
void GraphObserver<N,E>::associate(E& edgeObject, Graph::Edge edge)
{
  
  // nodes vector must be the right size. Eg: to store an edge with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  edgesToObjects.resize(subjectGraph->getHighestEdgeID()+1);
  
  // now storing the edge
  edgesToObjects.at(edge) = &edgeObject;
  objectsToNodes[&edgeObject] = edge;
}

template <class N, class E>
void GraphObserver<N,E>::forget(N& nodeObject)
{
  typename map<N*,Graph::Node>::iterator nodeToForget = objectsToNodes.find(&nodeObject);
  nodesToObjects.erase(nodeToForget->second);
  objectsToNodes.erase(nodeToForget);
}

template <class N, class E>
void GraphObserver<N,E>::forget(E& edgeObject)
{
  typename map<E*,Graph::Edge>::iterator edgeToForget = objectsToEdges.find(&edgeObject);
  edgesToObjects.erase(edgeToForget->second);
  objectsToEdges.erase(edgeToForget);
}

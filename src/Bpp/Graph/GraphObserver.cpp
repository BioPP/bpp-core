#include "GraphObserver.h"
#include "Graph.h"

#include <Bpp/Exceptions.h>

#include <vector>

using namespace bpp;
using namespace std;


template <class N, class E>
void GraphObserver<N,E>::createNode(N* objectNewNode)
{
  Graph::Node newGraphNode = subjectGraph->createNode();
  
  // nodes vector must be the right size. Eg: to store a node with
  // the ID 3, the vector must be of size 4: {0,1,2,3} (size = 4)
  if(nodes.size() < newGraphNode+1)
    nodes.resize(newGraphNode+1);
  
  // now storing the node
  nodes.at(newGraphNode) = objectNewNode;
  nodesID[objectNewNode] = newGraphNode;
}

template <class N, class E>
void GraphObserver<N,E>::createNode(N* objectOriginNode, N* objectNewNode)
{
  createNode(objectNewNode);
  link(objectOriginNode,objectNewNode);
}

template <class N, class E>
void GraphObserver<N,E>::link(N* nodeA, N* nodeB, E* edge)
{
  // checking the nodes
  typename map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = nodesID.find(nodeA);
  foundNodeB = nodesID.find(nodeB);
  if(foundNodeA == nodesID.end() || foundNodeB == nodesID.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  //checking if the edge is not already in the GraphObserver
  if(edges.find(edge) != edges.end())
    throw Exception("The given edge is already associated to a relation in the subjectGraph.");
  
  Graph::Edge newGraphEdge = subjectGraph->link(foundNodeA->second,foundNodeB->second);
  
  if(edges.size() < newGraphEdge+1)
    edges.resize(newGraphEdge+1);
  edges.at(newGraphEdge) = edge;
  
  edgesID[edge] = newGraphEdge;
}

template <class N, class E>
void GraphObserver<N,E>::unlink(N* nodeA, N* nodeB)
{
  //checking the nodes
  typename map<N*,Graph::Node>::iterator foundNodeA, foundNodeB;
  foundNodeA = nodesID.find(nodeA);
  foundNodeB = nodesID.find(nodeB);
  if(foundNodeA == nodesID.end() || foundNodeB == nodesID.end())
    throw Exception("One of the nodes is not in the graph observer.");
  
  subjectGraph->unlink(foundNodeA->second,foundNodeB->second);
}

template <class N, class E>
void GraphObserver<N,E>::update_removeDeletedEdges(std::vector<Graph::Edge> edgesToDelete)
{
  for(vector<Graph::Edge>::iterator currEdge = edgesToDelete.begin(); currEdge != edgesToDelete.end(); currEdge++){
    E edgeObject = edges.at(*currEdge);
    edges.at(*currEdge) = 00;
    
    edgesID.erase(edgeObject);
    
  }
}

template <class N, class E>
void GraphObserver<N,E>::update_removeDeletedNodes(std::vector<Graph::Node> nodesToDelete){
  for(vector<Graph::Edge>::iterator currEdge = nodesToDelete.begin(); currEdge != nodesToDelete.end(); currEdge++){
    E nodeObject = nodes.at(*currEdge);
    nodes.at(*currEdge) = 00;
    
    edgesID.erase(nodeObject);
    
  }
}

template <class N, class E>
const vector<N*> GraphObserver<N,E>::getOutgoingNeighbors(N* node)
{
  Graph::Node graphNode = nodesID.at(node);
  vector<Graph::Node> graphNodes = subjectGraph->getOutgoingNeighbors(graphNode);
  vector<N*> result;
  for(vector<Graph::Node>::iterator currGraphNode = graphNodes.begin(); currGraphNode != graphNodes.end(); currGraphNode++)
  {
    result.push_back(nodes.at(*currGraphNode));
  }
  return result;
}




#include "../Exceptions.h"

#include "TreeGraph.h"
#include "Graph.h"


using namespace bpp;
using namespace std;


bool SimpleTreeGraph::isValid() const
{
  return isValid_;
}

Graph::Node SimpleTreeGraph::getFather(Graph::Node node)
{
  mustBeValid_();
  mustBeRooted_();
  vector<Graph::Node> incomers = SimpleGraph::getIncomingNeighbors(node);
  if(incomers.size() != 1)
    throw Exception("SimpleTreeGraph::getFather: more than one father.");
  if(incomers.size() == 0)
    throw Exception("SimpleTreeGraph::getFather: node has no father.");
  return *incomers.begin();
}

bool SimpleTreeGraph::hasFather(Graph::Node node)
{
  mustBeValid_();
  mustBeRooted_();
  vector<Graph::Node> incomers = SimpleGraph::getIncomingNeighbors(node);
  return incomers.size() == 1;
}

void SimpleTreeGraph::mustBeRooted_()
{
  if(!isRooted())
    throw Exception("SimpleTreeGraph: The tree must be rooted.");
}

void SimpleTreeGraph::mustBeValid_()
{
  if(!isValid()||!validate_())
    throw Exception("SimpleTreeGraph: The tree is not valid.");
}

bool SimpleTreeGraph::isRooted() const
{
  return(SimpleGraph::isDirected());
}

bool SimpleTreeGraph::validate_()
{
  isValid_ = SimpleGraph::isTree();
  return(isValid());
}

void SimpleTreeGraph::reRoot(Graph::Node newRoot)
{
  // set the new root on the Graph
  SimpleGraph::setRoot(newRoot);
  // change edge direction between the new node and the former one
  propagateNewDirection_(newRoot);
}

void SimpleTreeGraph::propagateNewDirection_(Graph::Node node)
{
  if(hasFather(node)){
    Node father = getFather(node);
    unlink(father,node);
    link(node,father);
  }
  
}


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

void SimpleTreeGraph::mustBeRooted_() const
{
  if(!isRooted())
    throw Exception("SimpleTreeGraph: The tree must be rooted.");
}

void SimpleTreeGraph::mustBeValid_() const
{
  if(!isValid()||!validate_())
    throw Exception("SimpleTreeGraph: The tree is not valid.");
}

bool SimpleTreeGraph::isRooted() const
{
  return(SimpleGraph::isDirected());
}

bool SimpleTreeGraph::validate_() const
{
  isValid_ = SimpleGraph::isTree();
  return(isValid());
}

void SimpleTreeGraph::rootAt(Graph::Node newRoot)
{
  makeDirected();
  // set the new root on the Graph
  SimpleGraph::setRoot(newRoot);
  // change edge direction between the new node and the former one
  propagateDirection_(newRoot);
}

void SimpleTreeGraph::propagateDirection_(Graph::Node node)
{
  if(hasFather(node)){
    Node father = getFather(node);
    unlink(father,node);
    link(node,father);
    propagateDirection_(father);
  }
  
}

void SimpleTreeGraph::unRoot(bool joinRootSons)
{
  if(joinRootSons){
    // the root must have exactly two joinRootSons
    vector<Node> sons = getSons(getRoot());
    if(sons.size() != 2)
      throw Exception("The root must have two sons to join them.");
    unlink(getRoot(),sons.at(0));
    unlink(getRoot(),sons.at(1));
    link(sons.at(0),sons.at(1));
    setRoot(sons.at(0));
  }
  makeUndirected();
}

void SimpleTreeGraph::newOutGroup(Graph::Node newOutGroup)
{
  mustBeRooted_();
  Node newRoot = createNodeFromEdge(getEdge(getFather(newOutGroup),newOutGroup));
  rootAt(newRoot);
}

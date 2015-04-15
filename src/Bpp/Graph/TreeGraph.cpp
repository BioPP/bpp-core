
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
    throw Exception("More than one father.");
  return *incomers.begin();
}

void SimpleTreeGraph::mustBeRooted_() const
{
  if(!isRooted())
    throw Exception("SimpleTreeGraph: The tree must be rooted.");
}

void SimpleTreeGraph::mustBeValid_() const
{
  if(!isValid_)
    throw Exception("SimpleTreeGraph: The tree is not valid.");
}

bool SimpleTreeGraph::isRooted() const
{
  return(SimpleGraph::isDirected());
}

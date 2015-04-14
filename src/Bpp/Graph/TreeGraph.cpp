
#include "../Exceptions.h"

#include "TreeGraph.h"


using namespace bpp;
using namespace std;


bool SimpleTreeGraph::isValid() const
{
  return isValid_;
}

Graph::Node SimpleTreeGraph::getFather(Graph::Node node)
{
  if(!isRooted())
    throw Exception("SimpleTreeGraph::getFather Unable to get father on an unrooted tree.");
}

bool SimpleTreeGraph::isRooted() const
{
  return isRooted_;
}

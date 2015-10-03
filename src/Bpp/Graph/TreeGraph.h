#ifndef _TREEGRAPH_H_
#define _TREEGRAPH_H_

#include <vector>

#include "Graph.h"

#include "../Exceptions.h"

namespace bpp
{
  class TreeGraph:
  public virtual Graph
  {
  public:
    
    //TODO Incomplete specification. Waiting the basic implementation is functional
    // to specify the interface.
    
    /**
     * Is the graph a tree? A tree must be acyclic and with no isolated node.
     * @return true if valid tree
     */
    virtual bool isValid() const = 0;
  };
  
  template <class GraphImpl>
  class SimpleTreeGraph:
  public virtual TreeGraph,
  public virtual GraphImpl
  {
  private:
    /**
     * Is the graph a tree? Set to false when structure is modified, true after validation.
     */
    mutable bool isValid_;
    
    // unvalidate the tree
    virtual void topologyHasChanged_() const;
    
    // will throw an exception if the tree is not valid
    void mustBeValid_() const;
    
    // will throw an exception if the tree is not rooted
    void mustBeRooted_() const;
    
    // test the validity of the tree
    bool validate_() const;
    
    /**
     * Reorient all the edges starting from a node:
     * the father node becomes a son, and so on.
     */
    void propagateDirection_(Node node);
    
  public:
      
    SimpleTreeGraph();
    SimpleTreeGraph(bool rooted=true);
    
    /**
     * Is the graph a tree? A tree must be acyclic and with no isolated node.
     * @return true if valid tree
     */
    bool isValid() const;
    
    /**
     * Is the tree rooted?
     * @return true if rooted
     */
    bool isRooted() const;
    
    /**
     * Get the father node of a node in a rooted tree
     * @return the father node
     */
    Graph::Node getFather(Graph::Node node);
    
    /**
     * Get the branch leading to the father in a rooted tree
     * @return the branch between a node and its father
     */
    Graph::Edge getBranchToFather(Graph::Node node);
    
    /**
     * Get the father node of a node in a rooted tree
     * @return true if rooted
     */
    bool hasFather(Graph::Node node);
    
    /**
     * Get the father node of a node in a rooted tree
     * @return true if rooted
     */
    std::vector<Graph::Node> getSons(Graph::Node node);
    
    
    
    
    /**
     * Re-root the tree with the new root
     */
    void rootAt(Graph::Node newRoot);
    
    /**
     * Set the tree to its flat unrooted version.
     * As an algorithmical convenience, a root node is kept, but it has
     * no logical significance.
     */
    void unRoot(bool joinRootSons);
    
    /**
     * Set a node as a new outgroup in a rooted tree, will make a root between
     * the given node and its father.
     */
    void setOutGroup(Graph::Node newOutGroup);
    
    
  };
  
  template <class GraphImpl>
  SimpleTreeGraph<GraphImpl>::SimpleTreeGraph(bool rooted):
  GraphImpl(rooted),
  isValid_(false)
  {
  }
  
  
  template <class GraphImpl>
  bool SimpleTreeGraph<GraphImpl>::isValid() const
  {
    return (isValid_ || validate_());
  }
  
  template <class GraphImpl>
  Graph::Node SimpleTreeGraph<GraphImpl>::getFather(Graph::Node node)
  {
    mustBeValid_();
    mustBeRooted_();
    std::vector<Graph::Node> incomers = GraphImpl::getIncomingNeighbors(node);
    if(incomers.size() != 1)
      throw Exception("SimpleTreeGraph<GraphImpl>::getFather: more than one father. Should never happen since validity has been controled. Please report this bug.");
    if(incomers.size() == 0)
      throw Exception("SimpleTreeGraph<GraphImpl>::getFather: node has no father.");
    return *incomers.begin();
  }
  
  template <class GraphImpl>
  Graph::Edge SimpleTreeGraph<GraphImpl>::getBranchToFather(Graph::Node node)
  {
    Node father = getFather(node);
    return GraphImpl::getBranch(father,node);
  }
  
  template <class GraphImpl>
  bool SimpleTreeGraph<GraphImpl>::hasFather(Graph::Node node)
  {
    mustBeValid_();
    mustBeRooted_();
    std::vector<Graph::Node> incomers = SimpleGraph::getIncomingNeighbors(node);
    return incomers.size() == 1;
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::mustBeRooted_() const
  {
    if(!isRooted())
      throw Exception("SimpleTreeGraph: The tree must be rooted.");
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::mustBeValid_() const
  {
    if(!isValid())
      throw Exception("SimpleTreeGraph: The tree is not valid.");
  }
  
  template <class GraphImpl>
  bool SimpleTreeGraph<GraphImpl>::isRooted() const
  {
    return(SimpleGraph::isDirected());
  }
  
  template <class GraphImpl>
  bool SimpleTreeGraph<GraphImpl>::validate_() const
  {
    isValid_ = SimpleGraph::isTree();
    return(isValid_);
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::topologyHasChanged_() const
  {
    isValid_ = false;
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::rootAt(Graph::Node newRoot)
  {
    GraphImpl::makeDirected();
    // set the new root on the Graph
    GraphImpl::setRoot(newRoot);
    // change edge direction between the new node and the former one
    propagateDirection_(newRoot);
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::propagateDirection_(Graph::Node node)
  {
    if(hasFather(node)){
      Node father = getFather(node);
      GraphImpl::unlink(father,node);
      GraphImpl::link(node,father);
      propagateDirection_(father);
    }
    
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::unRoot(bool joinRootSons)
  {
    if(joinRootSons){
      // the root must have exactly two joinRootSons
      std::vector<Node> sons = getSons(GraphImpl::getRoot());
      if(sons.size() != 2)
        throw Exception("The root must have two sons to join them.");
      GraphImpl::unlink(GraphImpl::getRoot(),sons.at(0));
      GraphImpl::unlink(GraphImpl::getRoot(),sons.at(1));
      GraphImpl::link(sons.at(0),sons.at(1));
      GraphImpl::setRoot(sons.at(0));
    }
    GraphImpl::makeUndirected();
  }
  
  template <class GraphImpl>
  void SimpleTreeGraph<GraphImpl>::setOutGroup(Graph::Node newOutGroup)
  {
    mustBeRooted_();
    Node newRoot = createNodeFromEdge(getEdge(getFather(newOutGroup),newOutGroup));
    rootAt(newRoot);
  }
  
  
}



#else

namespace bpp {
  class TreeGraph;
  
  template <class GraphImpl>
  class SimpleTreeGraph;
}

#endif

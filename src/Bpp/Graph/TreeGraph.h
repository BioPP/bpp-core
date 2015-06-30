#ifndef _TREEGRAPH_H_
#define _TREEGRAPH_H_

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
  
  template <class I>
  class SimpleTreeGraph:
  public virtual TreeGraph,
  public I
  {
  private:
    /**
     * Is the graph a tree? Set to false when structure is modified, true after validation.
     */
    mutable bool isValid_;
    
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
     * @return true if rooted
     */
    Graph::Node getFather(Graph::Node node);
    
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
    void newOutGroup(Graph::Node newOutGroup);
    
    
  };
  
  template <class I>
  bool SimpleTreeGraph::isValid() const
  {
    return isValid_;
  }
  
  template <class I>
  Graph::Node SimpleTreeGraph::getFather(Graph::Node node)
  {
    mustBeValid_();
    mustBeRooted_();
    vector<Graph::Node> incomers = SimpleGraph::getIncomingNeighbors(node);
    if(incomers.size() != 1)
      throw Exception("SimpleTreeGraph::getFather: more than one father. Should never happen since validity has been controled. Please report this bug.");
    if(incomers.size() == 0)
      throw Exception("SimpleTreeGraph::getFather: node has no father.");
    return *incomers.begin();
  }
  
  template <class I>
  bool SimpleTreeGraph::hasFather(Graph::Node node)
  {
    mustBeValid_();
    mustBeRooted_();
    vector<Graph::Node> incomers = SimpleGraph::getIncomingNeighbors(node);
    return incomers.size() == 1;
  }
  
  template <class I>
  void SimpleTreeGraph::mustBeRooted_() const
  {
    if(!isRooted())
      throw Exception("SimpleTreeGraph: The tree must be rooted.");
  }
  
  template <class I>
  void SimpleTreeGraph::mustBeValid_() const
  {
    if(!isValid()||!validate_())
      throw Exception("SimpleTreeGraph: The tree is not valid.");
  }
  
  template <class I>
  bool SimpleTreeGraph::isRooted() const
  {
    return(SimpleGraph::isDirected());
  }
  
  template <class I>
  bool SimpleTreeGraph::validate_() const
  {
    isValid_ = SimpleGraph::isTree();
    return(isValid());
  }
  
  template <class I>
  void SimpleTreeGraph::rootAt(Graph::Node newRoot)
  {
    makeDirected();
    // set the new root on the Graph
    SimpleGraph::setRoot(newRoot);
    // change edge direction between the new node and the former one
    propagateDirection_(newRoot);
  }
  
  template <class I>
  void SimpleTreeGraph::propagateDirection_(Graph::Node node)
  {
    if(hasFather(node)){
      Node father = getFather(node);
      unlink(father,node);
      link(node,father);
      propagateDirection_(father);
    }
    
  }
  
  template <class I>
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
  
  template <class I>
  void SimpleTreeGraph::newOutGroup(Graph::Node newOutGroup)
  {
    mustBeRooted_();
    Node newRoot = createNodeFromEdge(getEdge(getFather(newOutGroup),newOutGroup));
    rootAt(newRoot);
  }
  
  
}



#else

namespace bpp {
  class TreeGraph;
  class SimpleTreeGraph
}

#endif
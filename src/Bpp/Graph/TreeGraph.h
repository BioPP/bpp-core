#ifndef _TREEGRAPH_H_
#define _TREEGRAPH_H_

#include "Graph.h"

namespace bpp
{
  class TreeGraph:
  public virtual Graph
  {
  public:
    
    
  /**
    * Is the graph a tree? A tree must be acyclic and with no isolated node.
    * @return true if valid tree
    */
    virtual bool isValid() const = 0;
  };
  
  class SimpleTreeGraph:
  public virtual TreeGraph,
  public SimpleGraph
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
  
  
}



#else

namespace bpp {
class TreeGraph;
class SimpleTreeGraph
}

#endif
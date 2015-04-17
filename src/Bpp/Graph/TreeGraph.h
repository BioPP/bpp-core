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
    bool isValid_;
    
    // will throw an exception if the tree is not valid
    void mustBeValid_();
    
    // will throw an exception if the tree is not rooted
    void mustBeRooted_();
    
    // test the validity of the tree
    bool validate_();
    
    /**
     * Reorient all the edges starting from a node:
     * the father node becomes a son, and so on.
     */
    void propagateNewDirection_(Node node);
    
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
     * Re-root the tree with the new root
     */
    void reRoot(Graph::Node newRoot);
    
     /**
     * Set a node as a new outgroup
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
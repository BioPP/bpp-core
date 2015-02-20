#ifndef _GRAPH_HPP_
#define _GRAPH_HPP_

#include<set>
#include<map>
#include<string>
#include<vector>



// forward declaration to avoid circular dependancies.
// since we do not need its size in this header file (only using pointers to it)
namespace bpp
{
class GraphObserverI;
}

namespace bpp
{
  class Graph
  {
  public:
    Graph() {}
    virtual ~Graph() {}
  };
}

namespace bpp
{
/**
 * @brief Defines a Graph. Contains edges and nodes. This class just define the structure of the graph
 * the content itself is contained in a GraphObserver template.
 * The observers are of the class GraphObservers, and can be called Subscribers.
 *
 * @author Thomas Bigot
 */

class SimpleGraph:
public virtual Graph
{
public:
    typedef unsigned int Node;
    typedef unsigned int Edge;

private:

    /**
    * is the graph directed
    */
    bool directed_;

    /**
    * List of all the subscribers.
    */
    std::set<GraphObserverI*> observers_;

    /**
    * Number of nodes.
    */
    unsigned int numberOfNodes_;
    /**
    * Highest used available ID for a Node.
    */
    unsigned int highestNodeID_;
    /**
    * Highest used available ID for an Edge.
    */
    unsigned int highestEdgeID_;

    /**
    * The structure type, used for more clarity
    */
    typedef std::map<Node,std::map<Node,Edge> > structure_type;


    /**
    * Set of existing nodes in this Graph
    */
    std::set<Node> nodes_;


    /**
    * Nodes and their relations.
    * TODO: explain the relation coding
    * nodeA -> [(nodeB, relationID),…]
    */
    structure_type structure_;
    /**
    * Nodes and their relations in the backwards direction, for directed graph only.
    * TODO: explain the relation coding
    * nodeA -> [(nodeB, relationID),…]
    */
    structure_type backwardsStructure_;
    /**
    * Usualy the first node of a graph. Used for algorithmic purposes.
    */
    Node root_;
    /**
    * Tell all the observers to get the last updates.
    * Calls the method update of all the subscribers.
    */
    void notify_();



    /**
    * Creates a link between two existing nodes. If directed graph: nodeA -> nodeB.
    * Private version of link, does not check for the reciprocity.
    * Mainly called by link().
    * @param nodeA source node (or first node if undirected)
    * @param nodeB target node (or second node if undirected)
    * @param edge the ID of the relation
    * @param toBackwards if this link has to be done in the backwardsStructure (for directed graphs)
    */
    void link_(Node nodeA, Node nodeB, Edge edge, bool toBackwards);


    /**
    * Erase a link between two existing nodes. If directed graph: nodeA -> nodeB.
    * Private version of unLink, does not check for the reciprocity.
    * Mainly called by unLink().
    * @param nodeA source node (or first node if undirected)
    * @param nodeB target node (or second node if undirected)
    * @param toBackwards if this link has to be UNdone in the backwardsStructure (for directed graphs)
    * @return the ID of the erased relation
    */
    Edge unlink_(Node nodeA, Node nodeB, bool toBackwards);


    /**
    * Check that a node exists. If not, throw an exception.
    * @param node node that has to be checked
    * @param name common name to give to the user (eg: "first node")
    */
    void checkNodeExistence_(Node node, std::string name);


    /**
    * Private version of getIncomingNeighbors or getOutgoingNeighbors.
    * Common code of these function shared here.
    * @param node node to find in or outgoing neighbors
    * @param outgoing boolean: if tue, outgoing; else incoming
    */
    const std::vector<Node> getInOrOutGoingNeighbors_(Node node,bool outgoing=true);

  /**
    * Separate a node from all its neighbors.
    * @param node node to isolate
    */
    void isolate_(Node node);
    
  /**
    * Get leaves from a starting node, filling a vector (private version).
    * @param startingNode root node
    * @param foundLeaves a vector containing all the found leaves
    * @param originNode the node where we come from, not to explore
    */
    void fillListOfLeaves_(Node startingNode, std::vector<Node>& foundLeaves, Node originNode, bool limitedRecursions = false, unsigned int maxRecursions = 0);
    
    
    

    
    
public:
  
  /** @name General Management
    *  Misc & constructors
    */
    ///@{

  
    /**
    * Constructor
    * @param directed true if the graph is directed.
    */
    SimpleGraph(bool directed=false);
   
    
    /**
      * get the Highest Node ID (for vector sizing)
      */
    unsigned int getHighestNodeID();
    
    /**
      * get the Highest Node ID (for vector sizing)
      */
    unsigned int getHighestEdgeID();
    
    
    ///@}


    /** @name Relations management
    *  Modificating the structure of the graph.
    */
    ///@{
    /**
    * Creates an orphaned node.
    * @return the new node
    */
    const Node createNode();
    /**
    * Creates an node linked to an existing node.
    * @param origin existing node. In a directed graph: origin -> newNode.
    * @return the new node
    */
    const Node createNode(Node origin);
    /**
    * Creates a link between two existing nodes. If directed graph: nodeA -> nodeB.
    * @param nodeA source node (or first node if undirected)
    * @param nodeB target node (or second node if undirected)
    * @return the new edge
    */
    const Edge link(bpp::SimpleGraph::Node nodeA, bpp::SimpleGraph::Node nodeB);
    /**
    * Remove all links between two existing nodes. If directed graph: nodeA -> nodeB.
    * @param nodeA source node (or first node if undirected)
    * @param nodeB target node (or second node if undirected)
    * @return vector of deleted edges
    */
    const std::vector<Edge> unlink(Node nodeA, Node nodeB);

    /**
    * Delete one node
    * @param node node to be deleted
    */
    void deleteNode(Node node);

    ///@}




    /** @name Observers Management
    *  Managing communication with the observers: subscribe, unsubscribe.
    */
    ///@{

    /**
    * Attach a new observer to this Graph.
    * As a subscriber, the observer will be warned of all the changes.
    */
    void registerObserver(bpp::GraphObserverI* observer);
    /**
    * Detach an observer from this Graph.
    * The observer will not be warned of changes anymore.
    */
    void unregisterObserver(bpp::GraphObserverI* observer);
    ///@}



    /** @name Nodes Functions
    *  These methodes of the graph concern the node management.
    */
    ///@{
    /**
    * Get all the neighbors of a node in the graph.
    * @param node the node one wants to get its neighbors
    * @return a vector containing the neighbors
    */
    const std::vector<Node> getNeighbors(Node node);
    /**
    * In an directed graph, get all the neighbors which
    * are leaving a node in the graph.
    * @param node the node one wants to get its neighbors
    * @return a vector containing the outgoing neighbors
    */
    const std::vector<Node> getOutgoingNeighbors(Node node);
    /**
    * In an directed graph, get all the neighbors which
    * are coming to a node in the graph.
    * @param node the node one wants to get its neighbors
    * @return a vector containing the incoming neighbors
    */
    const std::vector<Node> getIncomingNeighbors(Node node);
    /**
    * Get the leaves of a graph, ie, nodes with only one neighbor,
    * starting from a peculiar node.
    * @param node the starting node
    * @param maxDepth the maximum number of allowed depth, 0 means no max.
    * @return a vector containing the leaves
    */
    const std::vector<Node> getLeavesFromNode(Node node, unsigned int maxDepth = 0);
    /**
    * Get all the leaves of a graph, ie, nodes with only one neighbor,
    * @return a vector containing the leaves
    */
    const std::vector<Node> getLeaves();
    
    ///@}


    /** @name Edge Functions
    *  These methodes of the graph concern the edges.
    */
    ///@{
    /**
    * Returns the Edge between two nodes
    * @param nodes a pair of implied nodes (if directed graph nodeA then nodeB)
    * @return the edge between these two nodes
    */
    const Edge getEdge(bpp::SimpleGraph::Node nodeA, bpp::SimpleGraph::Node nodeB);
    /**
    * Returns the Edge between two nodes
    * @param nodes a pair of implied nodes (if directed graph nodeA then nodeB)
    * @return the edge between these two nodes
    */
    const std::vector<Edge> getEdges(Node node);
    ///@}


    /** @name Updating the changes on the observers
    *  These methodes aim to trigger some changes to the observers
    */
    ///@{

    /**
    * Trigger E objects deleting on the observers
    * @param edgesToDelete list of edges to delete
    */
    void notifyDeletedEdges(std::vector<SimpleGraph::Edge> edgesToDelete);

    /**
    * Trigger N objects deleting on the observers
    * @param nodesToDelete list of edges to delete
    */
    void notifyDeletedNodes(std::vector< SimpleGraph::Node > nodesToDelete);


    ///@}
    
};

}

#else

namespace bpp {
class Graph;
}

#endif

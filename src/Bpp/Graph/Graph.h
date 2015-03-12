#ifndef _GRAPH_H_
#define _GRAPH_H_

#include<set>
#include<map>
#include<string>
#include<vector>
#include<utility>
#include<ostream>



// forward declaration to avoid circular dependancies.
// since we do not need its size in this header file (only using pointers to it)
namespace bpp
{
class GraphObserver;
}

namespace bpp
{
  class Graph
  {
  public:
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
    std::set<GraphObserver*> observers_;

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
    * The node structure type
    * Node -> ("toNodes" [DestNode,Edge],"fromNodes" [DestNode,Edge])
    * directed example: (N1)-E1->(N2)-E2->(N3) is coded as
    *     N1 -> ((N2,E1),())
    *     N2 -> ((N3,E3),(N1,E1))
    *     N3 -> ((),(N2,E2))
    */
    typedef std::map<Node,std::pair<std::map<Node,Edge>,std::map<Node,Edge> > > nodeStructureType;

   /**
    * The edge structure type
    * directed example: N1--E1-->N2 is coded as E1 -> (N1,N2)
    * undirected example: N1--E1--N2 is coded as E1 -> (N1,N2)
    */
    typedef std::map<Edge,std::pair<Node,Node> > edgeStructureType;

    
    /**
    * Nodes and their relations.
    * TODO: explain the relation coding
    * nodeA -> [(nodeB, relationID),…]
    */
    nodeStructureType nodeStructure_;
    
    
    /**
    * Edges and their relations in the forward direction..
    * TODO: explain the relation coding
    * edgeA -> nodeA
    */
    edgeStructureType edgeStructure_;
    
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
    void linkInNodeStructure_(Node nodeA, Node nodeB, Edge edge, bool toBackwards);
    
    /**
    * Creates a link between two existing nodes in the edge structure.
    * If directed graph: nodeA -> nodeB.
    * Mainly called by link().
    * @param nodeA source node (or first node if undirected)
    * @param nodeB target node (or second node if undirected)
    * @param edge the ID of the relation
    */
    void linkInEdgeStructure_(Node nodeA, Node nodeB, Edge edge);


    /**
    * Erase a link between two existing nodes. If directed graph: nodeA -> nodeB.
    * Private version of unLink, does not check for the reciprocity.
    * Mainly called by unLink().
    * @param nodeA source node (or first node if undirected)
    * @param nodeB target node (or second node if undirected)
    * @param toBackwards if this link has to be UNdone in the backwardsStructure (for directed graphs)
    * @return the ID of the erased relation
    */
    Edge unlinkInNodeStructure_(Node nodeA, Node nodeB, bool toBackwards);

    /**
    * Erase a link between two existing nodes in the Edge structure.
    * Mainly called by unLink().
    * @param edge the edge to unregister
    */
    void unlinkInEdgeStructure_(Edge edge);
    
    
    /**
    * Check that a node exists. If not, throw an exception.
    * @param node node that has to be checked
    * @param name common name to give to the user in case of failure (eg: "first node")
    */
    void checkNodeExistence_(Node node, std::string name);
    
  /**
    * Check that a edge exists. If not, throw an exception.
    * @param edge edge that has to be checked
    * @param name common name to give to the user in case of failure (eg: "first node")
    */
    void checkEdgeExistence_(Edge edge, std::string name);

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
    
    
    
  /**
   * output a node to DOT format (recursive)
   */
  
  void nodeToDot_(Node node, std::ostream &out);
    
    
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
    * Creates a node linked to an existing node.
    * @param origin existing node. In a directed graph: origin -> newNode.
    * @return the new node
    */
    const Node createNodeFromNode(Node origin);
    
    /**
    * Creates new node on an existing Edge. A -> B will be A -> N -> B
    * @param edge existing edge.
    * @return the new node
    */
    const Node createNodeOnEdge(Edge edge);
    
    /**
    * Creates a node linked to new node, splitting an edge.
    * @param origin existing edge. In a directed graph: origin -> newNode.
    * @return the new node
    */
    const Node createNodeFromEdge(Edge origin);
    
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
    void registerObserver(bpp::GraphObserver* observer);
    /**
    * Detach an observer from this Graph.
    * The observer will not be warned of changes anymore.
    */
    void unregisterObserver(bpp::GraphObserver* observer);
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
    
    /**
     * Output the graph in DOT format
     */
    void outputToDot(std::ostream &out);
    
};

}

#else

namespace bpp {
class Graph;
}

#endif

#ifndef _ASSOCIATIONTREEGRAPHOBSERVER_HPP_
#define _ASSOCIATIONTREEGRAPHOBSERVER_HPP_

#include "TreeGraph.h"
#include "AssociationGraphObserver.h"

#include <vector>
#include <map>
#include <iostream>
#include <ostream>
#include <memory>

namespace bpp
{
/**
 * @brief Defines a Graph Associator. It is a template which follows
 * (subscribed to) a Graph.
 *
 * @author Thomas Bigot
 */

// interface

template<class N, class E>
class AssociationTreeGraphObserver :
  public virtual AssociationGraphObserver<N, E>
{};


template<class N, class E, class TreeGraphImpl>
class SimpleAssociationTreeGraphObserver :
  public virtual AssociationTreeGraphObserver<N, E>,
  public virtual SimpleAssociationGraphObserver<N, E, TreeGraphImpl>
{
public:
  typedef typename AssociationGraphObserver<N, E>::NodeIndex NodeIndex;
  typedef typename AssociationGraphObserver<N, E>::EdgeIndex EdgeIndex;

  typedef typename Graph::Node NodeGraphid;
  typedef typename Graph::Edge EdgeGraphid;

public:
  /**
   * Constructor
   * @param directed is the graph directed
   */
  SimpleAssociationTreeGraphObserver(bool rooted = false);

  /**
   * Constructor
   * @param subjectGraph the graph which is observed
   */
  SimpleAssociationTreeGraphObserver(std::shared_ptr<TreeGraphImpl> subjectTreeGraph = 00);

  /**
   * Copy Constructor
   * @param graphObserver the treeGraphObserver to be copied
   */
  SimpleAssociationTreeGraphObserver(bpp::SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl> const& treeGraphObserver);

  /**
   * = Operator
   * @param graphObserver the treeGraphObserver we want to copy the values
   */
  SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl> operator=(bpp::SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl> const& treeGraphObserver);

  /**
   * Destructor
   * @param graphObserver the treeGraphObserver to be copied
   */
  ~SimpleAssociationTreeGraphObserver();


  /**
   * clone function
   * @param graphObserver the graphObserver to be copied
   */
  SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>* clone() const { return new SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>(*this); }


  /**
   * Is the graph a tree? A tree must be acyclic and with no isolated node.
   * @return true if valid tree
   */
  bool isValid() const;

  /**
   * Return, in a rooted tree, the branch leading to the father
   * @param nodeObject the concerned node
   * @return an Edge which is the branch to the father
   */
  std::shared_ptr<E>  getEdgeToFather(const std::shared_ptr<N>  nodeObject) const;

  /**
   * Is the subject tree rooted?
   */
  bool isRooted() const;

  /**
   * @brief Sets the root and make the tree directed from root to leaves
   *
   */

  void rootAt(const std::shared_ptr<N> root);

  /*
   * @brief Supersedes AssociationGraphObserver function, to
   * avoid incoherent treeS
   *
   */
  
  void setRoot(const std::shared_ptr<N> newRoot)
  {
    rootAt(newRoot);
  }

  /**
   * @return the root
   */

  std::shared_ptr<N> getRoot() const;

  Graph::Node getRootId() const;

  /**
   * Return, in a rooted tree, the father node
   * @param nodeObject the concerned node
   * @return the father
   */
  std::shared_ptr<N>  getFather(const std::shared_ptr<N>  nodeObject) const;

  /**
   * Has the node a father?
   */
  bool hasFather(const std::shared_ptr<N>  nodeObject) const;

  /**
   * Return, in a rooted tree, the sons of a node 
   * @param nodeObject the concerned node
   * @return a vector of son Nodes
   */
  
  std::vector<std::shared_ptr<N> > getSons(const std::shared_ptr<N>  node) const;
  std::vector<NodeIndex> getSons(const NodeIndex node) const;

  /**
   * Return, in a rooted tree, the son of an edge
   * @param nodeObject the concerned node
   * @return the son Node
   */
  
  std::shared_ptr<N> getSon(const std::shared_ptr<E>  edge) const;
  NodeIndex getSon(const EdgeIndex edge) const;

  /**
   * Return, in a rooted tree, the father of an edge
   * @param nodeObject the concerned node
   * @return the father Node
   */
  
  std::shared_ptr<N> getFather(const std::shared_ptr<E>  edge) const;
  NodeIndex getFather(const EdgeIndex edge) const;

  /**
   * Return, in a rooted tree, the number of sons
   * @param nodeObject the concerned node
   * @return the number of sons
   */
  size_t getNumberOfSons(const std::shared_ptr<N>  node) const;

  /**
   * Return the number of leaves under a certain node
   * @param nodeObject the concerned node
   * @return the number of leaves
   */
  size_t getNumberOfLeaves(const std::shared_ptr<N>  node) const;

  /**
   * Return the number of leaves
   * @return the number of leaves
   */

  size_t getNumberOfLeaves() const;

  /**
   * Remove the sons of a node
   * @return a vector containing the removed nodes
   */
  std::vector<std::shared_ptr<N> > removeSons(const std::shared_ptr<N>  node);

  /**
   * Remove a sons of a node
   * @return a vector containing the removed nodes
   */

  void removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son);
  
  /**
   * Change / set the father of a node
   * @param nodeObject the concerned node
   * @param fatherNodeObject the node to be the father
   */
  void setFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  fatherNodeObject);

  /**
   * Add a son to a node
   * @param nodeObject the concerned node
   * @param sonNodeObject the node to be added as a son to the father
   */
  void addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  sonNodeObject);


  // / FROM TREETOOLS


  /**
   * @brief Get a vector of ancestor nodes between to nodes.
   *
   * @param nodeId1 first node.
   * @param nodeId2 second node.
   * @param includeAncestor Tell if the common ancestor must be included in the vector.
   * @return A vector of ancestor nodes ids.
   * @throw PhyloNodeNotFoundException If a node is not found.
   */
  std::vector<std::shared_ptr<N> > getNodePathBetweenTwoNodes(const std::shared_ptr<N>  nodeObjectA, const std::shared_ptr<N>  nodeObjectB, bool includeAncestor = true) const;

  std::vector<std::shared_ptr<E> > getEdgePathBetweenTwoNodes(const std::shared_ptr<N>  nodeObjectA, const std::shared_ptr<N>  nodeObjectB) const;

  std::vector<std::shared_ptr<N> > getSubtreeNodes(const std::shared_ptr<N> localRoot);
  std::vector<std::shared_ptr<E> > getSubtreeEdges(const std::shared_ptr<N> localRoot);
};


template<class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::SimpleAssociationTreeGraphObserver(bool rooted_p) :
  SimpleAssociationGraphObserver<N, E, TreeGraphImpl>::SimpleAssociationGraphObserver(rooted_p)
{
}


template<class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::SimpleAssociationTreeGraphObserver(SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl> const& treeGraphObserver) :
  SimpleAssociationGraphObserver<N, E, TreeGraphImpl>::SimpleAssociationGraphObserver(treeGraphObserver)
{
}


template<class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl> SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::operator=(SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl> const& treeGraphObserver)
{
  SimpleAssociationGraphObserver<N, E, TreeGraphImpl>::operator=(treeGraphObserver);
}

template<class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::~SimpleAssociationTreeGraphObserver()
{
}


template<class N, class E, class TreeGraphImpl>
bool SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::isValid() const
{
  return this->getGraph()->isValid();
}

template<class N, class E, class TreeGraphImpl>
bool SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::isRooted() const
{
  return this->getGraph()->isRooted();
}

template<class N, class E, class TreeGraphImpl>
void SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::rootAt(const std::shared_ptr<N> root)
{
  this->getGraph()->rootAt(this->getNodeGraphid(root));
}

template<class N, class E, class TreeGraphImpl>
std::shared_ptr<N>  SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getRoot() const
{
  return this->getNodeFromGraphid(this->getGraph()->getRoot());
}

template<class N, class E, class TreeGraphImpl>
bool SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::hasFather(const std::shared_ptr<N>  nodeObject) const
{
  return this->getGraph()->hasFather(this->getNodeGraphid(nodeObject));
}

template<class N, class E, class TreeGraphImpl>
std::shared_ptr<N>  SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getFather(const std::shared_ptr<N>  nodeObject) const
{
  return this->getNodeFromGraphid(this->getGraph()->getFather(this->getNodeGraphid(nodeObject)));
}

template<class N, class E, class TreeGraphImpl>
std::shared_ptr<E>  SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getEdgeToFather(const std::shared_ptr<N>  nodeObject) const
{
  return this->getEdgeFromGraphid(this->getGraph()->getEdgeToFather(this->getNodeGraphid(nodeObject)));
}

template<class N, class E, class TreeGraphImpl>
std::vector<std::shared_ptr<N> > SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getSons(const std::shared_ptr<N>  nodeObject) const
{
  return this->getNodesFromGraphid(this->getGraph()->getSons(this->getNodeGraphid(nodeObject)));
}

template<class N, class E, class TreeGraphImpl>
std::vector<typename SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::NodeIndex> SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getSons(SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::NodeIndex node) const
{
  return getNodeIndexes(this->getNodesFromGraphid(this->getGraph()->getSons(this->getNodeGraphid(getNode(node)))));
}

  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<N> SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getSon(const std::shared_ptr<E>  edgeObject) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getBottom(this->getEdgeGraphid(edgeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  typename SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::NodeIndex SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getSon(SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::EdgeIndex edge) const
  {
    return getNodeIndex(this->getNode(this->getGraph()->getBottom(this->getEdgeGraphid(getEdge(edge)))));
  }

  template<class N, class E, class TreeGraphImpl>
  std::shared_ptr<N> SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getFather(const std::shared_ptr<E>  edgeObject) const
  {
    return this->getNodeFromGraphid(this->getGraph()->getTop(this->getEdgeGraphid(edgeObject)));
  }

  template<class N, class E, class TreeGraphImpl>
  typename SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::NodeIndex SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getFather(SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::EdgeIndex edge) const
  {
    return getNodeIndex(this->getNode(this->getGraph()->getTop(this->getEdgeGraphid(getEdge(edge)))));
  }

template<class N, class E, class TreeGraphImpl>
size_t SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getNumberOfSons(const std::shared_ptr<N>  nodeObject) const
{
  return this->getGraph()->getNumberOfSons(this->getNodeGraphid(nodeObject));
}

template<class N, class E, class TreeGraphImpl>
size_t SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getNumberOfLeaves(const std::shared_ptr<N>  nodeObject) const
{
  return this->getNodesFromGraphid(this->getGraph()->getLeavesFromNode(this->getNodeGraphid(nodeObject))).size();
}

  template<class N, class E, class TreeGraphImpl>
  size_t SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getNumberOfLeaves() const
  {
    return SimpleAssociationGraphObserver<N, E, TreeGraphImpl>::getNumberOfLeaves();
  }

template<class N, class E, class TreeGraphImpl>
void SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::setFather(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  fatherNodeObject)
{
  this->getGraph()->setFather(this->getNodeGraphid(nodeObject), this->getNodeGraphid(fatherNodeObject));
}

template<class N, class E, class TreeGraphImpl>
void SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::addSon(const std::shared_ptr<N>  nodeObject, const std::shared_ptr<N>  sonNodeObject)
{
  this->getGraph()->addSon(this->getNodeGraphid(nodeObject), this->getNodeGraphid(sonNodeObject));
}

template<class N, class E, class TreeGraphImpl>
std::vector<std::shared_ptr<N> > SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::removeSons(const std::shared_ptr<N> node)
{
  return this->getNodesFromGraphid(this->getGraph()->removeSons(this->getNodeGraphid(node)));
}

template<class N, class E, class TreeGraphImpl>
void SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::removeSon(const std::shared_ptr<N> node, const std::shared_ptr<N> son)
{
  this->getGraph()->removeSon(this->getNodeGraphid(node), this->getNodeGraphid(son));
}

template<class N, class E, class TreeGraphImpl>
std::vector<std::shared_ptr<N> > SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getNodePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB, bool includeAncestor) const
{
  return this->getNodesFromGraphid(this->getGraph()->getNodePathBetweenTwoNodes(this->getNodeGraphid(nodeA), this->getNodeGraphid(nodeB), includeAncestor));
}

template<class N, class E, class TreeGraphImpl>
std::vector<std::shared_ptr<E> > SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getEdgePathBetweenTwoNodes(const std::shared_ptr<N>  nodeA, const std::shared_ptr<N>  nodeB) const
{
  return getEdgesFromGraphid(this->getGraph()->getEdgePathBetweenTwoNodes(this->getNodeGraphid(nodeA), this->getNodeGraphid(nodeB)));
}

template<class N, class E, class TreeGraphImpl>
std::vector<std::shared_ptr<N> > SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getSubtreeNodes(const std::shared_ptr<N> localRoot)
{
  return this->getNodesFromGraphid(this->getGraph()->getSubtreeNodes(this->getNodeGraphid(localRoot)));
}

template<class N, class E, class TreeGraphImpl>
std::vector<std::shared_ptr<E> > SimpleAssociationTreeGraphObserver<N, E, TreeGraphImpl>::getSubtreeEdges(const std::shared_ptr<N>  localRoot)
{
  return SimpleAssociationGraphObserver<N, E, TreeGraphImpl>::getEdgesFromGraphid(this->getGraph()->getSubtreeEdges(this->getNodeGraphid(localRoot)));
}
}

#else

namespace bpp
{class AssociationTreeGraphObserver; }

#endif

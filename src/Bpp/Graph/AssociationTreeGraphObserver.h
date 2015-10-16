#ifndef _ASSOCIATIONTREEGRAPHOBSERVER_HPP_
#define _ASSOCIATIONTREEGRAPHOBSERVER_HPP_

#include "TreeGraph.h"
#include "AssociationGraphObserver.h"

#include <vector>
#include <map>
#include <iostream>
#include <ostream>


namespace bpp
{
    
    /**
   * @brief Defines a Graph Associator. It is a template which follows
   * (subscribed to) a Graph.
   *
   * @author Thomas Bigot
   */
  
    // interface
    
    template <class N, class E>
    class AssociationTreeGraphObserver:
    public virtual AssociationGraphObserver<N,E>
    {
      
    
    };
    
    
    template <class N, class E, class TreeGraphImpl>
    class SimpleAssociationTreeGraphObserver:
    public virtual AssociationTreeGraphObserver<N,E>,
    public virtual SimpleAssociationGraphObserver<N,E,TreeGraphImpl>
    {

    private:
      TreeGraphImpl* subjectTreeGraph_;
      

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
      SimpleAssociationTreeGraphObserver(TreeGraphImpl* subjectTreeGraph = 00);
      
      /**
      * Copy Constructor
      * @param graphObserver the treeGraphObserver to be copied
      */
      SimpleAssociationTreeGraphObserver(bpp::SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl> const& treeGraphObserver);
      
      /**
      * = Operator
      * @param graphObserver the treeGraphObserver we want to copy the values
      */
      SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl> operator=(bpp::SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl> const& treeGraphObserver);
      
      /**
      * Destructor
      * @param graphObserver the treeGraphObserver to be copied
      */
      ~SimpleAssociationTreeGraphObserver();
      
      
      
      /**
      * clone function
      * @param graphObserver the graphObserver to be copied
      */
      #ifdef NO_VIRTUAL_COV
        Clonable*
      #else
        SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>*
      #endif
      clone() const { return new SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>(*this); };
      
      
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
     E* getBranchToFather(const N* nodeObject) const;
      
     /**
      * Is the subject tree rooted?
      */
     bool isRooted() const;
     
     
     /**
     * Return, in a rooted tree, the father node
     * @param nodeObject the concerned node
     * @return the father
     */
     N* getFather(const N* nodeObject) const;
     
     /**
      * Has the node a father?
      */
     bool hasFather(const N* nodeObject) const;
     
     
    /**
     * Return, in a rooted tree, the branch leading to the father
     * @param nodeObject the concerned node
     * @return an Edge which is the branch to the father
     */
     std::vector<N*> getSons(const N* nodeObject) const;
     
     /**
     * Return, in a rooted tree, the branch leading to the father
     * @param nodeObject the concerned node
     * @return an Edge which is the branch to the father
     */
     unsigned int getNumberOfSons(const N* nodeObject) const;
     
     
     /**
     * Change / set the father of a node
     * @param nodeObject the concerned node
     * @param fatherNodeObject the node to be the father
     */
     void setFather(const N* nodeObject,const N* fatherNodeObject);
     
     
    };
    


template <class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::SimpleAssociationTreeGraphObserver(bool rooted_p):
SimpleAssociationGraphObserver<N,E,TreeGraphImpl>::SimpleAssociationGraphObserver(rooted_p),
subjectTreeGraph_(00)
{
  subjectTreeGraph_ = static_cast<TreeGraphImpl*>(SimpleAssociationGraphObserver<N,E,TreeGraphImpl>::getGraph());
}


template <class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::SimpleAssociationTreeGraphObserver(SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl> const& treeGraphObserver):
SimpleAssociationGraphObserver<N,E,TreeGraphImpl>::SimpleAssociationGraphObserver(treeGraphObserver),
subjectTreeGraph_(00)
{
}


template <class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl> SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::operator=(SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl> const& treeGraphObserver)
{
  SimpleAssociationGraphObserver<N,E,TreeGraphImpl>::operator=(treeGraphObserver);
}

template <class N, class E, class TreeGraphImpl>
SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::~SimpleAssociationTreeGraphObserver()
{
  //delete subjectTreeGraph_;
}

 
template <class N, class E, class TreeGraphImpl>
bool SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::isValid() const
{
    return subjectTreeGraph_->isValid();
}

template <class N, class E, class TreeGraphImpl>
bool SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::isRooted() const
{
    return subjectTreeGraph_->isRooted();
}

template <class N, class E, class TreeGraphImpl>
bool SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::hasFather(const N* nodeObject) const
{
    return subjectTreeGraph_->hasFather(getNodeId(nodeObject));
}

template <class N, class E, class TreeGraphImpl>
N* SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::getFather(const N* nodeObject) const
{
    return getNode(subjectTreeGraph_->getFather(getNodeId(nodeObject)));
}

template <class N, class E, class TreeGraphImpl>
E* SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::getBranchToFather(const N* nodeObject) const
{
    return(getEdgeObject(subjectTreeGraph_->getBranchToFather(getNodeId(nodeObject))));
}

template <class N, class E, class TreeGraphImpl>
std::vector<N*> SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::getSons(const N* nodeObject) const
{
    return getNodes(subjectTreeGraph_->getSons(getNodeId(nodeObject)));
}

template <class N, class E, class TreeGraphImpl>
unsigned int SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::getNumberOfSons(const N* nodeObject) const
{
    return getNodes(subjectTreeGraph_->getSons(getNodeId(nodeObject))).size();
}
 
template <class N, class E, class TreeGraphImpl>
void SimpleAssociationTreeGraphObserver<N,E,TreeGraphImpl>::setFather(const N* nodeObject, const N* fatherNodeObject)
{
    subjectTreeGraph_->setFather(getNodeId(nodeObject),getNodeId(fatherNodeObject));
}
 
}

#else

namespace bpp {class AssociationTreeGraphObserver;}

#endif
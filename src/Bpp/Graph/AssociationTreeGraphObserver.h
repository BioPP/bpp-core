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

 
 
 
}

#else

namespace bpp {class AssociationTreeGraphObserver;}

#endif
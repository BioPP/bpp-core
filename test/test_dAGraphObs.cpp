//
// File: test_dAGraphObs.cpp
// Created by: Laurent Guéguen
// Created on: 2016
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for numerical calculus. This file is part of the Bio++ project.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#include "../src/Bpp/Graph/DAGraph.h"
#include "../src/Bpp/Graph/AssociationDAGraphImplObserver.h"

#include <vector>
#include <iostream>
using namespace bpp;
using namespace std;

int main() {
  AssociationDAGlobalGraphObserver<string,unsigned int> grObs;

  shared_ptr<string> zero(new string("zero"));
  shared_ptr<string> one(new string("one"));
  shared_ptr<string> two(new string("two"));
  shared_ptr<string> three(new string("three"));
  shared_ptr<string> four(new string("four"));
  shared_ptr<string> five(new string("five"));
  shared_ptr<string> six(new string("six"));
  shared_ptr<unsigned int> r3(new unsigned int(3));
  shared_ptr<unsigned int> r1(new unsigned int(5));

  cout << "Creating node zero." << endl;
  grObs.createNode(zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Creating node one from the number zero." << endl;
  grObs.createNode(one);
  grObs.addFather(one,zero,r1);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << "Creating node two from the number one." << endl;
  grObs.createNode(one,two);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Linking two to zero." << endl;
  grObs.addFather(zero,two,r3);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");  


  cout << "Linking one to three and two to four." << endl;
  grObs.createNode(one,three);
  grObs.createNode(two,four);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << endl;

  cout << "Linking three to zero." << endl;
  grObs.addFather(zero,three);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  // so now we have zero -> one -> two -> zero ...
  vector<shared_ptr<string> > fromOne = grObs.getOutgoingNeighbors(zero);
  vector<shared_ptr<string> > fromThree = grObs.getOutgoingNeighbors(two);
  bool test = (*(fromOne.begin()) == one) && (*(fromThree.begin()) == zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << "Is this a DAG?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  
  // the DAG must be considered as unvalid at this point
  test &= !grObs.isValid();
  
  
  cout << "Removing 2->0 and 3->0"<< endl;
  grObs.removeSon(two,zero);
  grObs.removeSon(three,zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << "Is this a DAG?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the DAG is valid at this point
  test &= grObs.isValid();

  cout << endl;
  cout << "Iteration on sons  of 1:" << endl;
  
  unique_ptr<AssociationDAGraphObserver<string, unsigned int>::NodeIterator> in0It=grObs.sonsIterator(one);

  for (;!in0It->end();in0It->next())
    cout << ***in0It << endl;

  cout << "Reroot on one" << endl;

  grObs.rootAt(one);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << "Linking 3->2"<< endl;
  grObs.addFather(three, two);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");

  cout << endl;

  cout << "Is this a DAG?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the DAG must be considered as valid at this point
  test &= grObs.isValid();
  cout << endl;

  cout << "Add nodes five and six" << endl;
  grObs.createNode(five);
  grObs.createNode(six);
  grObs.addFather(four,five);
  grObs.addFather(five,six);
  grObs.addFather(six,four);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");

  cout << endl;

  cout << "Is this a DAG?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the tree must be considered as unvalid at this point
  test &= !grObs.isValid();
  cout << endl;

  cout << "Reroot on two" << endl;

  grObs.rootAt(two);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");

  cout << "Is this a DAG?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the tree must be considered as unvalid at this point
  test &= grObs.isValid();
  cout << endl;

  cout << "Test " << (test? "passed":"failed") << endl;

  return (test ? 0 : 1);
}

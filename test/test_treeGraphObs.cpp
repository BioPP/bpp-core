// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../src/Bpp/Graph/TreeGraph.h"
#include "../src/Bpp/Graph/AssociationTreeGraphImplObserver.h"


#include <vector>
#include <iostream>
using namespace bpp;
using namespace std;

int main() {
  AssociationTreeGlobalGraphObserver<string,unsigned int> grObs(true);

  AssociationTreeGlobalGraphObserver<unsigned int,unsigned int> grObs2(true);

  shared_ptr<string> zero(new string("zero"));
  shared_ptr<string> one(new string("one"));
  shared_ptr<string> two(new string("two"));
  shared_ptr<string> three(new string("three"));
  shared_ptr<string> four(new string("four"));
  shared_ptr<string> five(new string("five"));
  shared_ptr<unsigned int> r3(new unsigned int(3));
  shared_ptr<unsigned int> r1(new unsigned int(5));
  shared_ptr<unsigned int> r2(new unsigned int(10));

  cout << "Creating node zero." << endl;
  grObs.createNode(zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Creating node one from the number zero." << endl;
  grObs.createNode(one);
  grObs.link(zero,one,r2);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "The father branch of zero is " << *(grObs.getEdgeToFather(one)) << endl;
  cout << "The father node of one is " << *(grObs.getFatherOfNode(one)) << endl;
  
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  
  cout << "Creating node two from the number one." << endl;
  grObs.createNode(one,two);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Linking one to three and two to four." << endl;
  grObs.createNode(one,three);
  grObs.createNode(two,four,r1);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << endl << "------------------------------------------" << endl << endl;

  cout << "Linking two and three to zero." << endl;
  grObs.link(two,zero,r3);
  grObs.link(three,zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGraph");

  vector<shared_ptr<string> > fromOne = grObs.getOutgoingNeighbors(zero);
  vector<shared_ptr<string> > fromThree = grObs.getOutgoingNeighbors(two);
  bool test = (*(fromOne.begin()) == one) && (*(fromThree.begin()) == zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
 
  cout << "Is this a tree?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  
  // the tree must be considered as unvalid at this point
  test &= !grObs.isValid();
  
  cout << "Removing 2->0 and 3->0"<< endl;
  grObs.unlink(two,zero);
  grObs.unlink(three,zero);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << "Is this a tree?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the tree is valid at this point
  test &= grObs.isValid();

  cout << endl;
  cout << "Iteration on sons  of 1:" << endl;
  
  unique_ptr<AssociationTreeGraphObserver<string, unsigned int>::NodeIterator> in0It=grObs.sonsIterator(one);

  for (;!in0It->end();in0It->next())
  {
    cout << ***in0It << endl;
  }

  cout << endl;
  cout << " Outgoing edges  of 2:" << endl;
 
  vector<shared_ptr<unsigned int> > ve=grObs.getBranches(two);
  for (size_t i=0; i<ve.size();i++)
    cout << *ve[i] << endl;

  cout << endl;
  
  cout << "Iteration on outgoing edges  of 2:" << endl;
  unique_ptr<AssociationTreeGraphObserver<string, unsigned int>::EdgeIterator> oe1It=grObs.branchesIterator(two);

  for (;!oe1It->end();oe1It->next())
  {
    cout << ***oe1It << endl;
  }

  cout << endl ;

  // add another root

  cout << "Add another root " << endl;
  
  grObs.createNode(five);
  grObs.link(five,two);

  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  grObs.outputToDot(std::cout,"myTestDirGrObs");

  cout << "Is this a tree?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the tree is not valid at this point
  test &= !grObs.isValid();

  cout << endl << "------------------------------------------" << endl << endl;

  cout << "Reroot on one" << endl;

  try {
    grObs.rootAt(one);
  }
  catch(Exception& e)
  {
    cout << "First undirect the tree." << endl;
    grObs.getGraph()->makeUndirected();
    grObs.getGraph()->outputToDot(std::cout,"undirected");
    cout << "Then root." << endl;
    grObs.rootAt(one);
  }

  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  grObs.outputToDot(std::cout,"myTestDirGrObs");

  cout << endl << "------------------------------------------" << endl << endl;
  
  cout << "Linking 3->2"<< endl;
  grObs.link(three, two);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Is this a tree?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the tree must be considered as unvalid at this point
  test &= !grObs.isValid();

  cout << endl << "------------------------------------------" << endl << endl;

  cout << "Remove a branch between 1 & 3" << endl;
  grObs.unlink(one, three);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Is this a tree?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl;
  // the tree must be considered as unvalid at this point
  test &= !grObs.isValid();

  cout << endl << "------------------------------------------" << endl << endl;

  cout << "Reroot on one" << endl;

  grObs.getGraph()->makeUndirected();
  grObs.rootAt(one);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  cout << "Is this a tree?\n    " << (grObs.isValid()? "TRUE":"FALSE") << endl << endl;
  // the tree must be considered as valid at this point
  test &= grObs.isValid();
  
  cout << "MRCA of 3 and 4: " << *grObs.MRCA({three, four})  << endl;
  cout << "MRCA of 3 and 0: " << *grObs.MRCA({three, zero})  << endl;
  

  cout << endl << "------------------------------------------" << endl << endl;

  cout << "Reroot on two" << endl;

  grObs.rootAt(two);
  grObs.getGraph()->outputToDot(std::cout,"myTestDirGrObs");
  
  cout << "Test " << (test? "passed":"failed") << endl;

  return (test ? 0 : 1);
}

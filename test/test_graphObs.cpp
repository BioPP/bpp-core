//
// File: test_bfgs.cpp
// Created by: Julien Dutheil
// Created on: Mon Jan 31 19:45 2011
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

#include "../src/Bpp/Graph/GraphObserver.h"
#include <vector>
#include <iostream>
using namespace bpp;
using namespace std;

int main() {
  GraphObserver<string,unsigned int> gos(true);
  string one = "one";
  string two = "two";
  string three = "three";
  unsigned int r3 = 3;
  cout << "Creating node one." << endl;
  gos.createNode(&one);
  cout << "Creating node two from the number one." << endl;
  gos.createNode(&two,&one);
  cout << "Creating node three from the number two." << endl;
  gos.createNode(&three,&two);
  cout << "Linking three to two." << endl;
  gos.link(&three,&one,&r3);
  // so now we have one -> two -> three -> one ...
  vector<string*> fromOne = gos.getOutgoingNeighbors(&one);
  vector<string*> fromThree = gos.getOutgoingNeighbors(&three);
  bool test = (*(fromOne.begin()) == &two) && (*(fromThree.begin()) == &one);
  return (test ? 0 : 1);
}

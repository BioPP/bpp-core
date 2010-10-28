//
// File: test_derivative1.cpp
// Created by: Julien Dutheil
// Created on: Thu Oct 28 12:49 2010
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

#include <Bpp/Numeric/Function.all>
#include <Bpp/Numeric/RandomTools.h>
#include <vector>
#include <iostream>
#include "PolynomialFunction.h"

using namespace bpp;
using namespace std;

int main() {
  double h = 1e-6; 
  PolynomialFunction1 f;
  PolynomialFunction1Der1 f1;
  ParameterList pl = f.getParameters();
  
  for (unsigned int repeat = 0; repeat < 10000; ++repeat) {
    for (unsigned int i = 0; i < pl.size(); ++i) {
      double val = RandomTools::giveRandomNumberBetweenZeroAndEntry(100) - 50;
      pl[i].setValue(val);
    }
    f.setParameters(pl);
    vector<double> derivativesNum(pl.size());
    for (unsigned int i = 0; i < pl.size(); ++i) {
      double bck = pl[i].getValue();
      pl[i].setValue(bck - h);
      f.setParameters(pl);
      double val1 = f.getValue();
      pl[i].setValue(bck + h);
      f.setParameters(pl);
      double val2 = f.getValue();
      pl[i].setValue(bck);
      f.setParameters(pl);
      derivativesNum[i] = (val2 - val1) / (2*h);
    }

    vector<double> derivativesAna(pl.size());
    f1.setParameters(pl);
    bool test = true;
    for (unsigned int i = 0; i < pl.size(); ++i) {
      derivativesAna[i] = f1.getFirstOrderDerivative(pl.getParameterNames()[i]);
      if (abs(derivativesAna[i] - derivativesNum[i]) > 1e-3)
        test = false;
    }

    //Test:
    if (!test) {
      //Failure!
      for (unsigned int i = 0; i < pl.size(); ++i) {
        cout << setprecision(20) << pl[i].getName() << "=" << pl[i].getValue() << "\tAna. Der.=" << derivativesAna[i] << "\tNum. Der.=" << derivativesNum[i] << endl;
      }
      return 1;
    }
  }
  return 0;
}

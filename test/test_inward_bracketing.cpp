//
// File: test_downhill.cpp
// Created by: Julien Dutheil
// Created on: Wed Oct 27 18:46 2010
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

#include <Bpp/Numeric/Function/BrentOneDimension.h>
#include <vector>
#include <iostream>
#include "PolynomialFunction.h"

using namespace bpp;
using namespace std;

int main() {
  PolynomialFunction1 f;
  double initFValue = f.getValue();
  cout << initFValue << endl;
  BrentOneDimension optimizer(&f);
  optimizer.setBracketing(BrentOneDimension::BRACKET_INWARD);
  ParameterList parameter;
  parameter.addParameter(f.getParameter("z"));
  const auto& parameterBounds = dynamic_pointer_cast<const IntervalConstraint>((parameter[0]).getConstraint());
  optimizer.setInitialInterval(parameterBounds->getLowerBound(), parameterBounds->getUpperBound()); // search within stricter bounds that the actual ones of pi0 to avoid failute of stochasitc mapping
  optimizer.init(parameter);
  optimizer.optimize();
  double minf = f.getValue();
  double x = f.getParameterValue("x");
  double y = f.getParameterValue("y");
  double z = f.getParameterValue("z");
  cout << "x=" << x << endl;
  cout << "y=" << y << endl;
  cout << "z=" << z << endl;
  cout << "f=" << minf << endl;
  bool test = f.getValue() < initFValue;
  return (test ? 0 : 1);
}

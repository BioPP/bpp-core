// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Function/BrentOneDimension.h>
#include <vector>
#include <iostream>
#include "PolynomialFunction.h"

using namespace bpp;
using namespace std;

int main() {
  auto f = make_shared<PolynomialFunction1>();
  double initFValue = f->getValue();
  cout << initFValue << endl;
  BrentOneDimension optimizer(f);
  optimizer.setBracketing(BrentOneDimension::BRACKET_INWARD);
  ParameterList parameter;
  parameter.addParameter(f->parameter("z"));
  const auto& parameterBounds = dynamic_pointer_cast<const IntervalConstraint>((parameter[0]).getConstraint());
  optimizer.setInitialInterval(parameterBounds->getLowerBound(), parameterBounds->getUpperBound()); // search within stricter bounds that the actual ones of pi0 to avoid failute of stochasitc mapping
  optimizer.init(parameter);
  optimizer.optimize();
  double minf = f->getValue();
  double x = f->getParameterValue("x");
  double y = f->getParameterValue("y");
  double z = f->getParameterValue("z");
  cout << "x=" << x << endl;
  cout << "y=" << y << endl;
  cout << "z=" << z << endl;
  cout << "f=" << minf << endl;
  bool test = f->getValue() < initFValue;
  return (test ? 0 : 1);
}

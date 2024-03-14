// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Function/BfgsMultiDimensions.h>
#include <Bpp/Numeric/AutoParameter.h>
#include <vector>
#include <iostream>
#include "PolynomialFunction.h"

using namespace bpp;
using namespace std;

int main() {
  auto f = make_shared<PolynomialFunction1Der1>(false);
  cout << f->getValue() << endl;
  BfgsMultiDimensions optimizer(f);
  optimizer.setConstraintPolicy(AutoParameter::CONSTRAINTS_AUTO);
  optimizer.init(f->getParameters());
  optimizer.optimize();
  double minf = optimizer.getFunctionValue();
  double x = f->getParameterValue("x");
  double y = f->getParameterValue("y");
  double z = f->getParameterValue("z");
  cout << "x=" << x << endl;
  cout << "y=" << y << endl;
  cout << "z=" << z << endl;
  cout << "f=" << minf << endl;
  cout << setprecision(20) << (abs(minf) + abs(x - 5) + abs(y + 2) + abs(z - 3)) << endl;
  bool test = abs(minf) + abs(x - 5) + abs(y + 2) + abs(z - 3) < 0.01;
  return (test ? 0 : 1);
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Function/DownhillSimplexMethod.h>
#include <Bpp/Numeric/AutoParameter.h>
#include <vector>
#include <iostream>
#include "PolynomialFunction.h"

using namespace bpp;
using namespace std;

int main() {
  auto f = make_shared<PolynomialFunction1>();
  cout << f->getValue() << endl;
  DownhillSimplexMethod optimizer(f);
  optimizer.setConstraintPolicy(AutoParameter::CONSTRAINTS_IGNORE);
  optimizer.init(f->getParameters());
  optimizer.optimize();
  double minf = f->getValue();
  double x = f->getParameterValue("x");
  double y = f->getParameterValue("y");
  double z = f->getParameterValue("z");
  cout << "x=" << x << endl;
  cout << "y=" << y << endl;
  cout << "z=" << z << endl;
  cout << "f=" << minf << endl;
  cout << setprecision(20) << (abs(minf) + abs(x - 5) + abs(y + 2) + abs(z - 3)) << endl;
  bool test = abs(minf) + abs(x - 5) + abs(y + 2) + abs(z - 3) < optimizer.getStopCondition()->getTolerance();
  return (test ? 0 : 1);
}

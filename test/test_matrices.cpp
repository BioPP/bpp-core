// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/Matrix/MatrixTools.h>
#include <vector>
#include <iostream>

using namespace bpp;
using namespace std;

int main() {
  RowMatrix<double> m(2,2);
  m(0,0) = 2.3;
  m(0,1) = 1.4;
  m(1,0) = 5.0;
  m(1,1) = -0.9;
  MatrixTools::print(m);
  RowMatrix<double> n(2, 2);
  MatrixTools::transpose(m, n),
  MatrixTools::print(n);
  RowMatrix<double> m2(2, 2);
  MatrixTools::transpose(n, m2);
  RowMatrix<double> o(2, 2);
  MatrixTools::mult(m, n, o);
  MatrixTools::print(o);
 
  bool test = m.equals(m2, 0.000001);
  ApplicationTools::displayBooleanResult("Test passed", test);
  return (test ? 0 : 1);
}

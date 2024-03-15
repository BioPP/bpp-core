// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/Matrix/MatrixTools.h>
#include <vector>
#include <iostream>

using namespace bpp;
using namespace std;

int main()
{
  RowMatrix<double> m(2, 2);
  m(0, 0) = 2.3;
  m(0, 1) = 1.4;
  m(1, 0) = 5.0;
  m(1, 1) = -0.9;
  EigenValue<double> eigen(m);
  RowMatrix<double> D  = eigen.getD();
  const vector<double> L  = eigen.getRealEigenValues();
  RowMatrix<double> V1 = eigen.getV();
  RowMatrix<double> V2;
  MatrixTools::inv(V1, V2);
  cout << "M=" << endl;
  MatrixTools::print(m);
  cout << "D=" << endl;
  MatrixTools::print(D);
  cout << "V1=" << endl;
  MatrixTools::print(V1);
  cout << "V2=" << endl;
  MatrixTools::print(V2);
  RowMatrix<double> test;
  MatrixTools::mult(V1, L, V2, test);
  cout << "V1 . D . V2=" << endl;
  MatrixTools::print(test);
  return test.equals(m) ? 0 : 1;
}

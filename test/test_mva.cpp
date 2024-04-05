// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>
#include <set>
#include <map>
#include <cmath>

using namespace std;

#include <Bpp/Numeric/Matrix/EigenValue.h>
#include <Bpp/Numeric/Matrix/MatrixTools.h>
#include <Bpp/Numeric/Matrix/Matrix.h>
#include <Bpp/Numeric/Stat/Mva/DualityDiagram.h>
#include <Bpp/Numeric/Stat/Mva/PrincipalComponentAnalysis.h>
#include <Bpp/Numeric/Stat/Mva/CorrespondenceAnalysis.h>
#include <Bpp/Numeric/VectorTools.h>

using namespace bpp;

/*This program performs some tests for the Pca, Coa and Dudi classes*/

int main(int args, char** argv)
{
  unsigned int nRows = 3;
  unsigned int nCols = 3;
  RowMatrix<double> matrix(nRows, nCols);
  RowMatrix<double> matrix2(nRows, nCols + 1);
  RowMatrix<double> matrix3(nRows, nCols);

  matrix(0, 0) = 10;
  matrix(1, 0) = 20;
  matrix(2, 0) = 30;
  matrix(0, 1) = 20;
  matrix(1, 1) = 10;
  matrix(2, 1) = 40;
  matrix(0, 2) = 30;
  matrix(1, 2) = 40;
  matrix(2, 2) = 10;

  cout << endl;
  cout << "First test for the Pca class, with a square matrix : " << endl;
  cout << endl;
  cout << "Here's the input matrix : " << endl;
  MatrixTools::print(matrix, cout);

  vector<double> rowW(nRows);
  vector<double> colW(nCols);
  VectorTools::fill(rowW, 1. / static_cast<double>(nRows));
  VectorTools::fill(colW, 1.);

  // The constructor with row and column weights is called
  PrincipalComponentAnalysis* pca1 = new PrincipalComponentAnalysis(matrix, 3, rowW, colW, true, true);

  cout << "The matrix of Row Coordinates : " << endl;
  MatrixTools::print(pca1->getRowCoordinates(), cout);
  cout << endl;
  cout << endl;


  matrix2(0, 0) = 10;
  matrix2(1, 0) = 20;
  matrix2(2, 0) = 30;
  matrix2(0, 1) = 20;
  matrix2(1, 1) = 10;
  matrix2(2, 1) = 40;
  matrix2(0, 2) = 30;
  matrix2(1, 2) = 40;
  matrix2(2, 2) = 10;
  matrix2(0, 3) = 50;
  matrix2(1, 3) = 10;
  matrix2(2, 3) = 10;

  cout << endl;
  cout << "Second test for the Pca class, with a matrix containing more columns than rows: " << endl;
  cout << endl;
  cout << "Here's the input matrix : " << endl;
  MatrixTools::print(matrix2, cout);

  // The constructor without row and column weigths is called. Default weights will be created.
  PrincipalComponentAnalysis* pca2 = new PrincipalComponentAnalysis(matrix2, 3, true, true);

  cout << "The matrix of Principal Axes : " << endl;
  MatrixTools::print(pca2->getPrincipalAxes(), cout);
  cout << endl;
  cout << endl;


  matrix3(0, 0) = 0.10;
  matrix3(1, 0) = 0.20;
  matrix3(2, 0) = 0.30;
  matrix3(0, 1) = 0.40;
  matrix3(1, 1) = 0.50;
  matrix3(2, 1) = 0.60;
  matrix3(0, 2) = 0.50;
  matrix3(1, 2) = 0.30;
  matrix3(2, 2) = 0.10;

  cout << endl;
  cout << "Test for the Coa class, with a square matrix : " << endl;
  cout << endl;
  cout << "Here's the input matrix : " << endl;
  MatrixTools::print(matrix3, cout);

  // The Coa constructor is called.
  CorrespondenceAnalysis* coa = new CorrespondenceAnalysis(matrix3, 3);

  cout << "The matrix of Principal Components : " << endl;
  MatrixTools::print(coa->getPrincipalComponents(), cout);
  cout << endl;
  cout << endl;


  delete pca1;
  delete pca2;
  delete coa;
  return 0;
}

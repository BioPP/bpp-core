// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <cmath>

#include "../../Matrix/Matrix.h"
#include "../../Matrix/MatrixTools.h"
#include "../../VectorTools.h"
#include "DualityDiagram.h"
#include "PrincipalComponentAnalysis.h"

using namespace bpp;
using namespace std;

PrincipalComponentAnalysis::PrincipalComponentAnalysis(
    const Matrix<double>& data,
    unsigned int nbAxes,
    const vector<double>& rowW,
    const vector<double>& colW,
    bool centered,
    bool scaled,
    double tol,
    bool verbose) :
  DualityDiagram(),
  columnMeans_(),
  columnSd_()
{
  RowMatrix<double> tmpData = data;

  // Centering of data?
  if (centered)
  {
    center(tmpData, rowW);
  }

  // Scaling of data?
  if (scaled)
  {
    scale(tmpData, rowW);
  }

  setData(tmpData, rowW, colW, nbAxes, tol, verbose);
}

/******************************************************************************/

PrincipalComponentAnalysis::PrincipalComponentAnalysis(
    const Matrix<double>& data,
    unsigned int nbAxes,
    bool centered,
    bool scaled,
    double tol,
    bool verbose) :
  DualityDiagram(),
  columnMeans_(),
  columnSd_()
{
  size_t nRow = data.getNumberOfRows();
  size_t nCol = data.getNumberOfColumns();

  vector<double> rowW(nRow);
  vector<double> colW(nCol);
  VectorTools::fill(rowW, 1. / static_cast<double>(nRow));
  VectorTools::fill(colW, 1.);

  RowMatrix<double> tmpData = data;

  // Centering of data?
  if (centered)
  {
    center(tmpData, rowW);
  }

  // Scaling of data?
  if (scaled)
  {
    scale(tmpData, rowW);
  }

  setData(tmpData, rowW, colW, nbAxes, tol, verbose);
}

/******************************************************************************/

void PrincipalComponentAnalysis::center(Matrix<double>& matrix, const vector<double>& rowW)
{
  size_t nRow = matrix.getNumberOfRows();
  size_t nCol = matrix.getNumberOfColumns();
  if (nRow != rowW.size())
    throw Exception("PrincipalComponentAnalysis::center. The number of row weigths have to be equal to the number of rows!");

  double sumRowWeights = VectorTools::sum(rowW);

  vector<double> columnMeans(nCol);
  for (unsigned int i = 0; i < nCol; i++)
  {
    double tmp = 0.;
    for (unsigned int j = 0; j < nRow; j++)
    {
      tmp += matrix(j, i) * rowW[j];
    }
    columnMeans[i] = tmp / sumRowWeights;
  }

  for (unsigned int i = 0; i < nCol; i++)
  {
    for (unsigned int j = 0; j < nRow; j++)
    {
      matrix(j, i) -= columnMeans[i];
    }
  }
}

/******************************************************************************/

void PrincipalComponentAnalysis::scale(Matrix<double>& matrix, const vector<double>& rowW)
{
  size_t nRow = matrix.getNumberOfRows();
  size_t nCol = matrix.getNumberOfColumns();
  if (nRow != rowW.size())
    throw Exception("PrincipalComponentAnalysis::scale. The number of row weigths have to be equal to the number of rows!");

  double sumRowWeights = VectorTools::sum(rowW);

  vector<double> columnSd(nCol);
  for (size_t i = 0; i < nCol; i++)
  {
    double tmp = 0.;
    for (unsigned int j = 0; j < nRow; j++)
    {
      tmp += pow(matrix(j, i), 2) * rowW[j];
    }
    columnSd[i] = sqrt(tmp / sumRowWeights);
  }

  for (size_t i = 0; i < nCol; i++)
  {
    for (unsigned int j = 0; j < nRow; j++)
    {
      if (columnSd[i] == 0.)
        matrix(j, i) = 0.;
      else
        matrix(j, i) /= columnSd[i];
    }
  }
}

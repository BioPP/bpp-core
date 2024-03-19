// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Matrix/Matrix.h"
#include "../../Matrix/MatrixTools.h"
#include "CorrespondenceAnalysis.h"
#include "DualityDiagram.h"

using namespace bpp;
using namespace std;

CorrespondenceAnalysis::CorrespondenceAnalysis(
    const Matrix<double>& data,
    unsigned int nbAxes,
    double tol, bool verbose) :
  DualityDiagram(),
  n_()
{
  size_t nRow = data.getNumberOfRows();
  size_t nCol = data.getNumberOfColumns();

  double tmp = 0.;
  for (size_t i = 0; i < nRow; i++)
  {
    for (unsigned int j = 0; j < nCol; j++)
    {
      if (data(i, j) < 0.)
        throw Exception("CorrespondenceAnalysis (constructor). Negative value(s) in the input data. This is not allowed !");
      tmp += data(i, j);
    }
  }
  n_ = tmp;

  if (n_ == 0)
    throw Exception("CorrespondenceAnalysis (constructor). All frequencies in the input data are zero...");

  RowMatrix<double> dataTmp(data);
  MatrixTools::scale(dataTmp, (1. / n_), 0.);

  vector<double> rowWeights(nRow);
  vector<double> colWeights(nCol);

  for (size_t i = 0; i < nRow; i++)
  {
    for (unsigned int j = 0; j < nCol; j++)
    {
      rowWeights[i] += dataTmp(i, j);
      colWeights[j] += dataTmp(i, j);
    }
  }

  vector<double> tmpRowWeigths(nRow);
  vector<double> tmpColWeigths(nCol);
  for (size_t i = 0; i < rowWeights.size(); i++)
  {
    if (rowWeights[i] == 0.)
      tmpRowWeigths[i] = 0.;
    else
      tmpRowWeigths[i] = 1. / rowWeights[i];
  }
  for (size_t j = 0; j < colWeights.size(); j++)
  {
    if (colWeights[j] == 0.)
      tmpColWeigths[j] = 0.;
    else
      tmpColWeigths[j] = 1. / colWeights[j];
  }

  RowMatrix<double> tmpWeightedData(nRow, nCol);
  RowMatrix<double> weightedData(nRow, nCol);
  MatrixTools::hadamardMult(dataTmp, tmpRowWeigths, tmpWeightedData, true);
  MatrixTools::hadamardMult(tmpWeightedData, tmpColWeigths, weightedData, false);
  MatrixTools::scale(weightedData, 1., -1.);

  setData(weightedData, rowWeights, colWeights, nbAxes, tol, verbose);
}

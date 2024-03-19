// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <cmath>

#include "../../../App/ApplicationTools.h"
#include "../../Matrix/EigenValue.h"
#include "../../Matrix/Matrix.h"
#include "../../Matrix/MatrixTools.h"
#include "DualityDiagram.h"

using namespace bpp;
using namespace std;

DualityDiagram::DualityDiagram(
    const Matrix<double>& matrix,
    const vector<double>& rowWeights,
    const vector<double>& colWeights,
    unsigned int nbAxes,
    double tol, bool verbose) :
  rowWeights_(rowWeights),
  colWeights_(colWeights),
  nbAxes_(nbAxes),
  eigenValues_(),
  eigenVectors_(),
  rowCoord_(),
  colCoord_(),
  ppalAxes_(),
  ppalComponents_()
{
  check_(matrix, rowWeights, colWeights, verbose);
  compute_(matrix, tol, verbose);
}

void DualityDiagram::check_(
    const Matrix<double>& matrix,
    const vector<double>& rowWeights,
    const vector<double>& colWeights,
    unsigned int nbAxes)
{
  size_t rowNb = matrix.getNumberOfRows();
  size_t colNb = matrix.getNumberOfColumns();

  if (rowWeights.size() != rowNb)
    throw Exception("DualityDiagram::check_. The number of row weigths has to be equal to the number of rows!");
  if (colWeights.size() != colNb)
    throw Exception("DualityDiagram::check_. The number of column weigths has to be equal to the number of columns!");

  // All row weigths have to be positive
  for (vector<double>::const_iterator it = rowWeights.begin(); it != rowWeights.end(); it++)
  {
    if (*it < 0.)
      throw Exception("DualityDiagram::check_. All row weights have to be positive");
  }

  // All column weigths have to be positive
  for (vector<double>::const_iterator it = colWeights.begin(); it != colWeights.end(); it++)
  {
    if (*it < 0.)
      throw Exception("DualityDiagram::check_. All column weights have to be positive");
  }
}


void DualityDiagram::setData(
    const Matrix<double>& matrix,
    const vector<double>& rowWeights,
    const vector<double>& colWeights,
    unsigned int nbAxes,
    double tol, bool verbose)
{
  check_(matrix, rowWeights, colWeights, verbose);
  rowWeights_ = rowWeights;
  colWeights_ = colWeights;
  nbAxes_     = nbAxes;
  compute_(matrix, tol, verbose);
}

void DualityDiagram::compute_(const Matrix<double>& matrix,
    double tol, bool verbose)
{
  size_t rowNb = matrix.getNumberOfRows();
  size_t colNb = matrix.getNumberOfColumns();

  // If there are less rows than columns, the variance-covariance or correlation matrix is obtain differently (see below)
  bool transpose = (rowNb < colNb);

  // The initial matrix is multiplied by the square root of the row weigths.
  vector<double> rW(rowWeights_);
  for (unsigned int i = 0; i < rowWeights_.size(); i++)
  {
    rW[i] = sqrt(rowWeights_[i]);
  }

  RowMatrix<double> M1(rowNb, colNb);
  MatrixTools::hadamardMult(matrix, rW, M1, true);

  // The resulting matrix is then multiplied by the square root of the column weigths.
  vector<double> cW(colWeights_);
  for (unsigned int i = 0; i < colWeights_.size(); i++)
  {
    cW[i] = sqrt(colWeights_[i]);
  }

  RowMatrix<double> M2;
  MatrixTools::hadamardMult(M1, cW, M2, false);

  // The variance-covariance (if the data is centered) or the correlation (if the data is centered and normalized) matrix is calculated
  RowMatrix<double> tM2;
  MatrixTools::transpose(M2, tM2);
  RowMatrix<double> M3;
  if (!transpose)
    MatrixTools::mult(tM2, M2, M3);
  else
    MatrixTools::mult(M2, tM2, M3);

  EigenValue<double> eigen(M3);
  if (!eigen.isSymmetric())
    throw Exception("DualityDiagram (constructor). The variance-covariance or correlation matrix should be symmetric...");

  eigenValues_ = eigen.getRealEigenValues();
  eigenVectors_ = eigen.getV();

  // How many significant axes have to be conserved?
  size_t rank = 0;
  for (size_t i = eigenValues_.size(); i > 0; i--)
  {
    if ((eigenValues_[i - 1] / eigenValues_[eigenValues_.size() - 1]) > tol)
      rank++;
  }

  if (nbAxes_ <= 0)
  {
    throw Exception("DualityDiagram (constructor). The number of axes to keep must be positive.");
  }
  if (nbAxes_ > rank)
  {
    if (verbose)
      ApplicationTools::displayWarning("The number of axes to kept has been reduced to conserve only significant axes");
    nbAxes_ = rank;
  }

  /*The eigen values are initially sorted into ascending order by the 'eigen' function. Here the significant values are sorted
     in the other way around.*/
  vector<double> tmpEigenValues(nbAxes_);
  size_t cpt = 0;
  for (size_t i = eigenValues_.size(); i > (eigenValues_.size() - nbAxes_); i--)
  {
    tmpEigenValues[cpt] = eigenValues_[i - 1];
    cpt++;
  }
  eigenValues_ = tmpEigenValues;

  for (vector<double>::iterator it = rowWeights_.begin(); it != rowWeights_.end(); it++)
  {
    if (*it == 0.)
      *it = 1.;
  }

  for (vector<double>::iterator it = colWeights_.begin(); it != colWeights_.end(); it++)
  {
    if (*it == 0.)
      *it = 1.;
  }

  vector<double> dval(nbAxes_);
  for (size_t i = 0; i < dval.size(); i++)
  {
    dval[i] = sqrt(eigenValues_[i]);
  }

  vector<double> invDval(nbAxes_);
  for (size_t i = 0; i < invDval.size(); i++)
  {
    invDval[i] = 1. / sqrt(eigenValues_[i]);
  }

  // Calculation of the row and column coordinates as well as the principal axes and components:
  if (!transpose)
  {
    vector<double> tmpColWeights(colNb);
    for (unsigned int i = 0; i < colWeights_.size(); i++)
    {
      tmpColWeights[i] = 1. / sqrt(colWeights_[i]);
    }

    // The eigen vectors are placed in the same order as their corresponding eigen value in eigenValues_.
    RowMatrix<double> tmpEigenVectors;
    tmpEigenVectors.resize(eigenVectors_.getNumberOfRows(), nbAxes_);
    size_t cpt2 = 0;
    for (size_t i = eigenVectors_.getNumberOfColumns(); i > (eigenVectors_.getNumberOfColumns() - nbAxes_); i--)
    {
      for (unsigned int j = 0; j < eigenVectors_.getNumberOfRows(); j++)
      {
        tmpEigenVectors(j, cpt2) = eigenVectors_(j, i - 1);
      }
      cpt2++;
    }

    // matrix of principal axes
    MatrixTools::hadamardMult(tmpEigenVectors, tmpColWeights, ppalAxes_, true);
    // matrix of row coordinates
    RowMatrix<double> tmpRowCoord_;
    tmpRowCoord_.resize(rowNb, nbAxes_);
    MatrixTools::hadamardMult(matrix, colWeights_, tmpRowCoord_, false);
    MatrixTools::mult(tmpRowCoord_, ppalAxes_, rowCoord_);

    // matrix of column coordinates
    MatrixTools::hadamardMult(ppalAxes_, dval, colCoord_, false);
    // matrix of principal components
    MatrixTools::hadamardMult(rowCoord_, invDval, ppalComponents_, false);
  }
  else
  {
    vector<double> tmpRowWeights(rowNb);
    for (unsigned int i = 0; i < rowWeights_.size(); i++)
    {
      tmpRowWeights[i] = 1. / sqrt(rowWeights_[i]);
    }

    // The eigen vectors are placed in the same order as their corresponding eigen value in eigenValues_.
    RowMatrix<double> tmpEigenVectors;
    tmpEigenVectors.resize(eigenVectors_.getNumberOfRows(), nbAxes_);
    size_t cpt2 = 0;
    for (size_t i = eigenVectors_.getNumberOfColumns(); i > (eigenVectors_.getNumberOfColumns() - nbAxes_); i--)
    {
      for (size_t j = 0; j < eigenVectors_.getNumberOfRows(); j++)
      {
        tmpEigenVectors(j, cpt2) = eigenVectors_(j, i - 1);
      }
      cpt2++;
    }

    // matrix of principal components
    MatrixTools::hadamardMult(tmpEigenVectors, tmpRowWeights, ppalComponents_, true);
    // matrix of column coordinates
    RowMatrix<double> tmpColCoord_;
    tmpColCoord_.resize(colNb, nbAxes_);
    MatrixTools::hadamardMult(matrix, rowWeights_, tmpColCoord_, true);
    RowMatrix<double> tTmpColCoord_;
    MatrixTools::transpose(tmpColCoord_, tTmpColCoord_);
    MatrixTools::mult(tTmpColCoord_, ppalComponents_, colCoord_);

    // matrix of row coordinates
    MatrixTools::hadamardMult(ppalComponents_, dval, rowCoord_, false);
    // matrix of principal axes
    MatrixTools::hadamardMult(colCoord_, invDval, ppalAxes_, false);
  }
}

/******************************************************************************/

DualityDiagram::~DualityDiagram() {}

/******************************************************************************/

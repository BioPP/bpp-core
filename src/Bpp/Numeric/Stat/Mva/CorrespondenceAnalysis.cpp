//
// File: CorrespondenceAnalysis.cpp
// Created by: Mathieu Groussin
// Created on: Sun Mar 06 10:03 2011
//

/*
   Copyright or © or Copr. Bio++ Development Team, (November 16, 2004)

   This software is a computer program whose purpose is to provide classes
   for phylogenetic data analysis.

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

#include "CorrespondenceAnalysis.h"
#include "../../Matrix/Matrix.h"
#include "../../Matrix/MatrixTools.h"
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



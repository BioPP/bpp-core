// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_STAT_MVA_DUALITYDIAGRAM_H
#define BPP_NUMERIC_STAT_MVA_DUALITYDIAGRAM_H


#include "../../Matrix/Matrix.h"

namespace bpp
{
/**
 * @brief The core class of a multivariate analysis.
 *
 * In the constructor, the eigen values and vectors of the variance-covariance or correlation matrix are calculated.
 * Eigen values and vectors are stored in the eigenValues_ and eigenVectors_ respectively.
 * Furthermore, four matrices are calculated: the row and column coordinates as well as the principal axes and components.
 *
 * The code of this class is deeply inspired from the R code of the as.dudi function available in the ade4 package.
 */
class DualityDiagram :
  public virtual Clonable
{
private:
  std::vector<double> rowWeights_;
  std::vector<double> colWeights_;
  size_t nbAxes_;
  std::vector<double> eigenValues_;
  RowMatrix<double> eigenVectors_;
  RowMatrix<double> rowCoord_;
  RowMatrix<double> colCoord_;
  RowMatrix<double> ppalAxes_;
  RowMatrix<double> ppalComponents_;

public:
  /**
   * @brief Build an empty DualityDiagram object.
   *
   */
  DualityDiagram() :
    rowWeights_(),
    colWeights_(),
    nbAxes_(),
    eigenValues_(),
    eigenVectors_(),
    rowCoord_(),
    colCoord_(),
    ppalAxes_(),
    ppalComponents_() {}

  /**
   * @brief Build a new DualityDiagram object.
   *
   * @param matrix The input data to analyse.
   * @param rowWeights A vector of values specifying the weights of rows.
   * @param colWeights A vector of values specifying the weights of columns.
   * @param nbAxes The number of kept axes during the analysis.
   * @param tol Tolerance threshold for null eigenvalues (a value less than tol times the first one is considered as null)
   * @param verbose Should warnings be dispayed.
   * @throw Exception if an error occured.
   */
  DualityDiagram(
      const Matrix<double>& matrix,
      const std::vector<double>& rowWeights,
      const std::vector<double>& colWeights,
      unsigned int nbAxes,
      double tol = 0.0000001,
      bool verbose = true);

  virtual ~DualityDiagram();

  DualityDiagram* clone() const { return new DualityDiagram(*this); }

private:
  void check_(
      const Matrix<double>& matrix,
      const std::vector<double>& rowWeights,
      const std::vector<double>& colWeights,
      unsigned int nbAxes);
  void compute_(const Matrix<double>& matrix, double tol, bool verbose);

public:
  /**
   * @brief Set the data and perform computations.
   *
   * @param matrix The input data to analyse.
   * @param rowWeights A vector of values specifying the weights of rows.
   * @param colWeights A vector of values specifying the weights of columns.
   * @param nbAxes The number of kept axes during the analysis.
   * @param tol Tolerance threshold for null eigenvalues (a value less than tol times the first one is considered as null)
   * @param verbose Should warnings be dispayed.
   * @throw Exception if an error occured.
   */
  void setData(
      const Matrix<double>& matrix,
      const std::vector<double>& rowWeights,
      const std::vector<double>& colWeights,
      unsigned int nbAxes,
      double tol = 0.0000001,
      bool verbose = true);

  std::vector<double> computeVariancePercentagePerAxis();

  size_t getNbOfKeptAxes() const { return nbAxes_; }
  const std::vector<double> getRowWeights() const { return rowWeights_; }
  const std::vector<double> getColumnWeights() const { return colWeights_; }
  const std::vector<double>& getEigenValues() const { return eigenValues_; }
  const RowMatrix<double>& getRowCoordinates() const { return rowCoord_; }
  const RowMatrix<double>& getColCoordinates() const { return colCoord_; }
  const RowMatrix<double>& getPrincipalAxes() const { return ppalAxes_; }
  const RowMatrix<double>& getPrincipalComponents() const { return ppalComponents_; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_STAT_MVA_DUALITYDIAGRAM_H

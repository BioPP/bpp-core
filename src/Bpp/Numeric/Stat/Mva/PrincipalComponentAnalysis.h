// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_STAT_MVA_PRINCIPALCOMPONENTANALYSIS_H
#define BPP_NUMERIC_STAT_MVA_PRINCIPALCOMPONENTANALYSIS_H


#include "../../Matrix/Matrix.h"
#include "DualityDiagram.h"

namespace bpp
{
/**
 * @brief This class allows to perform a principal component analysis.
 *
 * Two constructors are available. The first one allows the user to specify the row and column weights. The second one specify default weights:
 * uniform weights unit weights are created for rows and columns respectively.
 *
 * The code of this class is deeply inspired from the R code of the dudi.pca function available in the ade4 package.
 */
class PrincipalComponentAnalysis :
  public DualityDiagram
{
private:
  std::vector<double> columnMeans_;
  std::vector<double> columnSd_;

public:
  /**
   * @brief Build a new PrincipalComponentAnalysis object.
   *
   * @param data The input data (a RowMatrix) to analyse.
   * @param nbAxes The number of kept axes during the analysis.
   * @param rowW A vector of values specifying the weights of rows.
   * @param colW A vector of values specifying the weights of columns.
   * @param centered If true the input matrix is centered according to the column means.
   * @param scaled If true the input matrix is normalized according to the standard deviations of columns.
   * @param tol Tolerance threshold for null eigenvalues (a value less than tol times the first one is considered as null)
   * @param verbose Should warnings be dispayed.
   * @throw Exception if an error occured.
   */
  PrincipalComponentAnalysis(
    const Matrix<double>& data,
    unsigned int nbAxes,
    const std::vector<double>& rowW,
    const std::vector<double>& colW,
    bool centered = true,
    bool scaled = true,
    double tol = 0.0000001,
    bool verbose = true);

  /**
   * @brief Build a new PrincipalComponentAnalysis object and specify default row and column weights.
   *
   * @param data The input data (a RowMatrix) to analyse.
   * @param nbAxes The number of kept axes during the analysis.
   * @param centered If true the input matrix is centered according to the column means.
   * @param scaled If true the input matrix is normalized according to the standard deviations of columns.
   * @param tol Tolerance threshold for null eigenvalues (a value less than tol times the first one is considered as null)
   * @param verbose Should warnings be dispayed.
   * @throw Exception if an error occured.
   */
  PrincipalComponentAnalysis(
    const Matrix<double>& data,
    unsigned int nbAxes,
    bool centered = true,
    bool scaled = true,
    double tol = 0.0000001,
    bool verbose = true);

  virtual ~PrincipalComponentAnalysis() {}

  PrincipalComponentAnalysis* clone() const { return new PrincipalComponentAnalysis(*this); }

public:
  /**
   * @brief This function allows to center an input matrix from its column means.
   *
   * @param matrix The input data (a Matrix) to center.
   * @param rowW A vector with row weights.
   */
  static void center(Matrix<double>& matrix, const std::vector<double>& rowW);

  /**
   * @brief This function allows to center an input matrix from its column means.
   *
   * @param matrix The input data (a Matrix) to center.
   * @param rowW A vector with row weights.
   */
  static void scale(Matrix<double>& matrix, const std::vector<double>& rowW);

public:
  const std::vector<double>& getColumnMeans() const { return columnMeans_; }
  const std::vector<double>& getColumnSd() const { return columnSd_; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_STAT_MVA_PRINCIPALCOMPONENTANALYSIS_H

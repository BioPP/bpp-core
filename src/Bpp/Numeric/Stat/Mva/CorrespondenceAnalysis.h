// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_STAT_MVA_CORRESPONDENCEANALYSIS_H
#define BPP_NUMERIC_STAT_MVA_CORRESPONDENCEANALYSIS_H


#include "../../Matrix/Matrix.h"
#include "DualityDiagram.h"

namespace bpp
{
/**
 * @brief This class allows to perform a correspondence analysis.
 *
 * All values in the input table have to be non-negative.
 * The DualityDiagram class, core class of a multivariate analysis, is called internally.
 *
 * The code of this class is deeply inspired from the R code of the dudi.coa function available in the ade4 package.
 */

class CorrespondenceAnalysis :
  public DualityDiagram
{
private:
  double n_;

public:
  /**
   * @brief Build a new CorrespondenceAnalysis object.
   *
   * @param data The input data (a RowMatrix) to analyse.
   * @param nbAxes The number of kept axes during the analysis.
   * @param tol Tolerance threshold for null eigenvalues (a value less than tol times the first one is considered as null)
   * @param verbose Should warnings be dispayed.
   * @throw Exception if an error occured.
   */
  CorrespondenceAnalysis(
      const Matrix<double>& data,
      unsigned int nbAxes,
      double tol = 0.0000001,
      bool verbose = true);

  virtual ~CorrespondenceAnalysis() {}

  CorrespondenceAnalysis* clone() const { return new CorrespondenceAnalysis(*this); }

public:
  double getSumOfAllValues() const { return n_; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_STAT_MVA_CORRESPONDENCEANALYSIS_H

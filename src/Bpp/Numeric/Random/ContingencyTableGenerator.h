// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_RANDOM_CONTINGENCYTABLEGENERATOR_H
#define BPP_NUMERIC_RANDOM_CONTINGENCYTABLEGENERATOR_H


#include "../Matrix/Matrix.h"
#include "RandomTools.h"

// From the STL:
#include <cmath>
#include <vector>

namespace bpp
{
/**
 * @brief Generate a random contingency matrix with given marginal counts.
 *
 * This procedure was adapted from the original fortran code described in:
 * Patefield, W. M. (1981) Algorithm AS159.  An efficient method of
 * generating r x c tables with given row and column totals.
 * _Applied Statistics_ *30*, 91-97.
 * This algorithm is the one also used in R function chisq.test for instance.
 *
 * The code was adpated from file rcont.c, edited by Martin Maechler, Dec 2003,
 * available in the R software source distribution.
 *
 * @param nrowt Marginal counts.
 * @param ncolt Marginal counts.
 * @return A random matrix of counts with the same marginals as specified.
 */
class ContingencyTableGenerator
{
private:
  std::vector<size_t> nrowt_;
  std::vector<size_t> ncolt_;
  size_t nrow_;
  size_t ncol_;
  size_t nrowm_;
  size_t ncolm_;
  std::vector<size_t> jwork_; // workspace
  size_t ntot_; // total number of observations
  std::vector<double> fact_; // log factorial

public:
  ContingencyTableGenerator(const std::vector<size_t>& nrowt, const std::vector<size_t>& ncolt);

public:
  RowMatrix<size_t> rcont2();
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_RANDOM_CONTINGENCYTABLEGENERATOR_H

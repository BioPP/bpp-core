// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <iostream>

#include "../VectorTools.h"
#include "ContingencyTableGenerator.h"

using namespace bpp;
using namespace std;

/**************************************************************************/

ContingencyTableGenerator::ContingencyTableGenerator(
  const std::vector<size_t>& nrowt,
  const std::vector<size_t>& ncolt) :
  nrowt_(nrowt),
  ncolt_(ncolt),
  nrow_(nrowt.size()),
  ncol_(ncolt.size()),
  nrowm_(0),
  ncolm_(0),
  jwork_(ncolt.size()),
  ntot_(0),
  fact_(0)
{
  if (nrow_ < 2 || ncol_ < 2)
    throw Exception("ContingencyTableGenerator. Input marginals must have size greater than 1.");
  ntot_ = VectorTools::sum(nrowt_);
  if (ntot_ != VectorTools::sum(ncolt_))
    throw Exception("ContingencyTableGenerator. Marginal do not sum to the same value.");
  nrowm_ = nrow_ - 1;
  ncolm_ = ncol_ - 1;
  fact_.resize(ntot_ + 1);
  double x = 0.;
  fact_[0] = 0.;
  for (size_t i = 1; i <= ntot_; ++i)
  {
    x = x + log(static_cast<double>(i));
    fact_[i] = x;
  }
}

/* Algorithm AS 159 Applied Statistics (1981), vol. 30, no. 1
   original (C) Royal Statistical Society 1981

   Generate random two-way table with given marginal totals.

   Heavily pretty edited by Martin Maechler, Dec 2003
   use double precision for integer multiplication (against overflow);

   Taken from R source file rcont.c and adapted by Julien Dutheil, Dec 2010
 */
RowMatrix<size_t> ContingencyTableGenerator::rcont2()
{
  RowMatrix<size_t> table(nrow_, ncol_); // Result
  size_t j, l, m, ia, ib, ic, jc, id, ie, ii, nll, nlm, nr_1, nc_1;
  long double x, y, dummy, sumprb;
  bool lsm, lsp;

  nr_1 = nrow_ - 1;
  nc_1 = ncol_ - 1;

  ib = 0; /* -Wall */

  /* Construct random matrix */
  for (j = 0; j < nc_1; ++j)
  {
    jwork_[j] = ncolt_[j];
  }

  jc = ntot_;

  for (l = 0; l < nr_1; ++l)  /* -----  matrix[ l, * ] ----- */
  {
    ia = nrowt_[l];
    ic = jc;
    jc -= ia;/* = n_tot - sum(nr[0:l]) */

    for (m = 0; m < nc_1; ++m)
    {
      id = jwork_[m];
      ie = ic;
      ic -= id;
      ib = ie - ia;
      ii = ib - id;

      if (ie == 0)  /* Row [l,] is full, fill rest with zero entries */
      {
        for (j = m; j < nc_1; ++j)
        {
          table(l, j) = 0;
        }
        ia = 0;
        break;
      }

      /* Generate pseudo-random number */
      dummy = RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);

      do/* Outer Loop */

      /* Compute conditional expected value of MATRIX(L, M) */

      {
        nlm = static_cast<size_t>(ia * (static_cast<long double>(id) / static_cast<long double>(ie)) + 0.5);
        x = exp(fact_[ia] + fact_[ib] + fact_[ic] + fact_[id]
                - fact_[ie] - fact_[nlm]
                - fact_[id - nlm] - fact_[ia - nlm] - fact_[ii + nlm]);
        if (x >= dummy)
          break;

        sumprb = x;
        y = x;
        nll = nlm;

        do
        {
          /* Increment entry in row L, column M */
          j = static_cast<size_t>((id - nlm) * static_cast<long double>(ia - nlm));
          lsp = (j == 0);
          if (!lsp)
          {
            ++nlm;
            x = x * j / (static_cast<long double>(nlm) * (ii + nlm));
            sumprb += x;
            if (sumprb >= dummy)
              goto L160;
          }

          do
          {
            /* Decrement entry in row L, column M */
            j = nll * (ii + nll);
            lsm = (j == 0);
            if (!lsm)
            {
              --nll;
              y = y * j / (static_cast<long double>(id - nll) * (ia - nll));
              sumprb += y;
              if (sumprb >= dummy)
              {
                nlm = nll;
                goto L160;
              }
              /* else */
              if (!lsp)
                break; /* to while (!lsp) */
            }
          }
          while (!lsm);
        }
        while (!lsp);

        dummy = sumprb * RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);
      }
      while (true);

L160:
      table(l, m) = nlm;
      ia -= nlm;
      jwork_[m] -= nlm;
    }
    table(l, nc_1) = ia;/* last column in row l */
  }

  /* Compute entries in last row of MATRIX */
  for (m = 0; m < nc_1; ++m)
  {
    table(nr_1, m) = jwork_[m];
  }

  table(nr_1, nc_1) = ib - table(nr_1, nc_1 - 1);

  return table;
}

/**************************************************************************/

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <algorithm>
#include <iostream>

#include "../../App/ApplicationTools.h"
#include "../Random/ContingencyTableGenerator.h"
#include "../Random/RandomTools.h"
#include "../VectorTools.h"
#include "ContingencyTableTest.h"

using namespace bpp;
using namespace std;

ContingencyTableTest::ContingencyTableTest(const std::vector< std::vector<size_t>>& table, unsigned int nbPermutations, bool warn) :
  statistic_(0),
  pvalue_(0),
  df_(0),
  margin1_(table.size()),
  margin2_(0)
{
  // Compute marginals:
  size_t n = table.size();
  if (n < 2)
    throw Exception("ContingencyTableTest. Table size should be at least 2x2!");
  size_t m = table[0].size();
  if (m < 2)
    throw Exception("ContingencyTableTest. Table size should be at least 2x2!");
  margin2_.resize(m);
  for (size_t j = 0; j < m; ++j)
  {
    margin2_[j] = 0;
  }
  bool test = false;
  for (size_t i = 0; i < n; ++i)
  {
    if (table[i].size() != m)
      throw Exception("ContingencyTableTest. Input array has non-homogeneous dimensions!");
    for (size_t j = 0; j < m; ++j)
    {
      size_t c = table[i][j];
      if (c <= 5)
        test = true;
      margin1_[i] += c;
      margin2_[j] += c;
    }
  }
  for (size_t i = 0; i < n; ++i)
  {
    if (margin1_[i] == 0)
      throw Exception("ContingencyTableTest. Row " + TextTools::toString(i) + " sums to 0.");
  }
  for (size_t j = 0; j < m; ++j)
  {
    if (margin2_[j] == 0)
      throw Exception("ContingencyTableTest. Column " + TextTools::toString(j) + " sums to 0.");
  }


  size_t tot = VectorTools::sum(margin1_);
  df_ = static_cast<double>((m - 1) * (n - 1));

  RowMatrix<long double> expc(n, m);
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 0; j < m; ++j)
    {
      long double c = table[i][j];
      long double e = static_cast<long double>(margin1_[i] * margin2_[j]) / static_cast<long double>(tot);
      expc(i, j) = e;
      statistic_ += static_cast<double>(std::pow(c - e, 2.L) / e);
    }
  }

  if (nbPermutations > 0)
  {
    size_t count = 0;
    ContingencyTableGenerator ctgen(margin1_, margin2_);
    for (unsigned int k = 0; k < nbPermutations; ++k)
    {
      // Randomize:
      RowMatrix<size_t> table_rep = ctgen.rcont2();
      // Recompute statistic:
      double stat_rep = 0;
      for (size_t i = 0; i < n; ++i)
      {
        for (size_t j = 0; j < m; ++j)
        {
          long double c = table_rep(i, j);
          long double e = expc(i, j);
          stat_rep += static_cast<double>(std::pow(c - e, 2.L) / e);
        }
      }
      if (stat_rep >= statistic_)
        count++;
    }
    pvalue_ = static_cast<double>(count + 1) / static_cast<double>(nbPermutations + 1);
  }
  else
  {
    if (test && warn)
      ApplicationTools::displayWarning("Unsufficient observations, p-value might be incorrect.");

    // Compute p-value:
    pvalue_ = 1. - RandomTools::pChisq(statistic_, df_);
  }
}

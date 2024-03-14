// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "StatTools.h"

// From the STL:
#include <algorithm>

using namespace bpp;
using namespace std;

vector<double> StatTools::computeFdr(const vector<double>& pvalues)
{
  size_t n = pvalues.size();
  vector<PValue_> sortedPValues;
  for (size_t i = 0; i < n; ++i)
  {
    sortedPValues.push_back(PValue_(pvalues[i], i));
  }
  sort(sortedPValues.begin(), sortedPValues.end());
  vector<double> fdr(pvalues.size());
  for (size_t i = 0; i < sortedPValues.size(); ++i)
  {
    fdr[sortedPValues[i].index_] = sortedPValues[i].pvalue_ * static_cast<double>(n) / ( static_cast<double>(sortedPValues[i].index_ + 1));
  }
  return fdr;
}

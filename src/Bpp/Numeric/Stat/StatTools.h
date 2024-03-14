// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_STAT_STATTOOLS_H
#define BPP_NUMERIC_STAT_STATTOOLS_H



// From the STL:
#include <vector>
#include <cstddef>

namespace bpp
{
/**
 * @brief Statistics tools and utilitary functions.
 */
class StatTools
{
private:
  struct PValue_
  {
    double pvalue_;
    size_t index_;
    PValue_(double pvalue, size_t index) :
      pvalue_(pvalue), index_(index) {}

    bool operator<(const PValue_& pvalue) const
    {
      return pvalue.pvalue_ < pvalue_;
    }
  };

public:
  /**
   * @brief Compute the false discovery rate for a set of input p-values, using Benjamini and Hochberg's 'FDR' method.
   *
   * The false discovery rate is computed by sorting all pvalues.
   * The FDR r is calculated with the formula
   * @f$ r = p * n / i@f$
   * where p is the p-value, n is the number of tests (the size of the input vector) and i is the rank of the p-value, that is the index in the sorted array.
   *
   * References:
   * - Benjamini, Y and Hochberg, Y (1995). Controlling the false discovery rate: a practical and powerful approach to multiple testing. Journal of the Royal Statistical Society, Series B (Methodological) 57(1):289-300.
   * - Verhoeven, KJF; Simonsen, KL; M. McIntyre, LM (2005). Implementing false discovery rate control: increasing your power. Oikos. 108(3):643-647.
   *
   * @author Julien Dutheil
   * @param pvalues The input p-values.
   * @return The corresponding false discovery rates.
   */
  static std::vector<double> computeFdr(const std::vector<double>& pvalues);
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_STAT_STATTOOLS_H

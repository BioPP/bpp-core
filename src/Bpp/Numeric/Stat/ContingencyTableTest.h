// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_STAT_CONTINGENCYTABLETEST_H
#define BPP_NUMERIC_STAT_CONTINGENCYTABLETEST_H


#include "StatTest.h"

// From the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Implements tests on contingency tables.
 *
 * Performs a chi square test on contingency tables.
 */
class ContingencyTableTest :
  public virtual StatTest
{
private:
  double statistic_;
  double pvalue_;
  double df_;
  std::vector<size_t> margin1_;
  std::vector<size_t> margin2_;

public:
  /**
   * @brief Build a new test object and perform computations.
   *
   * @param table The input contingency table.
   * @param nbPermutations If greater than 0, performs a randomization test instead of using the chisquare approximation.
   * @param warn Should a warning message be displayed in case of unsufficient observations?
   */
  ContingencyTableTest(const std::vector< std::vector<size_t> >& table, unsigned int nbPermutations = 0, bool warn = true);
  virtual ~ContingencyTableTest() {}

  ContingencyTableTest* clone() const { return new ContingencyTableTest(*this); }

public:
  std::string getName() const { return "Test on contingency table."; }
  double getStatistic() const { return statistic_; }
  double getPValue() const { return pvalue_; }
  double getDegreesOfFreedom() const { return df_; }
  const std::vector<size_t> getMarginRows() const { return margin1_; }
  const std::vector<size_t> getMarginColumns() const { return margin2_; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_STAT_CONTINGENCYTABLETEST_H

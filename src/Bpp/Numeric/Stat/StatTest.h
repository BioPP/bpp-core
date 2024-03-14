// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_STAT_STATTEST_H
#define BPP_NUMERIC_STAT_STATTEST_H


#include "../../Clonable.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief Interface for statistical test results.
 *
 * Specific test may add specific methods to the ones provided here.
 */
class StatTest :
  public virtual Clonable
{
public:
  StatTest() {}
  virtual ~StatTest() {}

public:
  virtual std::string getName() const = 0;
  virtual double getStatistic() const = 0;
  virtual double getPValue() const = 0;
};
} // end of namespace bpp;
#endif // BPP_NUMERIC_STAT_STATTEST_H

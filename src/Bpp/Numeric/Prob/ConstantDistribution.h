// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_CONSTANTDISTRIBUTION_H
#define BPP_NUMERIC_PROB_CONSTANTDISTRIBUTION_H


#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Constant discrete distribution.
 *
 * Only one category with probability 1.0.
 */
class ConstantDistribution :
  public AbstractDiscreteDistribution
{
private:
  double value_;

public:
  /**
   * @brief Builds a new ConstantDistribution object from a
   *  value
   *
   * @param value The value of the distribution.
   *
   */

  ConstantDistribution(double value);

  ConstantDistribution(const ConstantDistribution&);

  ConstantDistribution& operator=(const ConstantDistribution&);

  virtual ~ConstantDistribution() {}

  ConstantDistribution* clone() const
  {
    return new ConstantDistribution(*this);
  }

public:
  void fireParameterChanged(const ParameterList& parameters);

  double randC() const { return value_; }

  std::string getName() const { return "Constant"; }

  double getLowerBound() const { return value_; }

  double getUpperBound() const { return value_; }

  double qProb(double x) const { return (x >= 1) ? value_ : -NumConstants::VERY_BIG(); }

  double pProb(double x) const { return x < value_ ? 0 : 1; }

  double Expectation(double a) const { return a < value_ ? 0 : 1; }

  void restrictToConstraint(const ConstraintInterface& c);

  void discretize() {}
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_CONSTANTDISTRIBUTION_H

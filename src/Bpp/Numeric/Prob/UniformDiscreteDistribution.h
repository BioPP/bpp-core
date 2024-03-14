// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_UNIFORMDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_UNIFORMDISCRETEDISTRIBUTION_H


#include "../Constraints.h"
#include "../Random/RandomTools.h"
#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Uniform distribution.
 * All categories are equidistributed all along a given interval.
 *
 * @author Laurent Gueguen
 */
class UniformDiscreteDistribution :
  public AbstractDiscreteDistribution
{
private:
  double min_;
  double max_;

public:
  /**
   * @brief Build a new discretized uniform distribution.
   * @param n the number of categories to use.
   * @param min The minimun value (default 0)
   * @param max The maximum value (default 1)
   */
  UniformDiscreteDistribution(unsigned int n, double min = 0., double max = 1.);

  UniformDiscreteDistribution(const UniformDiscreteDistribution&);

  UniformDiscreteDistribution& operator=(const UniformDiscreteDistribution&);

  virtual ~UniformDiscreteDistribution();

  UniformDiscreteDistribution* clone() const { return new UniformDiscreteDistribution(*this); }

public:
  std::string getName() const {return "Uniform";}

  void fireParameterChanged(const ParameterList& parameters);

  double randC() const
  {
    double x = RandomTools::giveRandomNumberBetweenZeroAndEntry(max_ - min_) + min_;
    while (!intMinMax_->isCorrect(x))
      x = RandomTools::giveRandomNumberBetweenZeroAndEntry(max_ - min_) + min_;
    return x;
  }

  double qProb(double x) const
  {
    return min_ + x * (max_ - min_);
  }

  double pProb(double x) const
  {
    return (x <= min_) ? 0 : (x - min_) / (max_ - min_);
  }

  double Expectation(double a) const
  {
    return (a <= min_) ? 0 : ((a >= max_) ? (max_ + min_) / 2 : (a * a - min_ * min_) / (max_ - min_) / 2);
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_UNIFORMDISCRETEDISTRIBUTION_H

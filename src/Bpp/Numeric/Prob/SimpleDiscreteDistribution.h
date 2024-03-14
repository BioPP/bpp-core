// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_SIMPLEDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_SIMPLEDISCRETEDISTRIBUTION_H


#include "AbstractDiscreteDistribution.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief A Discrete distribution object, where some specific
 * probabilities are assigned to a finite set of values.
 *
 * Optional parameters:
 *
 * For the probabilities: they are called \c "theta1",... and defined
 * as @f$ \theta_{i \in 1..\textrm{size-1}} @f$ such that probability of value @f$i @f$ is @f$ (1-\theta_1).(1-\theta_2)...\theta_{i} @f$
 *
 * For the values: they are called \c "V1", \c "V2" ...
 *
 */
class SimpleDiscreteDistribution :
  public AbstractDiscreteDistribution
{
private:
  std::map<size_t, std::vector<double> > givenRanges_;

public:
  /**
   * @brief Builds a new SimpleDiscreteDistribution object from a
   * map<double,double> object. With this constructor, the
   * probabilities are fixed (ie no parameters).
   *
   * Keys are taken to be interval values, and map values to be the corresponding probabilities.
   *
   * @param distribution The map object to use.
   * @param precision to discriminate the categories
   * @param fixed tells if there are parameters (default false means there are parameters).
   */

  SimpleDiscreteDistribution(const std::map<double, double>& distribution, double precision = NumConstants::TINY(), bool fixed = false);

  /**
   * @brief Builds a new SimpleDiscreteDistribution object from a
   * vector of values and a vector of probabilities
   *
   * @param values The vector of values.
   * @param probas The vector of probabilities.
   * @param prec precision used to discriminate the categories
   * @param fixed tells if there are parameters (default false means there are parameters).
   *
   */

  SimpleDiscreteDistribution(const std::vector<double>& values, const std::vector<double>& probas, double prec = NumConstants::TINY(), bool fixed = false);

  /**
   * @brief Builds a new SimpleDiscreteDistribution object from a
   * vector of values, a map of ranges and a vector of probabilities
   *
   * @param values The vector of values.
   * @param ranges The map of ranges. Each key is the index of the
   *        parameter in the given vector of values, and the
   *        associated value is a vector of two doubles, for the min
   *        and the max of the range.
   * @param probas The vector of probabilities.
   * @param prec precision to discriminate the categories
   * @param fixed tells if there are parameters (default false means there are parameters).
   *
   */

  SimpleDiscreteDistribution(const std::vector<double>& values, const std::map<size_t, std::vector<double> >& ranges, const std::vector<double>& probas, double prec = NumConstants::TINY(), bool fixed = false);

  virtual ~SimpleDiscreteDistribution() {}

  SimpleDiscreteDistribution(const SimpleDiscreteDistribution&);

  SimpleDiscreteDistribution& operator=(const SimpleDiscreteDistribution&);

  SimpleDiscreteDistribution* clone() const { return new SimpleDiscreteDistribution(*this); }

public:
  /*
   *@brief Returns the map of the given ranges for the values.
   *
   */
  const std::map<size_t, std::vector<double> > getRanges() const { return givenRanges_;}

  std::string getName() const {return "Simple";}

  void discretize();

  void fireParameterChanged(const ParameterList& parameters);

  double getLowerBound() const
  {
    return distribution_.begin()->first;
  }

  double getUpperBound() const
  {
    return distribution_.rbegin()->first;
  }

  double qProb(double x) const;

  double pProb(double x) const;

  double Expectation(double a) const;

  void restrictToConstraint(const ConstraintInterface& c);
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_SIMPLEDISCRETEDISTRIBUTION_H

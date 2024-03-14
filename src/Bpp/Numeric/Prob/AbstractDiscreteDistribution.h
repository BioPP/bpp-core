// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_ABSTRACTDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_ABSTRACTDISCRETEDISTRIBUTION_H

#include <map>

#include "../AbstractParameterAliasable.h"
#include "../Constraints.h"
#include "DiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Partial implementation of the DiscreteDistribution interface.
 *
 * This class uses a map to store the cateogry values as keys and probabilities as values.
 * It uses its own comparator class to deal with double precision.
 * By default, category values that differ less than 10E-9 will be considered identical.
 */
class AbstractDiscreteDistribution :
  public virtual DiscreteDistributionInterface,
  public AbstractParameterAliasable
{
public:
  static short DISCRETIZATION_EQUAL_PROB;
  static short DISCRETIZATION_EQUAL_INTERVAL;
  static short DISCRETIZATION_EQUAL_PROB_WHEN_POSSIBLE;

  /**
   * @brief Comparator class for AbstractDiscreteDistribution.
   */
  class Order
  {
private:
    double precision_;

public:
    Order(double prec = NumConstants::TINY()) :
      precision_(prec)
    {}

    Order(const Order& ord) :
      precision_(ord.precision_)
    {}

    Order& operator=(const Order& ord)
    {
      precision_ = ord.precision_;
      return *this;
    }

    double precision() const
    {
      return precision_;
    }

    void setPrecision(double prec)
    {
      precision_ = prec;
    }

    bool operator()(double l1, double l2) const
    {
      return l1 < l2 - precision_;
    }
  };

protected:
  /*
   * The number of categories
   */

  size_t numberOfCategories_;
  /**
   * These fields must be initialized in the constructor of the derived classes.
   */
  std::map<double, double, Order> distribution_;

  std::vector<double> bounds_;

  /**
   * @brief the interval where the distribution is defined/restricted.
   */
  std::shared_ptr<IntervalConstraint> intMinMax_;

  /**
   * Tells if the values in the classes is associated to the median or not (default: false)
   */
  bool median_;

  /**
   * Discretization policy.
   */
  short discretizationScheme_;

public:
  AbstractDiscreteDistribution(size_t nbClasses, const std::string& prefix = "", short discretization = DISCRETIZATION_EQUAL_PROB);

  /**
   * With additional precision value to discriminate categories (default 1e-12)
   */
  AbstractDiscreteDistribution(size_t nbClasses, double precision, const std::string& prefix = "", short discretization = DISCRETIZATION_EQUAL_PROB);

  AbstractDiscreteDistribution(const AbstractDiscreteDistribution& adde);

  AbstractDiscreteDistribution& operator=(const AbstractDiscreteDistribution& adde);

  virtual ~AbstractDiscreteDistribution() {}

public:
  /**
   * @name The DiscreteDistribution interface.
   *
   * @{
   */
  size_t getNumberOfCategories() const;
  void setNumberOfCategories(size_t nbClasses);
  double getCategory(size_t categoryIndex) const;
  double getProbability(size_t categoryIndex) const;
  double getProbability(double category) const;
  Vdouble getCategories() const;
  Vdouble getProbabilities() const;
  double getValueCategory(double value) const;
  size_t getCategoryIndex(double value) const;
  void set(double category, double probability);
  void add(double category, double probability);
  double getInfCumulativeProbability(double category) const;
  double getIInfCumulativeProbability(double category) const;
  double getSupCumulativeProbability(double category) const;
  double getSSupCumulativeProbability(double category) const;
  double rand() const;
  double randC() const { throw Exception("AbstractDiscreteDistribution::randC. No continuous version available for this distribution."); }

  /*
   *@return value of the internal bound
   *
   */
  double getBound(size_t i) const
  {
    if (i >= numberOfCategories_ - 1)
      throw IndexOutOfBoundsException("AbstractDiscreteDistribution::getBound(i)", i, 0, numberOfCategories_ - 1);
    return bounds_[i];
  }


  /*
   *@brief Information about the range of the distribution
   *
   */
  double getLowerBound() const
  {
    return intMinMax_->getLowerBound();
  }

  double getUpperBound() const
  {
    return intMinMax_->getUpperBound();
  }

  bool strictLowerBound() const
  {
    return intMinMax_->strictLowerBound();
  }

  bool strictUpperBound() const
  {
    return intMinMax_->strictUpperBound();
  }

  Vdouble getBounds() const;

  void print(OutputStream& out) const;

  double precision() const { return distribution_.key_comp().precision();}

  void setMedian(bool median)
  {
    if (median_ != median)
    {
      median_ = median;
      discretize();
    }
  }

  virtual void discretize();
  /** @} */

  /**
   * @brief Restricts the distribution to the domain where the
   * constraint is respected, in addition of other predefined
   * constraints.
   *
   * @param c The Constraint to respect.
   */
  virtual void restrictToConstraint(const ConstraintInterface& c);

protected:
  void discretizeEqualProportions();
  void discretizeEqualIntervals();
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_ABSTRACTDISCRETEDISTRIBUTION_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_INVARIANTMIXEDDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_INVARIANTMIXEDDISCRETEDISTRIBUTION_H


#include "AbstractDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discrete mixed distribution, with a one-category fixed value (called "invariant") and a user-specified multi-categories distribution.
 *
 * The term "invariant" comes from the use of such distributions in phylogenetics:
 * the fixed category corresponds to a value of 0 and describes invariant positions in an alignment.
 */
class InvariantMixedDiscreteDistribution :
  public AbstractDiscreteDistribution
{
private:
  std::unique_ptr<DiscreteDistributionInterface> dist_;
  double invariant_, p_;
  std::string nestedPrefix_;

public:
  /**
   * @brief Build a new InvariantMixedDiscreteDistribution object.
   *
   * @param dist            The distribution to use. The mixed distribution will "own" this distribution object.
   * This means that the distribution will be cloned in case of copy of this instance, and will be
   * deleted with this instance.
   * @param p               The probability of being in the invariant category.
   * @param invariant       The value of the invariant category (typically 0, but other values may be specified).
   */
  InvariantMixedDiscreteDistribution(std::unique_ptr<DiscreteDistributionInterface> dist, double p, double invariant = 0.);

  virtual ~InvariantMixedDiscreteDistribution() {}

  InvariantMixedDiscreteDistribution(const InvariantMixedDiscreteDistribution& imdd) :
    AbstractDiscreteDistribution(imdd),
    dist_(imdd.dist_->clone()),
    invariant_(imdd.invariant_),
    p_(imdd.p_),
    nestedPrefix_(imdd.nestedPrefix_)
  {}

  InvariantMixedDiscreteDistribution& operator=(const InvariantMixedDiscreteDistribution& imdd)
  {
    AbstractDiscreteDistribution::operator=(imdd);
    dist_.reset(imdd.dist_->clone());
    invariant_    = imdd.invariant_;
    p_            = imdd.p_;
    nestedPrefix_ = imdd.nestedPrefix_;
    return *this;
  }

  InvariantMixedDiscreteDistribution* clone() const { return new InvariantMixedDiscreteDistribution(*this); }

public:
  std::string getName() const { return "Invariant"; }

  void fireParameterChanged(const ParameterList& parameters);

  void setNamespace(const std::string& prefix);

  /**
   * @ brief Sets the number of categories of the embedded
   * distribution. The number of categories of this class equals this
   * plus one if the invariant_ is not in the embedded distribution
   * values.
   */
  void setNumberOfCategories(size_t nbClasses)
  {
    dist_->setNumberOfCategories(nbClasses);
    updateDistribution();
  }

  /**
   * @return The nested, conditional, sub-distribution.
   */
  const DiscreteDistributionInterface& variableSubDistribution() const { return *dist_; }

  double qProb(double x) const
  {
    return (x >= p_ + (1 - p_) * dist_->pProb(invariant_)) ? dist_->qProb((x - p_) / (1 - p_)) : dist_->qProb(x / (1 - p_));
  }

  double pProb(double x) const
  {
    return (1 - p_) * dist_->pProb(x) + (x < invariant_ ? 0 : p_);
  }

  double Expectation(double a) const
  {
    return (1 - p_) * dist_->Expectation(a) + (a < invariant_ ? 0 : p_);
  }

  void setMedian(bool median)
  {
    if (median_ != median)
    {
      median_ = median;
      dist_->setMedian(median);
      updateDistribution();
    }
  }

  void discretize()
  {
    dist_->discretize();
    updateDistribution();
  }

  void restrictToConstraint(const ConstraintInterface& c);

protected:
  void updateDistribution();
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_INVARIANTMIXEDDISCRETEDISTRIBUTION_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "InvariantMixedDiscreteDistribution.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

InvariantMixedDiscreteDistribution::InvariantMixedDiscreteDistribution(
    unique_ptr<DiscreteDistributionInterface> dist, double p, double invariant) :
  AbstractDiscreteDistribution(1, "Invariant."),
  dist_(std::move(dist)),
  invariant_(invariant),
  p_(p),
  nestedPrefix_(dist_->getNamespace())
{
  // We first change the namespace of the nested distribution:
  dist_->setNamespace("Invariant." + nestedPrefix_);
  addParameters_(dist_->getIndependentParameters());
  addParameter_(new Parameter("Invariant.p", p, Parameter::PROP_CONSTRAINT_IN));

  updateDistribution();
}

/******************************************************************************/

void InvariantMixedDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  AbstractDiscreteDistribution::fireParameterChanged(parameters);
  p_ = getParameterValue("p");
  dist_->matchParametersValues(parameters);

  updateDistribution();
}

/******************************************************************************/

void InvariantMixedDiscreteDistribution::updateDistribution()
{
  distribution_.clear();
  bounds_.clear();

  size_t distNCat = dist_->getNumberOfCategories();
  vector<double> probs = dist_->getProbabilities();
  vector<double> cats  = dist_->getCategories();

  distribution_[invariant_] = p_;
  for (size_t i = 0; i < distNCat; i++)
  {
    if (cats[i] == invariant_)
      distribution_[invariant_] += (1. - p_) * probs[i];
    else
      distribution_[cats[i]] = (1. - p_) * probs[i];
  }

  intMinMax_->setLowerBound(dist_->getLowerBound(), !dist_->strictLowerBound());
  intMinMax_->setUpperBound(dist_->getUpperBound(), !dist_->strictUpperBound());

  if (invariant_ <= intMinMax_->getLowerBound())
    intMinMax_->setLowerBound(invariant_, true);
  if (invariant_ >= intMinMax_->getUpperBound())
    intMinMax_->setUpperBound(invariant_, true);

  numberOfCategories_ = distribution_.size();

  // bounds_

  // if invariant_ is between 2 values of dist_, bounds_ are set in the
  // middle of the 3 values

  bool nv = true;

  double a = dist_->getCategory(0), b;
  if (nv && (invariant_ < a))
  {
    bounds_.push_back((a + invariant_) / 2);
    nv = false;
  }

  for (size_t i = 1; i < distNCat; i++)
  {
    b = dist_->getCategory(i);
    if (nv && (invariant_ < b))
    {
      bounds_.push_back((a + invariant_) / 2);
      bounds_.push_back((invariant_ + b) / 2);
      nv = false;
    }
    else
      bounds_.push_back(dist_->getBound(i - 1));
    a = b;
  }

  if (nv)
    bounds_.push_back((a + invariant_) / 2);
}

/******************************************************************************/

void InvariantMixedDiscreteDistribution::setNamespace(const string& prefix)
{
  AbstractDiscreteDistribution::setNamespace(prefix);
  // We also need to update the namespace of the nested distribution:
  dist_->setNamespace(prefix + nestedPrefix_);
}

/******************************************************************************/

void InvariantMixedDiscreteDistribution::restrictToConstraint(const ConstraintInterface& c)
{
  if (!c.isCorrect(invariant_))
    throw ConstraintException("Impossible to restrict to Constraint", &getParameter_("p"), invariant_);

  dist_->restrictToConstraint(c);
  updateDistribution();
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Random/RandomTools.h"
#include "../VectorTools.h"
#include "AbstractDiscreteDistribution.h"

using namespace bpp;
using namespace std;

short AbstractDiscreteDistribution::DISCRETIZATION_EQUAL_PROB = 1;
short AbstractDiscreteDistribution::DISCRETIZATION_EQUAL_INTERVAL = 2;
short AbstractDiscreteDistribution::DISCRETIZATION_EQUAL_PROB_WHEN_POSSIBLE = 3;
short AbstractDiscreteDistribution::DISCRETIZATION_FIXED_BOUNDS = 4;

AbstractDiscreteDistribution::AbstractDiscreteDistribution(size_t nbClasses, const std::string& prefix, short discretization) :
  AbstractParameterAliasable(prefix),
  numberOfCategories_(nbClasses),
  distribution_(),
  bounds_(nbClasses - 1),
  intMinMax_(new IntervalConstraint(-NumConstants::VERY_BIG(), NumConstants::VERY_BIG(), true, true)),
  median_(false),
  discretizationScheme_(discretization)
{}

AbstractDiscreteDistribution::AbstractDiscreteDistribution(size_t nbClasses, double delta, const std::string& prefix, short discretization) :
  AbstractParameterAliasable(prefix),
  numberOfCategories_(nbClasses),
  distribution_(Order(delta)),
  bounds_(nbClasses - 1),
  intMinMax_(new IntervalConstraint(-NumConstants::VERY_BIG(), NumConstants::VERY_BIG(), true, true)),
  median_(false),
  discretizationScheme_(discretization)
{}

AbstractDiscreteDistribution::AbstractDiscreteDistribution(const vector<double>& bounds, const std::string& prefix) :
  AbstractParameterAliasable(prefix),
  numberOfCategories_(bounds.size() - 1),
  distribution_(),
  bounds_(bounds.begin() + 1, bounds.end() - 2),
  intMinMax_(new IntervalConstraint(*bounds.begin(), *bounds.rbegin(), true, true)),
  median_(false),
  discretizationScheme_(DISCRETIZATION_FIXED_BOUNDS)
{}

AbstractDiscreteDistribution::AbstractDiscreteDistribution(const AbstractDiscreteDistribution& adde) :
  AbstractParameterAliasable(adde),
  numberOfCategories_(adde.numberOfCategories_),
  distribution_(adde.distribution_),
  bounds_(adde.bounds_),
  intMinMax_(adde.intMinMax_->clone()),
  median_(adde.median_),
  discretizationScheme_(adde.discretizationScheme_)
{}

AbstractDiscreteDistribution& AbstractDiscreteDistribution::operator=(const AbstractDiscreteDistribution& adde)
{
  AbstractParameterAliasable::operator=(adde);
  numberOfCategories_ = adde.numberOfCategories_;
  distribution_ = adde.distribution_;
  bounds_ = adde.bounds_;
  intMinMax_ = std::shared_ptr<IntervalConstraint>(adde.intMinMax_->clone());
  median_ = adde.median_;
  discretizationScheme_ = adde.discretizationScheme_;

  return *this;
}

/******************************************************************************/

size_t AbstractDiscreteDistribution::getNumberOfCategories() const
{
  return numberOfCategories_;
}

void AbstractDiscreteDistribution::setNumberOfCategories(size_t nbClasses)
{
  if (nbClasses <= 0)
    cerr << "DEBUG: ERROR!!! Number of categories is <= 0 in AbstractDiscreteDistribution::setNumberOfCategories()." << endl;

  if (numberOfCategories_ != nbClasses)
  {
    numberOfCategories_ = nbClasses;
    discretize();
  }
}


/******************************************************************************/

double AbstractDiscreteDistribution::getCategory(size_t categoryIndex) const
{
  map<double, double>::const_iterator it = distribution_.begin();
  for (unsigned int i = 0; i < categoryIndex; i++)
  {
    it++;
  }
  return it->first;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getProbability(size_t categoryIndex) const
{
  map<double, double>::const_iterator it = distribution_.begin();
  for (unsigned int i = 0; i < categoryIndex; i++)
  {
    it++;
  }
  return it->second;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getProbability(double category) const
{
  return distribution_.find(category)->second;
}

/******************************************************************************/

Vdouble AbstractDiscreteDistribution::getCategories() const
{
  Vdouble result(distribution_.size());
  unsigned int i = 0;
  for (map<double, double>::const_iterator it = distribution_.begin();
      it != distribution_.end();
      it++)
  {
    result[i] = it->first;
    i++;
  }
  return result;
}

/******************************************************************************/

Vdouble AbstractDiscreteDistribution::getProbabilities() const
{
  Vdouble result(distribution_.size());
  size_t i = 0;
  for (map<double, double>::const_iterator it = distribution_.begin();
      it != distribution_.end();
      it++)
  {
    result[i] = it->second;
    i++;
  }
  return result;
}

/******************************************************************************/

void AbstractDiscreteDistribution::set(double category, double probability)
{
  distribution_[category] = probability;
}

/******************************************************************************/

void AbstractDiscreteDistribution::add(double category, double probability)
{
  if (distribution_.find(category) == distribution_.end())
  {
    // new category
    distribution_[category] = probability;
  }
  else
  {
    // existing category
    distribution_[category] += probability;
  }
}

/******************************************************************************/

double AbstractDiscreteDistribution::rand() const
{
  double r = RandomTools::giveRandomNumberBetweenZeroAndEntry(1);
  double cumprob = 0;
  for (map<double, double>::const_iterator i = distribution_.begin();
      i != distribution_.end();
      i++)
  {
    cumprob += i->second;
    if (r <= cumprob)
      return i->first;
  }
  // This line can't be reached:
  return -1.;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getInfCumulativeProbability(double category) const
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  for (map<double, double>::const_iterator i = distribution_.begin();
      i != it;
      i++)
  {
    prob += i->second;
  }
  return prob;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getIInfCumulativeProbability(double category) const
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  if (it == distribution_.end())
    return 0;
  for (map<double, double>::const_iterator i = ++it;
      i != distribution_.end();
      i++)
  {
    prob += i->second;
  }
  return 1. - prob;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getSupCumulativeProbability(double category) const
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  if (it == distribution_.end())
    return 0;
  for (map<double, double>::const_iterator i = ++it;
      i != distribution_.end();
      i++)
  {
    prob += i->second;
  }
  return prob;
}

/******************************************************************************/

double AbstractDiscreteDistribution::getSSupCumulativeProbability(double category) const
{
  double prob = 0;
  map<double, double>::const_iterator it = distribution_.find(category);
  for (map<double, double>::const_iterator i = distribution_.begin();
      i != it;
      i++)
  {
    prob += i->second;
  }
  return 1. - prob;
}

/******************************************************************************/

void AbstractDiscreteDistribution::print(OutputStream& out) const
{
  for (map<double, double>::const_iterator i = distribution_.begin(); i != distribution_.end(); i++)
  {
    (out << "Pr(" << (i->first) << ") = " << (i->second)).endLine();
  }
}

/******************************************************************************/

double AbstractDiscreteDistribution::getValueCategory(double value) const
{
  if (!(intMinMax_->isCorrect(value)))
    throw Exception("AbstractDiscreteDistribution::getValueCategory out of bounds:" + TextTools::toString(value));

  map<double, double>::const_iterator it = distribution_.begin();
  for (unsigned int i = 1; i < bounds_.size(); i++)
  {
    if (value < bounds_[i])
      break;
    else
      it++;
  }

  return it->first;
}

/******************************************************************************/

size_t AbstractDiscreteDistribution::getCategoryIndex(double value) const
{
  if (!(intMinMax_->isCorrect(value)))
    throw Exception("AbstractDiscreteDistribution::getValueCategory out of bounds:" + TextTools::toString(value));

  for (unsigned int i = 1; i < bounds_.size(); i++)
  {
    if (value < bounds_[i])
      return i;
  }

  throw bounds_.size();
}

/***********************************************************************/

void AbstractDiscreteDistribution::discretizeEqualProportions()
{
  /* discretization of distribution with equal proportions in each
     category
   */

  distribution_.clear();
  bounds_.resize(numberOfCategories_ - 1);

  double minX = pProb(intMinMax_->getLowerBound());
  double maxX = pProb(intMinMax_->getUpperBound());

  double ec;
  size_t i;
  vector<double> values(numberOfCategories_);

  if (maxX != minX)
  {
    // divide the domain into equiprobable intervals
    ec = (maxX - minX) / static_cast<double>(numberOfCategories_);
    for (i = 1; i < numberOfCategories_; i++)
    {
      bounds_[i - 1] = qProb(minX + static_cast<double>(i) * ec);
    }

    // for each category, sets the value v as the median, adjusted
    //      such that the sum of the values = 1
    if (median_)
    {
      double t = 0;
      for (i = 0; i < numberOfCategories_; i++)
      {
        values[i] = qProb(minX + (static_cast<double>(i) + 0.5) * ec);
      }

      for (i = 0, t = 0; i < numberOfCategories_; i++)
      {
        t += values[i];
      }

      double mean = Expectation(intMinMax_->getUpperBound()) - Expectation(intMinMax_->getLowerBound());

      for (i = 0; i < numberOfCategories_; i++)
      {
        values[i] *= mean / t / ec;
      }
    }
    else
    // for each category, sets the value v such that
    //      v * length_of_the_interval = the surface of the category
    {
      double firstBound = intMinMax_->getLowerBound(), secondBound;
      double a = Expectation(firstBound), b;
      for (i = 0; i < numberOfCategories_ - 1; i++)
      {
        secondBound = bounds_[i];
        b = Expectation(secondBound);
        values[i] = (b - a) / ec;
        if (values[i] < firstBound || values[i] > secondBound)   // May happen if the two bounds are undistinguishable.
        {
          values[i] = (firstBound + secondBound) / 2.;
        }
        a = b;
        firstBound = secondBound;
      }
      secondBound = intMinMax_->getUpperBound();
      values[numberOfCategories_ - 1] = (Expectation(secondBound) - a) / ec;
      if (values[numberOfCategories_ - 1] < firstBound || values[numberOfCategories_ - 1] > secondBound)   // May happen if the two bounds are undistinguishable.
      {
        values[numberOfCategories_ - 1] = (firstBound + secondBound) / 2.;
      }
    }
  }
  else
  // if maxX==minX, uniform discretization of the range
  {
    ec = (intMinMax_->getUpperBound() - intMinMax_->getLowerBound()) / static_cast<double>(numberOfCategories_);
    for (i = 1; i < numberOfCategories_; i++)
    {
      bounds_[i - 1] = intMinMax_->getLowerBound() + static_cast<double>(i) * ec;
    }

    values[0] = (intMinMax_->getLowerBound() + bounds_[0]) / 2;

    for (i = 1; i < numberOfCategories_ - 1; i++)
    {
      values[i] = (bounds_[i - 1] + bounds_[i]) / 2;
    }

    values[numberOfCategories_ - 1] = (intMinMax_->getUpperBound() + bounds_[numberOfCategories_ - 1]) / 2;
  }

  // adjustments near the boundaries of the domain, according to the precision chosen
  if (intMinMax_->strictLowerBound())
  {
    for (i = 0; i < numberOfCategories_; i++)
    {
      if (values[i] < intMinMax_->getLowerBound() + precision())
        values[i] = intMinMax_->getLowerBound() + precision();
      else
        break;
    }
  }
  else
  {
    for (i = 0; i < numberOfCategories_; i++)
    {
      if (values[i] < intMinMax_->getLowerBound())
        values[i] = intMinMax_->getLowerBound() + precision();
      else
        break;
    }
  }

  if (intMinMax_->strictUpperBound())
  {
    for (i = numberOfCategories_; i > 0; i--)
    {
      if (values[i - 1] > intMinMax_->getUpperBound() - precision())
        values[i - 1] = intMinMax_->getUpperBound() - precision();
      else
        break;
    }
  }
  else
  {
    for (i = numberOfCategories_; i > 0; i--)
    {
      if (values[i - 1] > intMinMax_->getUpperBound())
        values[i - 1] = intMinMax_->getUpperBound() - precision();
      else
        break;
    }
  }

  // now the distribution_ map, taking care that all values are different

  double p = 1. / static_cast<double>(numberOfCategories_);
  for (i = 0; i < numberOfCategories_; i++)
  {
    if (distribution_.find(values[i]) != distribution_.end())
    {
      int j = 1;
      int f = ((values[i] + NumConstants::TINY()) >= intMinMax_->getUpperBound()) ? -1 : 1;
      while (distribution_.find(values[i] + f * j * precision()) != distribution_.end())
      {
        j++;
        f = ((values[i] + f * j * precision()) >= intMinMax_->getUpperBound()) ? -1 : 1;
      }
      distribution_[values[i] + f * j * precision()] = p;
    }
    else
      distribution_[values[i]] = p;
  }

  return;
}

/***********************************************************************/

void AbstractDiscreteDistribution::discretizeEqualIntervals()
{
  /* discretization of distribution with equal intervals
   */

  distribution_.clear();
  bounds_.resize(numberOfCategories_ - 1);
  vector<double> values(numberOfCategories_);

  double lowerBound = intMinMax_->getLowerBound();
  double upperBound = intMinMax_->getUpperBound();
  double condProb = pProb(upperBound) - pProb(lowerBound);
  double interval = (upperBound - lowerBound) / static_cast<double>(numberOfCategories_);

  // Compute bounds:
  for (size_t i = 0; i < numberOfCategories_ - 1; ++i)
  {
    bounds_[i] = lowerBound + (static_cast<double>(i) + 1.) * interval;
  }

  // Compute values:
  for (size_t i = 0; i < numberOfCategories_; ++i)
  {
    values[i] = lowerBound + (static_cast<double>(i) + 0.5) * interval;
  }

  // Compute proportions:
  deque<double> allBounds(bounds_.begin(), bounds_.end());
  allBounds.push_front(lowerBound);
  allBounds.push_back(upperBound);
  for (size_t i = 0; i < numberOfCategories_; ++i)
  {
    distribution_[values[i]] = (pProb(allBounds[i + 1]) - pProb(allBounds[i])) / condProb;
  }

  return;
}

/***********************************************************************/

void AbstractDiscreteDistribution::discretizeFixedBounds()
{
  /* discretization of distribution with a pre-defined set of bounds
   */

  distribution_.clear();
  vector<double> values(numberOfCategories_);

  double lowerBound = intMinMax_->getLowerBound();
  double upperBound = intMinMax_->getUpperBound();
  double condProb = pProb(upperBound) - pProb(lowerBound);

  // Compute values:
  deque<double> allBounds(bounds_.begin(), bounds_.end());
  allBounds.push_front(lowerBound);
  allBounds.push_back(upperBound);
  for (size_t i = 0; i < numberOfCategories_; ++i)
  {
    values[i] = (allBounds[i] + allBounds[i+1]) / 2.;
  }

  // Compute proportions:
  for (size_t i = 0; i < numberOfCategories_; ++i)
  {
    distribution_[values[i]] = (pProb(allBounds[i + 1]) - pProb(allBounds[i])) / condProb;
  }

  return;
}

/***********************************************************************/

void AbstractDiscreteDistribution::discretize()
{
  if (discretizationScheme_ == DISCRETIZATION_EQUAL_PROB)
  {
    discretizeEqualProportions();
  }
  else if (discretizationScheme_ == DISCRETIZATION_EQUAL_INTERVAL)
  {
    discretizeEqualIntervals();
  }
  else if (discretizationScheme_ == DISCRETIZATION_FIXED_BOUNDS)
  {
    discretizeFixedBounds();
  }
  else
  {
    discretizeEqualProportions();
    // Check bounds:
    deque<double> allBounds(bounds_.begin(), bounds_.end());
    allBounds.push_front(intMinMax_->getLowerBound());
    allBounds.push_back(intMinMax_->getUpperBound());
    bool check = true;
    for (size_t i = 1; check && i < numberOfCategories_ + 1; ++i)
    {
      if (allBounds[i] == allBounds[i - 1])
        check = false;
    }
    if (!check)
    {
      // cout << "Unidentifiable bounds. Falling back to equal intervals." << endl;
      discretizeEqualIntervals();
    }
  }
}

/***********************************************************************/

Vdouble AbstractDiscreteDistribution::getBounds() const
{
  Vdouble vb(numberOfCategories_ + 1);
  vb[0] = getLowerBound();
  for (unsigned int i = 0; i < numberOfCategories_ - 1; i++)
  {
    vb[i + 1] = getBound(i);
  }
  vb[numberOfCategories_] = getUpperBound();
  return vb;
}

void AbstractDiscreteDistribution::restrictToConstraint(const ConstraintInterface& c)
{
  try
  {
    const IntervalConstraint& pi = dynamic_cast<const IntervalConstraint&>(c);

    if (!(*intMinMax_ <= pi))
    {
      *intMinMax_ &= c;
      discretize();
    }
  }
  catch (exception& e)
  {
    throw Exception("AbstractDiscreteDistribution::restrictToConstraint: the constraint is not an interval");
  }
}

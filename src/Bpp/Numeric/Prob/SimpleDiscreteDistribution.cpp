// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Text/TextTools.h"
#include "../../Utils/MapTools.h"
#include "../NumConstants.h"
#include "SimpleDiscreteDistribution.h"

using namespace bpp;
using namespace std;


SimpleDiscreteDistribution::SimpleDiscreteDistribution(const map<double, double>& distribution,
    double prec,
    bool fixed) :
  AbstractDiscreteDistribution(distribution.size(), prec, "Simple."),
  givenRanges_()
{
  double sum = 0;
  for (map<double, double>::const_iterator i = distribution.begin(); i != distribution.end(); i++)
  {
    distribution_[i->first] = i->second;
    sum += i->second;
  }
  if (fabs(1. - sum) > precision())
    throw Exception("SimpleDiscreteDistribution. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  if (!fixed)
  {
    unsigned int n = 1;
    double x, y = 1;
    for (map<double, double>::const_iterator i = distribution.begin(); i != distribution.end(); i++)
    {
      addParameter_(new Parameter("Simple.V" + TextTools::toString(n), i->first));

      if (n != numberOfCategories_)
      {
        x = i->second;
        addParameter_(new Parameter("Simple.theta" + TextTools::toString(n), x / y, Parameter::PROP_CONSTRAINT_IN));
        y -= x;
      }
      n++;
    }
  }
  discretize();
}

SimpleDiscreteDistribution::SimpleDiscreteDistribution(const vector<double>& values,
    const vector<double>& probas,
    double prec,
    bool fixed
    ) :
  AbstractDiscreteDistribution(values.size(), prec, "Simple."),
  givenRanges_()
{
  if (values.size() != probas.size())
  {
    throw Exception("SimpleDiscreteDistribution. Values and probabilities vectors must have the same size (" + TextTools::toString(values.size()) + " != " + TextTools::toString(probas.size()) + ").");
  }
  size_t size = values.size();

  for (size_t i = 0; i < size; i++)
  {
    if (distribution_.find(values[i]) != distribution_.end())
      throw Exception("SimpleDiscreteDistribution: two given values are equal");
    else
      distribution_[values[i]] = probas[i];
  }

  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > precision())
    throw Exception("SimpleDiscreteDistribution. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  if (!fixed)
  {
    double y = 1;
    for (size_t i = 0; i < size - 1; i++)
    {
      addParameter_(new Parameter("Simple.V" + TextTools::toString(i + 1), values[i]));
      addParameter_(new Parameter("Simple.theta" + TextTools::toString(i + 1), probas[i] / y, Parameter::PROP_CONSTRAINT_IN));
      y -= probas[i];
    }
    addParameter_(new Parameter("Simple.V" + TextTools::toString(size), values[size - 1]));
  }

  discretize();
}

SimpleDiscreteDistribution::SimpleDiscreteDistribution(const std::vector<double>& values,
    const std::map<size_t, std::vector<double>>& ranges,
    const std::vector<double>& probas,
    double prec,
    bool fixed) :
  AbstractDiscreteDistribution(values.size(), prec, "Simple."),
  givenRanges_()
{
  if (values.size() != probas.size())
  {
    throw Exception("SimpleDiscreteDistribution. Values and probabilities vectors must have the same size (" + TextTools::toString(values.size()) + " != " + TextTools::toString(probas.size()) + ").");
  }
  size_t size = values.size();

  for (size_t i = 0; i < size; i++)
  {
    if (distribution_.find(values[i]) != distribution_.end())
      throw Exception("SimpleDiscreteDistribution: two given values are equal");
    else
      distribution_[values[i]] = probas[i];
  }

  double sum = VectorTools::sum(probas);
  if (fabs(1. - sum) > precision())
    throw Exception("SimpleDiscreteDistribution. Probabilities must equal 1 (sum =" + TextTools::toString(sum) + ").");

  if (!fixed)
  {
    double y = 1;
    for (size_t i = 0; i < size - 1; i++)
    {
      map<size_t, vector<double>>::const_iterator it = ranges.find(i + 1);
      if (it == ranges.end())
        addParameter_(new Parameter("Simple.V" + TextTools::toString(i + 1), values[i]));
      else
      {
        if (values[i] >= it->second[0] &&  values[i] <= it->second[1])
        {
          addParameter_(new Parameter("Simple.V" + TextTools::toString(i + 1), values[i], std::make_shared<IntervalConstraint>(it->second[0], it->second[1], true, true)));
          givenRanges_[i + 1] = it->second;
        }
        else
          throw Exception("SimpleDiscreteDistribution. Value and given range of parameter V" + TextTools::toString(i + 1) + " do not match: " + TextTools::toString(values[i]) + " vs [" + TextTools::toString(it->second[0]) + ";" + TextTools::toString(it->second[1]) + "]");
      }
      addParameter_(new Parameter("Simple.theta" + TextTools::toString(i + 1), probas[i] / y, Parameter::PROP_CONSTRAINT_IN));
      y -= probas[i];
    }

    map<size_t, vector<double>>::const_iterator it = ranges.find(size);
    if (it == ranges.end())
      addParameter_(new Parameter("Simple.V" + TextTools::toString(size), values[size - 1]));
    else
    {
      if (values[size - 1] >= it->second[0] &&  values[size - 1] <= it->second[1])
      {
        addParameter_(new Parameter("Simple.V" + TextTools::toString(size), values[size - 1], std::make_shared<IntervalConstraint>(it->second[0], it->second[1], true, true)));
        givenRanges_[size] = it->second;
      }
      else
        throw Exception("SimpleDiscreteDistribution. Value and given range of parameter V" + TextTools::toString(size) + " do not match: " + TextTools::toString(values[size - 1]) + " vs [" + TextTools::toString(it->second[0]) + ";" + TextTools::toString(it->second[1]) + "]");
    }
  }

  discretize();
}


SimpleDiscreteDistribution::SimpleDiscreteDistribution(const SimpleDiscreteDistribution& sdd) :
  AbstractDiscreteDistribution(sdd),
  givenRanges_(sdd.givenRanges_)
{}

SimpleDiscreteDistribution& SimpleDiscreteDistribution::operator=(const SimpleDiscreteDistribution& sdd)
{
  AbstractDiscreteDistribution::operator=(sdd);
  givenRanges_ = sdd.givenRanges_;

  return *this;
}

void SimpleDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{
  if (getNumberOfParameters() != 0)
  {
    AbstractDiscreteDistribution::fireParameterChanged(parameters);
    size_t size = distribution_.size();

    distribution_.clear();
    double x = 1.0;
    double v;
    for (size_t i = 0; i < size; i++)
    {
      v = getParameterValue("V" + TextTools::toString(i + 1));
      double v2(v);
      if (distribution_.find(v2) != distribution_.end())
      {
        int j(1);
        while (true)
        {
          v2 = v + j * precision();
          if (v2 < intMinMax_->getUpperBound() && (distribution_.find(v2) == distribution_.end()))
            break;

          v2 = v - j * precision();
          if (v2 > intMinMax_->getLowerBound() && (distribution_.find(v2) == distribution_.end()))
            break;
          j++;
        }
        // approximation to avoid useless computings:
        // setParameterValue("V"+TextTools::toString(i+1),v);
      }
      if (i < size - 1)
      {
        distribution_[v2] = getParameterValue("theta" + TextTools::toString(i + 1)) * x;
        x *= 1 - getParameterValue("theta" + TextTools::toString(i + 1));
      }
      else
        distribution_[v2] = x;
    }
  }

  discretize();
}

double SimpleDiscreteDistribution::qProb(double x) const
{
  double s = -NumConstants::VERY_BIG();
  double x2 = x;
  for (map<double, double>::const_iterator it = distribution_.begin(); it != distribution_.end(); it++)
  {
    x2 -= it->second;
    if (x2 < 0)
      return s;
    else
      s = it->second;
  }

  return s;
}

double SimpleDiscreteDistribution::pProb(double x) const
{
  double s = 0;
  for (map<double, double>::const_iterator it = distribution_.begin(); it != distribution_.end(); it++)
  {
    if (it->first >= x)
      s += it->second;
    else
      break;
  }

  return s;
}

double SimpleDiscreteDistribution::Expectation(double a) const
{
  double s = 0;
  for (map<double, double>::const_iterator it = distribution_.begin(); it != distribution_.end(); it++)
  {
    if (it->first >= a)
      s += it->second;
    else
      break;
  }

  return s;
}


void SimpleDiscreteDistribution::discretize()
{
  // Compute a new arbitray bounderi:
  vector<double> values = MapTools::getKeys<double, double, AbstractDiscreteDistribution::Order>(distribution_);

  // Fill from 0 to numberOfCategories_-2 with midpoints:
  for (unsigned int i = 0; i < numberOfCategories_ - 1; i++)
  {
    bounds_[i] = (values[i] + values[i + 1]) / 2.;
  }
}

void SimpleDiscreteDistribution::restrictToConstraint(const ConstraintInterface& c)
{
  if (getNumberOfParameters() == 0)
    return;

  const IntervalConstraint* pi = dynamic_cast<const IntervalConstraint*>(&c);

  if (!pi)
    throw Exception("SimpleDiscreteDistribution::restrictToConstraint: Non-interval exception");

  map<double, double>::const_iterator it;

  for (it = distribution_.begin(); it != distribution_.end(); it++)
  {
    if (!pi->isCorrect(it->first))
      throw Exception("Impossible to restrict to Constraint value " + TextTools::toString(it->first));
  }

  AbstractDiscreteDistribution::restrictToConstraint(c);

  size_t size = distribution_.size();
  for (size_t i = 0; i < size; i++)
  {
    map<size_t, vector<double>>::const_iterator itr = givenRanges_.find(i + 1);
    if (itr == givenRanges_.end())
      getParameter_("V" + TextTools::toString(i + 1)).setConstraint(intMinMax_);
    else
    {
      auto pc = getParameter_("V" + TextTools::toString(i + 1)).removeConstraint();
      getParameter_("V" + TextTools::toString(i + 1)).setConstraint(std::shared_ptr<ConstraintInterface>(*pc & *intMinMax_));
    }
  }
}

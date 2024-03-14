// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../Utils/MapTools.h"
#include "../Random/RandomTools.h"
#include "UniformDiscreteDistribution.h"

using namespace bpp;

// From the STL:
#include <cmath>

using namespace std;

/** Constructor: **************************************************************/

UniformDiscreteDistribution::UniformDiscreteDistribution(unsigned int n, double min, double max) :
  AbstractDiscreteDistribution(n, "Uniform."),
  min_((min < max) ? min : max),
  max_((min < max) ? max : min)
{
  intMinMax_->setLowerBound(min_, false);
  intMinMax_->setUpperBound(max_, false);

  discretize();
}

UniformDiscreteDistribution::UniformDiscreteDistribution(const UniformDiscreteDistribution& udd) :
  AbstractDiscreteDistribution(udd),
  min_(udd.min_),
  max_(udd.max_)
{}

UniformDiscreteDistribution& UniformDiscreteDistribution::operator=(const UniformDiscreteDistribution& udd)
{
  AbstractDiscreteDistribution::operator=(udd);
  min_ = udd.min_;
  max_ = udd.max_;

  return *this;
}

UniformDiscreteDistribution::~UniformDiscreteDistribution() {}

/******************************************************************************/

void UniformDiscreteDistribution::fireParameterChanged(const ParameterList& parameters)
{}

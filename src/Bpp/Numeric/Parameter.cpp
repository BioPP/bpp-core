//
// File: Parameter.cpp
// Created by: Julien Dutheil
// Created on: Wed Oct 15 15:40:47 2003
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

  This software is a computer program whose purpose is to provide classes
  for numerical calculus.

  This software is governed by the CeCILL  license under French law and
  abiding by the rules of distribution of free software.  You can  use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and,  more generally, to use and operate it in the
  same conditions as regards security.

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#include "Parameter.h"
#include <algorithm>

namespace bpp
{
  /******************************************************************************/

  ParameterEvent::ParameterEvent(Parameter* parameter) noexcept
    : parameter_(parameter)
  {
  }

  /** Constructors: *************************************************************/

  Parameter::Parameter() = default;

  Parameter::Parameter(const std::string& name,
                       double value,
                       Constraint* constraint,
                       bool attachConstraint,
                       double precision)
    : name_(name)
    , constraint_(constraint, ConditionalDeleter<Constraint>{attachConstraint})
  {
    // This may throw a ConstraintException:
    setValue(value);
    setPrecision(precision);
  }

  Parameter::Parameter(const std::string& name, double value, const Constraint* constraint, double precision)
    : name_(name)
    , constraint_(constraint ? constraint->clone() : nullptr)
  {
    // This may throw a ConstraintException:
    setValue(value);
    setPrecision(precision);
  }

  Parameter::~Parameter() = default;

  /** Name: *********************************************************************/

  void Parameter::setName(const std::string& name)
  {
    name_ = name;
    ParameterEvent event(this);
    fireParameterNameChanged(event);
  }

  /** Value: ********************************************************************/

  void Parameter::setValue(double value)
  {
    if (std::abs(value - value_) > precision_ / 2)
    {
      if (constraint_ && !constraint_->isCorrect(value))
        throw ConstraintException("Parameter::setValue", this, value);
      value_ = value;
      ParameterEvent event(this);
      fireParameterValueChanged(event);
    }
  }

  /** Precision: ********************************************************************/

  void Parameter::setPrecision(double precision) { precision_ = (precision < 0) ? 0 : precision; }

  /** Constraint: ***************************************************************/

  void Parameter::setConstraint(Constraint* constraint, bool attach)
  {
    if (constraint != nullptr && !constraint->isCorrect(value_))
      throw ConstraintException("Parameter::setConstraint", this, value_);
    constraint_.reset(constraint);
    constraint_.get_deleter().ownsPointer = attach;
  }

  Constraint* Parameter::removeConstraint() { return constraint_.release(); }

  /******************************************************************************/

  void Parameter::removeParameterListener(const std::string& listenerId)
  {
    using ConstRef = typename decltype(listeners_)::const_reference;
    listeners_.erase(std::remove_if(listeners_.begin(),
                                    listeners_.end(),
                                    [&listenerId](ConstRef pl) { return pl->getId() == listenerId; }),
                     listeners_.end());
  }

  /******************************************************************************/

  bool Parameter::hasParameterListener(const std::string& listenerId)
  {
    using ConstRef = typename decltype(listeners_)::const_reference;
    return std::any_of(
      listeners_.begin(), listeners_.end(), [&listenerId](ConstRef pl) { return pl->getId() == listenerId; });
  }

  /******************************************************************************/

  const IntervalConstraint Parameter::R_PLUS(true, 0, true);
  const IntervalConstraint Parameter::R_PLUS_STAR(true, 0, false);
  const IntervalConstraint Parameter::R_MINUS(false, 0, true);
  const IntervalConstraint Parameter::R_MINUS_STAR(false, 0, false);
  const IntervalConstraint Parameter::PROP_CONSTRAINT_IN(0, 1, true, true);
  const IntervalConstraint Parameter::PROP_CONSTRAINT_EX(0, 1, false, false);

  /******************************************************************************/
}

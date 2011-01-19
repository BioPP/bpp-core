//
// File: Constraints.h
// Created by: Julien Dutheil
// Created on: Thu Dec 25 19:35:17 2003
//

/*
   Copyright or © or Copr. CNRS, (November 17, 2004)

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

#ifndef _CONSTRAINTS_H_
#define _CONSTRAINTS_H_

// From the STL:
#include <string>

// From Utils:
#include "../Clonable.h"
#include "../Text/TextTools.h"

#include "NumConstants.h"

namespace bpp
{
/**
 * @brief The constraint interface.
 *
 * It provides a method that tells if a given value is correct.
 */
class Constraint : public Clonable
{
public:
  Constraint() {}
  virtual ~Constraint() {}

  Constraint* clone() const = 0;

public:
  /**
   * @brief Tell if a given value is correct.
   *
   * @param value The value to test.
   * @return True is the value is correct.
   */
  virtual bool isCorrect(double value) const = 0;

  /**
   * @brief Tell if all the values in a given interval are correct.
   *
   * @param min, max  The bounds of the interval.
   * @return True is the value is correct.
   */
  virtual bool includes(double min, double max) const = 0;

  /**
   * @brief Give the nearest limit for a bad value.
   *
   * @param value The bad value.
   * @return The nearer limit.
   */
  virtual double getLimit(double value) const = 0;

  /**
   * @brief Give the nearest accepted limit for a bad value.
   *
   * The difference with getLimit() is when the Constraint is open at
   * the limit, in which case the retruned value is the limit +- 1e-12.
   *
   * @param value The bad value.
   * @return The nearer limit.
   */
  virtual double getAcceptedLimit(double value) const = 0;

  /**
   * @brief Give a short description on the type of constraint.
   *
   * @return A string which describes the constraint.
   */
  virtual std::string getDescription() const = 0;

  /**
   * @brief Intersect this Constraint with another one
   *
   * @param c the intersected Constraint
   * @return the intersection
   */
  virtual Constraint* operator&(const Constraint& c) const = 0;
};

/**
 * @brief An interval, either bounded or not
 */

class Interval : public Constraint
{
protected:
  /**
   * @brief The boundaries of the interval
   *
   **/
  double lowerBound_, upperBound_;

  /**
   * @brief Boolean flags are true if the boundaries are included
   *
   **/
  bool inclLowerBound_, inclUpperBound_;

public:
  Interval() :  lowerBound_(-NumConstants::VERY_BIG),
    upperBound_(NumConstants::VERY_BIG),
    inclLowerBound_(true),
    inclUpperBound_(true) {}

  Interval(double lowerBound, double upperBound, bool inclLower, bool inclUpper) :  lowerBound_(lowerBound),
    upperBound_(upperBound),
    inclLowerBound_(inclLower),
    inclUpperBound_(inclUpper) {}
  virtual ~Interval() {}

  Interval* clone() const { return new Interval(*this); }

public:
  void setLowerBound(double lowerBound, bool strict) { lowerBound_ = lowerBound; inclLowerBound_ = !strict; }
  void setUpperBound(double upperBound, bool strict) { upperBound_ = upperBound; inclUpperBound_ = !strict; }

  double getLowerBound() const { return lowerBound_; }
  double getUpperBound() const { return upperBound_; }

  bool strictLowerBound() const {return !inclLowerBound_; }
  bool strictUpperBound() const {return !inclUpperBound_; }

  bool includes(double min, double max) const
  {
    return (inclLowerBound_ ? min >= getLowerBound() : min > getLowerBound()) &&
           (inclUpperBound_ ? max <= getUpperBound() : max < getUpperBound());
  }

  virtual bool isCorrect(double value) const
  {
    return (inclLowerBound_ ? value >= getLowerBound() : value > getLowerBound()) &&
           (inclUpperBound_ ? value <= getUpperBound() : value < getUpperBound());
  }

  bool operator<(double value) const
  {
    return inclUpperBound_ ? upperBound_ < value : upperBound_ <= value;
  }

  bool operator>(double value) const
  {
    return inclLowerBound_ ? lowerBound_ > value : lowerBound_ >= value;
  }

  bool operator<=(double value) const
  {
    return upperBound_ <= value;
  }

  bool operator>=(double value) const
  {
    return lowerBound_ >= value;
  }

  double getLimit(double value) const
  {
    return isCorrect(value) ? value :
           (*this >= value ? lowerBound_ : upperBound_);
  }

  double getAcceptedLimit(double value) const
  {
    return isCorrect(value) ? value :
           (*this >= value ?
            strictLowerBound() ? lowerBound_ + NumConstants::TINY : lowerBound_ :
            strictUpperBound() ? upperBound_ - NumConstants::TINY : upperBound_);
  }

  std::string getDescription() const
  {
    return (inclLowerBound_ ? "[ " : "]")
           + (lowerBound_ == -NumConstants::VERY_BIG ? "-inf" : TextTools::toString(lowerBound_))
           + ", "
           + (upperBound_ == NumConstants::VERY_BIG ? "+inf" : TextTools::toString(upperBound_))
           + (inclUpperBound_ ? "] " : "[");
  }

  /**
   * @brief Intersect this Interval with another one
   *
   * @param c the intersected Interval
   * @return the intersection, or NULL if c is not an Interval
   */
  Constraint* operator&(const Constraint& c) const
  {
    double lowerBound, upperBound;
    bool inclLowerBound, inclUpperBound;

    const Interval* pi = dynamic_cast<const Interval*>(&c);

    if (pi)
    {
      if (lowerBound_ <= pi->lowerBound_)
      {
        lowerBound = pi->lowerBound_;
        inclLowerBound = pi->inclLowerBound_;
      }
      else
      {
        lowerBound = lowerBound_;
        inclLowerBound = inclLowerBound_;
      }

      if (upperBound_ >= pi->upperBound_)
      {
        upperBound = pi->upperBound_;
        inclUpperBound = pi->inclUpperBound_;
      }
      else
      {
        upperBound = upperBound_;
        inclUpperBound = inclUpperBound_;
      }
      return new Interval(lowerBound, upperBound, inclLowerBound, inclUpperBound);
    }
    else
      return 0;
  }

  /**
   * @brief Intersect this Interval with another one
   *
   * @param c the intersected Interval
   * @return this Interval modified, or not modified if c is not an Interval
   */
  Interval& operator&=(const Constraint& c)
  {
    const Interval* pi = dynamic_cast<const Interval*>(&c);

    if (pi)
    {
      if (lowerBound_ <= pi->lowerBound_)
      {
        lowerBound_ = pi->lowerBound_;
        inclLowerBound_ = pi->inclLowerBound_;
      }
      else
      {
        lowerBound_ = lowerBound_;
        inclLowerBound_ = inclLowerBound_;
      }

      if (upperBound_ >= pi->upperBound_)
      {
        upperBound_ = pi->upperBound_;
        inclUpperBound_ = pi->inclUpperBound_;
      }
      else
      {
        upperBound_ = upperBound_;
        inclUpperBound_ = inclUpperBound_;
      }
    }

    return *this;
  }

  /**
   * @brief Tells if this interval equals another one
   *
   * @param i the compared Interval
   */
  bool operator==(const Interval& i) const
  {
    return lowerBound_ == i.lowerBound_
           && inclLowerBound_ == i.inclLowerBound_
           && upperBound_ == i.upperBound_
           && inclUpperBound_ == i.inclUpperBound_
    ;
  }

  /**
   * @brief Tells if this interval is different from another one
   *
   * @param i the compared Interval
   */
  bool operator!=(const Interval& i) const
  {
    return lowerBound_ != i.lowerBound_
           || inclLowerBound_ != i.inclLowerBound_
           || upperBound_ != i.upperBound_
           || inclUpperBound_ != i.inclUpperBound_
    ;
  }

  /**
   * @brief Tells if this interval is included or equal in another one
   *
   * @param i the compared Interval
   */
  bool operator<=(const Interval& i) const
  {
    return lowerBound_ >= i.lowerBound_
           && upperBound_ <= i.upperBound_;
  }
};


/**
 * @brief Including positive real constraint.
 */
class IncludingPositiveReal : public Interval
{
public:
  IncludingPositiveReal(double lowerBound) :
    Interval(lowerBound, NumConstants::VERY_BIG, true, true) {}
  bool isCorrect(double value) const
  {
    return value >= getLowerBound();
  }
};

/**
 * @brief Excluding positive real constraint.
 */
class ExcludingPositiveReal : public Interval
{
public:
  ExcludingPositiveReal(double lowerBound) :
    Interval (lowerBound, NumConstants::VERY_BIG, false, true) {}
  bool isCorrect(double value) const
  {
    return value > getLowerBound();
  }
};

/**
 * @brief Including neg tive real constraint.
 */
class IncludingNegativeReal : public Interval
{
public:
  IncludingNegativeReal(double upperBound) :
    Interval(-NumConstants::VERY_BIG, upperBound, true, true) {}
  bool isCorrect(double value) const
  {
    return value <= getUpperBound();
  }
};

/**
 * @brief Excluding positive real constraint.
 */
class ExcludingNegativeReal : public Interval
{
public:
  ExcludingNegativeReal(double upperBound) :
    Interval(-NumConstants::VERY_BIG, upperBound, true, false) {}

  bool isCorrect(double value) const
  {
    return value < getUpperBound();
  }
};


/**
 * @brief Including interval.
 */
class IncludingInterval : public Interval
{
public:
  /**
   * @brief Build a new including interval constraint.
   *
   * @param lowerBound The lower bound of the interval.
   * @param upperBound The upper bound of the interval.
   */
  IncludingInterval(double lowerBound, double upperBound) :
    Interval(lowerBound, upperBound, true, true) {}
  bool isCorrect(double value) const
  {
    return value >= getLowerBound() && value <= getUpperBound();
  }
};

/**
 * @brief Excluding interval.
 */
class ExcludingInterval : public Interval
{
public:
  /**
   * @brief Build a new excluding interval constraint.
   *
   * @param lowerBound The lower bound of the interval.
   * @param upperBound The upper bound of the interval.
   */
  ExcludingInterval(double lowerBound, double upperBound) :
    Interval(lowerBound, upperBound, false, false) {}
  bool isCorrect(double value) const
  {
    return value > getLowerBound() && value < getUpperBound();
  }
};

/**
 * @brief Left-including, right-excluding interval.
 */
class IncludingExcludingInterval : public Interval
{
public:
  /**
   * @brief Build a new left-including, right-excluding interval constraint.
   *
   * @param lowerBound The lower bound of the interval.
   * @param upperBound The upper bound of the interval.
   */
  IncludingExcludingInterval(double lowerBound, double upperBound) :
    Interval(lowerBound, upperBound, true, false) {}
  bool isCorrect(double value) const
  {
    return value >= getLowerBound() && value < getUpperBound();
  }
};

/**
 * @brief Left-excluding, right-including interval.
 */
class ExcludingIncludingInterval : public Interval
{
public:
  /**
   * @brief Build a new left-excluding, right-including interval constraint.
   *
   * @param lowerBound The lower bound of the interval.
   * @param upperBound The upper bound of the interval.
   */
  ExcludingIncludingInterval(double lowerBound, double upperBound) :
    Interval(lowerBound, upperBound, false, true) {}
  bool isCorrect(double value) const
  {
    return value > getLowerBound() && value <= getUpperBound();
  }
};
} // end of namespace bpp.

#endif  // _CONSTRAINTS_H_


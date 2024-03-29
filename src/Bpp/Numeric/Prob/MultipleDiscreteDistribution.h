// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_MULTIPLEDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_MULTIPLEDISCRETEDISTRIBUTION_H


#include "../../Exceptions.h"
#include "../../Io/OutputStream.h"
#include "../ParameterAliasable.h"
#include "../VectorTools.h"

namespace bpp
{
/**
 * @brief Interface for multiple discrete distribution objects.
 *
 * A multiple discrete distribution usually contains a vector of
 * finite set of categories and a probability associated to each.
 * The size of the vector is the dimension of the distribution.
 *
 * Each category (or class) is defined by two bounds, and sometimes
 * by a mean or a median value.
 *
 * A multiple discrete distribution may contain one or several
 * parameters. The probabilities associated to each class usually
 * depends on the parameter values. In some cases, the number and/or
 * bounds of the classes may also depend on the parameters values,
 * depending on the kind of discretization used.
 */

class MultipleDiscreteDistribution :
  public virtual ParameterAliasable
{
public:
  MultipleDiscreteDistribution() {}

  virtual ~MultipleDiscreteDistribution() {}

  MultipleDiscreteDistribution* clone() const = 0;

public:
  /**
   * @return The number of categories
   */
  virtual size_t getNumberOfCategories() const = 0;

  /**
   * @param Vvalue The vector of values to check.
   * @return The vector of categories of the classes the value is
   * in. Throws a ConstraintException if the value is off the domain
   * of the MultipleDiscreteDistribution.
   */

  virtual Vdouble getValueCategory(Vdouble& Vvalue) const = 0;

  /**
   * @param category The vector of values associated to the class.
   * @return The probability associated to a given class.
   */
  virtual double getProbability(Vdouble& category) const = 0;

public:
  /**
   * @brief Draw a random vector from this distribution.
   *
   * This vector will be one of the class values, drawn according
   * to the class probabilities.
   *
   * @return A random number according to this distribution.
   */
  virtual Vdouble rand() const = 0;

  /**
   * @brief Draw a random vector from the continuous version of this distribution, if it exists.
   *
   * Uses the continuous version of this distribution to draw a random vector.
   *
   * @return A random vector according to this distribution.
   * @throw Exception If there is no continuous version of this distribution.
   */
  virtual Vdouble randC() const = 0;

  /**
   * @brief Checks if the Parameters can respect the given
   * Constraints (one per dimension) and optionnaly tries to modify
   * their own Constraints.
   *
   * @param vc The vector of Constraint to respect.
   * @param f boolean flag to say if the Constraints must be changed
   * (if possible) (default: true)
   *
   * @return true if the Parameters Constraints are adapted to the
   * given Constraints, false otherwise.
   */
  // virtual bool adaptToConstraint(const std::vector<Constraint&>& vc, bool f=true) = 0;

  /**
   * @brief Print the distribution (categories and corresponding probabilities) to a stream.
   *
   * @param out The outstream where to print the distribution.
   */
  //    virtual void print(OutputStream& out) const = 0;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_MULTIPLEDISCRETEDISTRIBUTION_H

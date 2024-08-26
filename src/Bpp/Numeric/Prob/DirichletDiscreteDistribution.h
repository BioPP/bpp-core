// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_DIRICHLETDISCRETEDISTRIBUTION_H
#define BPP_NUMERIC_PROB_DIRICHLETDISCRETEDISTRIBUTION_H


#include "../../Exceptions.h"
#include "../../Io/OutputStream.h"
#include "../ParameterAliasable.h"
#include "../VectorTools.h"
#include "BetaDiscreteDistribution.h"
#include "MultipleDiscreteDistribution.h"

namespace bpp
{
/**
 * @brief Discretized Dirichlet distribution. If the distribution is
 * in n dimensions, the domain is the n-1 simplex.
 *
 * The discretization is made in the order of these dimensions. Then
 * the order of the dimensions is not neutral on the resulting
 * categories.
 *
 * Here is an example of the discretization process: if the
 * distribution is in 3 dimensions, and the number of categories is
 * 4 in the first dimension, 3 in the second and 5 in the third, the
 * first interval is split in 4 equi-probable intervals, and each
 * interval is split on the second dimension in 3 equi-probable
 * intervals, and each of the 12 resulting 2D intervals is split in
 * 5 equi-probable 3D-intervals. Eventually, there are 60
 * equi-probable categories.
 *
 * In the same example, if the distribution is followed by the
 * random vector @f$ (X_1,X_2,X_3) @f$, the 1D distribution used for
 * @f$ X_1 @f$ is the marginal distribution, which discretized
 * values are say, @f$ v_1^1, v_1^2, v_1^3, v_1^4 @f$, then the four
 * 1D distributions used for @f$ X_2 @f$ are the conditional
 * marginal distributions @f$ (X_2|X_1=v_1^1), (X_2|X_1=v_1^2),
 * (X_2|X_1=v_1^3), (X_2|X_1=v_1^4) @f$. And in a similar way for
 * @f$ X_3 @f$, with the 12 resulting conditions on @f$ X_1 @f$ and
 * @f$ X_2 @f$.
 *
 *
 * It uses the AbstractDiscreteDistribution comparator class to deal
 * with double precision. By default, category values that differ
 * less than 10E-9 will be considered identical.
 *
 *
 * The successive discretization process is done using the
 * distributions, if the parameters are @f$ (\alpha_1, ...,
 * \alpha_n)@f$:
 *
 * @f$ X_1 @f$ follows Beta(@f$ \alpha_1, \sum_{i=2}^n \alpha_i @f$).
 *
 * @f$ X_j @f$ follows @f$ (1-\sum_{i=1}^{j-1} X_i) @f$ *
 * Beta(@f$ \alpha_j, \sum_{i=j+1}^n \alpha_i @f$).
 *
 * @f$ X_n = 1 - \sum_{i=1}^{n-1} X_i @f$.
 *
 *
 * The parameters are: alpha_1, ... alpha_n @f$ \in [0.0001;\infty[
 * @f$.
 */

class DirichletDiscreteDistribution :
  public MultipleDiscreteDistribution,
  public AbstractParameterAliasable
{
private:
  std::vector<BetaDiscreteDistribution*> vpBDD_;

public:
  /**
   * @brief Build a new discretized Dirichlet distribution
   *
   * @param vn the vector of the dim-1 numbers of categories to use.
   * @param valpha the vector of the dim alpha parameters.
   */
  DirichletDiscreteDistribution(std::vector<size_t> vn, Vdouble valpha);

  ~DirichletDiscreteDistribution();

  DirichletDiscreteDistribution* clone() const
  {
    return new DirichletDiscreteDistribution(*this);
  }

protected:
  void applyParameters();

public:
  std::string getName() const {return "Dirichlet";}

  void fireParameterChanged(const ParameterList& parameters);

  /**
   * @return The number of categories
   */
  size_t getNumberOfCategories() const;

  /**
   * @param Vvalue
   * @return The vector of categoryIndex of the classes the value is
   * in. Throws a ConstraintException if the value is off the domain
   * of the DirichletDiscreteDistribution.
   */
  Vdouble getValueCategory(Vdouble& Vvalue) const;

  VVdouble getCategories() const;

  /**
   * @param category The vector of values associated to the class.
   * @return The probability associated to a given class.
   */
  virtual double getProbability(Vdouble& category) const;

  /**
   * @brief Draw a random vector from this distribution.
   *
   * This vector will be one of the class values, drawn according
   * to the class probabilities.
   *
   * @return A random number according to this distribution.
   */

  Vdouble rand() const;

  /**
   * @brief Draw a random vector from the continuous version of this
   * distribution.
   *
   * @return A random vector according to this distribution.
   */
  Vdouble randC() const;

protected:
  void discretize(Vdouble& valpha);
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PROB_DIRICHLETDISCRETEDISTRIBUTION_H

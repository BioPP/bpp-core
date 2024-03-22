// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PROB_SIMPLEX_H
#define BPP_NUMERIC_PROB_SIMPLEX_H


// From the STL:
#include <vector>

#include "../AbstractParameterAliasable.h"

namespace bpp
{
/**
 * @brief A Simplex object, used to define sets of probabilities that
 * sum 1.
 *
 * The probabilities are denoted @f$p_i@f$ for @f$i \in <1,n>@f$.
 *
 * If they are parametrized, the parameters are called \c "theta1",
 * ..., \c "theta(n-1)".
 *
 * Three ways of parametrization are available:
 *
 * Global ratio:
 *
 * @f$\forall i<n, \theta_i=\frac{p_i}{1-(p_1+...+p_{i-1})}@f$.
 *
 * In the reverse,
 * @f$\forall i<n, p_i= (1-\theta_1).(1-\theta_2)...\theta_{i}@f$
 * and @f$p_n=(1-\theta_1).(1-\theta_2)...(1-\theta_{n-1})@f$.
 *
 *
 * Local ratio:
 *
 * @f$\theta_i = \frac{p_i}{p_i+p_{i+1}} \forall i \in 1..\textrm{n-1}@f$.
 *
 * In the reverse if we denote @f$\alpha_i=\frac{1-\theta_i}{\theta_i}@f$,
 * @f$p_i=\frac{\alpha_1...\alpha_{i-1}}{1+\sum_{k=1}^{n-1}\alpha_1...\alpha_k}@f$.
 *
 * Binary:
 *
 * This parametrization is based on the binary coding.
 *
 * Given @f$a_b...a_1@f$ the writing of i in binary, we denote
 * @f$i_k=a_k...a_1@f$.
 *
 * Given @f$a_b...a_1@f$ the writing of i in binary where @f$a_b=1@f$,
 * we denote @f$1_i=\sum_j\{p_{j+1} \text{ such that }
 * j_b=1i_{b-1}\}@f$ and @f$0_i=\sum_j\{p_{j+1} \text{ such that }
 * j_b=0i_{b-1}\}@f$, and then we define:
 *
 *
 * @f$\theta_i=\frac{1_i}{1_i+0_i}@f$
 *
 * and on the reverse, we denote @f$\theta'_{0i_{b-1}}=1-\theta_i@f$
 * and @f$\theta'_{1i_{b-1}}=\theta_i@f$.
 *
 * Then, if @f$c=ceil(log_2(n))@f$, for @f$i \in <0,n-1>@f$.
 *
 * @f$p_{i+1}=\theta'_{i_c}....\theta'_{i_1} @f$
 *
 */
class Simplex :
  public AbstractParameterAliasable
{
private:
  /**
   * @brief The dimension+1 of the space simplex (ie the number of probabilities).
   *
   */
  size_t dim_;

  /**
   * @brief the method of parametrization.
   *
   * 0: No parametrization
   * 1: Global ratio
   * 2: Local ratio
   * 3: Binary
   */
  unsigned short method_;

  std::vector<double> vProb_;

  /**
   * @brief just used with local ratio (method 2)
   */
  std::vector<double> valpha_;

public:
  /**
   * @brief Builds a new Simplex object from a number of
   * probabilities. They are initialized equal.
   *
   * @param dim The number of probabilities.
   * @param method  tells the method of parametrization (default 0)
   *    0: No parametrization
   *    1: Global ratio
   *    2: Local ratio
   *    3: Binary
   * @param allowNull if null probabilites are allowed (default: false)
   * @param name The name passed to AbstractParameterAliasable constructor.
   *
   */
  Simplex(size_t dim, unsigned short method = 0, bool allowNull = false, const std::string& name = "Simplex.");

  /**
   * @brief Builds a new Simplex object from a vector of probabilities
   *
   * @param probas The vector of probabilities.
   * @param method  tells the method of parametrization (default 0)
   *    0: No parametrization
   *    1: Global ratio
   *    2: Local ratio
   *    3: Binary
   * @param allowNull if null probabilites are allowed (default: false)
   * @param name The name passed to AbstractParameterAliasable constructor.
   *
   */
  Simplex(const std::vector<double>& probas, unsigned short method = 0, bool allowNull = false, const std::string& name = "Simplex.");

  virtual ~Simplex() {}

  Simplex* clone() const { return new Simplex(*this); }

public:
  void fireParameterChanged(const ParameterList& parameters);

  size_t dimension() const { return dim_; }

  void setFrequencies(const std::vector<double>&);

  const std::vector<double>& getFrequencies() const { return vProb_;}

  std::vector<double>& getFrequencies() { return vProb_;}

  double prob(size_t i) const { return vProb_[i]; }

  unsigned short getMethod() const { return method_; }
};

/**
 * @brief Simplex where all values are in decreasing order.
 *
 * The parameters @f$\theta_i@f$ are similar to the parameters of
 * Simplex, with same methods, but there is an additionnal step to
 * compute the values from the parameters.
 *
 * From the probabilities @f$p_i@f$ for @f$i \in <1,n>@f$ computed
 * from the parameters, the decreasing values @f$v_i@f$ for @f$i \in
 * <1,n>@f$ are such that @f$p_i = i * (v_i - v_{i+1})@f$ if @f$ i<n
 * @f$ and @f$ p_n = n * v_n @f$. @f$\sum_i v_i = \sum_i p_i = 1 @f$
 * and @f$ v_i - v_{i+1} = \frac{p_i}{i} > 0@f$. On the reverse:
 *
 * @f$ v_i = \sum_{j=i}^n \frac{p_j}{j} @f$
 *
 *
 **/

class OrderedSimplex :
  public Simplex
{
private:
  std::vector<double> vValues_;

public:
  /**
   * @brief Builds a new Simplex object from a number of
   * probabilities. They are initialized equal.
   *
   * @param dim The number of probabilities.
   * @param method  tells the method of parametrization (default 0)
   *    0: No parametrization
   *    1: Global ratio
   *    2: Local ratio
   *    3: Binary
   * @param allowNull if null probabilites are allowed (default: false)
   * @param name The name passed to AbstractParameterAliasable constructor.
   *
   */
  OrderedSimplex(size_t dim, unsigned short method = 0, bool allowNull = false, const std::string& name = "Simplex.") :
    Simplex(dim, method, allowNull, name),
    vValues_(dim)
  {
    const auto& probs = Simplex::getFrequencies();

    double x = 0;
    for (auto i = dim; i > 0; i--)
    {
      x += probs[i - 1] / (int)i;
      vValues_[i - 1] = x;
    }
  }


  /**
   * @brief Builds a new Simplex object from a vector of probabilities
   *
   * @param probas The vector of probabilities.
   * @param method  tells the method of parametrization (default 0)
   *    0: No parametrization
   *    1: Global ratio
   *    2: Local ratio
   *    3: Binary
   * @param allowNull if null probabilites are allowed (default: false)
   * @param name The name passed to AbstractParameterAliasable constructor.
   *
   */

  OrderedSimplex(const std::vector<double>& probas, unsigned short method = 0, bool allowNull = false, const std::string& name = "Simplex.");

  void fireParameterChanged(const ParameterList& parameters);

  void setFrequencies(const std::vector<double>&);

  const std::vector<double>& getFrequencies() const { return vValues_;}
};
} // end of namespace bpp.

#endif // BPP_NUMERIC_PROB_SIMPLEX_H

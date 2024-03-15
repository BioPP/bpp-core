// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_RANDOM_RANDOMTOOLS_H
#define BPP_NUMERIC_RANDOM_RANDOMTOOLS_H


#include "../../Exceptions.h"
#include "../VectorExceptions.h"
#include "../VectorTools.h"

// From the STL:
#include <cmath>
#include <cassert>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>

namespace bpp
{
/**
 * @brief Utilitary function dealing with random numbers.
 *
 * This class is adapted from Pupko's SEMPHY library.
 * It also borrow some code from Yang's PAML package.
 *
 * Most of these function are provided for convenience, directly using
 * the "random" library might prove more efficient.
 *
 **/

class RandomTools
{
public:
  RandomTools() {}
  virtual ~RandomTools() {}

private:
  /**
   *@brief functions for the computation of incompleteBeta
   */
  static double incompletebetafe(double a,
                                 double b,
                                 double x,
                                 double big,
                                 double biginv);
  static double incompletebetafe2(double a,
                                  double b,
                                  double x,
                                  double big,
                                  double biginv);
  static double incompletebetaps(double a,
                                 double b,
                                 double x,
                                 double maxgam);

public:
  static std::random_device RANDOM_DEVICE;
  static std::mt19937 DEFAULT_GENERATOR;

  /**
   * @brief Set the default generator seed.
   *
   * @param seed New seed.
   */
  static void setSeed(std::mt19937::result_type seed)
  {
    DEFAULT_GENERATOR.seed(seed);
  }

  /**
   * @brief Get a double random value (between 0 and specified range).
   *
   * @param entry Max number to reach.
   */
  static double giveRandomNumberBetweenZeroAndEntry(double entry)
  {
    std::uniform_real_distribution<double> dis(0, entry);
    return dis(DEFAULT_GENERATOR);
  }

  /**
   * @brief Get a boolean random value.
   *
   * @param prob Probability of getting 'true'.
   */
  static bool flipCoin(double prob = 0.5)
  {
    std::bernoulli_distribution d(prob);
    return d(DEFAULT_GENERATOR);
  }

  /**
   * @brief Get an integer random value (between 0 and specified range).
   *
   * Note : the number you get is between 0 and entry not including entry !
   * @param entry Max number to reach.
   *
   */
  template<class intType>
  static intType giveIntRandomNumberBetweenZeroAndEntry(intType entry)
  {
    if (entry == 0)
      throw Exception("RandomTools::giveIntRandomNumberBetweenZeroAndEntry. Entry must be at least 1.");
    std::uniform_int_distribution<intType> dis(0, entry - 1);
    return dis(DEFAULT_GENERATOR);
  }

  /**
   * @return A random number drawn from a normal distribution.
   * @param mean The mean of the law.
   * @param variance The variance of the law.
   */
  static double randGaussian(double mean, double variance)
  {
    std::normal_distribution<double> dis(mean, sqrt(variance));
    return dis(DEFAULT_GENERATOR);
  }

  /**
   * @return A random number drawn from a gamma distribution with unit scale (beta=1).
   * @param alpha The alpha parameter.
   */
  static double randGamma(double alpha)
  {
    std::gamma_distribution<double> dis(alpha, 1.);
    return dis(DEFAULT_GENERATOR);
  }

  /**
   * @return A random number drawn from a gamma distribution.
   * @param alpha The alpha parameter.
   * @param beta The beta parameter.
   */
  static double randGamma(double alpha, double beta)
  {
    std::gamma_distribution<double> dis(alpha, beta);
    return dis(DEFAULT_GENERATOR);
  }

  /**
   * @return A random number drawn from a beta distribution.
   * @param alpha The alpha parameter.
   * @param beta The beta parameter.
   */
  static double randBeta(double alpha, double beta);

  /**
   * @return A random number drawn from an exponential distribution.
   * @param mean The mean of the distribution.
   */
  static double randExponential(double mean)
  {
    std::exponential_distribution<double> dis(mean);
    return dis(DEFAULT_GENERATOR);
  }

  /**
   * @brief Pick (and extract) one element randomly in a vector and return it.
   * @param v The vector of elements.

   * @param replace If set to yes, then elements are allowed to be
   *             picked more than once, and therefore can be
   *             re-"placed" in the final sample (default: false, in
   *             which case the vector will lost one element).
   *
   * @return One element of the vector.
   * @throw EmptyVectorException if the vector is empty.
   *
   * @author Sylvain Gaillard
   */
  template<class T>
  static T pickOne(std::vector<T>& v, bool replace = false)
  {
    if (v.empty())
      throw EmptyVectorException<T>("RandomTools::pickOne: input vector is empty", &v);
    size_t pos = RandomTools::giveIntRandomNumberBetweenZeroAndEntry<size_t>(v.size());
    if (replace)
      return v[pos];
    else
    {
      T e = v[pos];
      v[pos] = v.back();
      v.pop_back();
      return e;
    }
  }

  /**
   * @brief Pick one element randomly in a vector and return it.
   * @param v The vector of elements.
   *
   * @return One element of the vector.
   * @throw EmptyVectorException if the vector is empty.
   *
   * @author Sylvain Gaillard
   */
  template<class T>
  static T pickOne(const std::vector<T>& v)
  {
    if (v.empty())
      throw EmptyVectorException<T>("RandomTools::pickOne: input vector is empty", &v);
    size_t pos = RandomTools::giveIntRandomNumberBetweenZeroAndEntry<size_t>(v.size());
    return v[pos];
  }

  /**
   * @brief Sample a vector.
   *
   * The sample is a new vector of the specified size.
   * If the size of the sample is identical to the original vector,
   * the result is a shuffle of the original vector.
   *
   * @param vin The vector to sample.
   * @param vout [out] The output vector to fill, with the appropriate size.
   * @param replace Should sampling be with replacement?
   * @return A vector which is a sample of v.
   * @throw IndexOutOfBoundException if the sample size exceeds the original
   * size when sampling without replacement.
   * @throw EmptyVectorException if the vector is empty.
   *
   * @author Sylvain Gaillard
   */
  template<class T>
  static void getSample(const std::vector<T>& vin, std::vector<T>& vout, bool replace = false)
  {
    if (vout.size() > vin.size() && !replace)
      throw IndexOutOfBoundsException("RandomTools::getSample: size exceeded v.size.", vout.size(), 0, vin.size());
    if (replace)
    {
      for (size_t i = 0; i < vout.size(); ++i)
      {
        vout[i] = pickOne(vin);
      }
    }
    else
    {
      std::vector<size_t> hat(vin.size());
      std::iota(hat.begin(), hat.end(), 0);
      std::shuffle(hat.begin(), hat.end(), DEFAULT_GENERATOR);
      for (size_t i = 0; i < vout.size(); i++)
      {
        vout[i] = vin[hat[i]];
      }
    }
  }

  /**
   * @brief Pick one element in a vector, with associated probability weights.
   *
   * Pick one element randomly in a vector and return it.
   * If you choose to make the picking without replacement the element is
   * removed from the vector, and so is the corresponding weight.
   *
   * @param v The vector of elements.
   * @param w The vector of weight associated to the v elements.
   * @param replace Should pick with replacement? (default: false)
   * @return One element of the vector.
   * @throw EmptyVectorException if the vector is empty.
   *
   * @author Julien Dutheil
   */
  template<class T>
  static T pickOne(std::vector<T>& v, std::vector<double>& w, bool replace = false)
  {
    if (v.empty())
      throw EmptyVectorException<T>("RandomTools::pickOne (with weight): input vector is empty", &v);
    // Compute cumulative sum of weights:
    std::vector<double> sumw = VectorTools::cumSum(w);
    // Convert to cumulative distribution:
    sumw /= sumw.back();
    // Get random positions:
    double prob = RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);
    size_t pos = v.size() - 1;
    for (size_t i = 0; i < v.size(); ++i)
    {
      if (prob < sumw[i])
      {
        pos = i;
        break;
      }
    }
    if (replace)
      return v[pos];
    else
    {
      T e = v[pos];
      v[pos] = v.back();
      v.pop_back();
      w[pos] = w.back();
      w.pop_back();
      return e;
    }
  }

  /**
   * @brief Pick one element in a vector, with associated probability weights
   *
   * Pick one element randomly in a vector and return it, with no
   * change in the original vector.
   *
   * @param v The vector of elements.
   * @param w The vector of weight associated to the v elements.
   * @return One element of the vector.
   * @throw EmptyVectorException if the vector is empty.
   *
   * @author Julien Dutheil
   */
  template<class T>
  static T pickOne(const std::vector<T>& v, const std::vector<double>& w)
  {
    if (v.empty())
      throw EmptyVectorException<T>("RandomTools::pickOne (with weight): input vector is empty", &v);
    // Compute cumulative sum of weights:
    std::vector<double> sumw = VectorTools::cumSum(w);
    // Convert to cumulative distribution:
    sumw /= sumw.back();
    // Get random positions:
    double prob = RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);
    size_t pos = v.size() - 1;
    for (size_t i = 0; i < v.size(); ++i)
    {
      if (prob < sumw[i])
      {
        pos = i;
        break;
      }
    }
    return v[pos];
  }

  /**
   * @brief Pick one index from a cumsum vector of probabilities.
   *
   * Last probability of the vector is assumed to be one.
   *
   * @param w The vector of cumsumed weights.
   * @return An index from the vector.
   * @throw EmptyVectorException if the vector is empty.
   *
   * @author Laurent Guéguen
   */
  static size_t pickFromCumSum(const std::vector<double>& w)
  {
    double prob = RandomTools::giveRandomNumberBetweenZeroAndEntry(1.0);
    size_t pos = 0;
    while (pos < w.size() - 1)
    {
      if (prob <= w[pos])
        return pos;
      pos += 1;
    }
    return w.size() - 1;
  }

  /**
   * @brief Sample a vector, with associated probability weights.
   *
   * The sample is a new vector of the specified size.
   * If the size of the sample is identical to the original vector,
   * the result is a shuffle of the original vector.
   *
   * It has to be noted that in case of sampling without replacement,
   * the effect of the weighting scheme will be weaker as the sample size becomes
   * close to the population size. In case the two are equal (pure permutations),
   * the weigths have no effect at all.
   *
   * @param vin [in] The vector to sample.
   * @param w [in] The vector of weights.
   * @param vout [out] The output vector to fill, with the appropriate size already set.
   * @param replace Should sampling be with replacement?
   * @return A vector which is a sample of v.
   * @throw IndexOutOfBoundException if the sample size exceeds the original
   * size when sampling without replacement.
   * @throw EmptyVectorException if the vector is empty.
   * @warning: For large sampling with replacement, the function is not efficient,
   * as it recomputes the some of weights for each element picked.
   * @author Julien Dutheil
   */
  template<class T>
  static void getSample(const std::vector<T>& vin, const std::vector<double>& w, std::vector<T>& vout, bool replace = false)
  {
    if (vout.size() > vin.size() && !replace)
      throw IndexOutOfBoundsException("RandomTools::getSample (with weights): size exceeded v.size.", vout.size(), 0, vin.size());
    std::vector<size_t> hat(vin.size());
    std::iota(hat.begin(), hat.end(), 0);
    if (replace)
    {
      for (size_t i = 0; i < vout.size(); i++)
      {
        vout[i] = vin[pickOne(hat, w)];
      }
    }
    else
    {
      std::vector<double> w2(w); // non const copy
      for (size_t i = 0; i < vout.size(); i++)
      {
        vout[i] = vin[pickOne(hat, w2, false)];
      }
    }
  }

  /**
   * @brief Get a random state from a set of probabilities/scores.
   *
   * The input probabilities are scaled so that they sum to one.
   * If 'x' probabilities are provided as input, the output vector will contain values between 0 and 'x-1'.
   *
   * @param n The sample size.
   * @param probs The set of intput probabilities.
   * @return A vector of int values corresponding to the output states. States are supposed to be in the same order as the input probabilities, the first state being '0'.
   */
  static std::vector<size_t> randMultinomial(size_t n, const std::vector<double>& probs);

  /**
   * @name Probability functions.
   *
   * @{
   * Adapted from Yang's PAML package.
   *
   */

  /**
   * @brief Normal quantile function.
   *
   * Returns z so that Prob{x<z}=prob where x ~ N(0,1) and (1e-12)<prob<1-(1e-12)
   * returns (-9999) if in error
   * Odeh RE & Evans JO (1974) The percentage points of the normal distribution.
   * Applied Statistics 22: 96-97 (AS70)
   *
   * Newer methods:
   *  Wichura MJ (1988) Algorithm AS 241: the percentage points of the
   *    normal distribution.  37: 477-484.
   *  Beasley JD & Springer SG  (1977).  Algorithm AS 111: the percentage
   *    points of the normal distribution.  26: 118-121.
   *
   * @param prob The probability.
   * @return The quantile corresponding to prob.
   */
  static double qNorm(double prob);

  /**
   * @brief Normal quantile function.
   *
   * Returns z so that Prob{x<z}=prob where x ~ N(mu,sigma^2) and (1e-12)<prob<1-(1e-12)
   * returns (-9999) if in error
   * Odeh RE & Evans JO (1974) The percentage points of the normal distribution.
   * Applied Statistics 22: 96-97 (AS70)
   *
   * Newer methods:
   *  Wichura MJ (1988) Algorithm AS 241: the percentage points of the
   *    normal distribution.  37: 477-484.
   *  Beasley JD & Springer SG  (1977).  Algorithm AS 111: the percentage
   *    points of the normal distribution.  26: 118-121.
   *
   * @param prob The probability.
   * @param mu The mean of the distribution
   * @param sigma The standard deviation of the distribution
   * @return The quantile corresponding to prob.
   */
  static double qNorm(double prob, double mu, double sigma);

  /**
   * @brief Computes \f$ln\left(\Gamma\left(\alpha\right)\right)\f$ given \f$\alpha\f$.
   *
   * @param alpha Alpha parameter.
   * @return \f$ln\left(\Gamma\left(\alpha\right)\right)\f$
   */
  static double lnGamma(double alpha) { return std::lgamma(alpha); }

  /**
   * @brief Returns the incomplete gamma ratio I(x,alpha).
   *
   * X is the upper limit of the integration and alpha is the shape parameter.
   * returns (-1) if in error
   * ln_gamma_alpha = ln(Gamma(alpha)), is almost redundant.
   * (1) series expansion     if (alpha>x || x<=1)
   * (2) continued fraction   otherwise
   * RATNEST FORTRAN by
   * Bhattacharjee GP (1970) The incomplete gamma integral.  Applied Statistics,
   * 19: 285-287 (AS32)
   *
   * @param x the upper limit of the integration.
   * @param alpha the shape parameter.
   * @param ln_gamma_alpha ln(Gamma(alpha)).
   */
  static double incompleteGamma(double x, double alpha, double ln_gamma_alpha);


  /**
   * @brief \f$\chi^2\f$ quantile function.
   *
   * returns z so that Prob{x<z}=prob where x is Chi2 distributed with df=v
   * returns -1 if in error.   0.000002<prob<0.999998
   * RATNEST FORTRAN by
   * Best DJ & Roberts DE (1975) The percentage points of the
   * Chi2 distribution.  Applied Statistics 24: 385-388.  (AS91)
   * Converted into C by Ziheng Yang, Oct. 1993.
   *
   * @param prob The probability.
   * @param v number of degree of freedom.
   * @return The quantile corresponding to prob.
   */
  static double qChisq(double prob, double v);

  /**
   * @brief \f$\chi^2\f$ cumulative probability function.
   *
   * @param x The quantile for which the probability should be computed.
   * @param v number of degree of freedom.
   * @return The corresponding probability of the quantile.
   */
  static double pChisq(double x, double v)
  {
    if (x < 0) return 0;
    return pGamma(x, v / 2, 0.5);
  }

  /**
   * @brief The Gamma quantile function.
   *
   * @param prob The probability.
   * @param alpha Alpha parameter.
   * @param beta  Beta parameter.
   * @return The quantile corresponding to prob.
   */
  static double qGamma(double prob, double alpha, double beta)
  {
    return qChisq(prob, 2.0 * (alpha)) / (2.0 * (beta));
  }

  /**
   * @brief \f$\Gamma\f$ cumulative probability function.
   *
   * @param x The quantile for which the probability should be computed.
   * @param alpha Alpha parameter.
   * @param beta  Beta parameter.
   * @return The corresponding probability of the quantile.
   * @throw Exception If alpha or beta is invalid (<0).
   *
   */
  static double pGamma(double x, double alpha, double beta)
  {
    if (alpha < 0) throw Exception("RandomTools::pGamma. Negative alpha is not allowed.");
    if (beta < 0) throw Exception("RandomTools::pGamma. Negative beta is not allowed.");
    if (alpha == 0.) return 1.;
    return incompleteGamma(beta * x, alpha, lnGamma(alpha));
  }

  /** @} */

  /**
   * @name Other probability functions.
   *
   * Adapted from C routines for R programming langague
   *  Copyright (C) 1995, 1996  Robert Gentleman and Ross Ihaka
   *  Copyright (C) 1998    Ross Ihaka
   *  Copyright (C) 2000-2002 The R Development Core Team
   *  Copyright (C) 2003    The R Foundation

   * @{
   */

  /**
   * @brief Normal cumulative function.
   *
   * Returns Prob{x<=z} where x ~ N(0,1)
   *
   * @param z the value.
   * @return The corresponding probability.
   */
  static double pNorm(double z);

  /* @brief Normal cumulative function.
   *
   * Returns Prob{x<=z} where x ~ N(mu,sigma^2)

   * @param z the value.
   * @param mu The mean of the distribution
   * @param sigma The standard deviation of the distribution
   * @return The corresponding probability.
   */

  static double pNorm(double z, double mu, double sigma);

  /**
   * @brief Computes
   * \f$ln\left(Beta\left(\alpha,\beta\right)\right)\f$ given
   * \f$\alpha\f$ and \f$b\eta\f$.
   *
   * Returns ln(beta(alpha,beta)) for alpha>0 and beta>0.
   *
   * @param alpha, beta Alpha and Beta parameters.
   * @return \f$ln\left(Beta\left(\alpha,\beta\right)\right)\f$
   */
  static double lnBeta(double alpha, double beta);

  /**
   * @brief Returns the regularized incomplete beta function
   * @f$I_x(\alpha,\beta) = pbeta(x,\alpha,\beta@f$
   *
   * alpha and beta are the parameters of the function.
   *
   * Adapted From Cephes Math Library Release 2.8:  June, 2000
   * Copyright by Stephen L. Moshier
   * Under GPL License
   *
   * @param x the upper limit of the integration.
   * @param alpha, beta the shape parameters.
   */
  static double incompleteBeta(double x, double alpha, double beta);
  static double pBeta(double x, double alpha, double beta)
  {
    return incompleteBeta(x, alpha, beta);
  }

  /**
   * @brief The Beta quantile function.
   *
   * An adaptation from the C code of R
   *  Copyright (C) 1995, 1996  Robert Gentleman and Ross Ihaka
   *  Copyright (C) 1998--2007  The R Development Core Team
   *  based on code (C) 1979 and later Royal Statistical Society
   *
   * @param prob The probability.
   * @param alpha Alpha parameter.
   * @param beta  Beta parameter.
   * @return The quantile corresponding to prob.
   */
  static double qBeta(double prob, double alpha, double beta);

  /** @} */

private:
  static double DblGammaGreaterThanOne(double dblAlpha);
  static double DblGammaLessThanOne(double dblAlpha);
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_RANDOM_RANDOMTOOLS_H

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_ADAPTIVEKERNELDENSITYESTIMATION_H
#define BPP_NUMERIC_ADAPTIVEKERNELDENSITYESTIMATION_H


#include "Matrix/Matrix.h"

namespace bpp
{
/**
 * @brief Density estimation using the adaptive kernel method.
 *
 * For now this implementation is quite restricted, more options may be implemented later...
 *
 * The source for this method can be found is the appendix of the following paper:
 * Ivan Kojadinovic, _Computational Statistics and Data Analaysis_ (2004), 46:269-294
 *
 * @author Julien Dutheil
 */
class AdaptiveKernelDensityEstimation
{
private:
  RowMatrix<double> x_; // The original sample
  size_t n_;
  size_t r_;
  RowMatrix<double> covar_; // The covariance matrix, used for the linear transformation
  RowMatrix<double> invSqrtCovar_; // The inverse of the square root of the covariance matrix, used for the linear transformation
  std::vector<double> xMean_;
  double gamma_; // Tune the effect of the pilot density.
  double c1_;
  std::vector<double> c2_;
  double h_; // The bandwidth.
  std::vector<double> lambda_; // The local tuning coefficient of the bandwidth.
  std::vector<double> pilot_; // The pilot density

public:
  /**
   * @brief Build a new AdaptiveKernelDensityEstimation object.
   * @param x A mtrix contianing the sample point, one point per column.
   * The row of the matrix are the dimension of the sampled vectors, wich can be 1.
   * @param gamma Controls the influence of the pilot density. A value of 0
   * maximizes the impact of the pilot density, and hence corresponds to the standard
   * Kernel Density Estimation method. A value in ]0,1] allows a local adjustement of
   * the bandwith. The 0.5 value is commonly used.
   */
  AdaptiveKernelDensityEstimation(const Matrix<double>& x, double gamma = 0.5) :
    x_(x), n_(x.getNumberOfColumns()), r_(x.getNumberOfRows()),
    covar_(), invSqrtCovar_(), xMean_(), gamma_(gamma),
    c1_(0), c2_(x.getNumberOfColumns()), h_(0),
    lambda_(x.getNumberOfColumns()), pilot_(x.getNumberOfColumns())
  {
    init_();
  }
  virtual ~AdaptiveKernelDensityEstimation() {}

public:
  /**
   * @return The value of the estimated density for point x.
   * @param x The point where to estimate the density.
   */
  double kDensity(const std::vector<double>& x);

private:
  void init_();

  void sampleMean_(const Matrix<double>& x, std::vector<double>& mean);

  /**
   * @brief The kernel function.
   *
   * For now a standard normal density is used, further options may be added later,
   * including the possibility to use your own function.
   *
   * @param x The point for which to compute the density, as a matrix with 1 column and r_ rows.
   * @return The value of the kernel function at the corresponding point.
   */
  double kernel_(const Matrix<double>& x);
};
} // End of namespace bpp.
#endif // BPP_NUMERIC_ADAPTIVEKERNELDENSITYESTIMATION_H

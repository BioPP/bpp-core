// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "AdaptiveKernelDensityEstimation.h"
#include "Matrix/MatrixTools.h"
#include "NumConstants.h"

using namespace bpp;
using namespace std;

void AdaptiveKernelDensityEstimation::init_()
{
  // Compute the covariance matrix of the sample:
  MatrixTools::covar(x_, covar_);

  // Compute the mean vector
  sampleMean_(x_, xMean_);

  // Compute the inverse of the square root of the covariance matrix:
  MatrixTools::pow<double>(covar_, -0.5, invSqrtCovar_);

  // Compute the bandwidth:
  h_ = std::pow(4. / ((2 * static_cast<double>(r_) + 1.) * static_cast<double>(n_)), 1. / (static_cast<double>(r_) + 4.));
  // Compute as much as we can in advance to simplify the density calculation:
  c1_ = 1. / (std::sqrt(MatrixTools::det(covar_)) * static_cast<double>(n_) * std::pow(h_, static_cast<int>(r_)));

  // Now compute the local tuning of the bandwidth.
  // First estimate the pilot density:
  vector<double> xi(r_);
  LinearMatrix<double> diff_xi(r_, 1);
  LinearMatrix<double>  std_xi(r_, 1);
  for (unsigned int i = 0; i < n_; i++)
  {
    // Get the current xi point to evaluate:
    for (unsigned int k = 0; k < r_; k++)
    {
      xi[k] = x_(k, i);
    }

    // Sum loop, over all xi's:
    double sum = 0;
    for (unsigned int j = 0; j < n_; j++)
    {
      for (unsigned int k = 0; k < r_; k++)
      {
        diff_xi(k, 0) = xi[k] - x_(k, j);
      }
      MatrixTools::mult(invSqrtCovar_, diff_xi, std_xi);
      MatrixTools::scale(std_xi, 1. / h_);
      sum += kernel_(std_xi);
    }
    pilot_[i] = c1_ * sum;
  }

  // Compute the tuning parameters:
  double g = 0;
  for (unsigned int i = 0; i < n_; i++)
  {
    g += std::log(pilot_[i]);
  }
  g = std::exp(g / static_cast<double>(n_));
  for (unsigned int i = 0; i < n_; i++)
  {
    lambda_[i] = std::pow(g / pilot_[i], gamma_);
  }

  // Compute as much as we can in advance to simplify the density calculation:
  for (unsigned int i = 0; i < n_; i++)
  {
    c2_[i] = std::pow(lambda_[i], -static_cast<double>(r_));
  }
}

void AdaptiveKernelDensityEstimation::sampleMean_(const Matrix<double>& x, std::vector<double>& mean)
{
  size_t nc = x.getNumberOfColumns();
  size_t nr = x.getNumberOfRows();
  mean.resize(nr);
  for (size_t i = 0; i < nr; i++)
  {
    mean[i] = 0;
    for (size_t j = 0; j < nc; j++)
    {
      mean[i] += x(i, j);
    }
    mean[i] /= static_cast<double>(nc);
  }
}

double AdaptiveKernelDensityEstimation::kernel_(const Matrix<double>& x)
{
  // x is supposed to have only one column and r_ rows.
  // We compute the scalar product of the column with itself:
  double scalar = 0;
  for (size_t i = 0; i < r_; i++)
  {
    scalar += std::pow(x(i, 0), 2.);
  }

  return std::pow(2. * NumConstants::PI(), -static_cast<double>(r_) / 2.) * std::exp(-0.5 * scalar);
}

double AdaptiveKernelDensityEstimation::kDensity(const std::vector<double>& x)
{
  LinearMatrix<double> diff_xi(r_, 1);
  LinearMatrix<double> std_xi(r_, 1);
  // Sum loop, over all xi's:
  double sum = 0;
  for (unsigned int j = 0; j < n_; j++)
  {
    for (unsigned int k = 0; k < r_; k++)
    {
      diff_xi(k, 0) = x[k] - x_(k, j);
    }
    MatrixTools::mult(invSqrtCovar_, diff_xi, std_xi);
    MatrixTools::scale(std_xi, 1. / (h_ * lambda_[j]));
    sum += kernel_(std_xi) * c2_[j];
  }
  return c1_ * sum;
}

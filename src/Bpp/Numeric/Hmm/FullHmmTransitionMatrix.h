// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_FULLHMMTRANSITIONMATRIX_H
#define BPP_NUMERIC_HMM_FULLHMMTRANSITIONMATRIX_H


#include "../AbstractParameterAliasable.h"
#include "../Prob/Simplex.h"
#include "AbstractHmmTransitionMatrix.h"

namespace bpp
{
/**
 * @brief Describe the transition probabilities between hidden states of a Hidden Markov Model.
 *
 *
 * Transition probabilities are described through simplexes.
 *
 *
 * Parameters are denoted \c "I.Simplex.thetaJ" where \c I is the line number
 * of the transition matrix, and \c "thetaJ" is the matching parameter
 * in the Ith Simplex.
 *
 * @see Simplex
 */

class FullHmmTransitionMatrix :
  public virtual AbstractHmmTransitionMatrix,
  public AbstractParametrizable
{
private:
  std::vector<Simplex> vSimplex_;

public:
  FullHmmTransitionMatrix(std::shared_ptr<const HmmStateAlphabet> alph, const std::string& prefix = "");

  FullHmmTransitionMatrix(const FullHmmTransitionMatrix& hptm);

  FullHmmTransitionMatrix& operator=(const FullHmmTransitionMatrix& hptm);

  FullHmmTransitionMatrix* clone() const { return new FullHmmTransitionMatrix(*this);}

  /**
   * @brief Set the matrix of the transition probabilities.
   */
  void setTransitionProbabilities(const Matrix<double>& mat);

  /**
   * @brief Get the transition probability between two states.
   *
   * @param i initial state.
   * @param j final state.
   * @return the transition probability between the two states.
   */
  double Pij(size_t i, size_t j) const
  {
    return vSimplex_[i].prob(j);
  }

  /**
   * @brief Get all transition probabilities as a matrix.
   *
   * @return A n*n matrix will all transition probabilities (n being the number of hidden states).
   */

  const Matrix<double>& getPij() const;

  /**
   * @return The vector of equilibrium frequencies of the Markov chain described by the matrix.
   */
  const std::vector<double>& getEquilibriumFrequencies() const;


  /**
   * @brief From AbstractParametrizable interface
   */
  void fireParameterChanged(const ParameterList& parameters);
};
} // end of namespace bpp
#endif // BPP_NUMERIC_HMM_FULLHMMTRANSITIONMATRIX_H

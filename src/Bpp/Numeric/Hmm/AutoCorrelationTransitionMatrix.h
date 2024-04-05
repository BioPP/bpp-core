// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_AUTOCORRELATIONTRANSITIONMATRIX_H
#define BPP_NUMERIC_HMM_AUTOCORRELATIONTRANSITIONMATRIX_H


#include "../AbstractParameterAliasable.h"
#include "AbstractHmmTransitionMatrix.h"

namespace bpp
{
/**
 * @brief Describe the auto-correlation probabilities inside hidden
 *  states of a Hidden Markov Model.
 *
 *  This modelling behaves like a HMM in which, from a given state,
 * all transition probabilities to the other states are equal.
 *
 * The parameters are the within states transition probabilities,
 * denoted as \c "lambaN" with N the number of the state (1 is the
 * first).
 */
class AutoCorrelationTransitionMatrix :
  public virtual AbstractHmmTransitionMatrix,
  public AbstractParametrizable
{
private:
  std::vector<double> vAutocorrel_;

public:
  AutoCorrelationTransitionMatrix(std::shared_ptr<const HmmStateAlphabet> alph, const std::string& prefix = "");

  AutoCorrelationTransitionMatrix(const AutoCorrelationTransitionMatrix& hptm);

  AutoCorrelationTransitionMatrix& operator=(const AutoCorrelationTransitionMatrix& hptm);

  AutoCorrelationTransitionMatrix* clone() const override { return new AutoCorrelationTransitionMatrix(*this);}

  /**
   * @brief Get the transition probability between two states.
   *
   * @param i initial state.
   * @param j final state.
   * @return the transition probability between the two states.
   */
  double Pij(size_t i, size_t j) const override
  {
    return (i == j) ? vAutocorrel_[i] : (1 - vAutocorrel_[i]) / static_cast<double>(getNumberOfStates() - 1);
  }

  /**
   * @brief Get all transition probabilities as a matrix.
   *
   * @return A n*n matrix will all transition probabilities (n being the number of hidden states).
   */
  const Matrix<double>& getPij() const override;

  /**
   * @return The vector of equilibrium frequencies of the Markov chain described by the matrix.
   */
  const std::vector<double>& getEquilibriumFrequencies() const override;


  /*
   * @brief From AbstractParametrizable interface
   *
   */

  void fireParameterChanged(const ParameterList& parameters) override;
};
} // end of namespace bpp
#endif // BPP_NUMERIC_HMM_AUTOCORRELATIONTRANSITIONMATRIX_H

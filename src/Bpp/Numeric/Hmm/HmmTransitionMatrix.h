// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_HMMTRANSITIONMATRIX_H
#define BPP_NUMERIC_HMM_HMMTRANSITIONMATRIX_H


#include "../Matrix/Matrix.h"
#include "../Parametrizable.h"
#include "HmmExceptions.h"
#include "HmmStateAlphabet.h"

// Fron the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Describe the transition probabilities between hidden states of a Hidden Markov Model.
 *
 * This class is part of the HMM framework.
 */
class HmmTransitionMatrix :
  public virtual Parametrizable
{
public:
  virtual HmmTransitionMatrix* clone() const override = 0;

  /**
   * @return The hidden alphabet associated to this model.
   */
  virtual const HmmStateAlphabet& hmmStateAlphabet() const = 0;

  /**
   * @return A shared pointer toward the hidden alphabet associated to this model.
   */
  virtual std::shared_ptr<const HmmStateAlphabet> getHmmStateAlphabet() const = 0;

  /**
   * @brief Set the new hidden state alphabet.
   * @param stateAlphabet The new state alphabet.
   * @throw UnvalidStateAlphabetException if the new alphabet is uncorrect (for instance is NULL pointer).
   */
  virtual void setHmmStateAlphabet(std::shared_ptr<const HmmStateAlphabet> stateAlphabet) = 0;

  /**
   * @return The number of states in the model.
   */
  virtual size_t getNumberOfStates() const = 0;

  /**
   * @brief Get the transition probability between two states.
   *
   * @param i initial state.
   * @param j final state.
   * @return the transition probability between the two states.
   */
  virtual double Pij(size_t i, size_t j) const = 0;

  /**
   * @brief Get all transition probabilities as a matrix.
   *
   * @return A n*n matrix will all transition probabilities (n being the number of hidden states).
   */
  virtual const Matrix<double>& getPij() const = 0;

  /**
   * @return The vector of equilibrium frequencies of the Markov chain described by the matrix.
   */
  virtual const std::vector<double>& getEquilibriumFrequencies() const = 0;
};
} // end of namespace bpp
#endif // BPP_NUMERIC_HMM_HMMTRANSITIONMATRIX_H

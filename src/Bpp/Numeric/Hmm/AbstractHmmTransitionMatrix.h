// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_ABSTRACTHMMTRANSITIONMATRIX_H
#define BPP_NUMERIC_HMM_ABSTRACTHMMTRANSITIONMATRIX_H


#include "../VectorTools.h"
#include "HmmStateAlphabet.h"
#include "HmmTransitionMatrix.h"

namespace bpp
{
/**
 * @brief Partial implementation of HmmTransitionMatrix.
 */

class AbstractHmmTransitionMatrix :
  public virtual HmmTransitionMatrix
{
private:
  std::shared_ptr<const HmmStateAlphabet> alph_;

protected:
  mutable RowMatrix<double> pij_, tmpmat_;

  mutable Vdouble eqFreq_;

  mutable bool upToDate_;

public:
  AbstractHmmTransitionMatrix(std::shared_ptr<const HmmStateAlphabet> alph, const std::string& prefix = "");

  AbstractHmmTransitionMatrix(const AbstractHmmTransitionMatrix& hptm);

  AbstractHmmTransitionMatrix& operator=(const AbstractHmmTransitionMatrix& hptm);

  /**
   * @return A pointer toward the hidden alphabet associated to this model.
   */
  std::shared_ptr<const HmmStateAlphabet> getHmmStateAlphabet() const
  {
    return alph_;
  }

  /**
   * @return The hidden alphabet associated to this model.
   */
  const HmmStateAlphabet& hmmStateAlphabet() const
  {
    return *alph_;
  }

  /**
   * @brief Set the new hidden state alphabet
   * @param stateAlphabet The new state alphabet
   * @throw UnvalidStateAlphabetException if the new alphabet is uncorrect (for instance is NULL pointer).
   */
  void setHmmStateAlphabet(std::shared_ptr<const HmmStateAlphabet> stateAlphabet);

  /**
   * @return The number of states in the model.
   */
  size_t getNumberOfStates() const
  {
    return alph_->getNumberOfStates();
  }

  /**
   * @brief sampling of a sequence of states. Starting point is
   * sampled from the equilibrium distribution.
   *
   * @param size the length of the sequence
   * @return a vector of states index sampled from the Transition
   * probabilities
   */
  std::vector<size_t> sample(size_t size) const;
};
} // end of namespace bpp
#endif // BPP_NUMERIC_HMM_ABSTRACTHMMTRANSITIONMATRIX_H

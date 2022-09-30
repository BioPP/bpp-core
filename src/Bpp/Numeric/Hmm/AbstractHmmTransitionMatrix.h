//
// File: AbstractHmmTransitionMatrix.h
// Authors:
//   Laurent Guéguen
// Created: lundi 10 février 2014, à 10h 55
//

/*
  Copyright or © or Copr. Bio++Development Team, (November 16, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for phylogenetic data analysis.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

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

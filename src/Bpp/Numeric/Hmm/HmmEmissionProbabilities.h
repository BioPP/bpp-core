// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_HMMEMISSIONPROBABILITIES_H
#define BPP_NUMERIC_HMM_HMMEMISSIONPROBABILITIES_H


#include "../Parametrizable.h"
#include "HmmExceptions.h"
#include "HmmStateAlphabet.h"

// From the STL:
#include <vector>

namespace bpp
{
/**
 * @brief Interface for computing emission probabilities in a Hidden Markov Model.
 *
 * This class is part of the HMM framework. It compute the probabilities of the data
 * conditionned on each hidden state. The emission probabilities class also has in charge
 * the data, its putative compression, and the number of position in the sequence of
 * observed states.
 *
 * @see HmmStateAlphabet
 * @see HmmTransitionMatrix
 */
class HmmEmissionProbabilities :
  public virtual Parametrizable
{
public:
  virtual HmmEmissionProbabilities* clone() const override = 0;

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
   * @brief Operator access to the emission probabilities.
   *
   * This is the fastest way to get the values, but no checking is performed on the indices.
   * For debugging purpose, the getEmissionProbability would be a safer use.
   *
   * @param pos The position of the sequential data to consider.
   * @param state The index of the hidden state to consider, as defined by the HmmStateAlphabet object associated to this class.
   */
  virtual double operator()(size_t pos, size_t state) const = 0;

  virtual void computeDEmissionProbabilities(std::string& variable) const
  {
    throw (NotImplementedException("HmmEmissionProbabilities::computeDEmissionProbabilities is not overdefined."));
  }

  virtual void computeD2EmissionProbabilities(std::string& variable) const
  {
    throw (NotImplementedException("HmmEmissionProbabilities::computeD2EmissionProbabilities is not overdefined."));
  }

  virtual const std::vector<double>& getDEmissionProbabilities(size_t pos) const
  {
    throw (NotImplementedException("HmmEmissionProbabilities::getDEmissionProbability is not overdefined."));
  }

  virtual const std::vector<double>& getD2EmissionProbabilities(size_t pos) const
  {
    throw (NotImplementedException("HmmEmissionProbabilities::getD2EmissionProbability is not overdefined."));
  }

  /**
   * @brief Operator access to the emission probabilities.
   *
   * This is the fastest way to get the values, but no checking is performed on the indices.
   * For debugging purpose, the getEmissionProbability would be a safer use.
   *
   * @param pos The position of the sequential data to consider.
   * @return A vector of probabilities, whose size is the number of hidden states.
   */
  virtual const std::vector<double>& operator()(size_t pos) const = 0;

  /**
   * @return The number of positions in the data.
   */
  virtual size_t getNumberOfPositions() const = 0;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_HMM_HMMEMISSIONPROBABILITIES_H

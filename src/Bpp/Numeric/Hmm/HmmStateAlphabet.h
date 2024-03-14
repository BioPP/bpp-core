// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_HMMSTATEALPHABET_H
#define BPP_NUMERIC_HMM_HMMSTATEALPHABET_H


#include "../../Exceptions.h"
#include "../Parametrizable.h"
#include "HmmExceptions.h"

// From the STL:
#include <vector>

// using namespace bpp;

namespace bpp
{
class StateListener;
class StateChangedEvent;

/**
 * @brief Hidden states alphabet.
 *
 * Implementations of this interface describe the set of hidden states of a Hidden Markov Model.
 */
class HmmStateAlphabet :
  public virtual Parametrizable
{
public:
  HmmStateAlphabet() {}
  virtual ~HmmStateAlphabet() {}

  virtual HmmStateAlphabet* clone() const override = 0;
  
public:
  /**
   * @param stateIndex The index of a hidden state.
   * @return The corresponding hidden state.
   * @see getNumberOfStates
   */
  virtual const Clonable& getState(size_t stateIndex) const = 0;


  virtual size_t getNumberOfStates() const = 0;

  /**
   * @brief Tell if this instance can work with the instance of alphabet given as input.
   *
   * In many case, this will return true is the pointer provided as argument refers to this object.
   *
   * @param stateAlphabet The alphabet to check.
   * @return true if the matrix is compatible with the given alphabet.
   */
  virtual bool worksWith(const HmmStateAlphabet& stateAlphabet) const = 0;
};

class StateListener
{
public:
  StateListener() {}
  virtual ~StateListener() {}

public:
  virtual void stateChanged(StateChangedEvent& event) = 0;
};

class StateChangedEvent
{
protected:
  std::vector<unsigned int> states_;

public:
  StateChangedEvent(unsigned int stateIndex) : states_(1)
  {
    states_[0] = stateIndex;
  }
  StateChangedEvent(std::vector<unsigned int>& states) : states_(states) {}

public:
  const std::vector<unsigned int>& getStates() const { return states_; }
  std::vector<unsigned int>& getStates() { return states_; }
};
}
#endif // BPP_NUMERIC_HMM_HMMSTATEALPHABET_H

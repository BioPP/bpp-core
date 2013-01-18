//
// File: HmmEmissionProbabilities.h
// Created by: Julien Dutheil
// Created on: Fri Sep 04 13:14 2009 from file HmmLikelihood.
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 16, 2004)

This software is a computer program whose purpose is to provide classes
for phylogenetic data analysis.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef _HMMEMISSIONPROBABILITIES_H_
#define _HMMEMISSIONPROBABILITIES_H_

#include "HmmStateAlphabet.h"
#include "HmmExceptions.h"
#include "../Parametrizable.h"

//From the STL:
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
class HmmEmissionProbabilities:
  public virtual Parametrizable
{
  public:
    virtual HmmEmissionProbabilities* clone() const = 0;

    virtual const HmmStateAlphabet* getHmmStateAlphabet() const = 0;
 
    /**
     * @brief Set the new hidden state alphabet.
     * @param stateAlphabet The new state alphabet.
     * @throw UnvalidStateAlphabetException if the new alphabet is uncorrect (for instance is NULL pointer).
     */
    virtual void setHmmStateAlphabet(const HmmStateAlphabet* stateAlphabet) throw (HmmUnvalidAlphabetException) = 0;

    /**
     * @name Access emission probabilities per site.
     *
     * This functions are handy, but might not be the most efficient way to retrieve the probabilities.
     * @{
     */
   
    /**
     * @param pos The position of the sequential data to consider.
     * @param stateIndex The index of the hidden state to consider, as defined by the HmmStateAlphabet object associated to this class.
     */
    virtual double getEmissionProbability(size_t pos, size_t stateIndex) const throw (Exception) = 0;
    virtual void getEmissionProbabilities(size_t pos, std::vector<double>& probs) const throw (Exception) = 0;
    virtual void getEmissionProbabilitiesForEachPosition(std::vector< std::vector<double> >& probs, bool append) const throw (Exception) = 0;
    
    /**
     * @param pos The position of the sequential data to consider.
     * @param stateIndex The index of the hidden state to consider, as defined by the HmmStateAlphabet object associated to this class.
     */
    virtual double getLogEmissionProbability(size_t pos, size_t stateIndex) const throw (Exception) = 0;
    virtual void getLogEmissionProbabilities(size_t pos, std::vector<double>& probs) const throw (Exception) = 0;
    virtual void getLogEmissionProbabilitiesForEachPosition(std::vector< std::vector<double> >& probs, bool append) const throw (Exception) = 0;

    /**@} */

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

    /**
     * @brief Get the index of a given state in the list of observed states.
     * The indexing if up to the implementation of this interface.
     * 
     * @param pos The position of the observed state in the original sequence.
     * @return The index of the observed state in the list.
     */
    virtual size_t getObservedStateIndex(size_t pos) const = 0;
    
    /**
     * @brief Get all emission probabilities, in a compressed form.
     *
     * @return a 2-dimensions array E[i,j], where i is the index of the observed states (@see getObservedStateIndex), and j in the index of the hidden state.
     * @param probs a 2-dimensional array which is going to be resized and filled with the emission probabilities.
     */
    virtual void getEmissionProbabilities(std::vector< std::vector<double> >& probs) const = 0;


};

} //end of namespace bpp.

#endif //_HMMEMISSIONPROBABILITIES_H_


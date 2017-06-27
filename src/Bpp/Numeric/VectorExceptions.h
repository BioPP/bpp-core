//
// File: VectorExceptions.h
// Authors:
//   Julien Dutheil
//   Francois Gindraud (2017)
// Created: 2003-06-26 00:00:00
// Last modified: 2017-06-27
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for numerical calculus.
  
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

#ifndef BPP_NUMERIC_VECTOREXCEPTIONS_H
#define BPP_NUMERIC_VECTOREXCEPTIONS_H

#include <string>
#include <vector>

#include "../Exceptions.h"

namespace bpp
{
  /// General Exception dealing with vectors.
  template <class T>
  class VectorException : public Exception
  {
  protected:
    const std::vector<T>* vect_;

  public:
    VectorException(const std::string& text, const std::vector<T>* vect = nullptr)
      : Exception("VectorException: " + text)
      , vect_(vect){};
    VectorException(const VectorException&) = default;
    VectorException& operator=(const VectorException&) = default;

    const std::vector<T>* getVector() const { return vect_; }
  };

  /// Exception thrown when an empty vector was found.
  template <class T>
  class EmptyVectorException : public VectorException<T>
  {
  public:
    EmptyVectorException(const std::string& text, const std::vector<T>* vect = nullptr)
      : VectorException<T>("EmptyVectorException: " + text, vect){};
  };

  /// Exception thrown when a dimension problem occured.
  class DimensionException : public Exception
  {
  private:
    std::size_t dimension_;
    std::size_t correctDimension_;

  public:
    DimensionException(const std::string& text, size_t dimension, size_t correctDimension)
      : Exception("DimensionException (found " + std::to_string(dimension) + ", should be " +
                  std::to_string(correctDimension) + ") " + text)
      , dimension_(dimension)
      , correctDimension_(correctDimension){};

    size_t getDimension() const { return dimension_; }
    size_t getCorrectDimension() const { return correctDimension_; }
  };

  /// Exception thrown when a given element was not found in the vector.
  template <class T>
  class ElementNotFoundException : public VectorException<T>
  {
  private:
    const T* element_;

  public:
    ElementNotFoundException(const std::string& text, const std::vector<T>* vect = nullptr, const T* element = nullptr)
      : VectorException<T>("ElementNotFoundException: " + text, vect)
      , element_(element)
    {
    }
    ElementNotFoundException(const ElementNotFoundException&) = default;
    ElementNotFoundException& operator=(const ElementNotFoundException&) = default;

    const T* getElement() const { return element_; }
  };

} // end of namespace bpp.
#endif // BPP_NUMERIC_VECTOREXCEPTIONS_H

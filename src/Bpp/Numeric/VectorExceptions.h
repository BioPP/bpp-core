// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_VECTOREXCEPTIONS_H
#define BPP_NUMERIC_VECTOREXCEPTIONS_H

#include <string>
#include <vector>

#include "../Exceptions.h"

namespace bpp
{
/// General Exception dealing with vectors.
template<class T>
class VectorException : public Exception
{
protected:
  const std::vector<T>* vect_;

public:
  VectorException(const std::string& text, const std::vector<T>* vect = nullptr)
    : Exception("VectorException: " + text)
    , vect_(vect){}
  VectorException(const VectorException&) = default;
  VectorException& operator=(const VectorException&) = default;

  const std::vector<T>* getVector() const { return vect_; }
};

/// Exception thrown when an empty vector was found.
template<class T>
class EmptyVectorException : public VectorException<T>
{
public:
  EmptyVectorException(const std::string& text, const std::vector<T>* vect = nullptr)
    : VectorException<T>("EmptyVectorException: " + text, vect){}
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
    , correctDimension_(correctDimension){}

  size_t getDimension() const { return dimension_; }
  size_t getCorrectDimension() const { return correctDimension_; }
};

/// Exception thrown when a given element was not found in the vector.
template<class T>
class ElementNotFoundException : public VectorException<T>
{
private:
  const T* element_;

public:
  ElementNotFoundException(const std::string& text, const std::vector<T>* vect = nullptr, const T* element = nullptr)
    : VectorException<T>("ElementNotFoundException: " + text, vect)
    , element_(element)
  {}
  ElementNotFoundException(const ElementNotFoundException&) = default;
  ElementNotFoundException& operator=(const ElementNotFoundException&) = default;

  const T* getElement() const { return element_; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_VECTOREXCEPTIONS_H

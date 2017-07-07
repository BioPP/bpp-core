//
// File: Memory.h
// Authors:
//   Francois Gindraud (2017)
// Created: 2017-07-07
// Last modified: 2017-07-07
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

#ifndef BPP_UTILS_MEMORY_H
#define BPP_UTILS_MEMORY_H

#include "../Clonable.h"

#include <memory> // This file is guaranteed to include <memory>
#include <type_traits>

namespace bpp
{
  /// Missing std::make_unique() in C++11
  template<typename T, typename... Args>
  std::unique_ptr<T> makeUnique(Args&&... args)
  {
    return std::unique_ptr<T>{new T(std::forward<Args>(args)...)};
  }

  /** unique_ptr that calls clone from Clonable on copy.
   * This is a std::unique_ptr (same API, inherits from std::unique_ptr).
   * The constructor API is not exactly equivalent to unique_ptr, which requires ugly TMP with SFINAE.
   * TODO deprecate use of Clone and only use this as upgrade for legacy code ?
   */
  template<typename T, typename Deleter = std::default_delete<T>>
  class CopyUniquePtr : public std::unique_ptr<T, Deleter>
  {
  private:
    static_assert(std::is_base_of<Clonable, T>::value, "CopyUniquePtr requires type to derive from bpp::Clonable");
    using UP = std::unique_ptr<T, Deleter>;

  public:
    // Defaults
    constexpr CopyUniquePtr() = default;
    CopyUniquePtr(CopyUniquePtr&&) = default;
    CopyUniquePtr& operator=(CopyUniquePtr&&) = default;
    ~CopyUniquePtr() = default;

    // Generate copy constructor and assignement using clone
    CopyUniquePtr(const CopyUniquePtr& other)
      : UP(other ? other->clone() : nullptr, other.get_deleter())
    {
    }
    CopyUniquePtr& operator=(const CopyUniquePtr& other)
    {
      UP::operator=(UP(other ? other->clone() : nullptr, other.get_deleter()));
      return *this;
    }

    // Forward other constructors
    constexpr CopyUniquePtr(std::nullptr_t) noexcept
      : CopyUniquePtr()
    {
    }
    explicit CopyUniquePtr(typename UP::pointer p) noexcept
      : UP(p)
    {
    }
    CopyUniquePtr(typename UP::pointer p, const Deleter& d) noexcept(std::is_nothrow_copy_constructible<Deleter>::value)
      : UP(p, d)
    {
    }
    CopyUniquePtr(typename UP::pointer p, Deleter&& d) noexcept(std::is_nothrow_move_constructible<Deleter>::value)
      : UP(p, std::move(d))
    {
    }
    template<typename U, typename E>
    CopyUniquePtr(std::unique_ptr<U, E>&& other) noexcept
      : UP(std::move(other))
    {
    }

    // Forward other assignement
    CopyUniquePtr& operator=(std::nullptr_t) noexcept
    {
      UP::operator=(nullptr);
      return *this;
    }
    template<typename U, typename E>
    CopyUniquePtr& operator=(std::unique_ptr<U, E>&& other) noexcept
    {
      UP::operator=(std::move(other));
      return *this;
    }

    constexpr operator bool() const noexcept { return UP::operator bool(); }
  };

  /** Conditional deleter for std::unique_ptr.
   * Uses std::default_delete internally, to use its specialisation for arrays.
   */
  template<typename T>
  struct ConditionalDeleter : private std::default_delete<T>
  {
    /// Determines if the pointer is owned and should be deleted on destruction (mutable).
    bool ownsPointer;

    /// Create a deleter (defaults to owning the pointer).
    constexpr ConditionalDeleter(bool ownsPointer_ = true) noexcept
      : std::default_delete<T>()
      , ownsPointer(ownsPointer_)
    {
    }

    void operator()(T* ptr) const
    {
      if (ownsPointer)
        std::default_delete<T>::operator()(ptr);
    }
  };
}

#endif // BPP_UTILS_MEMORY_H

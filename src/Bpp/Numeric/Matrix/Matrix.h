// File: Matrix.h
// Authors:
//   Julien Dutheil
//   Sylvain Gaillard
//   Francois Gindraud (2017)
// Created on: Tue Apr 07 11:58 2004

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

This software is a computer program whose purpose is to provide classes
for numerical calculus.

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

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <Bpp/Clonable.h>
#include <Bpp/Numeric/NumConstants.h>
#include <Bpp/Numeric/VectorExceptions.h>
#include <Bpp/Utils/ForRange.h>
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

namespace bpp
{
  /** The matrix template interface.
   * Matrix implementation must provide the memory storage for the data.
   * Currently most matrix classes are final (non inheritable).
   * It allows the compiler to devirtualize calls like operator(i, j).
   * This means some of these calls will become static function calls (faster than virtual).
   */
  template <class Scalar>
  class Matrix : public Clonable
  {
  public:
    /// In subclasses, default constructors should fill the matrix with default constructed values (T()).
    Matrix() = default;
    virtual ~Matrix() = default;

    /** @return \f$m_{i,j}\f$.
     * @param i row index.
     * @param j column index.
     */
    virtual const Scalar& operator()(std::size_t i, std::size_t j) const = 0;

    /** @return \f$m_{i,j}\f$.
     * @param i row index.
     * @param j column index.
     */
    virtual Scalar& operator()(std::size_t i, std::size_t j) = 0;

    /// Floating point comparison with threshold.
    bool equals(const Matrix& m, double threshold = NumConstants::TINY())
    {
      if (m.getNumberOfRows() != getNumberOfRows() || m.getNumberOfColumns() != getNumberOfColumns())
        return false;
      for (auto i : makeRange(getNumberOfRows()))
        for (auto j : makeRange(getNumberOfColumns()))
          if (std::abs(static_cast<double>(operator()(i, j)) - static_cast<double>(m(i, j))) > threshold)
            return false;
      return true;
    }

    /// @return The number of rows.
    virtual std::size_t getNumberOfRows() const = 0;
    /// @return The number of columns.
    virtual std::size_t getNumberOfColumns() const = 0;

    /** @return the row at position i as a vector.
     * @param i The index of the row.
     * Can be reimplemented in a faster way in subclasses.
     */
    virtual std::vector<Scalar> row(std::size_t i) const
    {
      std::vector<Scalar> v(getNumberOfColumns());
      for (auto j : makeRange(v.size()))
        v[j] = operator()(i, j);
      return v;
    }
    /** @return the column at position j as a vector.
     * @param j The index of the column.
     * Can be reimplemented in a faster way in subclasses.
     */
    virtual std::vector<Scalar> col(std::size_t j) const
    {
      std::vector<Scalar> v(getNumberOfRows());
      for (auto i : makeRange(v.size()))
        v[i] = operator()(i, j);
      return v;
    }

    /** @brief Resize the matrix.
     * Keeps old values in cells common to new and old matrices.
     * New cells are default constructed (T()).
     * @param nRows The new number of rows.
     * @param nCols The new number of columns.
     */
    virtual void resize(std::size_t nRows, std::size_t nCols) = 0;
  };

  /** @brief Matrix storage by row.
   * This matrix is a vector of vector of Scalar.
   * Cell access in constant time but through two pointer indirections.
   * Does nbRows + 1 memory allocations.
   * Can easily access, modify, append data row by row, but not by column.
   * Prefer LinearRowMatrix if the matrix does not require row-by-row specific operations.
   */
  template <class Scalar>
  class RowMatrix final : public Matrix<Scalar>
  {
  private:
    std::vector<std::vector<Scalar>> data_{};

  public:
    /// Creates an empty Matrix.
    RowMatrix() = default;

    /// Creates a non initialized nbRows x nbCols matrix.
    RowMatrix(std::size_t nbRows, std::size_t nbCols)
      : data_(nbRows, std::vector<Scalar>(nbCols))
    {
    }

    // Default, as std::vector already does the right job
    RowMatrix(const RowMatrix&) = default;
    RowMatrix& operator=(const RowMatrix&) = default;
    RowMatrix(RowMatrix&&) = default;
    RowMatrix& operator=(RowMatrix&&) = default;
    ~RowMatrix() = default;

    // Generic Matrix<T> operators. Use the RowMatrix one if possible.
    RowMatrix(const Matrix<Scalar>& m)
    {
      *this = m; // Use operator=(const Matrix&)
    }
    RowMatrix(Matrix<Scalar>&& m)
    {
      *this = std::move(m); // Use operator=(Matrix&&)
    }
    RowMatrix& operator=(const Matrix<Scalar>& m)
    {
      auto rowMatrix = dynamic_cast<const RowMatrix*>(&m);
      if (rowMatrix)
        *this = *rowMatrix; // Use operator=(const RowMatrix &)
      else
        copyFromMatrix(m);
      return *this;
    }
    RowMatrix& operator=(Matrix<Scalar>&& m)
    {
      auto rowMatrix = dynamic_cast<RowMatrix*>(&m);
      if (rowMatrix)
        *this = std::move(*rowMatrix); // Use operator=(RowMatrix&&)
      else
        copyFromMatrix(m);
      return *this;
    }

    RowMatrix* clone() const override { return new RowMatrix(*this); }

    const Scalar& operator()(std::size_t r, std::size_t c) const override { return data_[r][c]; }
    Scalar& operator()(std::size_t r, std::size_t c) override { return data_[r][c]; }

    std::size_t getNumberOfRows() const override { return data_.size(); }
    std::size_t getNumberOfColumns() const override { return data_.empty() ? 0 : data_.front().size(); }

    std::vector<Scalar> row(std::size_t r) const override { return data_[r]; }

    const std::vector<Scalar>& getRow(std::size_t r) const { return data_[r]; }
    std::vector<Scalar>& getRow(std::size_t r) { return data_[r]; }

    void resize(std::size_t nbRows, std::size_t nbCols) override
    {
      data_.resize(nbRows);
      for (auto r : makeRange(nbRows))
        data_[r].resize(nbCols);
    }

    void addRow(const std::vector<Scalar>& newRow)
    {
      if (getNumberOfColumns() != 0 && newRow.size() != getNumberOfColumns())
        throw DimensionException("RowMatrix::addRow: invalid row dimension", newRow.size(), getNumberOfColumns());
      data_.emplace_back(newRow);
    }
    void addRow(std::vector<Scalar>&& newRow)
    {
      if (getNumberOfColumns() != 0 && newRow.size() != getNumberOfColumns())
        throw DimensionException("RowMatrix::addRow: invalid row dimension", newRow.size(), getNumberOfColumns());
      data_.emplace_back(std::move(newRow));
    }

  private:
    void copyFromMatrix(const Matrix<Scalar>& m)
    {
      auto nbRows = m.getNumberOfRows();
      data_.resize(nbRows);
      for (auto r : makeRange(nbRows))
        data_[r] = m.row(r); // Use move assignement of std::vector, no redundant copy.
    }
  };

  /** @brief Matrix storage by column.
   * This matrix is a vector of vector of Scalar.
   * Cell access in constant time but through two pointer indirections.
   * Does nbCols + 1 memory allocations.
   * Can easily access, modify, append data column by column, but not by row.
   * Prefer LinearRowMatrix if the matrix does not require column-by-column specific operations.
   */
  template <class Scalar>
  class ColMatrix final : public Matrix<Scalar>
  {
  private:
    std::vector<std::vector<Scalar>> data_{};

  public:
    /// Creates an empty Matrix.
    ColMatrix() = default;

    /// Creates a non initialized nbRows x nbCols matrix.
    ColMatrix(std::size_t nbRows, std::size_t nbCols)
      : data_(nbCols, std::vector<Scalar>(nbRows))
    {
    }

    // Default, as std::vector already does the right job
    ColMatrix(const ColMatrix&) = default;
    ColMatrix& operator=(const ColMatrix&) = default;
    ColMatrix(ColMatrix&&) = default;
    ColMatrix& operator=(ColMatrix&&) = default;
    ~ColMatrix() = default;

    // Generic Matrix<T> operators. Use the ColMatrix one if possible.
    ColMatrix(const Matrix<Scalar>& m)
    {
      *this = m; // Use operator=(const Matrix&)
    }
    ColMatrix(Matrix<Scalar>&& m)
    {
      *this = std::move(m); // Use operator=(Matrix&&)
    }
    ColMatrix& operator=(const Matrix<Scalar>& m)
    {
      auto colMatrix = dynamic_cast<const ColMatrix*>(&m);
      if (colMatrix)
        *this = *colMatrix; // Use operator=(const ColMatrix &)
      else
        copyFromMatrix(m);
      return *this;
    }
    ColMatrix& operator=(Matrix<Scalar>&& m)
    {
      auto colMatrix = dynamic_cast<ColMatrix*>(&m);
      if (colMatrix)
        *this = std::move(*colMatrix); // Use operator=(ColMatrix&&)
      else
        copyFromMatrix(m);
      return *this;
    }

    ColMatrix* clone() const override { return new ColMatrix(*this); }

    const Scalar& operator()(std::size_t r, std::size_t c) const override { return data_[c][r]; }
    Scalar& operator()(std::size_t r, std::size_t c) override { return data_[c][r]; }

    std::size_t getNumberOfRows() const override { return data_.empty() ? 0 : data_.front().size(); }
    std::size_t getNumberOfColumns() const override { return data_.size(); }

    std::vector<Scalar> col(std::size_t c) const override { return data_[c]; }

    const std::vector<Scalar>& getCol(std::size_t c) const { return data_[c]; }
    std::vector<Scalar>& getCol(std::size_t c) { return data_[c]; }

    void resize(std::size_t nbRows, std::size_t nbCols) override
    {
      data_.resize(nbCols);
      for (auto i : makeRange(nbCols))
        data_[i].resize(nbRows);
    }

    void addCol(const std::vector<Scalar>& newCol)
    {
      if (getNumberOfColumns() != 0 && newCol.size() != getNumberOfColumns())
        throw DimensionException("ColMatrix::addCol: invalid row dimension", newCol.size(), getNumberOfColumns());
      data_.emplace_back(newCol);
    }
    void addCol(std::vector<Scalar>&& newCol)
    {
      if (getNumberOfColumns() != 0 && newCol.size() != getNumberOfColumns())
        throw DimensionException("ColMatrix::addCol: invalid row dimension", newCol.size(), getNumberOfColumns());
      data_.emplace_back(std::move(newCol));
    }

  private:
    void copyFromMatrix(const Matrix<Scalar>& m)
    {
      auto nbCols = m.getNumberOfColumns();
      data_.resize(nbCols);
      for (auto c : makeRange(nbCols))
        data_[c] = m.col(c); // Use move assignement of std::vector, no redundant copy.
    }
  };

  /** @brief Linearized matrix (by row).
   * A vector a scalar, where cells are placed row-by-row.
   * Cell access function is vec[rowIndex * nbCols + colIndex].
   * Cell access is constant time, with only one indirection.
   * Does only one memory allocation.
   */
  template <typename Scalar>
  class LinearizedRowMatrix final : public Matrix<Scalar>
  {
  private:
    std::size_t nbRows_{};
    std::size_t nbCols_{};
    std::vector<Scalar> data_;

  public:
    /// Creates an empty matrix.
    LinearizedRowMatrix() = default;

    /// Creates a non initialized nbRows x nbCols matrix.
    LinearizedRowMatrix(std::size_t nbRows, std::size_t nbCols)
      : nbRows_(nbRows)
      , nbCols_(nbCols)
      , data_(size())
    {
    }

    // Copy ops default version works
    LinearizedRowMatrix(const LinearizedRowMatrix&) = default;
    LinearizedRowMatrix& operator=(const LinearizedRowMatrix&) = default;

    // Move ops need cleaning of size values.
    LinearizedRowMatrix(LinearizedRowMatrix&& m)
      : nbRows_(m.nbRows_)
      , nbCols_(m.nbCols_)
      , data_(std::move(m.data_))
    {
      m.nbRows_ = m.nbCols_ = 0;
    }
    LinearizedRowMatrix& operator=(LinearizedRowMatrix&& m)
    {
      nbRows_ = m.nbRows_;
      nbCols_ = m.nbCols_;
      data_ = std::move(m.data_);
      m.nbRows_ = m.nbCols_ = 0;
      return *this;
    }

    // Generic Matrix<T> operators. Use the LinearizedRowMatrix one if possible.
    LinearizedRowMatrix(const Matrix<Scalar>& m)
    {
      *this = m; // Use operator=(const Matrix&)
    }
    LinearizedRowMatrix(Matrix<Scalar>&& m)
    {
      *this = std::move(m); // Use operator=(Matrix&&)
    }
    LinearizedRowMatrix& operator=(const Matrix<Scalar>& m)
    {
      auto lrMatrix = dynamic_cast<const LinearizedRowMatrix*>(&m);
      if (lrMatrix)
        *this = *lrMatrix; // Use operator=(const LinearizedRowMatrix &)
      else
        copyFromMatrix(m);
      return *this;
    }
    LinearizedRowMatrix& operator=(Matrix<Scalar>&& m)
    {
      auto lrMatrix = dynamic_cast<LinearizedRowMatrix*>(&m);
      if (lrMatrix)
        *this = std::move(*lrMatrix); // Use operator=(LinearizedRowMatrix&&)
      else
        copyFromMatrix(m);
      return *this;
    }

    LinearizedRowMatrix* clone() const override { return new LinearizedRowMatrix(*this); }

    const Scalar& operator()(std::size_t r, std::size_t c) const override { return data_[index(r, c)]; }
    Scalar& operator()(std::size_t r, std::size_t c) override { return data_[index(r, c)]; }

    std::size_t getNumberOfRows() const override { return nbRows_; }
    std::size_t getNumberOfColumns() const override { return nbCols_; }

    std::vector<Scalar> row(std::size_t r) const override
    {
      return {data_.begin() + index(r, 0), data_.begin() + index(r + 1, 0)};
    }

    void resize(std::size_t nbRows, std::size_t nbCols) override
    {
      // Create and fill new matrix with values from old one
      LinearizedRowMatrix newMatrix(nbRows, nbCols);
      for (auto r : makeRange(std::min(nbRows, nbRows_)))
        for (auto c : makeRange(std::min(nbCols, nbCols_)))
          newMatrix(r, c) = operator()(r, c);
      *this = std::move(newMatrix); // move in place
    }

  private:
    std::size_t size() const { return nbRows_ * nbCols_; }
    std::size_t index(std::size_t i, std::size_t j) const { return i * nbCols_ + j; }

    void copyFromMatrix(const Matrix<Scalar>& m)
    {
      // Just copy cell by cell, as we have no inside information.
      nbRows_ = m.getNumberOfRows();
      nbCols_ = m.getNumberOfColumns();
      data_.resize(size());
      for (auto r : makeRange(nbRows_))
        for (auto c : makeRange(nbCols_))
          operator()(r, c) = m(r, c);
    }
  };

  // TODO add LinearizedColMatrix ? (just change index function from LinearizedRowMatrix).

  /// Let old code map to the default linearized matrix.
  template <typename Scalar>
  using LinearMatrix = LinearizedRowMatrix<Scalar>;

  /** Default matrix implementation.
   * Use this type if no specific operations are requested on the matrix.
   */
  template <typename Scalar>
  using DefaultMatrix = LinearizedRowMatrix<Scalar>;

  template <class Scalar>
  bool operator==(const Matrix<Scalar>& lhs, const Matrix<Scalar>& rhs)
  {
    if (lhs.getNumberOfRows() != rhs.getNumberOfRows() || lhs.getNumberOfColumns() != rhs.getNumberOfColumns())
      return false;
    for (auto i : makeRange(lhs.getNumberOfRows()))
      for (auto j : makeRange(lhs.getNumberOfColumns()))
        if (lhs(i, j) != rhs(i, j))
          return false;
    return true;
  }
} // end of namespace bpp.

#endif // _MATRIX_H_

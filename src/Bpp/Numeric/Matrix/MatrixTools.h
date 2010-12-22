//
// File: MatrixTools.h
// Created by: Julien Dutheil
// Created on: Mon Jan 19 16:42:25 2004
//

/*
  Copyright or © or Copr. CNRS, (November 17, 2004)

  This software is a computer program whose purpose is to provide classes
  for numerical calculus. This file is part of the Bio++ project.

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

#ifndef _MATRIXTOOLS_H_
#define _MATRIXTOOLS_H_

#include "../VectorTools.h"
#include "Matrix.h"
#include "LUDecomposition.h"
#include "EigenValue.h"

#include <cstdio>
#include <iostream>

namespace bpp
{

  /**
   * @brief Functions dealing with matrices.
   */
  class MatrixTools
  {
  public:
    MatrixTools() {}
    ~MatrixTools() {}

  public:
    /**
     * @brief Copy operation. This function supplies the lack of inheritence of the assigment operator :D .
     *
     * @param A [in] Original matrix.
     * @param O [out] A copy of the given matrix.
     */
    template<class MatrixA, class MatrixO>
    static void copy(const MatrixA& A, MatrixO& O)
    { 
      O.resize(A.getNumberOfRows(), A.getNumberOfColumns());
      for (unsigned int i = 0; i < A.getNumberOfRows(); i++)
        for (unsigned int j = 0; j < A.getNumberOfColumns(); j++)
          O(i, j) = A(i, j);
    }
	
    /**
     * @brief Get a identity matrix of a given size.
     *
     * @param n the size of the matrix.
     * @param O [out] A identity matrix of size n.
     */
    template<class Matrix>
    static void getId(unsigned int n, Matrix& O)
    {
      O.resize(n, n);
      for (unsigned int i = 0; i < n; i++)
        {
          for (unsigned int j = 0; j < n; j++) O(i, j) = (i == j) ? 1 : 0;
        }
    }

    /**
     * @param D [in] A vector of diagonal elements.
     * @param O [out] A diagonal matrix with diagonal elements taken from a vector.
     */
    template<class Scalar>
    static void diag(const std::vector<Scalar>& D, Matrix<Scalar>& O)
    {
      unsigned int n = D.size();
      O.resize(n, n);
      for (unsigned int i = 0; i < n; i++)
        {
          for (unsigned int j = 0; j < n; j++) O(i, j) = (i == j) ? D[i] : 0;
        }
    }

    /**
     * @param M [in] The matrix.
     * @param O [out] The diagonal elements of a square matrix as a vector.
     * @throw DimensionException If M is not a square matrix.
     */
    template<class Scalar>
    static void diag(const Matrix<Scalar>& M, std::vector<Scalar>& O) throw (DimensionException)
    {
      unsigned int nc = M.getNumberOfColumns();
      unsigned int nr = M.getNumberOfRows();
      if (nc != nr) throw DimensionException("MatrixTools::diag(). M must be a square matrix.", nr, nc); 
      O.resize(nc);
      for (unsigned int i = 0; i < nc; i++) O[i] = M(i, i);
    }

    /**
     * @brief Set all elements in M to value x.
     * @param M A matrix.
     * @param x The value to use.
     */
    template<class Matrix, class Scalar>
    static void fill(Matrix& M, Scalar x)
    {
      for (unsigned int i = 0; i < M.getNumberOfRows(); i++)
        {
          for (unsigned int j = 0; j < M.getNumberOfColumns(); j++)
            {
              M(i, j) = x;
            }
        }
    }

    /**
     * @brief Multiply all elements of a matrix by a given value, and add a constant.
     *
     * Performs \f$\forall i \forall j m_{i,j} = a.m_{i,j}+b\f$.
     * 
     * @param A A matrix.
     * @param a Multiplicator.
     * @param b Constant.
     */
    template<class Matrix, class Scalar>
    static void scale(Matrix& A, Scalar a, Scalar b = 0)
    {
      for (unsigned int i = 0; i < A.getNumberOfRows(); i++)
        {
          for (unsigned int j = 0; j < A.getNumberOfColumns(); j++)
            {
              A(i,j) = a * A(i, j) + b;
            }
        }
    }

    /**
     * @param A [in] First matrix.
     * @param B [in] Second matrix.
     * @param O [out] The dot product of two matrices.
     */
    template<class Scalar>
    static void mult(const Matrix<Scalar>& A, const Matrix<Scalar>& B, Matrix<Scalar>& O) throw (DimensionException)
    {
      unsigned int ncA = A.getNumberOfColumns();
      unsigned int nrA = A.getNumberOfRows();
      unsigned int nrB = B.getNumberOfRows();
      unsigned int ncB = B.getNumberOfColumns();
      if (ncA != nrB) throw DimensionException("MatrixTools::mult(). nrows B != ncols A.", nrB, ncA); 
      O.resize(nrA, ncB);
      for (unsigned int i = 0; i < nrA; i++)
        {
          for (unsigned int j = 0; j < ncB; j++)
            {
              O(i, j) = 0;
              for (unsigned int k = 0; k < ncA; k++)
                {
                  O(i, j) += A(i, k) * B(k, j);
                }
            }
        }
    }

    /**
     * @brief Compute A . D . B where D is a diagonal matrix in O(n^3).
     *
     * Since D is a diagonal matrix, this function is more efficient than doing
     * mult(mult(A, diag(D)), B), which involves two 0(n^3) operations.
     *
     * @param A [in] The first matrix.
     * @param D [in] The diagonal matrix (only diagonal elements in a vector)
     * @param B [in] The second matrix.
     * @param O [out] The result matrix.
     * @throw DimensionException If matrices have not the appropriate size.
     */
    template<class Scalar>
    static void mult(const Matrix<Scalar>& A, const std::vector<Scalar>& D, const Matrix<Scalar>& B, Matrix<Scalar>& O) throw (DimensionException)
    {
      unsigned int ncA = A.getNumberOfColumns();
      unsigned int nrA = A.getNumberOfRows();
      unsigned int nrB = B.getNumberOfRows();
      unsigned int ncB = B.getNumberOfColumns();
      if(ncA != nrB) throw DimensionException("MatrixTools::mult(). nrows B != ncols A.", nrB, ncA); 
      if(ncA != D.size()) throw DimensionException("MatrixTools::mult(). Vector size is not eual to matrix size.", D.size(), ncA); 
      O.resize(nrA, ncB);
      for(unsigned int i = 0; i < nrA; i++)
        {
          for(unsigned int j = 0; j < ncB; j++)
            {
              O(i, j) = 0;
              for(unsigned int k = 0; k < ncA; k++)
                {
                  O(i, j) += A(i, k) * B(k, j) * D[k];
                }
            }
        }
    }

    /**
     * @brief Add matrix B to matrix A.
     *
     * @param A [in] Matrix A
     * @param B [in] Matrix B
     * @throw DimensionException If A and B have note the same size.
     */
    template<class MatrixA, class MatrixB>
    static void add(MatrixA& A, const MatrixB& B) throw (DimensionException)
    {
      unsigned int ncA = A.getNumberOfColumns();
      unsigned int nrA = A.getNumberOfRows();
      unsigned int nrB = B.getNumberOfRows();
      unsigned int ncB = B.getNumberOfColumns();
      if(ncA != ncB) throw DimensionException("MatrixTools::operator+(). A and B must have the same number of colums.", ncB, ncA); 
      if(nrA != nrB) throw DimensionException("MatrixTools::operator+(). A and B must have the same number of rows.", nrB, nrA); 
      for(unsigned int i = 0; i < A.getNumberOfRows(); i++)
        {
          for(unsigned int j = 0; j < A.getNumberOfColumns(); j++)
            {
              A(i, j) += B(i, j);
            }
        }
    }		
	
    /**
     * @brief Compute the power of a given matrix.
     *
     * @param A [in] The matrix.
     * @param p The number of multiplications.
     * @param O [out]\f$\prod_{i=1}^p m\f$.
     * If p = 0, sends the identity matrix.
     * @throw DimensionException If m is not a square matrix.
     */
    template<class Matrix>
    static void pow(const Matrix& A, unsigned int p, Matrix& O) throw (DimensionException)
    {
      unsigned int n = A.getNumberOfRows();
      if (n != A.getNumberOfColumns()) throw DimensionException("MatrixTools::pow(). nrows != ncols.", A.getNumberOfColumns(), A.getNumberOfRows()); 
      getId<Matrix>(n, O);
      Matrix tmp;
      for(unsigned int i = 0; i < p; i++)
        {
          tmp = O;
          mult(tmp, A, O);
        }
    }
	
    /**
     * @brief Compute the power of a given matrix, using eigen value decomposition.
     *
     * @param A [in] The matrix.
     * @param p The power of the matrix.
     * @param O [out]\f$\prod_{i=1}^p m\f$.
     * If p = 0, sends the identity matrix.
     * @throw DimensionException If m is not a square matrix.
     */
    template<class Scalar>
    static void pow(const Matrix<Scalar>& A, double p, Matrix<Scalar>& O) throw (DimensionException)
    {
      unsigned int n = A.getNumberOfRows();
      if (n != A.getNumberOfColumns()) throw DimensionException("MatrixTools::pow(). nrows != ncols.", A.getNumberOfColumns(), A.getNumberOfRows()); 
      EigenValue<Scalar> eigen(A);
      RowMatrix<Scalar> rightEV, leftEV;
      rightEV = eigen.getV();
      inv(rightEV, leftEV);
      mult(rightEV, VectorTools::pow(eigen.getRealEigenValues(), p), leftEV, O);
    }

    /**
     * @brief Perform matrix exponentiation.
     *
     * @warning This method currently relies only on diagonalization, so it won't work if your matrix is not diagonalizable.
     * The function may be extended later to deal with other cases.
     *
     * @param A [in] The matrix.
     * @param O [out]\f$\prod_{i=1}^p m\f$.
     * @throw DimensionException If m is not a square matrix.
     */
    template<class Scalar>
    static void exp(const Matrix<Scalar>& A, Matrix<Scalar>& O) throw (DimensionException)
    {
      unsigned int n = A.getNumberOfRows();
      if (n != A.getNumberOfColumns()) throw DimensionException("MatrixTools::exp(). nrows != ncols.", A.getNumberOfColumns(), A.getNumberOfRows()); 
      EigenValue<Scalar> eigen(A);
      RowMatrix<Scalar> rightEV, leftEV;
      rightEV = eigen.getV();
      inv(rightEV, leftEV);
      mult(rightEV, VectorTools::exp(eigen.getRealEigenValues()), leftEV, O);
    }
	
    /**
     * @return The position of the maximum value in the matrix.
     * @param m The matrix.
     */
    template<class Matrix>
    static std::vector<unsigned int> whichMax(const Matrix & m)
    {
      unsigned int nrows = m.getNumberOfRows();
      unsigned int ncols = m.getNumberOfColumns();
      std::vector<unsigned int> pos(2);
      unsigned int imax = 0;
      unsigned int jmax = 0;
      double currentMax = log(0.);
      for(unsigned int i = 0; i < nrows; i++)
        {
          for(unsigned int j = 0; j < ncols; j++)
            {
              double currentValue = m(i, j);
              //cout << currentValue << "\t" << (currentValue > currentMax) << endl;
              if(currentValue > currentMax)
                {
                  imax = i;
                  jmax = j;
                  currentMax = currentValue;
                }
            }
        }
      pos[0] = imax;
      pos[1] = jmax;
      return pos;
    }

    /**
     * @return The position of the minimum value in the matrix.
     * @param m The matrix.
     */
    template<class Matrix>
    static std::vector<unsigned int> whichMin(const Matrix& m)
    {
      unsigned int nrows = m.getNumberOfRows();
      unsigned int ncols = m.getNumberOfColumns();
      std::vector<unsigned int> pos(2);
      unsigned int imin = 0;
      unsigned int jmin = 0;
      double currentMin = -log(0.);
      for (unsigned int i = 0; i < nrows; i++)
        {
          for (unsigned int j = 0; j < ncols; j++)
            {
              double currentValue = m(i, j);
              if (currentValue < currentMin)
                {
                  imin = i;
                  jmin = j;
                  currentMin = currentValue;
                }
            }
        }
      pos[0] = imin;
      pos[1] = jmin;
      return pos;
    }

    /**
     * @brief Print a matrix to a stream.
     * 
     * @param m The matrix to print.
     * @param out The stream to use.
     */
    template<class Matrix>
    static void print(const Matrix& m, std::ostream& out = std::cout)
    {
      out << m.getNumberOfRows() << "x" << m.getNumberOfColumns() << std::endl;
      out << "[" << std::endl;
      for (unsigned int i = 0; i < m.getNumberOfRows(); i++)
        {
          out << "[";
          for (unsigned int j = 0; j < m.getNumberOfColumns() - 1; j++)
            {
              out << m(i, j) << ", ";
            }
          if (m.getNumberOfColumns() > 0) out << m(i, m.getNumberOfColumns() - 1) << "]" << std::endl;
        }
      out << "]" << std::endl;
    }
		
    /**
     * @brief Print a vector to a stream.
     * 
     * @param v The vector to print.
     * @param out The stream to use.
     */
    template<class Real>
    static void print(const std::vector<Real>& v, std::ostream& out = std::cout)
    {
      out << v.size() << std::endl;
      out << "[";
      for(unsigned int i = 0; i < v.size() - 1; i++)
        {
          out << v[i] << ", ";
        }
      if(v.size() > 0) out << v[v.size() - 1];
      out << "]" << std::endl;
    }

    /**
     * @return True if the matrix is a square matrix.
     * @param A A matrix.
     */
    template<class Matrix>
    static bool isSquare(const Matrix& A) { return A.getNumberOfRows() == A.getNumberOfColumns(); }

    /**
     * @param A [in] The matrix to inverse.
     * @param O [out] The inverse matrix of A.
     * @throw DimensionException If A is not a square matrix.
     */
    template<class Scalar>
    static void inv(const Matrix<Scalar>& A, Matrix<Scalar>& O) throw (DimensionException)
    {
      if (! isSquare(A)) throw DimensionException("MatrixTools::inv(). Matrix A is not a square matrix.", A.getNumberOfRows(), A.getNumberOfColumns());
      LUDecomposition<Scalar> lu(A);
      RowMatrix<Scalar> I;
      getId(A.getNumberOfRows(), I);
      copy(lu.solve(I), O);
    }

    /**
     * @brief Get determinant of a square matrix.
     *
     * This implementation is in @f$o(n^3)@f$ and uses the LU decomposition method.
     *
     * @param A [in] The inputm atrix.
     * @return The determinant of A.
     * @throw DimensionException If A is not a square matrix.
     */
    template<class Scalar>
    static double det(const Matrix<Scalar>& A) throw (DimensionException)
    {
      if (! isSquare(A)) throw DimensionException("MatrixTools::det(). Matrix A is not a square matrix.", A.getNumberOfRows(), A.getNumberOfColumns());
      LUDecomposition<Scalar> lu(A);
      return lu.det();
    }

    /**
     * @param A [in] The matrix to transpose.
     * @param O [out] The transposition of A.
     */
    template<class MatrixA, class MatrixO>
    static void transpose(const MatrixA& A, MatrixO& O)
    {
      O.resize(A.getNumberOfColumns(), A.getNumberOfRows());
      for (unsigned int i = 0; i < A.getNumberOfColumns(); i++)
        {
          for (unsigned int j = 0; j < A.getNumberOfRows(); j++)
            {
              O(i, j) = A(j, i);
            }
        }
    }
	
    /**
     * @brief Compute the variance-covariance matrix of an input matrix.
     *
     * The input matrix represent a n-sample of a random vector of dimension r.
     * It is assumed to have r rows and n columns.
     * The variance matrix is then computed as @f[ V = A\cdot A^T - \mu\cdot\mu^T@f],
     * where @f$\mu@f$ is the mean vector of the sample.
     * the output matrix is a square matrix of size r.
     *
     * @param A [in] The intput matrix.
     * @param O [out] The resulting variance covariance matrix.
     */
    template<class Scalar>
    static void covar(const Matrix<Scalar>& A, Matrix<Scalar>& O)
    {
      unsigned int r = A.getNumberOfRows();
      unsigned int n = A.getNumberOfColumns();
      O.resize(r, r);
      RowMatrix<Scalar> tA;
      transpose(A, tA);
      mult(A, tA, O);
      scale(O, 1. / static_cast<double>(n));
      RowMatrix<Scalar> mean(r, 1);
      for (unsigned int i = 0; i < r; i++)
        {
          for (unsigned int j = 0; j < n; j++)
            mean(i, 0) += A(i, j);
          mean(i, 0) /= static_cast<double>(n);
        }
      RowMatrix<Scalar> tMean;
      transpose(mean, tMean);
      RowMatrix<Scalar> meanMat;
      mult(mean, tMean, meanMat);
      scale(meanMat, -1.);
      add(O, meanMat);
    }

    /**
     * @brief Compute the Kronecker product of two row matrices.
     *
     * @param A [in] The first row matrix.
     * @param B [in] The second row matrix.
     * @param O [out] The product \f$A \otimes B\f$.
     */
    template<class Scalar>
    static void kroneckerMult(const Matrix<Scalar>& A, const Matrix<Scalar>& B, Matrix<Scalar>& O)
    {
      unsigned int ncA = A.getNumberOfColumns();
      unsigned int nrA = A.getNumberOfRows();
      unsigned int nrB = B.getNumberOfRows();
      unsigned int ncB = B.getNumberOfColumns();
      O.resize(nrA*nrB, ncA*ncB);
      for (unsigned int ia = 0; ia < nrA; ia++)
        {
          for (unsigned int ja = 0; ja < ncA; ja++)
            {
              Scalar aij = A(ia, ja);
              for (unsigned int ib = 0; ib < nrB; ib++)
                {
                  for (unsigned int jb = 0; jb < ncB; jb++)
                    {
                      O(ia * nrB + ib, ja * ncB + jb) = aij * B(ib,jb);
                    }
                }
            }
        }
    }

    /**
     * @brief Compute the Kronecker sum of two row matrices.
     *
     * @param A [in] The first row matrix.
     * @param B [in] The second row matrix.
     * @param O [out] The product \f$A \oplus B\f$.
     */
    template<class Scalar>
    static void kroneckerSum(const Matrix<Scalar>& A, const Matrix<Scalar>& B, Matrix<Scalar>& O)
    {
      unsigned int ncA = A.getNumberOfColumns();
      unsigned int nrA = A.getNumberOfRows();
      unsigned int nrB = B.getNumberOfRows();
      unsigned int ncB = B.getNumberOfColumns();
      O.resize(nrA + nrB, ncA + ncB);
      for (unsigned int ia = 0; ia < nrA; ia++)
        {
          for (unsigned int ja = 0; ja < ncA; ja++)
            {
              O(ia, ja) = A(ia,ja);
            }
        }
      for (unsigned int ib = 0; ib < nrB; ib++)
        {
          for (unsigned int jb = 0; jb < nrB; jb++)
            {
              O(nrA + ib, ncA + jb) = B(ib,jb);
            }
        }
    }

    /**
     * @brief Compute the Kronecker sum of n row matrices.
     *
     * @param vA [in] A vector of row matrices of any size.
     * @param O [out] The product \f$\bigoplus_i A_i\f$.
     */
    template<class Scalar>
    static void kroneckerSum(const std::vector< Matrix<Scalar> *>& vA, Matrix<Scalar>& O)
    {
      unsigned int nr = 0;
      unsigned int nc = 0;
      for (unsigned int k = 0; k < vA.size(); k++)
        {
          nr += vA[k]->getNumberOfRows();
          nc += vA[k]->getNumberOfColumns();
        }
      O.resize(nr, nc);
      unsigned int rk = 0; //Row counter
      unsigned int ck = 0; //Col counter
      for (unsigned int k = 0; k < vA.size(); k++)
        {
          const Matrix<Scalar> * Ak = vA[k];
          for (unsigned int i = 0; i < Ak->getNumberOfRows(); i++)
            {
              for (unsigned int j = 0; j < Ak->getNumberOfColumns(); j++)
                {
                  O(rk + i, ck + j) = (*Ak)(i,j);
                }
            }
          rk += Ak->getNumberOfRows();
          ck += Ak->getNumberOfColumns();
        }
    }

    /**
     * @brief Convert to a vector of vector.
     *
     * @param M [in] A matrix object.
     * @param vO [out] The output vector of vector (will be resized accordingly).
     */
    template<class Scalar>
    static void toVVdouble(const Matrix<Scalar>& M, std::vector< std::vector<Scalar> >& vO)
    {
      unsigned int n =M.getNumberOfRows();
      unsigned int m =M.getNumberOfColumns();
      vO.resize(n);
      for (unsigned int i = 0; i < n; i++)
        {
          vO[i].resize(m);
          for (unsigned int j = 0; j < m; j++)
            {
              vO[i][j] = M(i, j);
            }
        }
    }

    /**
     * @brief Sum all elements in M.
     * @param M A matrix.
     * @return The sum of all elements.
     */
    template<class Scalar>
    static Scalar sumElements(const Matrix<Scalar>& M)
    {
      Scalar sum = 0;
      for (unsigned int i = 0; i < M.getNumberOfRows(); i++) {
        for (unsigned int j = 0; j < M.getNumberOfColumns(); j++) {
          sum += M(i, j);
        }
      }
      return sum;
    }

	
	
  };

  /* DEPRECATED 
     namespace MatrixOperators {
	
     MatrixB operator*(const MatrixA & A, const MatrixB & B) throw (DimensionException)
     {
     return MatrixTools::mult<MatrixA, MatrixB>(A, B);
     }
	
     template<class MatrixA, class MatrixB>
     MatrixA operator+(const MatrixA & A, const MatrixB & B) throw (DimensionException)
     {
     MatrixA C = A;
     MatrixTools::add<MatrixA, MatrixB>(C, B);
     return C;
     }

     template<class MatrixA, class MatrixB>
     MatrixA operator+=(MatrixA & A, const MatrixB & B) throw (DimensionException)
     {
     MatrixTools::add<MatrixA, MatrixB>(A, B);
     return A;
     }

     template<class Matrix>
     Matrix operator-(const Matrix A)
     {
     Matrix B(A.getNumberOfRows(), A.getNumberOfColumns());
     for(unsigned int i = 0; i < B.getNumberOfRows(); i++) {
     for(unsigned int j = 0; j < B.getNumberOfColumns(); j++) {
     B(i, j) = -A(i, j);
     }
     }
     return B;
     }

     template<class MatrixA, class MatrixB>
     MatrixA operator-(const MatrixA & A, const MatrixB & B) throw (DimensionException)
     {
     //	  unsigned int ncA = A.getNumberOfColumns();
     //		unsigned int nrA = A.getNumberOfRows();
     //		unsigned int nrB = B.getNumberOfRows();
     //		unsigned int ncB = B.getNumberOfColumns();
     //		if(ncA != ncB) throw DimensionException("MatrixTools::operator-(). A and B must have the same number of colums.", ncB, ncA); 
     //		if(nrA != nrB) throw DimensionException("MatrixTools::operator-(). A and B must have the same number of rows.", nrB, nrA); 
     //		MatrixB C(A.getNumberOfRows(), A.getNumberOfColumns());
     //		for(unsigned int i = 0; i < A.getNumberOfRows(); i++) {
     //			for(unsigned int j = 0; j < A.getNumberOfColumns(); j++) {
     //				C(i, j) = A(i, j) - B(i, j);
     //			}
     //		}
     //		return C;
     MatrixA C = A;
     MatrixTools::add<MatrixA, MatrixB>(C, -B);
     return C;
     }
	
     template<class MatrixA, class MatrixB>
     MatrixA operator-=(MatrixA & A, const MatrixB & B) throw (DimensionException)
     {
     MatrixTools::add<MatrixA, MatrixB>(A, -B);
     return A;
     }

     };
  */

} //end of namespace bpp.

#endif	//_MATRIXTOOLS_H_

//
// File: LU.h
// Created by: Julien Dutheil
// Created on: Tue Apr 7 16:24 2004
//

/*
  Copyright or © or Copr. CNRS, (November 17, 2004)

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

#ifndef _LU_H_
#define _LU_H_

#include "Matrix.h"
#include "../NumTools.h"
#include "../../Exceptions.h"

//From the STL:
#include <algorithm>
#include <vector>
//for min(), max() below

namespace bpp
{

  /** 
   * @brief LU Decomposition.
   *
   * [This class and its documentation is adpated from the C++ port of the JAMA library.]
   * 
   * For an m-by-n matrix A with m >= n, the LU decomposition is an m-by-n
   * unit lower triangular matrix L, an n-by-n upper triangular matrix U,
   * and a permutation vector piv of length m so that A(piv,:) = L*U.
   * If m < n, then L is m-by-m and U is m-by-n.
   *
   * The LU decompostion with pivoting always exists, even if the matrix is
   * singular, so the constructor will never fail.  The primary use of the
   * LU decomposition is in the solution of square systems of simultaneous
   * linear equations. This will fail if isNonsingular() returns false.
   */
  template <class Real>
  class LUDecomposition
  {

  private:
    /* Array for internal storage of decomposition.  */
    RowMatrix<Real> LU;
    RowMatrix<Real> L_;
    RowMatrix<Real> U_;
    unsigned int m, n;
    int pivsign; 
    std::vector<unsigned int> piv;

  private:

    static void permuteCopy(const Matrix<Real>& A, const std::vector<unsigned int>& piv, unsigned int j0, unsigned int j1, Matrix<Real>& X)
    {
      unsigned int piv_length = piv.size();

      X.resize(piv_length, j1-j0+1);

      for (unsigned int i = 0; i < piv_length; i++) 
      	for (unsigned int j = j0; j <= j1; j++) 
          X(i,j-j0) = A(piv[i],j);
    }

    static void permuteCopy(const std::vector<Real>& A, const std::vector<unsigned int>& piv, std::vector<Real>& X)
    {
      unsigned int piv_length = piv.size();
      if (piv_length != A.size())
        X.clean();
      
      X.resize(piv_length);

      for (unsigned int i = 0; i < piv_length; i++) 
        X[i] = A[piv[i]];
    }


  public :

    /** 
     * @brief LU Decomposition
     * 
     * @param  A   Rectangular matrix
     * @return LU Decomposition object to access L, U and piv.
     */

    // This is the constructor in JAMA C++ port. However, it seems to have some bug...
    // We use the original JAMA's 'experimental' port, which gives good results instead.
    /*	  LUDecomposition (const Matrix<Real> &A) : LU(A), m(A.getNumberOfRows()), n(A.getNumberOfColumns()), piv(A.getNumberOfRows())
          {

          // Use a "left-looking", dot-product, Crout/Doolittle algorithm.


          for (unsigned int i = 0; i < m; i++) {
          piv[i] = i;
          }
          pivsign = 1;
          //Real *LUrowi = 0;;
          vector<Real> LUrowi;
          vector<Real> LUcolj;

          // Outer loop.

          for (unsigned int j = 0; j < n; j++) {

          // Make a copy of the j-th column to localize references.

          LUcolj = LU.col(j);
          //for (int i = 0; i < m; i++) {
     	  //  LUcolj[i] = LU(i,j);
     	  //}

    	  // Apply previous transformations.

     	  for (unsigned int i = 0; i < m; i++) {
          //LUrowi = LU[i];
          LUrowi = LU.row(i);

          // Most of the time is spent in the following dot product.

          unsigned int kmax = NumTools::min<unsigned int>(i,j);
          double s = 0.0;
          for (unsigned int k = 0; k < kmax; k++) {
          s += LUrowi[k] * LUcolj[k];
          }

          LUrowi[j] = LUcolj[i] -= s;
          }
   
          // Find pivot and exchange if necessary.

          unsigned int p = j;
          for (unsigned int i = j+1; i < m; i++) {
          if (NumTools::abs<Real>(LUcolj[i]) > NumTools::abs<Real>(LUcolj[p])) {
          p = i;
          }
          }
          if (p != j) {
          unsigned int k=0;
          for (k = 0; k < n; k++) {
          double t = LU(p,k); 
          LU(p,k) = LU(j,k); 
          LU(j,k) = t;
          }
          k = piv[p]; 
          piv[p] = piv[j]; 
          piv[j] = k;
          pivsign = -pivsign;
          }

          // Compute multipliers.
         
          if ((j < m) && (LU(j,j) != 0.0)) {
          for (unsigned int i = j+1; i < m; i++) {
          LU(i,j) /= LU(j,j);
          }
          }
          }
          }
    */

    LUDecomposition (const Matrix<Real>& A) :
      LU(A),
      L_(A.getNumberOfRows(), A.getNumberOfColumns()), U_(A.getNumberOfColumns(), A.getNumberOfColumns()),
      m(A.getNumberOfRows()), n(A.getNumberOfColumns()),
      pivsign(1), piv(A.getNumberOfRows())
    {
      for (unsigned int i = 0; i < m; i++)
        {
          piv[i] = i;
    	}
      // Main loop.
      for (unsigned int k = 0; k < n; k++)
        {
          // Find pivot.
          unsigned int p = k;
          for (unsigned int i = k+1; i < m; i++)
            {
              if (NumTools::abs<Real>(LU(i,k)) > NumTools::abs<Real>(LU(p,k)))
                {
                  p = i;
                }
            }
          // Exchange if necessary.
          if (p != k)
            {
              for (unsigned int j = 0; j < n; j++)
                {
                  double t = LU(p,j); LU(p,j) = LU(k,j); LU(k,j) = t;
        	}
              unsigned int t = piv[p]; piv[p] = piv[k]; piv[k] = t;
              pivsign = -pivsign;
            }
          // Compute multipliers and eliminate k-th column.
          if (LU(k,k) != 0.0)
            {
              for (unsigned int i = k+1; i < m; i++)
                {
                  LU(i,k) /= LU(k,k);
                  for (unsigned int j = k+1; j < n; j++)
                    {
                      LU(i,j) -= LU(i,k)*LU(k,j);
                    }
         	}
            }
        }
    }

    /**
     * @brief Return lower triangular factor
     * 
     * @return L
     */
    const RowMatrix<Real>& getL() 
    {
      for (unsigned int i = 0; i < m; i++)
        {
          for (unsigned int j = 0; j < n; j++)
            {
              if (i > j)
                {
                  L_(i,j) = LU(i,j);
       		}
              else if (i == j)
                {
                  L_(i,j) = 1.0;
       		}
              else
                {
                  L_(i,j) = 0.0;
       		}
            }
        }
      return L_;
    }

    /**
     * @brief Return upper triangular factor
     * 
     * @return U portion of LU factorization.
     */
    const RowMatrix<Real>& getU ()
    {
      for (unsigned int i = 0; i < n; i++)
        {
          for (unsigned int j = 0; j < n; j++)
            {
              if (i <= j)
                {
                  U_(i,j) = LU(i,j);
       		}
              else
                {
                  U_(i,j) = 0.0;
       		}
            }
        }
      return U_;
    }

    /**
     * @brief Return pivot permutation vector
     *
     * @return piv
     */
    std::vector<unsigned int> getPivot () const
    {
      return piv;
    }


    /**
     * @brief Compute determinant using LU factors.
     * 
     * @return determinant of A, or 0 if A is not square.
     */
    Real det() const
    {
      if (m != n)
        {
          return Real(0);
    	}
      Real d = Real(pivsign);
      for (unsigned int j = 0; j < n; j++)
        {
          d *= LU(j,j);
        }
      return d;
    }

    /** 
     * @brief Solve A*X = B
     *
     * @param  const B [in]  A Matrix with as many rows as A and any number of columns.
     * @param  X [out]  A RowMatrix that will be changed such that L*U*X = B(piv,:).
     * @return  the lowest diagonal term (in absolute value), for further checkings
     *             of non-singularity of LU.
     *
     * If B is nonconformant or LU is singular, an Exception is raised.
     *
     */
    Real solve (const Matrix<Real>& B, Matrix<Real>& X) const throw (BadIntegerException, ZeroDivisionException)
    {
      /* Dimensions: A is mxn, X is nxk, B is mxk */
      
      if (B.getNumberOfRows() != m) {
        throw BadIntegerException("Wrong dimension in LU::solve", B.getNumberOfRows());
      }

      Real minD = NumTools::abs<Real>(LU(0,0));
      for (unsigned int i = 1; i < m; i++)
        {
          Real currentValue = NumTools::abs<Real>(LU(i, i));
          if (currentValue < minD)
            minD = currentValue;
        }

      if (minD<NumConstants::TINY){
      	throw ZeroDivisionException("Singular matrix in LU::solve.");
      }

      // Copy right hand side with pivoting
      //int nx = B.dim2();
      unsigned int nx = B.getNumberOfColumns();

      permuteCopy(B, piv, 0, nx-1, X);

      // Solve L*Y = B(piv,:)
      for (unsigned int k = 0; k < n; k++) {
        for (unsigned int i = k+1; i < n; i++) {
          for (unsigned int j = 0; j < nx; j++) {
            X(i,j) -= X(k,j) * LU(i,k);
          }
        }
      }
      // Solve U*X = Y;
      // !!! Do not use unsigned int with -- loop!!!
      //for (int k = n-1; k >= 0; k--) {
      unsigned int k = n;
      
      do {
        k--;
        for (unsigned int j = 0; j < nx; j++) {
          X(k,j) /= LU(k,k);
        }
        for (unsigned int i = 0; i < k; i++) {
          for (unsigned int j = 0; j < nx; j++) {
            X(i,j) -= X(k,j) * LU(i,k);
          }
        }
      } while(k > 0);

      return minD;
    }


    /** 
     * @brief Solve A*x = b, where x and b are vectors of length equal	to the number of rows in A.
     *
     * @param  b [in] a vector (Array1D> of length equal to the first dimension	of A.
     * @param  x [out] a vector that will be changed so that so that L*U*x = b(piv).
     * @return  the lowest diagonal term (in absolute value), for further checkings
     *             of non-singularity of LU.
     *
     * If B is nonconformant or LU is singular, an Exception is raised.
     */

    Real solve (const std::vector<Real> &b, std::vector<Real>& x)  const throw (BadIntegerException, ZeroDivisionException)
    {

      /* Dimensions: A is mxn, X is nxk, B is mxk */
      
      if (b.dim1() != m) {
        throw BadIntegerException("Wrong dimension in LU::solve", b.dim1());
      }

      Real minD = NumTools::abs<Real>(LU(0,0));
      for (unsigned int i = 1; i < m; i++)
        {
          Real currentValue = NumTools::abs<Real>(LU(i, i));
          if (currentValue < minD)
            minD = currentValue;
        }

      if (minD<NumConstants::TINY){
      	throw ZeroDivisionException("Singular matrix in LU::solve.");
      }

      permuteCopy(b, piv, x);

      // Solve L*Y = B(piv)
      for (unsigned int k = 0; k < n; k++) {
        for (unsigned int i = k+1; i < n; i++) {
          x[i] -= x[k] * LU(i,k);
        }
      }
      
      // Solve U*X = Y;
      //for (unsigned int k = n-1; k >= 0; k--) {
      unsigned int k = n;
      do {
        k--;
        x[k] /= LU(k,k);
        for (unsigned int i = 0; i < k; i++) {
          x[i] -= x[k] * LU(i,k);
        }
      } while(k > 0);
     
      return minD;
    }

  }; /* class LU */

} //end of namespace bpp.

#endif //_LU_H_


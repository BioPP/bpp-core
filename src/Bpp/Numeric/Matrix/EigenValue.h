//
// File: EigenValue.h
// Created by: Julien Dutheil
// Created on: Tue Apr 7 16:24 2004
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



#ifndef _EIGENVALUE_H_
#define _EIGENVALUE_H_

#include <algorithm>
// for min(), max() below

#include <cmath>
// for abs() below

#include "Matrix.h"
#include "../NumTools.h"

namespace bpp
{

/** 
 * @brief Computes eigenvalues and eigenvectors of a real (non-complex) matrix. 
 * 
 * [This class and its documentation is adpated from the C++ port of the JAMA library.]
 * 
 * If A is symmetric, then A = V*D*V' where the eigenvalue matrix D is
 * diagonal and the eigenvector matrix V is orthogonal. That is,
 * the diagonal values of D are the eigenvalues, and
 * V*V' = I, where I is the identity matrix.  The columns of V 
 * represent the eigenvectors in the sense that A*V = V*D.
 *
 * If A is not symmetric, then the eigenvalue matrix D is block diagonal
 * with the real eigenvalues in 1-by-1 blocks and any complex eigenvalues,
 * a + i*b, in 2-by-2 blocks, [a, b; -b, a].  That is, if the complex
 * eigenvalues look like
 * <pre>
 * 
 *         u + iv     .        .          .      .    .
 *           .      u - iv     .          .      .    .
 *           .        .      a + ib       .      .    .
 *           .        .        .        a - ib   .    .
 *           .        .        .          .      x    .
 *           .        .        .          .      .    y
 * </pre>
 * then D looks like
 * <pre>
 * 
 *           u        v        .          .      .    .
 *          -v        u        .          .      .    . 
 *           .        .        a          b      .    .
 *           .        .       -b          a      .    .
 *           .        .        .          .      x    .
 *           .        .        .          .      .    y
 * </pre>
 * This keeps V a real matrix in both symmetric and non-symmetric
 *  cases, and A*V = V*D.
 *
 *
 * The matrix V may be badly
 * conditioned, or even singular, so the validity of the equation
 * A = V*D*inverse(V) depends upon the condition number of V.
 *
 * (Adapted from JAMA, a Java Matrix Library, developed by jointly 
 *  by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).
 */
template <class Real>
class EigenValue
{

   /** 
    * @brief Row and column dimension (square matrix).
    */
   int _n;

   /**
    * @brief Tell if the matrix is symmetric.
    */
   bool _issymmetric;

   /**
    * @name Arrays for internal storage of eigenvalues.
    *
    * @{
    */
   std::vector<Real> _d;         /* real part */
   std::vector<Real> _e;         /* img part */
   /** @} */
   
   /**
    * @brief Array for internal storage of eigenvectors.
    */
   RowMatrix<Real> _V;

   /**
    * @brief Matrix for internal storage of nonsymmetric Hessenberg form.
    *
    * Internal storage of nonsymmetric Hessenberg form.
    */
   RowMatrix<Real> _H;
   

   /**
    * @brief Matrix for internal storage of eigen values in a matrix form.
    *
    * Internal storage of eigen values in a matrix form.
    */
   mutable RowMatrix<Real> _D;

   /**
    * @brief Working storage for nonsymmetric algorithm.
    *
    * Working storage for nonsymmetric algorithm.
    */
   std::vector<Real> ort_;


   /**
    * @brief Symmetric Householder reduction to tridiagonal form.
    *
    * This is derived from the Algol procedures tred2 by
    * Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
    * Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
    * Fortran subroutine in EISPACK.
    */
   void tred2()
   {
     for (int j = 0; j < _n; j++)
     {
       _d[j] = _V(_n-1,j);
     }

     // Householder reduction to tridiagonal form.
   
     for (int i = _n-1; i > 0; i--)
     {
       // Scale to avoid under/overflow.
   
       Real scale = 0.0;
       Real h = 0.0;
       for (int k = 0; k < i; k++)
       {
         scale = scale + NumTools::abs<Real>(_d[k]);
       }
       if (scale == 0.0)
       {
         _e[i] = _d[i-1];
         for (int j = 0; j < i; j++)
         {
           _d[j] = _V(i-1,j);
           _V(i,j) = 0.0;
           _V(j,i) = 0.0;
         }
       }
       else
       {
         // Generate Householder vector.
   
         for (int k = 0; k < i; k++)
         {
           _d[k] /= scale;
           h += _d[k] * _d[k];
         }
         Real f = _d[i-1];
         Real g = sqrt(h);
         if (f > 0)
         {
           g = -g;
         }
         _e[i] = scale * g;
         h = h - f * g;
         _d[i-1] = f - g;
         for (int j = 0; j < i; j++)
         {
           _e[j] = 0.0;
         }
   
         // Apply similarity transformation to remaining columns.
   
         for (int j = 0; j < i; j++)
         {
           f = _d[j];
           _V(j,i) = f;
           g = _e[j] + _V(j,j) * f;
           for (int k = j+1; k <= i-1; k++)
           {
             g += _V(k,j) * _d[k];
             _e[k] += _V(k,j) * f;
           }
           _e[j] = g;
         }
         f = 0.0;
         for (int j = 0; j < i; j++)
         {
           _e[j] /= h;
           f += _e[j] * _d[j];
         }
         Real hh = f / (h + h);
         for (int j = 0; j < i; j++)
         {
           _e[j] -= hh * _d[j];
         }
         for (int j = 0; j < i; j++)
         {
           f = _d[j];
           g = _e[j];
           for (int k = j; k <= i-1; k++)
           {
             _V(k,j) -= (f * _e[k] + g * _d[k]);
           }
           _d[j] = _V(i-1,j);
           _V(i,j) = 0.0;
         }
       }
       _d[i] = h;
     }
   
     // Accumulate transformations.
   
     for (int i = 0; i < _n-1; i++)
     {
       _V(_n-1,i) = _V(i,i);
       _V(i,i) = 1.0;
       Real h = _d[i+1];
       if (h != 0.0)
       {
         for (int k = 0; k <= i; k++)
         {
           _d[k] = _V(k,i+1) / h;
         }
         for (int j = 0; j <= i; j++)
         {
           Real g = 0.0;
           for (int k = 0; k <= i; k++)
           {
             g += _V(k,i+1) * _V(k,j);
           }
           for (int k = 0; k <= i; k++)
           {
             _V(k,j) -= g * _d[k];
           }
         }
       }
       for (int k = 0; k <= i; k++)
       {
         _V(k,i+1) = 0.0;
       }
     }
     for (int j = 0; j < _n; j++)
     {
       _d[j] = _V(_n-1,j);
       _V(_n-1,j) = 0.0;
     }
     _V(_n-1,_n-1) = 1.0;
     _e[0] = 0.0;
   } 

   /**
    * @brief Symmetric tridiagonal QL algorithm.
    *
    * This is derived from the Algol procedures tql2, by
    * Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
    * Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
    * Fortran subroutine in EISPACK.
    */
   void tql2 ()
   {
     for (int i = 1; i < _n; i++)
     {
       _e[i-1] = _e[i];
     }
     _e[_n-1] = 0.0;
   
     Real f = 0.0;
     Real tst1 = 0.0;
     Real eps = pow(2.0,-52.0);
     for (int l = 0; l < _n; l++)
     {
       // Find small subdiagonal element
   
       tst1 = std::max(tst1, NumTools::abs<Real>(_d[l]) + NumTools::abs<Real>(_e[l]));
       int m = l;

       // Original while-loop from Java code
       while (m < _n)
       {
         if (NumTools::abs<Real>(_e[m]) <= eps*tst1)
         {
           break;
         }
         m++;
       }

       // If m == l, _d[l] is an eigenvalue,
       // otherwise, iterate.
   
       if (m > l)
       {
         int iter = 0;
         do
         {
           iter = iter + 1;  // (Could check iteration count here.)
   
           // Compute implicit shift
   
           Real g = _d[l];
           Real p = (_d[l+1] - g) / (2.0 * _e[l]);
           Real r = hypot(p,1.0);
           if (p < 0)
           {
             r = -r;
           }
           _d[l] = _e[l] / (p + r);
           _d[l+1] = _e[l] * (p + r);
           Real dl1 = _d[l+1];
           Real h = g - _d[l];
           for (int i = l+2; i < _n; i++)
           {
             _d[i] -= h;
           }
           f = f + h;
   
           // Implicit QL transformation.
   
           p = _d[m];
           Real c = 1.0;
           Real c2 = c;
           Real c3 = c;
           Real el1 = _e[l+1];
           Real s = 0.0;
           Real s2 = 0.0;
           for (int i = m-1; i >= l; i--)
           {
             c3 = c2;
             c2 = c;
             s2 = s;
             g = c * _e[i];
             h = c * p;
             r = hypot(p,_e[i]);
             _e[i+1] = s * r;
             s = _e[i] / r;
             c = p / r;
             p = c * _d[i] - s * g;
             _d[i+1] = h + s * (c * g + s * _d[i]);
   
             // Accumulate transformation.
   
             for (int k = 0; k < _n; k++)
             {
               h = _V(k,i+1);
               _V(k,i+1) = s * _V(k,i) + c * h;
               _V(k,i) = c * _V(k,i) - s * h;
             }
           }
           p = -s * s2 * c3 * el1 * _e[l] / dl1;
           _e[l] = s * p;
           _d[l] = c * p;
   
           // Check for convergence.
   
         } while (NumTools::abs<Real>(_e[l]) > eps*tst1);
       }
       _d[l] = _d[l] + f;
       _e[l] = 0.0;
     }
     
     // Sort eigenvalues and corresponding vectors.
   
     for (int i = 0; i < _n-1; i++)
     {
       int k = i;
       Real p = _d[i];
       for (int j = i+1; j < _n; j++)
       {
         if (_d[j] < p)
         {
           k = j;
           p = _d[j];
         }
       }
       if (k != i)
       {
         _d[k] = _d[i];
         _d[i] = p;
         for (int j = 0; j < _n; j++)
         {
           p = _V(j,i);
           _V(j,i) = _V(j,k);
           _V(j,k) = p;
         }
       }
     }
   }

   /**
    * @brief Nonsymmetric reduction to Hessenberg form.
    *
    * This is derived from the Algol procedures orthes and ortran,
    * by Martin and Wilkinson, Handbook for Auto. Comp.,
    * Vol.ii-Linear Algebra, and the corresponding
    * Fortran subroutines in EISPACK.
    */
   void orthes ()
   {
     int low = 0;
     int high = _n-1;
   
     for (int m = low+1; m <= high-1; m++)
     {
       // Scale column.
   
       Real scale = 0.0;
       for (int i = m; i <= high; i++)
       {
         scale = scale + NumTools::abs<Real>(_H(i,m-1));
       }
       if (scale != 0.0)
       {
         // Compute Householder transformation.
   
         Real h = 0.0;
         for (int i = high; i >= m; i--)
         {
           ort_[i] = _H(i,m-1)/scale;
           h += ort_[i] * ort_[i];
         }
         Real g = sqrt(h);
         if (ort_[m] > 0)
         {
           g = -g;
         }
         h = h - ort_[m] * g;
         ort_[m] = ort_[m] - g;
   
         // Apply Householder similarity transformation
         // H = (I-u*u'/h)*H*(I-u*u')/h)
   
         for (int j = m; j < _n; j++)
         {
           Real f = 0.0;
           for (int i = high; i >= m; i--)
           {
             f += ort_[i]*_H(i,j);
           }
           f = f/h;
           for (int i = m; i <= high; i++)
           {
             _H(i,j) -= f*ort_[i];
           }
         }
   
         for (int i = 0; i <= high; i++)
         {
           Real f = 0.0;
           for (int j = high; j >= m; j--)
           {
             f += ort_[j]*_H(i,j);
           }
           f = f/h;
           for (int j = m; j <= high; j++)
           {
             _H(i,j) -= f*ort_[j];
           }
         }
         ort_[m] = scale*ort_[m];
         _H(m,m-1) = scale*g;
       }
     }
   
     // Accumulate transformations (Algol's ortran).

     for (int i = 0; i < _n; i++)
     {
       for (int j = 0; j < _n; j++)
       {
         _V(i,j) = (i == j ? 1.0 : 0.0);
       }
     }

     for (int m = high-1; m >= low+1; m--)
     {
       if (_H(m,m-1) != 0.0)
       {
         for (int i = m+1; i <= high; i++)
         {
           ort_[i] = _H(i,m-1);
         }
         for (int j = m; j <= high; j++)
         {
           Real g = 0.0;
           for (int i = m; i <= high; i++)
           {
             g += ort_[i] * _V(i,j);
           }
           // Double division avoids possible underflow
           g = (g / ort_[m]) / _H(m,m-1);
           for (int i = m; i <= high; i++)
           {
             _V(i,j) += g * ort_[i];
           }
         }
       }
     }
   }


   // Complex scalar division.

   Real cdivr, cdivi;
   void cdiv(Real xr, Real xi, Real yr, Real yi)
   {
     Real r,d;
     if (NumTools::abs<Real>(yr) > NumTools::abs<Real>(yi))
     {
       r = yi/yr;
       d = yr + r*yi;
       cdivr = (xr + r*xi)/d;
       cdivi = (xi - r*xr)/d;
     }
     else
     {
       r = yr/yi;
       d = yi + r*yr;
       cdivr = (r*xr + xi)/d;
       cdivi = (r*xi - xr)/d;
     }
   }


   // Nonsymmetric reduction from Hessenberg to real Schur form.

  void hqr2 ()
  {
    //  This is derived from the Algol procedure hqr2,
    //  by Martin and Wilkinson, Handbook for Auto. Comp.,
    //  Vol.ii-Linear Algebra, and the corresponding
    //  Fortran subroutine in EISPACK.
  
    // Initialize
   
    int nn = this->_n;
    int n = nn-1;
    int low = 0;
    int high = nn-1;
    Real eps = pow(2.0,-52.0);
    Real exshift = 0.0;
    Real p=0,q=0,r=0,s=0,z=0,t,w,x,y;
   
    // Store roots isolated by balanc and compute matrix norm
   
    Real norm = 0.0;
    for (int i = 0; i < nn; i++)
    {
      if ((i < low) || (i > high))
      {
        _d[i] = _H(i,i);
        _e[i] = 0.0;
      }
      for (int j = std::max(i-1,0); j < nn; j++)
      {
        norm = norm + NumTools::abs<Real>(_H(i,j));
      }
    }
   
    // Outer loop over eigenvalue index
  
    int iter = 0;
    while (n >= low)
    {
      // Look for single small sub-diagonal element
  
      int l = n;
      while (l > low)
      {
        s = NumTools::abs<Real>(_H(l-1,l-1)) + NumTools::abs<Real>(_H(l,l));
        if (s == 0.0)
        {
          s = norm;
        }
        if (NumTools::abs<Real>(_H(l,l-1)) < eps * s)
        {
          break;
        }
        l--;
      }
      
      // Check for convergence
      // One root found
  
      if (l == n)
      {
        _H(n,n) = _H(n,n) + exshift;
        _d[n] = _H(n,n);
        _e[n] = 0.0;
        n--;
        iter = 0;
  
        // Two roots found
  
      }
      else if (l == n-1)
      {
        w = _H(n,n-1) * _H(n-1,n);
        p = (_H(n-1,n-1) - _H(n,n)) / 2.0;
        q = p * p + w;
        z = sqrt(NumTools::abs<Real>(q));
        _H(n,n) = _H(n,n) + exshift;
        _H(n-1,n-1) = _H(n-1,n-1) + exshift;
        x = _H(n,n);
  
        // Real pair
  
        if (q >= 0)
        {
          if (p >= 0)
          {
            z = p + z;
          }
          else
          {
            z = p - z;
          }
          _d[n-1] = x + z;
          _d[n] = _d[n-1];
          if (z != 0.0)
          {
            _d[n] = x - w / z;
          }
          _e[n-1] = 0.0;
          _e[n] = 0.0;
          x = _H(n,n-1);
          s = NumTools::abs<Real>(x) + NumTools::abs<Real>(z);
          p = x / s;
          q = z / s;
          r = sqrt(p * p+q * q);
          p = p / r;
          q = q / r;
  
          // Row modification
  
          for (int j = n-1; j < nn; j++)
          {
            z = _H(n-1,j);
            _H(n-1,j) = q * z + p * _H(n,j);
            _H(n,j) = q * _H(n,j) - p * z;
          }
  
          // Column modification
  
          for (int i = 0; i <= n; i++)
          {
            z = _H(i,n-1);
            _H(i,n-1) = q * z + p * _H(i,n);
            _H(i,n) = q * _H(i,n) - p * z;
          }
  
          // Accumulate transformations
  
          for (int i = low; i <= high; i++)
          {
            z = _V(i,n-1);
            _V(i,n-1) = q * z + p * _V(i,n);
            _V(i,n) = q * _V(i,n) - p * z;
          }
   
          // Complex pair
  
        }
        else
        {
          _d[n-1] = x + p;
          _d[n] = x + p;
          _e[n-1] = z;
          _e[n] = -z;
        }
        n = n - 2;
        iter = 0;
  
        // No convergence yet
  
      }
      else
      {
        // Form shift
  
        x = _H(n,n);
        y = 0.0;
        w = 0.0;
        if (l < n)
        {
          y = _H(n-1,n-1);
          w = _H(n,n-1) * _H(n-1,n);
        }
  
        // Wilkinson's original ad hoc shift
  
        if (iter == 10)
        {
          exshift += x;
          for (int i = low; i <= n; i++)
          {
            _H(i,i) -= x;
          }
          s = NumTools::abs<Real>(_H(n,n-1)) + NumTools::abs<Real>(_H(n-1,n-2));
          x = y = 0.75 * s;
          w = -0.4375 * s * s;
        }

        // MATLAB's new ad hoc shift
        if (iter == 30)
        {
          s = (y - x) / 2.0;
          s = s * s + w;
          if (s > 0)
          {
            s = sqrt(s);
            if (y < x)
            {
              s = -s;
            }
            s = x - w / ((y - x) / 2.0 + s);
            for (int i = low; i <= n; i++)
            {
              _H(i,i) -= s;
            }
            exshift += s;
            x = y = w = 0.964;
          }
        }
  
        iter = iter + 1;   // (Could check iteration count here.)
  
        // Look for two consecutive small sub-diagonal elements
  
        int m = n-2;
        while (m >= l)
        {
          z = _H(m,m);
          r = x - z;
          s = y - z;
          p = (r * s - w) / _H(m+1,m) + _H(m,m+1);
          q = _H(m+1,m+1) - z - r - s;
          r = _H(m+2,m+1);
          s = NumTools::abs<Real>(p) + NumTools::abs<Real>(q) + NumTools::abs<Real>(r);
          p = p / s;
          q = q / s;
          r = r / s;
          if (m == l)
          {
            break;
          }
          if (NumTools::abs<Real>(_H(m,m-1)) * (NumTools::abs<Real>(q) + NumTools::abs<Real>(r)) <
               eps * (NumTools::abs<Real>(p) * (NumTools::abs<Real>(_H(m-1,m-1)) + NumTools::abs<Real>(z) +
               NumTools::abs<Real>(_H(m+1,m+1)))))
          {
            break;
          }
          m--;
        }
  
        for (int i = m+2; i <= n; i++)
        {
          _H(i,i-2) = 0.0;
          if (i > m+2)
          {
            _H(i,i-3) = 0.0;
          }
        }
 
        // Double QR step involving rows l:n and columns m:n
   
        for (int k = m; k <= n-1; k++)
        {
          int notlast = (k != n-1);
          if (k != m)
          {
            p = _H(k,k-1);
            q = _H(k+1,k-1);
            r = (notlast ? _H(k+2,k-1) : 0.0);
            x = NumTools::abs<Real>(p) + NumTools::abs<Real>(q) + NumTools::abs<Real>(r);
            if (x != 0.0)
            {
              p = p / x;
              q = q / x;
              r = r / x;
            }
          }
          if (x == 0.0)
          {
            break;
          }
          s = sqrt(p * p + q * q + r * r);
          if (p < 0)
          {
            s = -s;
          }
          if (s != 0)
          {
            if (k != m)
            {
              _H(k,k-1) = -s * x;
            }
            else if (l != m)
            {
              _H(k,k-1) = -_H(k,k-1);
            }
            p = p + s;
            x = p / s;
            y = q / s;
            z = r / s;
            q = q / p;
            r = r / p;
   
            // Row modification
   
            for (int j = k; j < nn; j++)
            {
              p = _H(k,j) + q * _H(k+1,j);
              if (notlast)
              {
                p = p + r * _H(k+2,j);
                _H(k+2,j) = _H(k+2,j) - p * z;
              }
              _H(k,j) = _H(k,j) - p * x;
              _H(k+1,j) = _H(k+1,j) - p * y;
            }
   
            // Column modification
   
            for (int i = 0; i <= std::min(n,k+3); i++)
            {
              p = x * _H(i,k) + y * _H(i,k+1);
              if (notlast)
              {
                p = p + z * _H(i,k+2);
                _H(i,k+2) = _H(i,k+2) - p * r;
              }
              _H(i,k) = _H(i,k) - p;
              _H(i,k+1) = _H(i,k+1) - p * q;
            }
   
            // Accumulate transformations
   
            for (int i = low; i <= high; i++)
            {
              p = x * _V(i,k) + y * _V(i,k+1);
              if (notlast)
              {
                p = p + z * _V(i,k+2);
                _V(i,k+2) = _V(i,k+2) - p * r;
              }
              _V(i,k) = _V(i,k) - p;
              _V(i,k+1) = _V(i,k+1) - p * q;
            }
          }  // (s != 0)
        }  // k loop
      }  // check convergence
    }  // while (n >= low)
      
    // Backsubstitute to find vectors of upper triangular form

    if (norm == 0.0)
    {
       return;
    }
   
    for (n = nn-1; n >= 0; n--)
    {
      p = _d[n];
      q = _e[n];
   
      // Real vector
   
      if (q == 0)
      {
        int l = n;
        _H(n,n) = 1.0;
        for (int i = n-1; i >= 0; i--)
        {
          w = _H(i,i) - p;
          r = 0.0;
          for (int j = l; j <= n; j++)
          {
            r = r + _H(i,j) * _H(j,n);
          }
          if (_e[i] < 0.0)
          {
            z = w;
            s = r;
          }
          else
          {
            l = i;
            if (_e[i] == 0.0)
            {
              if (w != 0.0)
              {
                _H(i,n) = -r / w;
              }
              else
              {
                _H(i,n) = -r / (eps * norm);
              }
   
              // Solve real equations
   
            }
            else
            {
              x = _H(i,i+1);
              y = _H(i+1,i);
              q = (_d[i] - p) * (_d[i] - p) + _e[i] * _e[i];
              t = (x * s - z * r) / q;
              _H(i,n) = t;
              if (NumTools::abs<Real>(x) > NumTools::abs<Real>(z))
              {
                _H(i+1,n) = (-r - w * t) / x;
              }
              else
              {
                _H(i+1,n) = (-s - y * t) / z;
              }
            }
   
            // Overflow control
   
            t = NumTools::abs<Real>(_H(i,n));
            if ((eps * t) * t > 1)
            {
              for (int j = i; j <= n; j++)
              {
                _H(j,n) = _H(j,n) / t;
              }
            }
          }
        }
   
        // Complex vector
   
      }
      else if (q < 0)
      {
        int l = n-1;

        // Last vector component imaginary so matrix is triangular
   
        if (NumTools::abs<Real>(_H(n,n-1)) > NumTools::abs<Real>(_H(n-1,n)))
        {
          _H(n-1,n-1) = q / _H(n,n-1);
          _H(n-1,n) = -(_H(n,n) - p) / _H(n,n-1);
        }
        else
        {
          cdiv(0.0,-_H(n-1,n),_H(n-1,n-1)-p,q);
          _H(n-1,n-1) = cdivr;
          _H(n-1,n) = cdivi;
        }
        _H(n,n-1) = 0.0;
        _H(n,n) = 1.0;
        for (int i = n-2; i >= 0; i--)
        {
          Real ra,sa,vr,vi;
          ra = 0.0;
          sa = 0.0;
          for (int j = l; j <= n; j++)
          {
            ra = ra + _H(i,j) * _H(j,n-1);
            sa = sa + _H(i,j) * _H(j,n);
          }
          w = _H(i,i) - p;
   
          if (_e[i] < 0.0)
          {
            z = w;
            r = ra;
            s = sa;
          }
          else
          {
            l = i;
            if (_e[i] == 0)
            {
              cdiv(-ra,-sa,w,q);
              _H(i,n-1) = cdivr;
              _H(i,n) = cdivi;
            }
            else
            {
              // Solve complex equations
 
              x = _H(i,i+1);
              y = _H(i+1,i);
              vr = (_d[i] - p) * (_d[i] - p) + _e[i] * _e[i] - q * q;
              vi = (_d[i] - p) * 2.0 * q;
              if ((vr == 0.0) && (vi == 0.0))
              {
                vr = eps * norm * (NumTools::abs<Real>(w) + NumTools::abs<Real>(q) +
                NumTools::abs<Real>(x) + NumTools::abs<Real>(y) + NumTools::abs<Real>(z));
              }
              cdiv(x*r-z*ra+q*sa,x*s-z*sa-q*ra,vr,vi);
              _H(i,n-1) = cdivr;
              _H(i,n) = cdivi;
              if (NumTools::abs<Real>(x) > (NumTools::abs<Real>(z) + NumTools::abs<Real>(q)))
              {
                _H(i+1,n-1) = (-ra - w * _H(i,n-1) + q * _H(i,n)) / x;
                _H(i+1,n) = (-sa - w * _H(i,n) - q * _H(i,n-1)) / x;
              }
              else
              {
                cdiv(-r-y*_H(i,n-1),-s-y*_H(i,n),z,q);
                _H(i+1,n-1) = cdivr;
                _H(i+1,n) = cdivi;
              }
            }
 
            // Overflow control
            t = std::max(NumTools::abs<Real>(_H(i,n-1)),NumTools::abs<Real>(_H(i,n)));
            if ((eps * t) * t > 1)
            {
              for (int j = i; j <= n; j++)
              {
                _H(j,n-1) = _H(j,n-1) / t;
                _H(j,n) = _H(j,n) / t;
              }
            }
          }
        }
      }
    }
   
    // Vectors of isolated roots
   
    for (int i = 0; i < nn; i++)
    {
      if (i < low || i > high)
      {
        for (int j = i; j < nn; j++)
        {
          _V(i,j) = _H(i,j);
        }
      }
    }
   
    // Back transformation to get eigenvectors of original matrix
   
    for (int j = nn-1; j >= low; j--)
    {
      for (int i = low; i <= high; i++)
      {
        z = 0.0;
        for (int k = low; k <= std::min(j,high); k++)
        {
          z = z + _V(i,k) * _H(k,j);
        }
        _V(i,j) = z;
      }
    }
  }

  public:


    /**
     * @brief Check for symmetry, then construct the eigenvalue decomposition
     *
     * @param A    Square real (non-complex) matrix
     */
    EigenValue(const Matrix<Real>& A) :
      _n(A.getNumberOfColumns()),
      _issymmetric(true),
      _d(_n),
      _e(_n),
      _V(_n,_n),
      _H(),
      _D(_n, _n),
      ort_(),
      cdivr(), cdivi()
    {
      for (int j = 0; (j < _n) && _issymmetric; j++)
      {
        for (int i = 0; (i < _n) && _issymmetric; i++)
        {
          _issymmetric = (A(i,j) == A(j,i));
        }
      }

      if (_issymmetric)
      {
        for (int i = 0; i < _n; i++)
        {
          for (int j = 0; j < _n; j++)
          {
            _V(i,j) = A(i,j);
          }
        }
   
        // Tridiagonalize.
        tred2();
   
        // Diagonalize.
        tql2();
      }
      else
      {
        _H.resize(_n,_n);
        ort_.resize(_n);
         
        for (int j = 0; j < _n; j++)
        {
          for (int i = 0; i < _n; i++)
          {
            _H(i,j) = A(i,j);
          }
        }
   
        // Reduce to Hessenberg form.
        orthes();
   
        // Reduce Hessenberg to real Schur form.
        hqr2();
      }
    }


    /**
     * @brief Return the eigenvector matrix
     *
     * @return V
     */
    const RowMatrix<Real>& getV() const { return _V; }

    /**
     * @brief Return the real parts of the eigenvalues
     *
     * @return real(diag(D))
     */
    const std::vector<Real>& getRealEigenValues() const { return _d; }

    /**
     * @brief Return the imaginary parts of the eigenvalues in parameter e.
     *
     * @return e: new matrix with imaginary parts of the eigenvalues.
     */
    const std::vector<Real>& getImagEigenValues() const { return _e; }
   
    /**
     * @brief Computes the block diagonal eigenvalue matrix.
     * 
     * If the original matrix A is not symmetric, then the eigenvalue 
     * matrix D is block diagonal with the real eigenvalues in 1-by-1 
     * blocks and any complex eigenvalues,
     * a + i*b, in 2-by-2 blocks, [a, b; -b, a].  That is, if the complex
     * eigenvalues look like
     * <pre>
     *
     *       u + iv     .        .          .      .    .
     *         .      u - iv     .          .      .    .
     *         .        .      a + ib       .      .    .
     *         .        .        .        a - ib   .    .
     *         .        .        .          .      x    .
     *         .        .        .          .      .    y
     * </pre>
     *     then D looks like
     * <pre>
     *
     *         u        v        .          .      .    .
     *        -v        u        .          .      .    . 
     *         .        .        a          b      .    .
     *         .        .       -b          a      .    .
     *         .        .        .          .      x    .
     *         .        .        .          .      .    y
     * </pre>
     * This keeps V a real matrix in both symmetric and non-symmetric
     * cases, and A*V = V*D.
     *
     * @return D: upon return, the matrix is filled with the block diagonal 
     * eigenvalue matrix.
     */
    const RowMatrix<Real>& getD() const
    {
      for (int i = 0; i < _n; i++)
      {
        for (int j = 0; j < _n; j++)
        {
          _D(i,j) = 0.0;
        }
        _D(i,i) = _d[i];
        if (_e[i] > 0)
        {
          _D(i,i+1) = _e[i];
        }
        else if (_e[i] < 0)
        {
          _D(i,i-1) = _e[i];
        }
      }
      return _D;
    }
};

} //end of namespace bpp.

#endif //_EIGENVALUE_H_


//
// File: TruncatedExponentialDiscreteDistribution.h
// Created by: Julien Dutheil
// Created on: Fri Jan 25 15:24 2008
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

#ifndef _TRUNCATEDEXPONENTIALDISCRETEDISTRIBUTION_H_
#define _TRUNCATEDEXPONENTIALDISCRETEDISTRIBUTION_H_

#include "AbstractDiscreteDistribution.h"
#include "../Constraints.h"
#include "../RandomTools.h"

namespace bpp
{

  /**
   * @brief Discretized Truncated (on the right) Exponential distribution.
   *
   * This distribution has two parameters: the traditional exponential law parameter,
   * and the abscissa of the truncation. The distribution will be truncated on the right 
   * of this point.
   */
  class TruncatedExponentialDiscreteDistribution:
    public AbstractDiscreteDistribution
  {
  protected:
    std::vector<double> bounds_;
    bool useMedian_;

  public:
    /**
     * @brief Build a new truncated exponential discrete distribution.
     * @param n the number of categories to use.
     * @param lambda The lambda parameter
     * @param truncationPoint The trucation point
     * @param useMedian Tells if median should be used as a value for each category instead of the mean.
     *
     * The Parameters are: lambda @f$ \in [0.000001;\infty[ @f$ and tp .@f$ \in [0;\infty[ @f$
     *
     */

    TruncatedExponentialDiscreteDistribution(unsigned int n, double lambda = 1., double truncationPoint = 10., bool useMedian = false);

    TruncatedExponentialDiscreteDistribution(const TruncatedExponentialDiscreteDistribution& dist):
      AbstractDiscreteDistribution(dist),
      bounds_(dist.bounds_), useMedian_(dist.useMedian_) {}
    
    TruncatedExponentialDiscreteDistribution& operator=(const TruncatedExponentialDiscreteDistribution& dist)
    {
      AbstractDiscreteDistribution::operator=(dist);
      bounds_ = dist.bounds_;
      return *this;
    }

    virtual ~TruncatedExponentialDiscreteDistribution();

    TruncatedExponentialDiscreteDistribution* clone() const { return new TruncatedExponentialDiscreteDistribution(*this); }
	
  public:
    Domain getDomain() const;
    void fireParameterChanged(const ParameterList & parameters);
	
  public:
    double randC() const throw (Exception)
    {
      double lambda = getParameter_(0).getValue();
      double limit = getParameter_(1).getValue();
      double value = limit + 1;
      while(value >= limit)
        value = RandomTools::randExponential(lambda);
      return value;
    }
    
    double getLowerBound() const{
      return getParameterValue("tp");
    }

    /**
     * @brief Checks if the Parameters can respect the given
     * Constraint and optionnaly tries to modify their Constraints.
     *
     * @param c The Constraint to respect.
     * @param f boolean flag to say if the Constraints must be changed
     * (if possible) (default: true)
     *
     * @return true iff the Constraint is the interval @f$ [x;\infty[
     * @f$ with @f$ x >= 0 @f$ and , and the Parameter tp value accept
     * the new Constraint, if needed.
     *
     * If true, the constraint of Parameters tp is restricted to @f$ \in
     * [x;\infty[ @f$.
     */
  
    bool adaptToConstraint(const Constraint& c, bool f=true);

  protected:
    void applyParameters(unsigned int numberOfCategories);
    void discretize(unsigned int nbClasses, double lambda, double tp, bool median);

    

  };

} //end of namespace bpp.

#endif	//_TRUNCATEDEXPONENTIALDISCRETEDISTRIBUTION_H_


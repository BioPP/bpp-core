//
// File: ExponentialDiscreteDistribution.h
// Created by: Julien Dutheil
// Created on: Tue Nov 13 12:37 2007
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

#ifndef _EXPONENTIALDISCRETEDISTRIBUTION_H_
#define _EXPONENTIALDISCRETEDISTRIBUTION_H_

#include "AbstractDiscreteDistribution.h"
#include "../Constraints.h"
#include "../RandomTools.h"

namespace bpp
{
  
  /**
   * @brief Discretized Exponential distribution.
   *
   * @author Julien Dutheil
   */
  class ExponentialDiscreteDistribution:
    public AbstractDiscreteDistribution
  {
  protected:
    std::vector<double> bounds_;
    IncludingPositiveReal* lambdaConstraint_;
    bool median_;
    
  public:
    /**
     * @brief Build a new discretized exponential distribution.
     * @param n the number of categories to use.
     * @param lambda The lambda parameter.
     * @param median Tells if median should be used as a value for each category instead of the mean.
     *
     * The Parameter is: lambda @f$ \in [0;\infty[ @f$.
     *
     */
    
    ExponentialDiscreteDistribution(unsigned int n, double lambda = 1., bool median=false);
    
    ExponentialDiscreteDistribution(const ExponentialDiscreteDistribution & dist):
      //AbstractParametrizable(dist),
      AbstractDiscreteDistribution(dist),
      bounds_(dist.bounds_),
      lambdaConstraint_(dynamic_cast<IncludingPositiveReal *>(dist.lambdaConstraint_->clone())),
      median_(dist.median_)
    {}
  
    ExponentialDiscreteDistribution& operator=(const ExponentialDiscreteDistribution& dist)
    {
      AbstractDiscreteDistribution::operator=(dist);
      bounds_ = dist.bounds_;
      lambdaConstraint_ = dynamic_cast<IncludingPositiveReal *>(dist.lambdaConstraint_->clone());
      median_ = dist.median_;
      return *this;
    }
  
    virtual ~ExponentialDiscreteDistribution();
  
    ExponentialDiscreteDistribution* clone() const { return new ExponentialDiscreteDistribution(*this); }
  
  public:
    Domain getDomain() const;
    void fireParameterChanged(const ParameterList& parameters);
  
  public:
    double randC() const throw (Exception)
    {
      return RandomTools::randExponential(getParameterValue("lambda"));
    }

    double getLowerBound() const {
      return 0;
    }

    /**
     * @brief Checks if the Parameters can respect the given
     * Constraint and optionnaly tries to modify their Constraints.
     *
     * @param c The Constraint to respect.
     * @param f boolean flag to say if the Constraints must be changed
     * (if possible) (default: true)
     *
     * @return true iff the Constraint is the interval @f$ [0;\infty[
     * @f$.
     *
     */
  
    bool adaptToConstraint(const Constraint& c, bool f=true);


  protected:
    void applyParameters(unsigned int numberOfCategories);
    void discretize(unsigned int nbClasses, double lambda, bool median);
  
  };
  
} //end of namespace bpp.

#endif	//_EXPONENTIALDISCRETEDISTRIBUTION_H_


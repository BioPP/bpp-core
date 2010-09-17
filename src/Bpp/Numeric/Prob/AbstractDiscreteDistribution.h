//
// File: AbstractDiscreteDistribution.h
// Created by: Julien Dutheil
// Created on: ?
//

/*
  Copyright or © or Copr. CNRS, (November 19, 2004)

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

#ifndef _ABSTRACTDISCRETEDISTRIBUTION_H_
#define _ABSTRACTDISCRETEDISTRIBUTION_H_

#include "DiscreteDistribution.h"
#include "../AbstractParameterAliasable.h"

#include <map>

namespace bpp
{

  /**
   * @brief Partial implementation of the DiscreteDistribution interface.
   *
   * This class uses a map to store the cateogry values as keys and probabilities as values.
   * It uses its own comparator class to deal with double precision.
   * By default, category values that differ less than 10E-9 will be considered identical.
   */
  class AbstractDiscreteDistribution:
    public DiscreteDistribution,
    public AbstractParameterAliasable
  {

  public:
    
    /**
     * @brief Comparator class for AbstractDiscreteDistribution.
     */
    class Order
    {
    public:
      bool operator() (double l1, double l2) const
      {
        return (l1 < l2 - 0.000000001); //precision of E9
      }
    };

  protected:
    /**
     * This field must be initialized in the constructor of the derived classes.
     */
    std::map<double, double, Order> distribution_;
  
  public:
    AbstractDiscreteDistribution(const std::string& prefix = "") :
      AbstractParameterAliasable(prefix), distribution_() {}

    virtual ~AbstractDiscreteDistribution() {}

  public:

    /**
     * @name The DiscreteDistribution interface.
     *
     * @{
     */
    unsigned int getNumberOfCategories() const;
    double getCategory(unsigned int categoryIndex) const;
    double getProbability(unsigned int categoryIndex) const;
    double getProbability(double category) const;
    Vdouble getCategories() const;
    Vdouble getProbabilities() const;
    double getValueCategory(double value) const throw (OutOfRangeException);
    void set(double category, double probability);
    void add(double category, double probability);
    double  getInfCumulativeProbability(double category) const;
    double getIInfCumulativeProbability(double category) const;
    double  getSupCumulativeProbability(double category) const;
    double getSSupCumulativeProbability(double category) const;
    double rand() const;
    double randC() const throw (Exception) { throw Exception("AbstractDiscreteDistribution::randC. No continuous version available for this distribution."); }
    double getLowerBound() const;
    double getUpperBound() const;

    /**
     * @brief Checks if the Parameters can respect the given
     * Constraint and optionnaly tries to modify their Constraints.
     *
     * @param c The Constraint to respect.
     * @param f boolean flag to say if the Constraints must be changed
     * (if possible) (default: true)
     *
     * @return true if the Parameters Constraints are adapted to the
     * given Constraints, false otherwise.
     */
    bool adaptToConstraint(const Constraint& c, bool f=true);
      
    void print(OutputStream& out) const;

    /** @} */

  };

} //end of namespace bpp.

#endif  //_ABSTRACTDISCRETEDISTRIBUTION_H_


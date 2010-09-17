//
// File: BetaDiscreteDistribution.h
// Created by: Laurent Guéguen
// Created on: jeudi 27 mai 2010, à 23h 34
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

#ifndef _BETADISCRETEDISTRIBUTION_H_
#define _BETADISCRETEDISTRIBUTION_H_

#include "AbstractDiscreteDistribution.h"
#include "../Constraints.h"
#include "../RandomTools.h"

namespace bpp
{

  /**
   * @brief Discretized Beta distribution with parameters alpha and
   * beta.
   *
   * The minimum (resp maximum) value of this distribution is set to
   * 1e-12 (resp 1-1e-12) if alpha>1 (resp beta>1), otherwise it is 0
   * (resp 1).
   *
   * The Parameters are: alpha and beta @f$ \in [0.0001;\infty[ @f$.
   * @author Laurent Guéguen
   */
  
  class BetaDiscreteDistribution:
    public AbstractDiscreteDistribution
  {
  private:
    std::vector<double> bounds_;
  
  public:
    /**
     * @brief Build a new discretized beta distribution.
     *
     * @param n the number of categories to use.
     * @param alpha The alpha parameter.
     * @param beta The beta parameter.
     *
     */
    
    BetaDiscreteDistribution(unsigned int n, double alpha = 1, double beta = 1);

    virtual ~BetaDiscreteDistribution();

    BetaDiscreteDistribution* clone() const { return new BetaDiscreteDistribution(*this); }
  
  public:
    Domain getDomain() const;
    void fireParameterChanged(const ParameterList & parameters);
  
    double randC() const throw (Exception)
    {
      return RandomTools::randBeta(getParameterValue("alpha"),
                                   getParameterValue("beta"));
    }

    double getLowerBound() const {
      return (getParameterValue("alpha")<=1?0:NumConstants::VERY_TINY);
    }

    double getUpperBound() const {
      return (getParameterValue("beta")<=1?1:1- NumConstants::VERY_TINY);
    }
    /**
     * @brief Checks if the Parameters can respect the given
     * Constraint and optionnaly tries to modify their Constraints.
     *
     * @param c The Constraint to respect.
     * @param f boolean flag to say if the Constraints must be changed
     * (if possible) (default: true)
     *
     * If the Constraint has a strict boundary in 0 (resp 1), the
     *  Parameter alpha (resp beta) is restricted to @f$ \in
     *  ]1;\infty[ @f$, otherwise it is not changed.

     * @return true if the Constraint contains the interval @f$
     * ]0,1[ @f$ and optionnaly the Parameters Constraint change
     * is valid (otherwise the Parameters constraints are not
     * changed).
     *
     */
  
    bool adaptToConstraint(const Constraint& c, bool f=true);

  protected:
    void applyParameters(unsigned int numberOfCategories);
    void discretize(unsigned int nbClasses, double alpha, double beta, bool median);
  };

} //end of namespace bpp.

#endif  //_BETADISCRETEDISTRIBUTION_H_


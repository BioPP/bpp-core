//
// File: GammaDiscreteDistribution.h
// Created by: Julien Dutheil
// Created on: Sun Oct 26 20:36:12 2003
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

#ifndef _GAMMADISCRETEDISTRIBUTION_H_
#define _GAMMADISCRETEDISTRIBUTION_H_

#include "AbstractDiscreteDistribution.h"
#include "../Constraints.h"
#include "../RandomTools.h"

namespace bpp
{

/**
 * @brief Discretized Gamma distribution.
 *
 * @author Julien Dutheil, David Fournier, with original code from Tal Pupko and Ziheng Yang.
 */
class GammaDiscreteDistribution:
  public AbstractDiscreteDistribution
{
  private:
    std::vector<double> bounds_;
  
  public:
    /**
     * @brief Build a new discretized gamma distribution.
     * @param n the number of categories to use.
     * @param alpha The alpha parameter (shape)
     * @param beta The beta parameter (rate)
     * @param minimumAlpha the minimum possible value for alpha
     * @param minimumBeta the minimum possible value for beta
     *
     * The Parameters are: alpha and beta @f$ \in [0.0001;\infty[ @f$.
     *
     * If @f$ alpha > 1 @f$, the minimum value of the distribution is
     * set to 1e-12, otherwise it is 0.
     */
    GammaDiscreteDistribution(unsigned int n, double alpha = 1., double beta = 1., double minimumAlpha = 0.05, double minimumBeta = 0.05);

    virtual ~GammaDiscreteDistribution();

    GammaDiscreteDistribution* clone() const { return new GammaDiscreteDistribution(*this); }
  
  public:
    Domain getDomain() const;
    void fireParameterChanged(const ParameterList & parameters);
  
  double randC() const throw (Exception)
    {
      return RandomTools::randGamma(getParameterValue("alpha"),getParameterValue("beta"));
    }

  double getLowerBound() const {
    if (getParameterValue("alpha")>=1)
      return NumConstants::VERY_TINY;
    else
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
   *
   * @return true if the Constraint constrains @f$ ]0;\infty[ @f$, and
   * the Parameters values accept the new Constraint, if needed.
   *
   * If the Constraint excludes 0, the constraint of Parameters
   * alpha is restricted to @f$ \in [0.05;1[ @f$, otherwise
   * it is not changed.
   *
   */
  
  bool adaptToConstraint(const Constraint& c, bool f=true);

  protected:
    void applyParameters(unsigned int numberOfCategories);
    static std::vector<double> computeBounds(unsigned int nbClasses, double alpha, double beta);
    static std::vector<double> computeValues(unsigned int nbClasses, double alpha, double beta, bool median);
    
};

} //end of namespace bpp.

#endif  //_GAMMADISCRETEDISTRIBUTION_H_


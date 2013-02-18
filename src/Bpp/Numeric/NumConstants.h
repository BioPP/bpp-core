//
// File: NumConstants.h
// Created by: Julien Dutheil
// Created on: Tue Feb 03 14:21 2009
//

/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#ifndef _NUMCONSTANTS_H_
#define _NUMCONSTANTS_H_

#include <cmath>
#include <limits>

namespace bpp {

  /**
   * @brief this static class contains several useful constant values.
   */
  namespace NumConstants
  {

    /**
     * @name Golden ratio.
     *
     * The golden ratio, @f$\phi@f$ is equal to @f$\frac{1+\sqrt{5}}{2} = 1.6180339887498948482\ldots@f$.
     * We also define @f$R=\phi-1@f$ and @f$C = 1 - R@f$.
     * @{
     */
    const double GOLDEN_RATIO_PHI = (1. + sqrt(5.)) / 2.;
    const double GOLDEN_RATIO_R = NumConstants::GOLDEN_RATIO_PHI - 1.;
    const double GOLDEN_RATIO_C = 1. - NumConstants::GOLDEN_RATIO_R;

    /** @} */

    const double MEGA = 1e6;
    const double KILO = 1e3;
    const double DECI = 1e-1;
    const double CENTI = 1e-2;
    const double MILLI = 1e-3;
    const double MICRO = 1e-6;
    const double NANO = 1e-9;
    const double PICO = 1e-12;

    const double SMALL = 1e-6;
    const double TINY =  1e-12;
    const double VERY_TINY = 1e-20;
    const double VERY_BIG = static_cast<double>(1.7E+23);
    
    /**
     * @name Define those constants in case they would not be available in stl/limits.
     *
     * @{
     */
    //NB: numeric_limits<double>::infinity can throw an exception, so cannot be used directly here.
    extern "C" const double INF = std::numeric_limits<double>::has_infinity ? -log(0) : std::numeric_limits<double>::max();
    extern "C" const double PINF = std::numeric_limits<double>::has_infinity ? -log(0) : std::numeric_limits<double>::max();
    extern "C" const double MINF = std::numeric_limits<double>::has_infinity ? log(0) : std::numeric_limits<double>::min();
    extern "C" const double NaN = 3./0.;
    /** @} */

    const double PI = 3.141593;
  } //end of namespace NumConstants.

}//end of namespace bpp.

#endif	//_NUMCONSTANTS_H_


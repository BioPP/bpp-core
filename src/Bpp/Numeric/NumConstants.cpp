//
// File: Constants.cpp
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

#include "NumConstants.h"

using namespace bpp;

#include <cmath>
#include <limits>

using namespace std;

const double NumConstants::GOLDEN_RATIO_PHI = (1. + sqrt(5.)) / 2.; 
const double NumConstants::GOLDEN_RATIO_R   = NumConstants::GOLDEN_RATIO_PHI - 1.;
const double NumConstants::GOLDEN_RATIO_C   = 1. - NumConstants::GOLDEN_RATIO_R;


const double NumConstants::MEGA = 1e6;
const double NumConstants::KILO = 1e3;
const double NumConstants::DECI = 1e-1;
const double NumConstants::CENTI = 1e-2;
const double NumConstants::MILLI = 1e-3;
const double NumConstants::MICRO = 1e-6;
const double NumConstants::NANO = 1e-9;
const double NumConstants::PICO = 1e-12;

const double NumConstants::SMALL = 1e-6;
const double NumConstants::TINY =  1e-12;
const double NumConstants::VERY_TINY = 1e-20;
const double NumConstants::VERY_BIG = static_cast<double>(1.7E+23);
const double NumConstants::INF = numeric_limits<double>::infinity();

const double NumConstants::PI = 3.141593;


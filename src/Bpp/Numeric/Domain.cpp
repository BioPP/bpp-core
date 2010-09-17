//
// File: Domain.cpp
// Created by: Julien Dutheil
// Created on: Wed Feb  4 19:12:44 2004
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

#include "Domain.h"
#include "../Text/TextTools.h"

using namespace bpp;
using namespace std;

Domain::Domain(double a, double b, unsigned int n) throw (Exception) :
  bounds_(n + 1), midPoints_(n)
{
  if (n == 0) throw Exception("Domain::constructor1. Number of classes should be > 0.");
  double mini = min(a, b);
  double maxi = max(a, b);
  double w = (maxi - mini) / n;
  bounds_[0] = mini;
  for (unsigned int i = 1; i < n + 1; i++)
  {
    bounds_[i] = mini + i * w;
    midPoints_[i - 1] = mini + (i - .5) * w;
  }
}

Domain::Domain(const Vdouble& bounds) throw (Exception): bounds_(bounds), midPoints_(bounds.size() - 1)
{
  unsigned int n = bounds_.size();
  for (unsigned int i = 0; i < n - 1; i++)
  {
    if (bounds[i+1] < bounds[i]) throw Exception(
                   "Bound " + TextTools::toString(i+1) + " (" + TextTools::toString(bounds[i+1]) +
         ") is < to bound " + TextTools::toString(i)   + " (" + TextTools::toString(bounds[i]) + ").");
    midPoints_[i] = (bounds_[i] + bounds_[i+1]) / 2.;
  }
}

Domain::Domain(const Vdouble& bounds, const Vdouble& midPoints) throw (Exception): bounds_(bounds), midPoints_(midPoints)
{
  if (bounds.size() != midPoints.size() + 1) throw Exception("Domain::Domain(). Number of midpoints must equal number of bounds - 1.");
  
  unsigned int n = bounds_.size();
  for (unsigned int i = 0; i < n-1; i++)
  {
    if (bounds[i+1] < bounds[i]) throw Exception(
                   "Bound " + TextTools::toString(i+1) + " (" + TextTools::toString(bounds[i+1]) +
         ") is < to bound " + TextTools::toString(i)   + " (" + TextTools::toString(bounds[i]) + ").");
  }
  // Check if midPoint are really midPoints ;-)
  for (unsigned int i = 0; i < midPoints_.size(); i++)
  {
    if (! ((bounds_[i] == bounds_[i+1] && midPoints_[i] == bounds_[i])
       || (midPoints_[i] >= bounds_[i] && midPoints_[i] < bounds_[i+1])))
        throw Exception("Domain::Domain(). Midpoint " +
            TextTools::toString((int)i) +
            " = " +
            TextTools::toString(midPoints_[i]) +
            " does not belong to interval [" +
            TextTools::toString(bounds_[i]) +
            ", " +
            TextTools::toString(bounds_[i+1]) +
            "[."
        );
  }
}

double Domain::getNearestValue(double x) const throw (OutOfRangeException) 
{
  if(x < getLowerBound() || x >= getUpperBound())
    throw OutOfRangeException("Domain::getNearestValue", x, getLowerBound(), getUpperBound());
  for(unsigned int i = 1; i < bounds_.size(); i++)
    if(x < bounds_[i])
      return midPoints_[i - 1];
  // This line can't be reached:
  return 0;
}

unsigned int Domain::getIndex(double x) const throw (OutOfRangeException)
{
  if(x < getLowerBound() || x >= getUpperBound())
    throw OutOfRangeException("Domain::getIndex", x, getLowerBound(), getUpperBound());
  for(unsigned int i = 1; i < bounds_.size(); i++)
    if(x < bounds_[i])
      return i - 1;
  // This line can't be reached:
  throw Exception("Unexpected error!");
}


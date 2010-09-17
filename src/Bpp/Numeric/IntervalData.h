//
// File: IntervalData.h
// Created by: Julien Dutheil
// Created on: Wed Feb  4 19:59:29 2004
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

#ifndef _INTERVALDATA_H_
#define _INTERVALDATA_H_

//#include "VectorTools.h"
#include "Domain.h"

// from the STL:
#include <iostream>

namespace bpp
{

/**
 * @brief Interval data storage.
 *
 * This class uses a Domain object for bounds and midpoints storage.
 * the number of points in each interval/class is also stored.
 *
 * Several statistical computations can be performed.
 *
 * Adapted from the Java PSOL library.
 */ 
class IntervalData
{
	private:
		const Domain _domain;
    std::vector<unsigned int> _freqs;
    std::string _name;
		double _sum, _sumsquare, _min, _max;
		unsigned int _n;
	
	public:
		IntervalData(const Domain& domain, const std::string& name = "");
	
		virtual ~IntervalData() {}
			
	public:
		
		/**
		 * @return The domain associated to this data.
		 */
		virtual const Domain& getDomain() const { return _domain; }

		/**
		 * @return The value of the class the given value belongs to.
		 */
		virtual double getDomainValue(double x) const{ return _domain.getNearestValue(x); }

		/**
		 * @brief Set the name of this data.
		 * 
		 * @param name A name.
		 */
		virtual void setName(const std::string& name) { _name = name; }
		/**
		 * @return The name of this data.
		 */
		virtual const std::string& getName() const { return _name; }

		/**
		 * @return The number of elements in interval @c i.
		 * @param i The index of the interval.
		 */
		virtual unsigned int getFreq(unsigned int i) const { return _freqs[i]; }
		
		/**
		 * @return The proportion of elements in interval @c i.
		 * @param i the index of the interval.
		 */
		virtual double getDensity(unsigned int i) const{ return (double)_freqs[i] / (double)_n; }
		
		/**
		 * @return A vector with all element counts.
		 */
		virtual const std::vector<unsigned int>& getFrequencies() const { return _freqs; }

		/**
		 * @return A vector with all element proportions.
		 */
		virtual Vdouble getDensities() const;

		/**
		 * @brief Add a value to this data set.
		 *
		 * Check for the interval this value belongs to, and update inner data (counts, sums, etc.).
		 *
		 * @param value The value to add.
		 * @throw OutOfRangeException If the value does not belong to the domain of this data set.
		 */
		virtual void addValue(double value) throw (OutOfRangeException);

		/**
		 * @return The total number of elements in this data set.
		 */
		virtual unsigned int getSize() const { return _n; }

		/**
		 * @return The minimum value in the dataset.
		 */
		virtual double getMinValue() const { return _min; }
		
		/**
		 * @return The maximum value in the dataset.
		 */
		virtual double getMaxValue() const { return _max; }
	
		/**
		 * @return The true mean of the dataset.
		 */
		virtual double getMean() const { return _sum / _n; }
		
		/**
		 * @return The estimation of the standard deviation of the data set
		 * (that is, the sum is divided by @f$n-1@f$ where @f$n@f$ is the number of points).
		 */
		virtual double getSD() const { return (_n / (_n - 1)) * getSDP(); }
	
		/**
		 * @return The standard deviation of the data set
		 * (that is, the sum is divided by the number of points @f$n@f$ rather than @f$n-1@f$).
		 */
		virtual double getSDP() const { return _sumsquare / _n - _sum * _sum / (_n * _n); }
		
		/**
		 * @brief Reset the container.
		 *
		 * Remove all data and reinitialize all values (counts, sum, etc.).
		 */
		virtual void reset();
		
		/**
		 * @brief Print a summary of this data.
		 *
		 * @param out The stream where to print the summary.
		 */
		virtual void print(std::ostream & out) const;

};

} //end of namespace bpp.

#endif	//_INTERVALDATA_H_


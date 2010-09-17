//
// File: IntervalData.cpp
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

#include "IntervalData.h"
using namespace bpp;

#include <cmath>
using namespace std;

IntervalData::IntervalData(const Domain& domain, const std::string& name):
	_domain(domain), _freqs(), _name(name), _sum(0), _sumsquare(0), _min(0), _max(0), _n(0)
{
	reset();
}

Vdouble IntervalData::getDensities() const
{
	Vdouble densities(_freqs.size());
	for(unsigned int i = 0; i < _freqs.size(); i++) densities[i] = getDensity(i);
	return densities;
}

void IntervalData::addValue(double value) throw (OutOfRangeException)
{
	//_data.push_back(value);
	_n++;
	_sum += value;
	_sumsquare += value * value;
	if(value < _min) _min = value;
	if(value > _max) _max = value;
	int index = _domain.getIndex(value);
	_freqs[index]++;
}

void IntervalData::reset()
{
	_freqs.resize(_domain.getSize());
	_sum = 0;
	_sumsquare = 0;
	_min = -log(0.);
	_max = log(0.);
	_n = 0;
}

void IntervalData::print(ostream & out) const
{
	out << "midpoint\tlowerB\tupperB\tfreq\tdensity" << endl;
	for(unsigned int i = 0; i < _domain.getSize(); i++) {
		out << _domain.getValue(i);
		out << "\t";
		out << _domain.getBound(i);
		out << "\t";
		out << _domain.getBound(i + 1);
		out << "\t";
		out << _freqs[i];
		out << "\t";
		out << getDensity(i);
		out << endl;
	}
}


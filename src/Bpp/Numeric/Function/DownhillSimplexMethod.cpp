//
// File: DownhillSimplexMethod.cpp
// Created by: Julien Dutheil
// Created on: Tue Nov  4 17:10:05 2003
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

#include "DownhillSimplexMethod.h"
#include "../NumTools.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

bool DownhillSimplexMethod::DSMStopCondition::isToleranceReached() const
{
	const DownhillSimplexMethod* dsm = dynamic_cast<const DownhillSimplexMethod *>(optimizer_);
	double rTol = 2.0 * NumTools::abs(dsm->_y[dsm->_iHighest] - dsm->_y[dsm->_iLowest]) /
		(NumTools::abs(dsm->_y[dsm->_iHighest]) + NumTools::abs(dsm->_y[dsm->_iLowest]));
	return rTol < tolerance_;
}
	
/******************************************************************************/
			
DownhillSimplexMethod::DownhillSimplexMethod(Function* function):
  AbstractOptimizer(function), _simplex(), _y(), _pSum(), _iHighest(0), _iNextHighest(0), _iLowest(0)
{
	// Default values:
	nbEvalMax_ = 5000;
	setDefaultStopCondition_(new DSMStopCondition(this));
	setStopCondition(*getDefaultStopCondition());
}

/******************************************************************************/

void DownhillSimplexMethod::doInit(const ParameterList& params) throw (Exception)
{
	unsigned int nDim = getParameters().size();
	nbEval_ = 0;

	// Initialize the simplex:
	_simplex.resize(nDim + 1);
	_y.resize(nDim + 1);
	double lambda = 0.2; //20% of the parameter value.
  for(unsigned int i = 1; i < nDim + 1; i++)
  {
		// Copy the vector...
		_simplex[i] = getParameters();
		// ... and set the initial values.
		for(unsigned int j = 0; j < nDim; j++)
    {
      //Hummm... that does not work when the first point is 0!!! where does this come from???
			//_simplex[i][j].setValue(getParameters()[j].getValue() * (1. + (j == i - 1 ? lambda : 0.)));
			_simplex[i][j].setValue(getParameters()[j].getValue() + (j == i - 1 ? lambda : 0.));
    }
		//Compute the corresponding f value:
		_y[i] = getFunction()->f(_simplex[i]);
    nbEval_++;
	}
  //Last function evaluation, setting current value:
	_simplex[0] = getParameters();
	_y[0] = getFunction()->f(_simplex[0]);
  nbEval_++;
	
	_pSum = getPSum();
}
	
/******************************************************************************/

double DownhillSimplexMethod::doStep() throw (Exception)
{
	// The number of dimensions of the parameter space:
	unsigned int nDim = _simplex.getDimension();
	unsigned int mpts = nDim + 1;

	_iLowest = 0;
	// First we must determine which point is the highest (worst),
	// next-highest, and lowest (best), by looping over the points
	// in the simplex.
	if(_y[0] > _y[1])
  {
		_iHighest = 0;
		_iNextHighest = 1;
	}
  else
  {
		_iHighest = 1;
		_iNextHighest = 0;
	}
	
	for(unsigned int i = 0; i < mpts; i++)
  {
		if (_y[i] <= _y[_iLowest]) _iLowest = i;
		if (_y[i] > _y[_iHighest])
    {
			_iNextHighest = _iHighest;
			_iHighest = i;
		}
    else if(_y[i] > _y[_iNextHighest] && i != _iHighest) _iNextHighest = i;
	}
		
  // Set current best point:
	getParameters_() = _simplex[_iLowest];
		
	// Begin a new iteration.
	// First extrapolate by a factor -1 through the face of the simplex
	// across from high point, i.e., reflect the simplex from the high point.</p>

	double yTry = tryExtrapolation(-1.0);
	if(yTry <= _y[_iLowest])
  {
		// Expansion.
		yTry = tryExtrapolation(2.0);
	}
  else if(yTry >= _y[_iNextHighest])
  {
		// Contraction.
		double ySave = _y[_iHighest];
		yTry = tryExtrapolation(0.5);
		if(yTry >= ySave)
    {
			for(unsigned int i = 0; i < mpts; i++)
      {
				if(i != _iLowest)
        {
					for(unsigned int j = 0; j < nDim; j++)
          {
						_pSum[j].setValue(0.5 * (_simplex[i][j].getValue() + _simplex[_iLowest][j].getValue()));
						_simplex[i][j].setValue(_pSum[j].getValue());
					}
					_y[i] = getFunction()->f(_pSum);
	        nbEval_++;
				}
			}
			nbEval_ += nDim;
			_pSum = getPSum();
		}
	}

	return _y[_iLowest];
}

/******************************************************************************/

double DownhillSimplexMethod::optimize() throw (Exception)
{
  AbstractOptimizer::optimize();

	// set best shot:
	return getFunction()->f(_simplex[_iLowest]);
}

/******************************************************************************/

ParameterList DownhillSimplexMethod::getPSum()
{
	int ndim = _simplex.getDimension();
	int mpts = ndim + 1;
	
	// Get a copy...
	ParameterList pSum = getParameters();
	// ... and initializes it.
	for (int j = 0; j < ndim; j++)
  {
		double sum = 0.;
		for (int i = 0; i < mpts; i++)
    {
			sum += _simplex[i][j].getValue();
		}
		pSum[j].setValue(sum);
	}
	return pSum;
}

/******************************************************************************/

double DownhillSimplexMethod::tryExtrapolation(double fac)
{
	int ndim = _simplex.getDimension();
	double fac1, fac2, yTry;

	fac1 = (1.0 - fac) / ndim;
	fac2 = fac1 - fac;
	
	// Get a copy...
	ParameterList pTry = getParameters();
	// and initialize it:
	for (int j = 0; j < ndim; j++)
  {
		pTry[j].setValue(_pSum[j].getValue() * fac1 - _simplex[_iHighest][j].getValue() * fac2);
	}
	// Now compute the function for this new set of parameters:
	yTry = getFunction()->f(pTry);
  nbEval_++;
	
	// Then test this new point:
	if (yTry < _y[_iHighest])
  {
		_y[_iHighest] = yTry;
		for (int j = 0; j < ndim; j++)
    {
			_pSum[j].setValue(_pSum[j].getValue() + pTry[j].getValue() - _simplex[_iHighest][j].getValue());
			_simplex[_iHighest][j].setValue(pTry[j].getValue());
		}
	}
	return yTry;
}

/******************************************************************************/


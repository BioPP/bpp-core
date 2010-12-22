//
// File: DirectionFunction.cpp
// Created by: Julien Dutheil
// Created on: Wed Apr 11 17:28 2007
// From file PowellMultiDimensions.cpp
//

/*
  Copyright or © or Copr. CNRS, (November 17, 2004)

  This software is a computer program whose purpose is to provide classes
  for phylogenetic data analysis.

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

#include "DirectionFunction.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

void DirectionFunction::setParameters(const ParameterList & params)
  throw (ParameterNotFoundException, ConstraintException)
{
  _params = params;
  double x = _params[0].getValue();
  for(unsigned int j = 0; j < _p.size(); j++)
    {
      //      cout << _p[j].getValue() << " " << x << " " << _xi[j] << endl;
      _xt[j].setValue((_p[j].getValue()) + x * _xi[j]);
    }
  _function->setParameters(_xt);
}

/******************************************************************************/

double DirectionFunction::getValue() const throw (Exception)
{
  return _function->getValue();
}

/******************************************************************************/

const ParameterList & DirectionFunction::getParameters() const throw (Exception)
{
  return _params;
}

/******************************************************************************/

void DirectionFunction::init(const ParameterList & p, const vector<double> & xi)
{
  _p = p;
  _xi = xi;
  if(_constraintPolicy == AutoParameter::CONSTRAINTS_AUTO)   autoParameter();
  else if(_constraintPolicy == AutoParameter::CONSTRAINTS_IGNORE) ignoreConstraints();
  _xt = _p;
}

/******************************************************************************/

void DirectionFunction::autoParameter()
{
  for(unsigned int i = 0; i < _p.size(); i++)
    {
      AutoParameter ap(_p[i]);
      ap.setMessageHandler(_messenger);
      _p.setParameter(i, ap);
    }
}

/******************************************************************************/

void DirectionFunction::ignoreConstraints()
{
  for(unsigned int i = 0; i < _p.size(); i++)
    {
      _p[i].removeConstraint();
    }
}

/******************************************************************************/


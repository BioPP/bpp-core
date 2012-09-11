//
// File: DirectionFunction.h
// Created by: Julien Dutheil
// Created on: Wed Apr 11 17:28 2007
// From file PowellMultiDimensions.h
//

/*
Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)

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

#ifndef _DIRECTIONFUNCTION_H_
#define _DIRECTIONFUNCTION_H_

#include "Functions.h"
#include "../Parametrizable.h"
#include "../AutoParameter.h"
#include "../../App/ApplicationTools.h"
#include "../../Io/OutputStream.h"

namespace bpp
{

class DirectionFunction:
  public Function,
  public ParametrizableAdapter
{
  private:
    mutable ParameterList _params, _p, _xt;
    std::vector<double> _xi;
    Function* _function;
    std::string _constraintPolicy;
    OutputStream* _messenger;
      
  public:
    DirectionFunction(Function* function = 0) :
      _params(), _p(), _xt(), _xi(),
      _function(function), _constraintPolicy(AutoParameter::CONSTRAINTS_KEEP),
      _messenger(ApplicationTools::message) {}

    DirectionFunction(const DirectionFunction& df) :
      ParametrizableAdapter(df), _params(df._params), _p(df._p), _xt(df._p), _xi(df._xi),
      _function(df._function), _constraintPolicy(df._constraintPolicy), _messenger(df._messenger) {}

    DirectionFunction& operator=(const DirectionFunction& df)
    {
      ParametrizableAdapter::operator=(df);
      _params = df._params;
      _p = df._p;
      _xt = df._p;
      _xi = df._xi;
      _function = df._function;
      _constraintPolicy = df._constraintPolicy;
      _messenger = df._messenger;
      return *this;
    }

    virtual ~DirectionFunction() {}

    DirectionFunction* clone() const { return new DirectionFunction(*this); }

  public: // Function interface implementation:
    void setParameters(const ParameterList& parameters)
      throw (ParameterNotFoundException, ConstraintException);
    double getValue() const throw (Exception);
    const ParameterList & getParameters() const throw (Exception);

  public: // Specific methods:
    void init(const ParameterList & p, const std::vector<double> & xi);
    void autoParameter();
    void ignoreConstraints();
    void setConstraintPolicy(const std::string & constraintPolicy) { _constraintPolicy = constraintPolicy; }
    std::string getConstraintPolicy() const { return _constraintPolicy; }
    void setMessageHandler(OutputStream* messenger) { _messenger = messenger; }
    Function * getFunction() const { return _function; }
    /**
     * @return The set of parameters associated to the function, as specified by the init() method.
     */
    ParameterList getFunctionParameters() const { return _p; }
    unsigned int getNumberOfParameters() const { return _p.size(); }

};

} //end of namespace bpp.

#endif //_DIRECTIONFUNCTION_H_


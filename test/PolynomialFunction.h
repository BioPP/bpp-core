//
// File: PolynomialFunction.cpp
// Created by: Julien Dutheil
// Created on: Wed Oct 27 18:46 2010
//

// Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Function/Functions.h>
#include <Bpp/Numeric/AbstractParametrizable.h>

#include <map>
#include <string>

using namespace bpp;
using namespace std;

class PolynomialFunction1 :
  public virtual FunctionInterface,
  public AbstractParametrizable
{
private:
  double fval_;

public:
  PolynomialFunction1(bool withConstraint = true) : AbstractParametrizable(""), fval_(0)
  {
    // We declare parameters here:
    addParameter_(new Parameter("x", 0));
    addParameter_(new Parameter("y", 0));
    if (withConstraint)
      addParameter_(new Parameter("z", 0.5, std::make_shared<IntervalConstraint>(0.01, 5, true, true)));
    else
      addParameter_(new Parameter("z", 0));
    fireParameterChanged(getParameters());
  }

  PolynomialFunction1* clone() const { return new PolynomialFunction1(*this); }

public:
  void setParameters(const ParameterList& pl)
  {
    matchParametersValues(pl);
  }
  double getValue() const { return fval_; }

  void fireParameterChanged(const ParameterList& pl)
  {
    double x = getParameterValue("x");
    double y = getParameterValue("y");
    double z = getParameterValue("z");
    fval_ = (x - 5) * (x - 5) + (y + 2) * (y + 2) + (z - 3) * (z - 3);
  }
};

class PolynomialFunction1Der1 :
  public PolynomialFunction1,
  public virtual FirstOrderDerivable
{
protected:
  bool compFirstDer_;
  mutable map<string, double> firstDer_;

public:
  PolynomialFunction1Der1(bool withConstraint = true) :
    PolynomialFunction1(withConstraint),
    compFirstDer_(true),
    firstDer_()
  {
    // Need to compute derivatives:
    fireParameterChanged(getParameters());
  }

  PolynomialFunction1Der1* clone() const { return new PolynomialFunction1Der1(*this); }

public:
  void setParameters(const ParameterList& pl)
  {
    matchParametersValues(pl);
  }

  void fireParameterChanged(const ParameterList& pl)
  {
    PolynomialFunction1::fireParameterChanged(pl);
    if (compFirstDer_)
    {
      double x = getParameterValue("x");
      double y = getParameterValue("y");
      double z = getParameterValue("z");
      firstDer_["x"] = 2 * (x - 5);
      firstDer_["y"] = 2 * (y + 2);
      firstDer_["z"] = 2 * (z - 3);
    }
  }

  void enableFirstOrderDerivatives(bool yn) { compFirstDer_ = yn; }
  bool enableFirstOrderDerivatives() const { return compFirstDer_; }

  double getFirstOrderDerivative(const std::string& variable) const
  {
    if (!compFirstDer_)
      throw Exception("PolynomialFunction1Der1::getFirstOrderDerivative. First order derivatives are not computed.");
    return firstDer_[variable];
  }
};

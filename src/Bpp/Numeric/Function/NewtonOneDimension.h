// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_NEWTONONEDIMENSION_H
#define BPP_NUMERIC_FUNCTION_NEWTONONEDIMENSION_H


#include "AbstractOptimizer.h"

namespace bpp
{
/**
 * @brief Newton's optimization for one parameter.
 */
class NewtonOneDimension :
  public AbstractOptimizer
{
private:
  std::string _param;
  unsigned int _maxCorrection;

public:
  NewtonOneDimension(std::shared_ptr<SecondOrderDerivable> function = nullptr);
  virtual ~NewtonOneDimension() {}

  NewtonOneDimension* clone() const { return new NewtonOneDimension(*this); }

public:
  void doInit(const ParameterList& params);

  double doStep();

  void setMaximumNumberOfCorrections(unsigned int mx) { _maxCorrection = mx; }

  const FirstOrderDerivable& firstOrderDerivableFunction() const
  {
    if (function_) { 
      return *dynamic_pointer_cast<const FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("NewtonOneDimension::firstOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }

  FirstOrderDerivable& firstOrderDerivableFunction()
  {
    if (function_) { 
      return *dynamic_pointer_cast<FirstOrderDerivable>(function_);
    } else {
      throw NullPointerException("NewtonOneDimension::firstOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }
  
  std::shared_ptr<const FirstOrderDerivable> getFirstOrderDerivableFunction() const
  {
    return dynamic_pointer_cast<const FirstOrderDerivable>(function_);
  }

  std::shared_ptr<FirstOrderDerivable> getFirstOrderDerivableFunction()
  {
    return dynamic_pointer_cast<FirstOrderDerivable>(function_);
  }

  const SecondOrderDerivable& secondOrderDerivableFunction() const
  {
    if (function_) { 
      return *dynamic_pointer_cast<const SecondOrderDerivable>(function_);
    } else {
      throw NullPointerException("NewtonOneDimension::secondOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }

  SecondOrderDerivable& secondOrderDerivableFunction()
  {
    if (function_) { 
      return *dynamic_pointer_cast<SecondOrderDerivable>(function_);
    } else {
      throw NullPointerException("NewtonOneDimension::secondOrderDerivableFunction() : no function associated to this optimizer.");
    } 
  }
  
  std::shared_ptr<const SecondOrderDerivable> getSecondOrderDerivableFunction() const
  {
    return dynamic_pointer_cast<const SecondOrderDerivable>(function_);
  }

  std::shared_ptr<SecondOrderDerivable> getSecondOrderDerivableFunction()
  {
    return dynamic_pointer_cast<SecondOrderDerivable>(function_);
  }


};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_NEWTONONEDIMENSION_H

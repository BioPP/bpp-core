// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_DIRECTIONFUNCTION_H
#define BPP_NUMERIC_FUNCTION_DIRECTIONFUNCTION_H


#include "../../App/ApplicationTools.h"
#include "../../Io/OutputStream.h"
#include "../AutoParameter.h"
#include "../Parametrizable.h"
#include "Functions.h"

namespace bpp
{
class DirectionFunction :
  public virtual FunctionInterface,
  public ParametrizableAdapter
{
private:
  mutable ParameterList params_, p_, xt_;
  std::vector<double> xi_;
  std::shared_ptr<FunctionInterface> function_;
  std::string constraintPolicy_;
  std::shared_ptr<OutputStream> messenger_;

public:
  DirectionFunction(std::shared_ptr<FunctionInterface> function = nullptr) :
    params_(), p_(), xt_(), xi_(),
    function_(function), constraintPolicy_(AutoParameter::CONSTRAINTS_KEEP),
    messenger_(ApplicationTools::message) {}

  DirectionFunction(const DirectionFunction& df) :
    ParametrizableAdapter(df), params_(df.params_), p_(df.p_), xt_(df.p_), xi_(df.xi_),
    function_(df.function_), constraintPolicy_(df.constraintPolicy_), messenger_(df.messenger_) {}

  DirectionFunction& operator=(const DirectionFunction& df)
  {
    ParametrizableAdapter::operator=(df);
    params_ = df.params_;
    p_ = df.p_;
    xt_ = df.p_;
    xi_ = df.xi_;
    function_ = df.function_;
    constraintPolicy_ = df.constraintPolicy_;
    messenger_ = df.messenger_;
    return *this;
  }

  virtual ~DirectionFunction() {}

  DirectionFunction* clone() const override { return new DirectionFunction(*this); }

public:
  // Function interface implementation:
  void setParameters(const ParameterList& parameters) override;
  double getValue() const override;
  const ParameterList& getParameters() const override;

public:
  // Specific methods:
  void init(const ParameterList& p, const std::vector<double>& xi);
  void autoParameter();
  void ignoreConstraints();
  void setConstraintPolicy(const std::string& constraintPolicy) { constraintPolicy_ = constraintPolicy; }

  std::string getConstraintPolicy() const { return constraintPolicy_; }

  void setMessageHandler(std::shared_ptr<OutputStream> messenger) { messenger_ = messenger; }
  std::shared_ptr<FunctionInterface> getFunction() const { return function_; }
  /**
   * @return The set of parameters associated to the function, as specified by the init() method.
   */
  ParameterList getFunctionParameters() const { return p_; }

  size_t getNumberOfParameters() const override { return p_.size(); }

protected:
  ParameterList& getParameters_() override;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_DIRECTIONFUNCTION_H

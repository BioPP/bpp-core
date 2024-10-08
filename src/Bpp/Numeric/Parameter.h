// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PARAMETER_H
#define BPP_NUMERIC_PARAMETER_H


#include "../Clonable.h"
#include "Constraints.h"
#include "ParameterExceptions.h"

// From the STL:
#include <string>
#include <iostream>
#include <vector>
#include <memory>

namespace bpp
{
class Parameter;

class ParameterEvent :
  public virtual Clonable
{
protected:
  Parameter* parameter_;

public:
  ParameterEvent(Parameter* parameter);

  ParameterEvent(const ParameterEvent& pe) : parameter_(pe.parameter_) {}
  ParameterEvent& operator=(const ParameterEvent& pe)
  {
    parameter_ = pe.parameter_;
    return *this;
  }

  ParameterEvent* clone() const { return new ParameterEvent(*this); }

public:
  const Parameter* parameter() const { return parameter_; }
  Parameter* parameter() { return parameter_; }
};

/**
 * @brief The parameter listener interface.
 *
 * Imlementing this interface allows to catch events associated to parameters modifications.
 * Listeners must have an identifier that will be used to pinpoint it when attached to a list.
 * This identifier needs not be unique though, but listeners with identical id will be undistinguishable.
 */
class ParameterListener :
  public virtual Clonable
{
public:
  ParameterListener* clone() const = 0;

public:
  /**
   * @return The identifier of this listener.
   */
  virtual const std::string& getId() const = 0;

  /**
   * @brief Notify a renaming action.
   *
   * @param event Event associated to the acion.
   */
  virtual void parameterNameChanged(ParameterEvent& event) = 0;

  /**
   * @brief Notify a value change.
   *
   * @param event Event associated to the acion.
   */
  virtual void parameterValueChanged(ParameterEvent& event) = 0;

  /**
   * @brief Notify a constraint change.
   *
   * @param event Event associated to the acion.
   */
  virtual void parameterConstraintChanged(ParameterEvent& event) = 0;
};

/**
 * @brief This class is designed to facilitate the manipulation of parameters.
 *
 * A parameter object contains a <i>value</i> stored as a double.
 * It also contains a <i>name</i> and optionaly a constraint.
 * Constraint objects allows to apply restriction on the value of the parameter,
 * for instance positive number, or a particular interval and so on.
 *
 * @see ParameterList, Parametrizable, Constraint.
 */
class Parameter :
  public virtual Clonable
{
protected:
  std::string name_;             // Parameter name
  double value_;            // Parameter value
  double precision_;  // Precision needed for Parameter value
  std::shared_ptr<ConstraintInterface> constraint_;
  std::vector<std::shared_ptr<ParameterListener>> listeners_;

public:
  // Class constructors and destructors:

  /**
   * @brief Default contructor. Creates a parameter with no name, no constraint, and a value of 0.
   */
  Parameter() : name_(""), value_(0), precision_(0), constraint_(0), listeners_() {}

  /**
   * @brief Build a new parameter.
   *
   * @param name       The parameter name.
   * @param value      The parameter value.
   * @param constraint An optional pointer toward a constraint Object. The constraint will be copied and attached to this instance.
   * @param precision An optional parameter precision (default 0)
   * @throw ConstraintException If the parameter value does not match the contraint.
   */
  Parameter(const std::string& name, double value, std::shared_ptr<ConstraintInterface> constraint = 0, double precision = 0);

  Parameter(const std::string& name, double value, std::shared_ptr<ConstraintInterface> constraint, bool precision) = delete;

  /**
   * @brief Copy constructor.
   */
  Parameter(const Parameter& param);

  /**
   * @brief Assignment operator.
   */
  Parameter& operator=(const Parameter& param);

  virtual ~Parameter();

  Parameter* clone() const { return new Parameter(*this); }

public:
  /**
   * @brief Set the name of this parameter.
   *
   * @param name the new parameter name.
   */
  virtual void setName(const std::string& name)
  {
    name_ = name;
    ParameterEvent event(this);
    fireParameterNameChanged(event);
  }

  /**
   * @brief Set the value of this parameter.
   *
   * @param value the new parameter value.
   */
  virtual void setValue(double value);

  /**
   * @brief Set the precision of this parameter.
   *
   * @param precision the new parameter precision.
   */
  void setPrecision(double precision);

  /**
   * @brief Get the name of this parameter.
   *
   * @return The parameter name.
   */
  virtual const std::string& getName() const { return name_; }

  /**
   * @brief Get the value of this parameter.
   *
   * @return The parameter value.
   */
  virtual double getValue() const { return value_; }

  /**
   * @brief Get the precision of this parameter.
   *
   * @return The precision value.
   */
  virtual double getPrecision() const { return precision_; }

  /**
   * @brief Return the constraint associated to this parameter if there is one.
   *
   * @return A shared pointer toward the constraint, or NULL if
   * there is no constraint.
   */
  virtual std::shared_ptr<const ConstraintInterface> getConstraint() const { return constraint_; }

  /**
   * @brief Return the constraint associated to this parameter if there is one.
   *
   * @return A referec toward the constraint if there is one, otherwise throw an exception.
   */
  virtual const ConstraintInterface& constraint() const
  {
    if (constraint_)
      return *constraint_;
    else
      throw NullPointerException("Parameter::constraint(). No contraint is associated to this parameter.");
  }

  /**
   * @brief Return the constraint associated to this parameter if
   * there is one.
   *
   * @return A shared pointer toward the constraint, or NULL if there is no
   * constraint.
   */
  virtual std::shared_ptr<ConstraintInterface> getConstraint() { return constraint_; }

  /**
   * @brief Return the constraint associated to this parameter if there is one.
   *
   * @return A referec toward the constraint if there is one, otherwise throw an exception.
   */
  virtual ConstraintInterface& constraint()
  {
    if (constraint_)
      return *constraint_;
    else
      throw NullPointerException("Parameter::constraint(). No contraint is associated to this parameter.");
  }

  /**
   * @brief Tells if this parameter has a constraint.
   *
   * @return True if this parameter has a contraint.
   */
  virtual bool hasConstraint() const { return constraint_ != nullptr; }

  /**
   * @brief Remove the constraint associated to this parameter.
   *
   * Warning! The contraint objet is not deleted.
   *
   * @return A pointer toward the formerly used contraint.
   */
  virtual std::shared_ptr<ConstraintInterface> removeConstraint();

  /**
   * @brief Set a constraint to this parameter.
   *
   * @param constraint A pointer to the constraint (may be null)
   */
  virtual void setConstraint(std::shared_ptr<ConstraintInterface> constraint);

  /**
   * @brief Add a new listener to this parameter.
   *
   * @param listener The listener to add.
   */
  virtual void addParameterListener(std::shared_ptr<ParameterListener> listener)
  {
    listeners_.push_back(listener);
  }

  /**
   * @brief Remove all listeners with a given id from this parameter.
   *
   * @param listenerId The id of listener to remove.
   */
  virtual void removeParameterListener(const std::string& listenerId);

  /**
   * @brief Tell is there is a listener with a given id from this parameter.
   *
   * @param listenerId The id of listener to remove.
   * @return True if at list one listener with the given id was found.
   */
  virtual bool hasParameterListener(const std::string& listenerId);

protected:
  void fireParameterNameChanged(ParameterEvent& event)
  {
    for (auto listener : listeners_)
    {
      listener->parameterNameChanged(event);
    }
  }

  void fireParameterValueChanged(ParameterEvent& event)
  {
    for (auto listener : listeners_)
    {
      listener->parameterValueChanged(event);
    }
  }

  void fireParameterConstraintChanged(ParameterEvent& event)
  {
    for (auto listener : listeners_)
    {
      listener->parameterConstraintChanged(event);
    }
  }

public:
  static const std::shared_ptr<IntervalConstraint> R_PLUS;
  static const std::shared_ptr<IntervalConstraint> R_PLUS_STAR;
  static const std::shared_ptr<IntervalConstraint> R_MINUS;
  static const std::shared_ptr<IntervalConstraint> R_MINUS_STAR;
  static const std::shared_ptr<IntervalConstraint> PROP_CONSTRAINT_IN;
  static const std::shared_ptr<IntervalConstraint> PROP_CONSTRAINT_EX;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PARAMETER_H

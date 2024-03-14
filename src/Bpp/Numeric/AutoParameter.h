// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_AUTOPARAMETER_H
#define BPP_NUMERIC_AUTOPARAMETER_H


#include "Parameter.h"

// From Utils:
#include "../Io/OutputStream.h"

namespace bpp
{
/**
 * @brief The AutoParameter class.
 *
 * This class overides the setValue() method of class Parameter so that no
 * Exception is thrown. This allows to perform optimization under constraint.
 */

class AutoParameter :
  public Parameter
{
private:
  std::shared_ptr<OutputStream> messageHandler_;

public:
  /**
   * @brief Build a new AutoParameter.
   *
   * @param name The parameter name.
   * @param value The parameter value.
   * @param constraint An optional shared pointer toward a Constraint object.
   * @throw ConstraintException If the parameter value does not match the contraint.
   */
  AutoParameter(const std::string& name = "", double value = 0, std::shared_ptr<ConstraintInterface> constraint = nullptr);

  /**
   * @brief Copy constructor.
   *
   * @param param The parameter to copy.
   */
  AutoParameter(const Parameter& param);

  /**
   * @brief Copy constructor.
   *
   * @param param The parameter to copy.
   */
  AutoParameter(const AutoParameter& param);

  /**
   * @brief Assignment operator.
   *
   * @param param The parameter to copy.
   */
  AutoParameter& operator=(const AutoParameter& param);

  virtual ~AutoParameter() {}

  AutoParameter* clone() const { return new AutoParameter(*this); }

public:
  /**
   * @brief Set the value of this parameter.
   *
   * This method is redefined so that no constraintException is thrown!
   * When a Constraint is match, we automatically apply a correct value instead.
   * This correct value is the nearest limit reached by the value, or a value next to
   * the limit if the limit is not reachable.
   *
   * This allow to perform optimization under constraint whith algorithms that are not
   * initially built for this.
   *
   * @param value the new parameter value.
   */
  virtual void setValue(double value);

public:
  // Specific method:

  /**
   * @brief Set the message handler for this AutoParameter.
   *
   * The message handler keeps all messages that the parameter may send.
   * The default handler is set to standard output, but you can pass any
   * ostream object (cerr, ofstream, etc.).
   *
   * A NULL pointer disable message output.
   *
   * @param mh The message handler to use.
   */
  virtual void setMessageHandler(std::shared_ptr<OutputStream> mh) { messageHandler_ = mh; }

public:
  static std::string CONSTRAINTS_AUTO;
  static std::string CONSTRAINTS_IGNORE;
  static std::string CONSTRAINTS_KEEP;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_AUTOPARAMETER_H

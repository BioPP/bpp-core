// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_OPTIMIZER_H
#define BPP_NUMERIC_FUNCTION_OPTIMIZER_H


#include "../../Clonable.h"
#include "../../Io/OutputStream.h"
#include "../ParameterList.h"
#include "Functions.h"
#include "OptimizationStopCondition.h"

namespace bpp
{
class OptimizerInterface;

/**
 * @brief An event object which indicates that an optimization event occured.
 */
class OptimizationEvent
{
private:
  /**
   * @brief Stores a pointer toward the optimizer that cast the event.
   *
   * A regular pointer is used, as the Optimizer itself may not be managed by a smart pointer.
   */
  OptimizerInterface* optimizer_;

public:
  /**
   * @param optimizer A pointer toward the optimizer that launched the event.
   */
  OptimizationEvent(OptimizerInterface* optimizer) : optimizer_(optimizer) {}
  OptimizationEvent(const OptimizationEvent& oe) : optimizer_(oe.optimizer_) {}
  OptimizationEvent& operator=(const OptimizationEvent& oe)
  {
    optimizer_ = oe.optimizer_;
    return *this;
  }
  virtual ~OptimizationEvent() {}

public:
  /**
   * @return A pointer toward the optimizer that launched the event.
   */
  OptimizerInterface* getOptimizer() { return optimizer_; }

  /**
   * @return A pointer toward the optimizer that launched the event.
   */
  const OptimizerInterface* getOptimizer() const { return optimizer_; }
};


/**
 * @brief The listener interface for receiving optimization events.
 *
 * The class that is interested in processing an optimization event implements this interface,
 * and the object created with that class is registered with a component,
 * using the component's addOptimizationListener method.
 * More kinds of events may be processed in the future.
 */
class OptimizationListener
{
public:
  OptimizationListener() {}
  virtual ~OptimizationListener() {}

public:
  virtual void optimizationInitializationPerformed(const OptimizationEvent& event) = 0;
  virtual void optimizationStepPerformed(const OptimizationEvent& event) = 0;
  /**
   * @return 'true' If this listener modifies the parameter set.
   */
  virtual bool listenerModifiesParameters() const = 0;
};


/**
 * @brief This is the basal interface for all optimization methods.
 *
 * An optimizer deals with Function objects.
 * Optimizer objects are event-driven: they notify listeners when a step is performed.
 */
class OptimizerInterface :
  public virtual Clonable
{
public:
  OptimizerInterface() {}
  virtual ~OptimizerInterface() {}

  OptimizerInterface* clone() const = 0;

public:
  /**
   * @brief Set the initial values of the parameters.
   *
   * @param params The initial values of parameters.
   * @throw Exception If a problem occured during initialization.
   */
  virtual void init(const ParameterList& params) = 0;

  /**
   * @return 'true' if this optimizer has been initialized.
   */
  virtual bool isInitialized() const = 0;

  /**
   * @brief Perform an optimization step.
   *
   * @return the value of the function after this step.
   * @throw Exception If a problem occured during optimization or if the optimizer has not been initialized.
   */
  virtual double step() = 0;

  /**
   * @return The parameters with their current values.
   */
  virtual const ParameterList& getParameters() const = 0;

  /**
   * @return The value associated with a given parameter name.
   */

  virtual double getParameterValue(const std::string& name) const = 0;
  /**
   * @brief Get the current function value.
   *
   * @return The value of the function at the point specified by the current parameters.
   * @throw NullPointerException If no function is associated with this optimizer.
   */
  virtual double getFunctionValue() const = 0;

  /**
   * @brief Perform as many optimization steps untill the stop condition is met.
   *
   * @return The value of the function after optimization is completed.
   * @throw Exception If a problem occured during optimization or if the optimizer has not been initialized.
   */
  virtual double optimize() = 0;

  /**
   * @brief Set the function to optimize.
   *
   * @param function The function to optimize.
   */
  virtual void setFunction(std::shared_ptr<FunctionInterface> function) = 0;

  /**
   * @brief Get the current function being optimized.
   *
   * @return A const reference toward the function being optimized.
   * @throw NullPointerException if no function is associated with the optimizer.
   */
  virtual const FunctionInterface& function() const = 0;

  /**
   * @brief Get the current function being optimized.
   *
   * @return A const reference toward the function being optimized.
   * @throw NullPointerException if no function is associated with the optimizer.
   */
  virtual FunctionInterface& function() = 0;

  /**
   * @brief Get the current function being optimized.
   *
   * @return A const pointer toward the function being optimized.
   */
  virtual std::shared_ptr<const FunctionInterface> getFunction() const = 0;

  /**
   * @brief Get the current function being optimized.
   *
   * @return A const pointer toward the function being optimized.
   */
  virtual std::shared_ptr<FunctionInterface> getFunction() = 0;

  /**
   * @brief Tell if a funciton is associatied to this optimizer.
   *
   * @return True if a function has been associated to this optimizer.
   */
  virtual bool hasFunction() const = 0;

  /**
   * @brief Set the message handler for this optimizer.
   *
   * The message handler keeps all messages that the optimizer may send.
   * The default handler is set to standard output, but you can pass any
   * ostream object (cerr, ofstream, etc.).
   *
   * A null pointer disables message output.
   *
   * @param mh The message handler to use.
   */
  virtual void setMessageHandler(std::shared_ptr<OutputStream> mh) = 0;

  /**
   * @return The stream used for handling messages, if any.
   */
  virtual std::shared_ptr<OutputStream> getMessageHandler() const = 0;

  /**
   * @brief Set the profiler for this optimizer.
   *
   * The profiler keeps all the intermediate values taken by the parameters.
   * The default profiler is set to standard output, but you can pass any
   * ostream object (cerr, ofstream, etc.).
   *
   * A NULL pointer disables message output.
   *
   * @param profiler The profiler to use.
   */
  virtual void setProfiler(std::shared_ptr<OutputStream> profiler) = 0;

  /**
   * @return The stream used for profiling, if any.
   */
  virtual std::shared_ptr<OutputStream> getProfiler() const = 0;

  /**
   * @brief Get the number of function evaluations performed since the
   * call of the init function.
   *
   * @return The number of function evaluations.
   */
  virtual unsigned int getNumberOfEvaluations() const = 0;

  /**
   * @brief Set the stop condition of the optimization algorithm.
   *
   * @param stopCondition The stop condition to use while optimizing.
   * @see OptimizationStopCondition.
   */
  virtual void setStopCondition(std::shared_ptr<OptimizationStopCondition> stopCondition) = 0;

  /**
   * @brief Get the stop condition of the optimization algorithm.
   *
   * @return The stop condition used while optimizing.
   */
  virtual std::shared_ptr<OptimizationStopCondition> getStopCondition() = 0;

  /**
   * @brief Get the stop condition of the optimization algorithm.
   *
   * @return The stop condition used while optimizing.
   */
  virtual std::shared_ptr<const OptimizationStopCondition> getStopCondition() const = 0;

  /**
   * @brief Get the default stop condition of the optimization algorithm.
   *
   * @return The default stop condition used while optimizing.
   */
  virtual std::shared_ptr<OptimizationStopCondition> getDefaultStopCondition() = 0;

  /**
   * @brief Get the default stop condition of the optimization algorithm.
   *
   * @return The default stop condition used while optimizing.
   */
  virtual std::shared_ptr<const OptimizationStopCondition> getDefaultStopCondition() const = 0;

  /**
   * @brief Tell if the tolerance level is reached.
   *
   * @return Whether the tolerance is reached or not.
   * @see OptimizationStopCondition
   */
  virtual bool isToleranceReached() const = 0;

  /**
   * @brief Tell if the maximum number of function evaluations is reached.
   *
   * @return Whether the maximum number of function evaluations is reached or not.
   */
  virtual bool isMaximumNumberOfEvaluationsReached() const = 0;

  /**
   * @brief Set the maximum number of function evaluation to perform during optimization.
   *
   * @param max The maximum number of evaluations to perform.
   */
  virtual void setMaximumNumberOfEvaluations(unsigned int max) = 0;

  /**
   * @brief Set the verbose level.
   *
   * 0 = off
   * 1 = on
   * 2 = more verbose
   * 3 = even more, etc.
   *
   * In most cases, only the 0 and 1 levels are implemented.
   *
   * @param v verbose level.
   */
  virtual void setVerbose(unsigned int v) = 0;

  /**
   * @brief Get the verbose level.
   *
   * @return verbose level.
   */
  virtual unsigned int getVerbose() const = 0;

  /**
   * @brief Set the constraint policy for this optimizer.
   *
   * @param constraintPolicy The constraint policy.
   */
  virtual void setConstraintPolicy(const std::string& constraintPolicy) = 0;

  /**
   * @brief Get the constraint policy for this optimizer.
   *
   * @return The constraint policy.
   */
  virtual std::string getConstraintPolicy() const = 0;

  /**
   * @brief Register a listener to this class.
   *
   * All registered listeners will be informed when an optimization event occur.
   * See the documentation of the class to know what kind of events are supported.
   *
   * @param listener A listener to be registered with.
   */
  virtual void addOptimizationListener(std::shared_ptr<OptimizationListener> listener) = 0;

  /**
   * @brief Tell if we shall update all parameters after one optimization step.
   *
   * This is required only for functions that have non-independent parameters,
   * which means that setting one parameter value may modify one or several other parameters.
   * Depending on the optimizer, this may have no effect.
   *
   * @param yn true/false
   */
  virtual void updateParameters(bool yn) = 0;

  /**
   * @brief Tell if we shall update all parameters after one optimization step.
   *
   * This is required only for functions that have non-independent parameters,
   * which means that setting one parameter value may modify one or several other parameters.
   * Depending on the optimizer, this may have no effect.
   *
   * @return yn true/false
   */
  virtual bool updateParameters() const = 0;
};


/**
 * @brief Save intermediate optimization results to file.
 */
class BackupListener :
  public OptimizationListener
{
private:
  std::string backupFile_;

public:
  BackupListener(const string& backupFile) :
    backupFile_(backupFile) {}

  virtual ~BackupListener() {}

public:
  void optimizationInitializationPerformed(const OptimizationEvent& event) {}

  void optimizationStepPerformed(const OptimizationEvent& event)
  {
    std::ofstream bck(backupFile_.c_str(), std::ios::out);
    bck << "f(x)=" << setprecision(20) << event.getOptimizer()->getFunction()->getValue() << endl;
    ParameterList pl = event.getOptimizer()->getFunction()->getParameters();
    for (unsigned int i = 0; i < pl.size(); ++i)
    {
      bck << pl[i].getName() << "=" <<  setprecision(20) << pl[i].getValue() << std::endl;
    }
    bck.close();
  }

  bool listenerModifiesParameters() const { return false; }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_OPTIMIZER_H

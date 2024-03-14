
/*
  Copyright or © or Copr. Bio++ Development Team, (November 17, 2004)
  
  This software is a computer program whose purpose is to provide classes
  for numerical calculus.
  
  This software is governed by the CeCILL license under French law and
  abiding by the rules of distribution of free software. You can use,
  modify and/ or redistribute the software under the terms of the CeCILL
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".
  
  As a counterpart to the access to the source code and rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty and the software's author, the holder of the
  economic rights, and the successive licensors have only limited
  liability.
  
  In this respect, the user's attention is drawn to the risks associated
  with loading, using, modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean that it is complicated to manipulate, and that also
  therefore means that it is reserved for developers and experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and, more generally, to use and operate it in the
  same conditions as regards security.
  
  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL license and that you accept its terms.
*/

#ifndef BPP_NUMERIC_FUNCTION_ABSTRACTOPTIMIZER_H
#define BPP_NUMERIC_FUNCTION_ABSTRACTOPTIMIZER_H


#include "Optimizer.h"

namespace bpp
{
/**
 * @brief Partial implementation of the Optimizer interface.
 *
 * This implementation is designed for unconstrained or simple-bounded optimization.
 * You should not use it with global contraints.
 * It also enables the gestion of listeners by maintaining a vector of pointers toward the listener.
 * Important note: this list of listener is not duplicated in cas of copy of the Optimizer, as
 * listeners are expected to be bounded ot a particular instance.:if expand("%") == ""|browse confirm w|else|confirm w|endif
 */
class AbstractOptimizer :
  public virtual OptimizerInterface
{
protected:
  /**
   * @brief The function to optimize.
   */
  std::shared_ptr<FunctionInterface> function_;

  /**
   * @brief The parameters that will be optimized.
   */
  ParameterList parameters_;

  /**
   * @brief The message handler.
   */
  std::shared_ptr<OutputStream> messageHandler_;

  /**
   * @brief The profiler.
   */
  std::shared_ptr<OutputStream> profiler_;

  /**
   * @brief The constraint policy.
   *
   * Must be one the following:
   * - CONSTRAINTS_KEEP: keep the constraint associated to the parameters (default).
   * - CONSTRAINTS_IGNORE: remove all constraints.
   * - CONSTRAINTS_AUTO: use AutoParameters to deal with constraints.
   *
   * @see AutoParameter
   */
  std::string constraintPolicy_;

  /**
   * @brief The stoping condition to use while optimizing.
   */
  std::shared_ptr<OptimizationStopCondition> stopCondition_;

  /**
   * @brief The default stoping condition to use while optimizing.
   */
  std::shared_ptr<OptimizationStopCondition> defaultStopCondition_;

  /**
   * @brief State of the verbose mode: > 0 = enabled.
   *
   * This may not be used by the Optimizer.
   */
  unsigned int verbose_;

  /**
   * @brief Check if the optimizer have been feeded with initial parameters values.
   */
  bool isInitialized_;

  time_t startTime_;

  std::vector< std::shared_ptr<OptimizationListener> > listeners_;

  bool updateParameters_;

  std::string stepChar_;

protected:
  /**
   * @brief The maximum number of function evaluations allowed.
   */
  unsigned int nbEvalMax_;

  /**
   * @brief The current number of function evaluations achieved.
   */
  unsigned int nbEval_;

  /**
   * @brief The current value of the function.
   */
  double currentValue_;

  /**
   * @brief Tell if the tolerance level has been reached.
   *
   * This field is initilaised by the init() method, maintained by the
   * step() method and used in the optimize() method.
   */
  bool tolIsReached_;

public:
  AbstractOptimizer(std::shared_ptr<FunctionInterface> function = nullptr);

  AbstractOptimizer(const AbstractOptimizer& opt);

  AbstractOptimizer& operator=(const AbstractOptimizer& opt);

  virtual ~AbstractOptimizer() {}

public:
  /**
   * @name The Optimizer interface.
   *
   * @{
   */
  /**
   * @brief Basic implementation.
   *
   * Store all parameters, call the doInit method, print to profiler, initialize timer and notify all listeners.
   */
  void init(const ParameterList& params) override ;
  /**
   * @brief Basic implementation.
   *
   * Check if the optimizer is initialized, check if parameters need update because of listeners, call the doStep method, print the current point to the profiler, notify all listeners and return the current value of the function.
   */
  double step() override;

  /**
   * @brief Basic implementation.
   *
   * Call the step method untill tolerance is reached.
   */
  double optimize() override;

  bool isInitialized() const override { return isInitialized_; }

  const ParameterList& getParameters() const override { return parameters_; }

  double getParameterValue(const std::string& name) const override
  { 
    return parameters_.getParameterValue(name);
  }
  
  void setFunction(std::shared_ptr<FunctionInterface> function) override
  {
    function_ = function;
    if (function) stopCondition_->init();
  }
  
  const FunctionInterface& function() const override
  {
    if (function_) { 
      return *function_;
    } else {
      throw NullPointerException("AbstractOptimizer::function() : no function associated to this optimizer.");
    } 
  }

  FunctionInterface& function() override
  {
    if (function_) { 
      return *function_;
    } else {
      throw NullPointerException("AbstractOptimizer::function() : no function associated to this optimizer.");
    } 
  }

  std::shared_ptr<const FunctionInterface> getFunction() const override { return function_; }

  std::shared_ptr<FunctionInterface> getFunction() override { return function_; }

  bool hasFunction() const override { return function_ != 0; }
  
  double getFunctionValue() const override
  {
    if (!function_) throw NullPointerException("AbstractOptimizer::getFunctionValue. No function associated to this optimizer.");
    return currentValue_;
  }

  void setMessageHandler(std::shared_ptr<OutputStream> mh) override { messageHandler_ = mh; }

  std::shared_ptr<OutputStream> getMessageHandler() const override { return messageHandler_; }

  void setProfiler(std::shared_ptr<OutputStream> profiler) override { profiler_ = profiler; }

  std::shared_ptr<OutputStream> getProfiler() const override { return profiler_; }

  unsigned int getNumberOfEvaluations() const override { return nbEval_; }
  
  void setStopCondition(std::shared_ptr<OptimizationStopCondition> stopCondition) override
  {
    stopCondition_ = stopCondition;
  }
  
  std::shared_ptr<OptimizationStopCondition> getStopCondition() override { return stopCondition_; }

  std::shared_ptr<const OptimizationStopCondition> getStopCondition() const override { return stopCondition_; }

  std::shared_ptr<OptimizationStopCondition> getDefaultStopCondition() override { return defaultStopCondition_; }

  std::shared_ptr<const OptimizationStopCondition> getDefaultStopCondition() const override { return defaultStopCondition_; }

  bool isToleranceReached() const override { return tolIsReached_; }

  bool isMaximumNumberOfEvaluationsReached() const override { return nbEval_ >= nbEvalMax_; }

  void setMaximumNumberOfEvaluations(unsigned int max) override { nbEvalMax_ = max; }
  
  void setVerbose(unsigned int v) override { verbose_ = v; }
  
  unsigned int getVerbose() const override { return verbose_; }
  
  void setConstraintPolicy(const std::string& constraintPolicy) override { constraintPolicy_ = constraintPolicy; }
  
  std::string getConstraintPolicy() const override { return constraintPolicy_; }
  
  void addOptimizationListener(std::shared_ptr<OptimizationListener> listener) override
  {
    if (listener)
      listeners_.push_back(listener);
  }
  /** @} */

  /**
   * @brief Tell if we shall update all parameters after one optimization step.
   *
   * This is required only for functions that have non-independent parameters,
   * which means that setting one parameter value may modify one or several other parameters.
   * Depending on the optimizer, this may have no effect.
   *
   * @param yn true/false
   */
  void updateParameters(bool yn) override { updateParameters_ = yn; }

  /**
   * @brief Tell if we shall update all parameters after one optimization step.
   *
   * This is required only for functions that have non-independent parameters,
   * which means that setting one parameter value may modify one or several other parameters.
   * Depending on the optimizer, this may have no effect.
   *
   * @return yn true/false
   */
  bool updateParameters() const override { return updateParameters_; }

  /**
   * @brief Set the character to be displayed during optimization.
   *
   * @param c A character.
   */
  void setOptimizationProgressCharacter(const std::string& c) { stepChar_ = c; }
  /**
   * @return The character to be displayed during optimization.
   */
  const std::string& getOptimizationProgressCharacter() const { return stepChar_; }

protected:
  /**
   * @brief This function is called by the init() method and contains all calculations.
   *
   * @param params The parameters to use for initialization.
   */
  virtual void doInit(const ParameterList& params) = 0;

  /**
   * @brief This function is called by the step() method and contains all calculations.
   *
   * @return The value of the function after the optimization step.
   */
  virtual double doStep() = 0;

  /**
   * @name Inner utilitary functions
   *
   * @{
   */

  /**
   * @brief Build a list of AutoParameter instead of Parameter.
   */
  void autoParameter();

  /**
   * @brief Remove the constraints of all the arguments.
   */
  void ignoreConstraints();

  /**
   * @brief Print to the profile if there is one.
   *
   * @param v The double value to print.
   */
  void profile(double v);

  /**
   * @brief Print to the profile if there is one.
   *
   * @param v The unsigned int value to print.
   */
  void profile(unsigned int v);

  /**
   * @brief Print to the profile if there is one.
   *
   * @param s The string to print to the profile.
   */
  void profile(const std::string& s);

  /**
   * @brief Print to the profile if there is one and end line.
   *
   * @param v The double value to print.
   */
  void profileln(double v);

  /**
   * @brief Print to the profile if there is one and end line.
   *
   * @param v The unsigned int value to print.
   */
  void profileln(unsigned int v);

  /**
   * @brief Print to the profile if there is one and end line.
   *
   * @param s The string to print to the profile.
   */
  void profileln(const std::string& s);

  /**
   * @brief Print parameters and corresponding function evaluation to profiler.
   *
   * @param params The parameters to print.
   * @param value  The function evaluation.
   */
  void printPoint(const ParameterList& params, double value);

  /**
   * @brief Give a message to print to the message handler.
   *
   * @param message The message to print.
   */
  void printMessage(const std::string& message);

  /**
   * @brief Notify all listeners that optimizer initialization was performed.
   *
   * This method should be called by the init method.
   *
   * @param event An OptimizationEvent object.
   */
  void fireOptimizationInitializationPerformed(const OptimizationEvent& event);

  /**
   * @brief Notify all listeners that an optimization step was performed.
   *
   * This method should be called by the step method.
   *
   * @param event An OptimizationEvent object.
   */
  void fireOptimizationStepPerformed(const OptimizationEvent& event);

  bool listenerModifiesParameters() const;
  /** @} */

protected:
  ParameterList& getParameters_() { return parameters_; }
  
  Parameter& getParameter_(size_t i) { return parameters_[i]; }
  
  std::shared_ptr<FunctionInterface> getFunction_() { return function_; }

  void setDefaultStopCondition_(std::shared_ptr<OptimizationStopCondition> osc)
  {
    defaultStopCondition_ = osc;
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_ABSTRACTOPTIMIZER_H

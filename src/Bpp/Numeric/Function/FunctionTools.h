// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_FUNCTIONTOOLS_H
#define BPP_NUMERIC_FUNCTION_FUNCTIONTOOLS_H


#include "../VectorTools.h"
#include "../DataTable.h"
#include "Functions.h"

namespace bpp
{
/**
 * @brief This class is a data structure to specify a set of parameter values (most likely for evaluation by a Function)
 *
 * @see FunctionTools
 */
class ParameterGrid
{
private:
  std::vector<std::string> names_;
  VVdouble grid_;

public:
  ParameterGrid() : names_(), grid_() {}
  virtual ~ParameterGrid() {}

public:
  /**
   * @brief Add a new dimension (parameter name + corresponding values).
   *
   * @param name The name of the dimension (parameter name).
   * @param values The values the parameter will take.
   * @throw Exception in case the dimension is note valid (duplicated parameter name for instance).
   */
  void addDimension(const std::string& name, const Vdouble& values);

  const std::vector<std::string>& getDimensionNames() const { return names_; }

  const std::string& getDimensionName(unsigned int i) const
  {
    if (i >= names_.size()) throw IndexOutOfBoundsException("ParameterGrid::getDimensionName().", i, 0, names_.size() - 1);
    return names_[i];
  }

  size_t getNumberOfDimensions() const { return names_.size(); }

  /**
   * @return The total number of points in the grid, that is the product of all dimension sizes.
   */
  size_t getTotalNumberOfPoints() const;

  const VVdouble& getPoints() const { return grid_; }
  const Vdouble& getPointsForDimension(unsigned int i) const;
  const Vdouble& getPointsForDimension(const std::string& name) const;
};

/**
 * @brief This class contains static methods to deal with Function objects.
 */
class FunctionTools
{
public:
  /**
   * @brief Evaluates a function on all points in a given grid.
   *
   * @param function The function to use for the evaluation.
   * @param grid     The grid defining the set of points to evaluate.
   *
   * @return A shared pointer toward a dynamically created DataTable.
   *         Each row corresponds to a combination of parameters
   *         values and the corresponding function value. There is
   *         hence one column per parameter, and one additional column
   *         containing the corresponding function evaluations. When
   *         DataTable supports different column type, we will
   *         probably return a DataTable instead.
   */
  
  static std::shared_ptr<DataTable> computeGrid(
    FunctionInterface& function,
    const ParameterGrid& grid);
};
} // end of namespace bpp
#endif // BPP_NUMERIC_FUNCTION_FUNCTIONTOOLS_H

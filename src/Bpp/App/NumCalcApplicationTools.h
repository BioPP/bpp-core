// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_APP_NUMCALCAPPLICATIONTOOLS_H
#define BPP_APP_NUMCALCAPPLICATIONTOOLS_H


#include "../Numeric/Function/FunctionTools.h"
#include "../Numeric/VectorTools.h"
#include "../Text/StringTokenizer.h"
#include "ApplicationTools.h"

namespace bpp
{
class NumCalcApplicationTools
{
public:
  NumCalcApplicationTools();
  virtual ~NumCalcApplicationTools();

public:
  /**
   * @brief Build a vector of integers as described by a string
   *
   * Build a vector of integers following a description like:
   * "2, 5, 7-10, 4" => [2, 5, 7, 8, 9, 10, 4]
   *
   * @author Sylvain Gaillard
   * @param s The string to parse.
   * @param delim Delimiter between elements.
   * @param seqdelim Delimiter between min and max for a sequence.
   * @return A vector containing the integers
   */
  static std::vector<int> seqFromString(const std::string& s, const std::string& delim = ",", const std::string& seqdelim = "-");

  /**
   * @brief Build a vector of double from a structured text description.
   *
   * The syntax may be one of the following:
   * - Specified values: 1.23, 2.34, 3.45, 4.56
   * - Sequence macro: seq(from=1.23,to=2.45,step=0.1)
   *   or              seq(from=1.23,to=2.45,size=5)
   *   The meaning of these to form is equivalent as the R function:
   *   The first one start from 1.23 and increment 0.1 until it reaches
   *   the 2.45 value, wheras the seocnd one will compute 3 values at
   *   equal distance from 1.23 and 2.45. the 'from' and 'to' values are
   *   included, except for the first syntax when the interval is not an
   *   exact multiple of the 'step' argument.
   *
   * @author Julien Dutheil
   * @param desc The string to parse.
   * @return A vector containing the corresponding values as double.
   * @throw Exception If the syntax describing the set is not correct.
   */
  static std::vector<double> getVector(const std::string& desc);

  /**
   * @brief Returns the value of the Parameter of the given name
   *  if it exists; otherwise returns the default value.
   *
   * @author Laurent Gueguen
   * @param pl A parameter list to look in.
   * @param name A string name
   * @param x A double value
   */
  static double getDefaultValue(const ParameterList& pl, const std::string& name, double x);

  /**
   * @brief Design a parameter grid from input options.
   *
   * Example:
   * @code
   * grid.number_of_parameters=3
   * grid.parameter1.name=x
   * grid.parameter1.values=0.1,0.2,0.3,0.4,0.5
   * grid.parameter2.name=y
   * grid.parameter2.values=seq(from=0.1,to=0.5,step=0.1)
   * grid.parameter3.name=z
   * grid.parameter3.values=seq(from=0.1,to=0.5,size=5)
   * @endcode
   *
   * @param params           The attribute map where options may be found.
   * @param suffix           A suffix to be applied to the parameter name.
   * @param suffixIsOptional Tell if the suffix is absolutely required.
   * @param warn             Tell if a warning must be sent in case the parameter is not found.
   * @return a parameter grid object.
   */
  static std::shared_ptr<ParameterGrid> getParameterGrid(
    std::map<std::string, std::string>& params,
    const std::string& suffix = "",
    bool suffixIsOptional = true,
    bool warn = true);
};
} // End of namespace bpp.
#endif // BPP_APP_NUMCALCAPPLICATIONTOOLS_H

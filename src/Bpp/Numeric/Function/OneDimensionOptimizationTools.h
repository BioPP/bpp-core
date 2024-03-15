// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_FUNCTION_ONEDIMENSIONOPTIMIZATIONTOOLS_H
#define BPP_NUMERIC_FUNCTION_ONEDIMENSIONOPTIMIZATIONTOOLS_H


#include "../../Io/OutputStream.h"
#include "DirectionFunction.h"
#include "Functions.h"

namespace bpp
{
class BracketPoint
{
public:
  double x;
  double f;

public:
  // Constructor and destructor:
  BracketPoint() : x(0),
    f(0) {}
  BracketPoint(double xval, double fval) : x(xval),
    f(fval) {}
  virtual ~BracketPoint() {}

public:
  void set(double x, double f);
};

class Bracket
{
public:
  // Constructor and destructor::
  Bracket() : a(),
    b(),
    c() {}
  virtual ~Bracket() {}

public:
  // Methods:
  void setA(double xa, double fa);
  void setB(double xb, double fb);
  void setC(double xc, double fc);

public:
  BracketPoint a, b, c;
};

/**
 * @brief Tools of one parameter-functions optimizations.
 *
 * For now, contains only one method to bracket a minimum.
 */
class OneDimensionOptimizationTools
{
public:
  OneDimensionOptimizationTools() {}
  virtual ~OneDimensionOptimizationTools() {}

public:
  /**
   * @brief Bracket a minimum.
   *
   * Given a function func, and given distinct initial points x1 and x2,
   * this routine searches in the downhill direction (defined by the function as
   * evaluated at the initial points) and returns a Bracket object with new points
   * a.x, b.x and c.x that bracket a minimum of the function. Also returned are the
   * function values at the three points, a.f, b.f and c.f.
   *
   * @param a, b       Two initial values for the parameter.
   * @param function   The function to bracket.
   * @param parameters The parameter to use as a variable.
   * @return           A bracket object.
   */
  static Bracket bracketMinimum(double a, double b, FunctionInterface& function, ParameterList parameters);

  /**
   * @brief Bracket a minimum by a search within the parameter's bounds.
   *
   * This implementation follows numerical recipes, page 352, inwards bracketing
   *
   * @param a, b       Two initial values for the parameter.
   * @param function   The function to bracket.
   * @param parameters The parameter to use as a variable.
   * @param intervalsNum number of segments in which [a;b] is splitted.
   *
   * @return           A bracket object.
   */
  static Bracket inwardBracketMinimum(double a, double b, FunctionInterface& function, ParameterList parameters, uint intervalsNum = 10);


  static unsigned int lineMinimization(
    std::shared_ptr<DirectionFunction> f1dim,
    ParameterList& parameters,
    std::vector<double>& xi,
    double tolerance,
    std::shared_ptr<OutputStream> profiler = nullptr,
    std::shared_ptr<OutputStream> messenger = nullptr,
    unsigned int verbose = 2);

  /**
   * @brief Search a 'sufficiently low' value for a function in a given direction.
   *
   * This function performs a similar computation as the lnsrch function defined at page 385 of
   *
   * <pre>
   * NUMERICAL RECIPES IN C: THE ART OF SCIENTIFIC COMPUTING
   * (ISBN 0-521-43108-5)
   * </pre>
   *
   * without the stpmax argument, since the steps are bounded in another way.
   */
  static unsigned int lineSearch(
    std::shared_ptr<DirectionFunction> f1dim,
    ParameterList& parameters,
    std::vector<double>& xi,
    std::vector<double>& gradient,
    std::shared_ptr<OutputStream> profiler = nullptr,
    std::shared_ptr<OutputStream> messenger = nullptr,
    unsigned int verbose = 2);

public:
  /**
   * @brief Maximum magnification allowed for a parabolic-fit step.
   */
  static double GLIMIT;
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_FUNCTION_ONEDIMENSIONOPTIMIZATIONTOOLS_H

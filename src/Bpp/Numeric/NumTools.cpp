// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "Matrix/Matrix.h"
#include "NumTools.h"

using namespace bpp;
using namespace std;

/******************************************************************************/

double NumTools::uniRoot(FunctionInterface& f, const std::string& param, double a, double b, double tolerance)
{
  ParameterList pl;
  pl.addParameter(Parameter(param, a));
  double fa = f.f(pl);
  pl[0].setValue(b);
  double fb = f.f(pl);
  if (fa * fb > 0.)
    throw Exception("NumTools::uniRoot(). Initial interval values are not of opposite sign.");
  double c = (a + b) / 2.;
  double fc;
  while (abs(fb - fa) > tolerance)
  {
    c = (a + b) / 2.; // Better use golden section here...
    pl[0].setValue(c);
    fc = f.f(pl);

    if (fc * fa < 0.)
    {
      b = c;
      fb = fc;
    }
    else
    {
      a = c;
      fa = fc;
    }
  }
  return c;
}

/******************************************************************************/

unique_ptr<RowMatrix<double> > NumTools::computeHessianMatrix(SecondOrderDerivable& function, const ParameterList& parameters)
{
  size_t n = parameters.size();
  vector<string> variables = parameters.getParameterNames();
  auto hessian = make_unique<RowMatrix<double> >(n, n);
  for (unsigned int i = 0; i < n; i++)
  {
    for (unsigned int j = 0; j < n; j++)
    {
      if (j == i)
        (*hessian)(i, j) = function.d2f(variables[i], parameters);
      else
        (*hessian)(i, j) = function.d2f(variables[i], variables[j], parameters);
    }
  }
  return hessian;
}

/******************************************************************************/

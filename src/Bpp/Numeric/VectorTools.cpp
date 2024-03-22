// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

// From Utils:
#include "../Text/TextTools.h"

#include "VectorTools.h"
using namespace bpp;

// From the STL:
#include <cmath>
#include <iostream>
using namespace std;

/******************************************************************************/

vector<double> VectorTools::breaks(const vector<double>& v, unsigned int n)
{
  vector<double> out;
  vector<double> r = VectorTools::range(v);
  double part = (r[1] - r[0]) / n;
  for (unsigned int i = 0; i < n; ++i)
  {
    out.push_back(r[0] + (part * i));
  }
  out.push_back(r[1]);
  return out;
}

/******************************************************************************/

bool VectorTools::test()
{
  vector<double> x1(5);
  vector<double> x2(5);
  x1[0] = -3.4;
  x1[1] =  1.8;
  x1[2] = -2.1;
  x1[3] = -2.5;
  x1[4] =  1.0;

  x2[0] = -5.3;
  x2[1] = -4.8;
  x2[2] =  2.7;
  x2[3] =  7.2;
  x2[4] =  0.4;

  print(x1);
  print(x2);
  double m1 = mean<double, double>(x1);
  double m2 = mean<double, double>(x2);
  double v1 = var<double, double>(x1);
  double v2 = var<double, double>(x2);
  cout << "Mean x1 = " << m1 << "\tVar x1 = " << v1 << endl;
  cout << "Mean x2 = " << m2 << "\tVar x2 = " << v2 << endl;
  cov<double, double>(x1, x2);
  cor<double, double>(x1, x2);
  cos<double, double>(x1, x2);
  shannon<double, double>(x1);
  return m1 == -0.2 && m2 == 0.04 && v1 == 6.565 && v2 == 27.603;
}

/******************************************************************************/

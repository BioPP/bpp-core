// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Function/TwoPointsNumericalDerivative.h>
#include <Bpp/Numeric/Function/ThreePointsNumericalDerivative.h>
#include <Bpp/Numeric/Function/FivePointsNumericalDerivative.h>
#include <Bpp/Numeric/Random/RandomTools.h>
#include <vector>
#include <iostream>
#include "PolynomialFunction.h"

using namespace bpp;
using namespace std;

int main() {
  auto f = make_shared<PolynomialFunction1>();
  auto f1 = make_shared<PolynomialFunction1Der1>();
  ParameterList pl = f->getParameters();
  TwoPointsNumericalDerivative nd2pt(f)  ; nd2pt.setParametersToDerivate(pl.getParameterNames());
  ThreePointsNumericalDerivative nd3pt(f); nd3pt.setParametersToDerivate(pl.getParameterNames());
  FivePointsNumericalDerivative nd5pt(f) ; nd5pt.setParametersToDerivate(pl.getParameterNames());
  
  for (unsigned int repeat = 0; repeat < 10000; ++repeat) {
    for (size_t i = 0; i < pl.size(); ++i) {
      double val;
      if (!pl[i].hasConstraint())
        val = RandomTools::giveRandomNumberBetweenZeroAndEntry(100) - 50;
      else
      {
        const auto& parameterBounds = dynamic_pointer_cast<const IntervalConstraint>(pl[i].getConstraint());
        val = RandomTools::giveRandomNumberBetweenZeroAndEntry(parameterBounds->getUpperBound() - parameterBounds->getLowerBound()) + parameterBounds->getLowerBound();
      }

      pl[i].setValue(val);
    }

    nd2pt.setParameters(pl);
    vector<double> derivativesNum2pt(pl.size());
    for (size_t i = 0; i < pl.size(); ++i) {
      derivativesNum2pt[i] = nd2pt.getFirstOrderDerivative(pl.getParameterNames()[i]);
    }

    nd3pt.setParameters(pl);
    vector<double> derivativesNum3pt(pl.size());
    for (size_t i = 0; i < pl.size(); ++i) {
      derivativesNum3pt[i] = nd3pt.getFirstOrderDerivative(pl.getParameterNames()[i]);
    }

    nd5pt.setParameters(pl);
    vector<double> derivativesNum5pt(pl.size());
    for (size_t i = 0; i < pl.size(); ++i) {
      derivativesNum5pt[i] = nd5pt.getFirstOrderDerivative(pl.getParameterNames()[i]);
    }

    vector<double> derivativesAna(pl.size());
    f1->setParameters(pl);
    bool test = true;
    for (size_t i = 0; i < pl.size(); ++i) {
      derivativesAna[i] = f1->getFirstOrderDerivative(pl.getParameterNames()[i]);
      if (abs(derivativesAna[i] - derivativesNum2pt[i]) > std::sqrt(nd2pt.getInterval())) test = false;
      if (abs(derivativesAna[i] - derivativesNum3pt[i]) > std::sqrt(nd3pt.getInterval())) test = false;
      if (abs(derivativesAna[i] - derivativesNum5pt[i]) > std::sqrt(nd5pt.getInterval())) test = false;
    }

    //Test:
    if (!test) {
      //Failure!
      for (size_t i = 0; i < pl.size(); ++i) {
        cout << setprecision(20) << pl[i].getName() << "=" << pl[i].getValue() << endl;
        cout << setprecision(20) << "Ana. Der.     =" << derivativesAna[i]    << endl;
        cout << setprecision(20) << "Num. Der. 2pts=" << derivativesNum2pt[i] << " " << std::sqrt(nd2pt.getInterval()) << endl;
        cout << setprecision(20) << "Num. Der. 3pts=" << derivativesNum3pt[i] << " " << std::sqrt(nd3pt.getInterval()) << endl;
        cout << setprecision(20) << "Num. Der. 5pts=" << derivativesNum5pt[i] << " " << std::sqrt(nd5pt.getInterval()) <<endl;
      }
      return 1;
    } else {
      //for (unsigned int i = 0; i < pl.size(); ++i) {
      //  cout << pl[i].getName();
      //  cout << "\t" << pl[i].getValue();
      //  cout << "\t" << derivativesAna[i];
      //  cout << "\t" << derivativesNum2pt[i];
      //  cout << "\t" << derivativesNum3pt[i];
      //  cout << "\t" << derivativesNum5pt[i];
      //  cout << endl;
      //}
    }
  }
  return 0;
}

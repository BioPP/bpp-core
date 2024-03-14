// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <Bpp/Numeric/Random/RandomTools.h>
#include <Bpp/Numeric/Function/PowellMultiDimensions.h>
#include <Bpp/Numeric/Function/ReparametrizationFunctionWrapper.h>
#include <vector>
#include <iostream>

using namespace bpp;
using namespace std;

class MyFunction:
  public virtual FunctionInterface,
  public AbstractParametrizable
{
  private:
    double fval_;
 
  public:
    MyFunction() : AbstractParametrizable(""), fval_(0) {
      //We declare parameters here:
      addParameter_(new Parameter("x", 0, std::make_shared<IntervalConstraint>(-1, 7, true, true)));
      addParameter_(new Parameter("y", 0, std::make_shared<IntervalConstraint>(-4, 4, true, true)));
      fireParameterChanged(getParameters());
    }
 
    MyFunction* clone() const { return new MyFunction(*this); }
 
  public:
    void setParameters(const ParameterList& pl)
    {
      matchParametersValues(pl);
    }
    double getValue() const { return fval_; }
 
    void fireParameterChanged(const ParameterList& pl) {
      double x = getParameterValue("x");
      double y = getParameterValue("y");
      fval_ = cos(x) + sin(y);
    }
};

int main() {
  auto f = make_shared<MyFunction>();
  auto fw = make_shared<ReparametrizationFunctionWrapper> (f);
  ParameterList pl = fw->getParameters();
  PowellMultiDimensions optimizer(fw);
  optimizer.init(pl);
  optimizer.optimize();
  double minf = f->getValue();
  double x = f->getParameterValue("x");
  double y = f->getParameterValue("y");
  cout << "x=" << x << endl;
  cout << "y=" << y << endl;
  cout << "f=" << minf << endl;

  cout << setprecision(20) << (abs(x - 3.141593) + abs(y + 1.570796)) << endl;
  bool test = (abs(x - 3.141593) + abs(y + 1.570796)) < optimizer.getStopCondition()->getTolerance();
  return (test ? 0 : 1);
}

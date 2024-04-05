// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Numeric/NumConstants.h"
#include "../Text/KeyvalTools.h"
#include "ApplicationTools.h"
#include "NumCalcApplicationTools.h"

using namespace bpp;
using namespace std;

vector<int> NumCalcApplicationTools::seqFromString(const std::string& s, const std::string& delim, const std::string& seqdelim)
{
  vector<int> seq;
  unique_ptr<StringTokenizer> st(new StringTokenizer(s, delim, true));
  while (st->hasMoreToken())
  {
    unique_ptr<StringTokenizer> st2(new StringTokenizer(st->nextToken(), seqdelim, true));

    if (st2->numberOfRemainingTokens() > 1)
    {
      vector<int> tmp = VectorTools::seq(TextTools::toInt(st2->getToken(0)), TextTools::toInt(st2->getToken(1)), 1);
      VectorTools::append(seq, tmp);
    }
    else
    {
      seq.push_back(TextTools::toInt(st2->getToken(0)));
    }
  }
  return seq;
}


vector<double> NumCalcApplicationTools::getVector(const std::string& desc)
{
  vector<double> values;
  string key, val;

  if (desc.substr(0, 3) == "seq") // Bounds specified as sequence
  {
    map<string, string> keyvals;
    KeyvalTools::multipleKeyvals(desc.substr(4, desc.size() - 5), keyvals);
    if (keyvals.find("from") == keyvals.end())
      throw Exception("Unvalid sequence specification, missing 'from' key: " + desc.substr(3, desc.size() - 5));
    if (keyvals.find("to") == keyvals.end())
      throw Exception("Unvalid sequence specification, missing 'to' key: " + desc.substr(3, desc.size() - 5));
    if (keyvals.find("step") == keyvals.end() && keyvals.find("size") == keyvals.end())
      throw Exception("Unvalid sequence specification, missing 'step' or 'size' key: " + desc.substr(3, desc.size() - 5));
    
    const short ONE=0;
    const short LOG=1;
    const short EXP=2;
    const short DIX=3;

    short scale=ONE;
    
    if (keyvals.find("scale") != keyvals.end())
    {
      string sc=keyvals["scale"];
      if (sc == "log")
        scale=LOG;
      else {
        if (sc == "exp")
          scale=EXP;
        else {
          if (sc == "10^")
            scale = DIX;
          else
            throw Exception("Unknown scale " + sc + " for vector. Ask developpers.");
        }
      }
    }
    
    double start = TextTools::toDouble(keyvals["from"]);
    double end   = TextTools::toDouble(keyvals["to"]);
    if (keyvals.find("step") != keyvals.end())
    {
      double step = TextTools::toDouble(keyvals["step"]);
      for (double x = start; x <= end + NumConstants::TINY(); x += step)
      {
        double y;
        switch (scale)
        {
        case ONE:
          y=x;
          break;
        case LOG:
          y=log(x);
          break;
        case EXP:
          y=exp(x);
          break;
        case DIX:
          y=pow(10,x);
          break;
        default:
          y=x;
        }
        values.push_back(y);
      }
    }
    else
    {
      int size = TextTools::toInt(keyvals["size"]);
      double step = (end - start) / (double)size;
      for (int i = 0; i < size - 1; i++)
      {
        double x = start + i * step;
        double y;
        switch (scale)
        {
        case ONE:
          y=x;
          break;
        case LOG:
          y=log(x);
          break;
        case EXP:
          y=exp(x);
          break;
        case DIX:
          y=pow(10,x);
          break;
        default:
          y=x;
        }
        values.push_back(y);
      }
      double y;
      switch (scale)
      {
      case ONE:
        y=end;
        break;
      case LOG:
        y=log(end);
        break;
      case EXP:
        y=exp(end);
        break;
      case DIX:
        y=pow(10,end);
        break;
      default:
        y=end;
      }
      values.push_back(y); // for rounding purpose.
    }
  }
  else // Direct enumaration of values
  {
    StringTokenizer st(desc, ",");
    while (st.hasMoreToken())
      values.push_back(TextTools::toDouble(st.nextToken()));
  }
  return values;
}


double NumCalcApplicationTools::getDefaultValue(const ParameterList& pl, const std::string& name, double x)
{
  for (unsigned int i = 0; i < pl.size(); i++)
  {
    const Parameter& p = pl[i];
    if (p.getName() == name)
      return p.getValue();
  }
  return x;
}


shared_ptr<ParameterGrid> NumCalcApplicationTools::getParameterGrid(
    map<string, string>& params,
    const string& suffix,
    bool suffixIsOptional,
    bool warn)
{
  ApplicationTools::displayMessage("");
  ApplicationTools::displayMessage("ParameterGrid");
  unsigned int nbParams = ApplicationTools::getParameter<unsigned int>("grid.number_of_parameters", params, 1, suffix, suffixIsOptional, warn);
  auto grid = std::make_shared<ParameterGrid>();
  for (unsigned int i = 0; i < nbParams; i++)
  {
    string name = ApplicationTools::getStringParameter("grid.parameter" + TextTools::toString(i + 1) + ".name", params, "", suffix, suffixIsOptional, warn);
    vector<double> values = getVector(ApplicationTools::getStringParameter("grid.parameter" + TextTools::toString(i + 1) + ".values", params, "", suffix, suffixIsOptional, warn));
    grid->addDimension(name, values);
    IntervalConstraint bb(VectorTools::min(values),VectorTools::max(values),true,true);
    ApplicationTools::displayResult(name,bb.getDescription());
  }
  return grid;
}

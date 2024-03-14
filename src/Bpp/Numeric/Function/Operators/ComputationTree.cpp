// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <algorithm>

#include "BinaryOperator.h"
#include "ComputationTree.h"
#include "ConstantOperator.h"
#include "FunctionOperator.h"
#include "MathOperator.h"
#include "NegativeOperator.h"

using namespace std;
using namespace bpp;

ComputationTree::ComputationTree(const std::string& formula, const std::map<std::string, std::shared_ptr<FunctionInterface> >& functionNames) :
  AssociationTreeGlobalGraphObserver<Operator, short>(true)
{
  getGraph();

  std::string str2 = formula;

  str2.erase(std::remove_if(str2.begin(),
                            str2.end(),
                            [](char x){
    return std::isspace(x);
  }),
             str2.end());
  string form2 = TextTools::removeWhiteSpaces(formula);

  setRoot(readFormula_(str2, functionNames));
}

std::shared_ptr<Operator> ComputationTree::readFormula_(const std::string& formula, const std::map<std::string, std::shared_ptr<FunctionInterface> >& functionNames)
{
  unsigned int level = 0;
  // inside parentheses check
  // case + or -
  // most right '+' or '-' (but not inside '()') search and split

  for (size_t i = formula.size(); i > 0; --i)
  {
    char c = formula[i - 1];

    if (c == ')')
    {
      ++level;
      continue;
    }

    if (c == '(')
    {
      --level;
      continue;
    }

    if (level > 0)
      continue;

    if ((c == '+' || c == '-') && !(i == 1 || formula[i - 2] == '*' || formula[i - 2] == '/'
                                    || formula[i - 2] == '+' || formula[i - 2] == '-' || formula[i - 2] == '('))
    {
      std::shared_ptr<Operator> left = readFormula_(formula.substr(0, i - 1), functionNames);
      std::shared_ptr<Operator> right = readFormula_(formula.substr(i), functionNames);

      shared_ptr<Operator> here(new BinaryOperator(c, left, right));

      createNode(here);

      setFather(left, here);
      setFather(right, here);

      return here;
    }
  }

  // case * or /
  // most right '*' or '/' (but not inside '()') search and split
  for (size_t i = formula.size(); i > 0; --i)
  {
    char c = formula[i - 1];

    if (c == ')')
    {
      ++level;
      continue;
    }

    if (c == '(')
    {
      --level;
      continue;
    }

    if (level > 0)
      continue;

    if (c == '*' || c == '/')
    {
      std::shared_ptr<Operator> left = readFormula_(formula.substr(0, i - 1), functionNames);
      std::shared_ptr<Operator> right = readFormula_(formula.substr(i), functionNames);

      shared_ptr<Operator> here(new BinaryOperator(c, left, right));

      createNode(here);

      setFather(left, here);
      setFather(right, here);

      return here;
    }
  }


  if (formula[0] == '(')
    return readFormula_(formula.substr(1, formula.size() - 2), functionNames);

  else
  // case value
  {
    shared_ptr<Operator> here;
    try
    {
      double v = TextTools::toDouble(formula);
      here =  shared_ptr<Operator>(new ConstantOperator(v));
    }
    catch (Exception& e)
    {
      if (formula[0] == '-')
      {
        std::shared_ptr<Operator> son = readFormula_(formula.substr(1), functionNames);
        here = shared_ptr<Operator>(new NegativeOperator(son));
      }
      else
      {
        auto it = functionNames.find(formula);

        if (it != functionNames.end())
        {
          if (dynamic_pointer_cast<const SecondOrderDerivable>(it->second))
            here = shared_ptr<Operator>(new FunctionOperator<SecondOrderDerivable>(*dynamic_pointer_cast<SecondOrderDerivable>(it->second), formula));
          else if (dynamic_pointer_cast<const FirstOrderDerivable>(it->second))
            here = shared_ptr<Operator>(new FunctionOperator<FirstOrderDerivable>(*dynamic_pointer_cast<FirstOrderDerivable>(it->second), formula));
          else
            here = shared_ptr<Operator>(new FunctionOperator<FunctionInterface>(*it->second, formula));
        }
        else
        {
          size_t posp = formula.find("(");
          if (posp == string::npos)
            throw Exception("ComputationTree::readFormula_ : unknown formula : " + formula);

          std::shared_ptr<Operator> son = readFormula_(formula.substr(posp), functionNames);
          string fonc = formula.substr(0, posp);

          if (fonc == "exp")
            here = shared_ptr<Operator>(new MathOperator(&exp, "exp", son));
          else if (fonc == "log")
            here = shared_ptr<Operator>(new MathOperator(&log, "log", son));
          else
            throw Exception("ComputationTree::readFormula_ : unknown formula : " + formula);
        }
      }
    }

    this->getGraph();
    createNode(here);
    return here;
  } return NULL;
  // never
}

bool ComputationTree::isAllSum()
{
  std::unique_ptr<NodeIterator> it = allNodesIterator();

  for ( ; it->end(); it->next())
  {
    const BinaryOperator* op = dynamic_cast<const BinaryOperator*>((**it).get());
    if (op && op->getSymbol() != '+' && op->getSymbol() != '-')
      return false;
  }
  return true;
}


std::string ComputationTree::output() const
{
  std::string op = getRoot()->output();
  while (op.size() > 1 && op[0] == '(' && op[op.size() - 1] == ')')
  {
    op = op.substr(1, op.size() - 2);
  }
  return op;
}

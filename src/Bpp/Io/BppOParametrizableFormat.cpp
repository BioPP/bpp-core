// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "BppOParametrizableFormat.h"

using namespace bpp;

// From the STL:
#include <iomanip>
#include <algorithm>


using namespace std;

void BppOParametrizableFormat::write(const Parametrizable& parametrizable,
                                     OutputStream& out,
                                     std::vector<std::string>& writtenNames,
                                     bool printComma) const
{
  ParameterList pl = parametrizable.getParameters();
  int p = out.getPrecision();
  out.setPrecision(12);
  bool flag = printComma;
  for (size_t i = 0; i < pl.size(); ++i)
  {
    if (find(writtenNames.begin(), writtenNames.end(), pl[i].getName()) == writtenNames.end())
    {
      if (flag)
        out << ",";
      else
        flag = true;
      string pname = parametrizable.getParameterNameWithoutNamespace(pl[i].getName());

      (out << pname << "=").enableScientificNotation(false) << pl[i].getValue();
    }
  }
  out.setPrecision(p);
}

void BppOParametrizableFormat::write(const ParameterAliasable& parametrizable,
                                     OutputStream& out,
                                     std::map<std::string, std::string>& globalAliases,
                                     const std::vector<std::string>& names,
                                     std::vector<std::string>& writtenNames,
                                     bool printLocalAliases,
                                     bool printComma) const
{
  ParameterList pl = parametrizable.getIndependentParameters().createSubList(names);
  int p = out.getPrecision();
  out.setPrecision(12);
  bool flag = printComma;
  for (size_t i = 0; i < pl.size(); ++i)
  {
    if (find(writtenNames.begin(), writtenNames.end(), pl[i].getName()) == writtenNames.end())
    {
      if (flag)
        out << ",";
      else
        flag = true;
      string pname = parametrizable.getParameterNameWithoutNamespace(pl[i].getName());

      // Check for global aliases:
      if (globalAliases.find(pl[i].getName()) == globalAliases.end())
      {
        (out << pname << "=").enableScientificNotation(false) << pl[i].getValue();
      }
      else
        out << pname << "=" << globalAliases[pl[i].getName()];

      // Now check for local aliases:
      if (printLocalAliases)
      {
        vector<string> aliases = parametrizable.getAlias(pname);
        for (size_t j = 0; j < aliases.size(); ++j)
        {
          out << ", " << aliases[j] << "=" << pname;
        }
      }
      writtenNames.push_back(pl[i].getName());
    }
  }
  out.setPrecision(p);
}

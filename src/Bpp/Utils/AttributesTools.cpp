// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

// From the STL:
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;

#include "AttributesTools.h"
#include "../App/ApplicationTools.h"
#include "../Text/TextTools.h"
#include "../Io/FileTools.h"

using namespace bpp;

std::vector<std::string> AttributesTools::vParam_;

/******************************************************************************/

std::vector<std::string> AttributesTools::getVector(int argc, char* argv[])
{
  size_t n = static_cast<size_t>(argc);
  vector<string> result(n);
  for (size_t i = 1; i < n; ++i)
  {
    result[i] = string(argv[i]);
  }
  // Ignore first argc which is the program name!
  return result;
}

/******************************************************************************/

std::map<std::string, std::string> AttributesTools::getAttributesMap(
    const std::vector<std::string>& argv,
    const std::string& delimiter)
{
  map<string, string> am;
  getAttributesMap(argv, am, delimiter);
  return am;
}

/******************************************************************************/

void AttributesTools::getAttributesMap(
    const std::vector<std::string>& argv,
    std::map<std::string, std::string>& am,
    const std::string& delimiter)
{
  vector<string> argv2(argv.size());
  // First make a few cleaning:
  for (size_t i = 0; i < argv.size(); ++i)
  {
    // Make a few corrections first:
    string arg = removeComments(argv[i], "#", "\n"); // remove shell comments.
    arg = removeComments(arg, "//", "\n"); // remove C simple comments.
    arg = removeComments(arg, "/*", "*/"); // remove C multiple comments.
    arg = TextTools::removeWhiteSpaces(arg);
    argv2[i] = arg;
  }
  // Now parse arguments:
  for (size_t i = 0; i < argv.size(); i++)
  {
    string arg = argv2[i];
    if (arg == "")
      continue; // Skipping void line.
    while (arg[arg.size() - 1] == '\\')
    {
      // Splitted line
      i++;
      arg = arg.substr(0, arg.length() - 1) + argv2[i];
    }
    // Parsing:
    string::size_type limit = arg.find(delimiter, 0);
    if (limit == string::npos)
    {
      // Invalid parameter
      (*ApplicationTools::warning << "WARNING!!! Parameter '" << arg << "' has been ignored.").endLine();
    }
    else
    {
      string name  = string(arg.begin(), arg.begin() + static_cast<ptrdiff_t>(limit));
      string value = string(arg.begin() + static_cast<ptrdiff_t>(limit + delimiter.size()), arg.end());
      // if ((name == "param") || (name == "params"))
      // {
      //   if (std::find(vParam_.begin(),vParam_.end(),value)!=vParam_.end())
      //     throw Exception("Param name " + value + " already seen.");

      //   //Recursive inclusion:
      //   getAttributesMapFromFile(value, am, delimiter);
      //   vParam_.push_back(value);
      // }
      // else
      am[name] = value;
    }
  }
}

/******************************************************************************/

void AttributesTools::getAttributesMapFromFile(
    const std::string& file,
    std::map<std::string, std::string>& params,
    const std::string& delimiter)
{
  cout << "Parsing file " << file << " for options." << endl;
  ifstream input(file.c_str(), ios::in);
  vector<string> lines = FileTools::putStreamIntoVectorOfStrings(input);
  getAttributesMap(lines, params, delimiter);
}

/******************************************************************************/

std::map<std::string, std::string> AttributesTools::getAttributesMapFromFile(
    const std::string& file,
    const std::string& delimiter)
{
  map<string, string> params;
  getAttributesMapFromFile(file, params, delimiter);
  return params;
}

/******************************************************************************/

void AttributesTools::actualizeAttributesMap(
    std::map<std::string, std::string>& attMap,
    const std::map<std::string, std::string>& atts,
    bool replace)
{
  for (const auto& i : atts)
  {
    if (replace || attMap.find(i.first) == attMap.end())
      attMap[i.first] = i.second;
  }
}

/******************************************************************************/

void AttributesTools::resolveVariables(
    std::map<std::string, std::string>& am,
    char varCode,
    char varBeg,
    char varEnd)
{
  // Now resolve any variable:
  for (map<string, string>::iterator it = am.begin(); it != am.end(); it++)
  {
    string value = it->second;
    string::size_type index1 = value.find(TextTools::toString(varCode) + TextTools::toString(varBeg));
    while (index1 != string::npos)
    {
      string::size_type index2 = value.find(TextTools::toString(varEnd), index1);
      if (index2 != string::npos)
      {
        string varName  = value.substr(index1 + 2, index2 - index1 - 2);
        map<string, string>::iterator varIt = am.find(varName);
        string varValue = "";
        if (varIt == am.end())
        {
          if (ApplicationTools::error)
            (*ApplicationTools::error << "Variable '" << varName << "' is undefined and was ignored.").endLine();
        }
        else
        {
          if (varIt->second == value)
          {
            if (ApplicationTools::error)
              (*ApplicationTools::error << "Variable '" << varName << "' definition is cyclic and was ignored.").endLine();
          }
          else
            varValue = varIt->second;
        }
        // Modify original field:
        string newValue = value.substr(0, index1) + varValue + value.substr(index2 + 1);
        it->second = newValue;
      }
      else
        throw Exception("Syntax error, variable name is not closed.");
      value = it->second;
      index1 = value.find(TextTools::toString(varCode) + TextTools::toString(varBeg));
    }
  }
}

/******************************************************************************/

std::string AttributesTools::removeComments(
    const std::string& s,
    const std::string& begin,
    const std::string& end)
{
  string r = s;
  string::size_type last = 0;
  do
  {
    auto first = r.find(begin, last);
    if (first == string::npos)
      return r; // No shell comment.
    // else:
    last = r.find(end, first);
    if (last == string::npos)
    {
      r.erase(r.begin() + static_cast<ptrdiff_t>(first), r.end());
    }
    else
    {
      r.erase(r.begin() + static_cast<ptrdiff_t>(first), r.begin() + static_cast<ptrdiff_t>(last));
    }
  }
  while (last != string::npos);
  return r;
}

/******************************************************************************/

std::map<std::string, std::string> AttributesTools::parseOptions(int args, char** argv)
{
  // Get the parameters from command line:
  map<string, string> cmdParams = AttributesTools::getAttributesMap(
        AttributesTools::getVector(args, argv), "=");
  resolveVariables(cmdParams);

  // Look for specified files with parameters:
  // With priority to the deeper
  
  map<string, string> paramsfile, params;
  actualizeAttributesMap(paramsfile, cmdParams);
  
  while (paramsfile.find("param") != paramsfile.end() || paramsfile.find("params") != paramsfile.end())
  {
    string file = (paramsfile.find("param") != paramsfile.end()) ? paramsfile["param"] : paramsfile["params"];

    if (std::find(vParam_.begin(), vParam_.end(), file) != vParam_.end())
    {
      cout << file << " already seen. Skipping." << endl;
      if (paramsfile.find("param") != paramsfile.end())
        paramsfile.erase("param");
      else
        paramsfile.erase("params");
      continue;
    }

    if (!FileTools::fileExists(file))
      throw Exception("AttributesTools::parseOptions(). Parameter file not found.: " + file);

    params = getAttributesMapFromFile(file, "=");
    if (paramsfile.find("param") != paramsfile.end())
      paramsfile.erase("param");
    else
      paramsfile.erase("params");

    // Actualize attributes with ones passed to last file:
    actualizeAttributesMap(paramsfile, params, true);
    resolveVariables(paramsfile);

    vParam_.push_back(file);
  }

  actualizeAttributesMap(cmdParams, paramsfile, false);
  resolveVariables(cmdParams);
  
  return cmdParams;
}

/******************************************************************************/

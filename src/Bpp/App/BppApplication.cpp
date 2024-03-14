// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Numeric/Random/RandomTools.h"
#include "../Utils/AttributesTools.h"
#include "ApplicationTools.h"
#include "BppApplication.h"

// From the STL:
#include <iostream>

using namespace bpp;
using namespace std;

BppApplication::BppApplication(int argc, char* argv[], const std::string& name, bool verbose, int warningLevel) :
  appName_(name), params_(), timerStarted_(false), verbose_(verbose), warn_(warningLevel)
{
  if (verbose_)
    cout << "Parsing options:" << endl;
  params_ = AttributesTools::parseOptions(argc, argv);
  ApplicationTools::warningLevel = ApplicationTools::getIntParameter("--warning", params_, 0, "", true, 3);
  bool noint = ApplicationTools::getBooleanParameter("--noninteractive", params_, false, "", true, 3);
  ApplicationTools::interactive = !noint;
  std::mt19937::result_type seed = ApplicationTools::getParameter<std::mt19937::result_type>("--seed", params_, 5489u, "", true, 3);
  if (seed != 5489u)
  {
    RandomTools::setSeed(seed);
    if (verbose_)
      ApplicationTools::displayResult("Random seed set to", seed);
  }
}

void BppApplication::startTimer()
{
  ApplicationTools::startTimer();
  timerStarted_ = true;
}

void BppApplication::done()
{
  if (verbose_)
  {
    cout << appName_ << "'s done. Bye." << endl;
    if (timerStarted_)
      ApplicationTools::displayTime("Total execution time:");
  }
}

void BppApplication::help(const string& program) const
{
  (*ApplicationTools::message << "__________________________________________________________________________").endLine();
  (*ApplicationTools::message << program << " parameter1_name=parameter1_value parameter2_name=parameter2_value").endLine();
  (*ApplicationTools::message << "      ... param=option_file").endLine();
  (*ApplicationTools::message).endLine();
  (*ApplicationTools::message << "  Refer to the Bio++ Program Suite Manual for a list of available options.").endLine();
  (*ApplicationTools::message << "__________________________________________________________________________").endLine();
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "AbstractParametrizable.h"

using namespace bpp;
using namespace std;

void AbstractParametrizable::setNamespace(const std::string& prefix)
{
  // We need to update all parameter names!
  // First correct the global parameter list
  string currentName;
  for (unsigned int i = 0; i < parameters_.size(); i++)
  {
    currentName = parameters_[i].getName();
    if (TextTools::startsWith(currentName, prefix_))
      parameters_[i].setName(prefix + currentName.substr(prefix_.size()));
    else
      parameters_[i].setName(prefix + currentName);
  }

  // Then we store the new namespace:
  prefix_ = prefix;
}

std::string AbstractParametrizable::getParameterNameWithoutNamespace(const std::string& name) const
{
  if (TextTools::startsWith(name, prefix_))
    return name.substr(prefix_.size());
  else
    return name;
}

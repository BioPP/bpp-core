// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "AbstractParameterAliasable.h"
#include "VectorTools.h"

using namespace bpp;
using namespace std;

AbstractParameterAliasable::AbstractParameterAliasable(const AbstractParameterAliasable& ap) :
  AbstractParametrizable(ap),
  independentParameters_(),
  aliasListenersRegister_()
{
  for (size_t i = 0; i < ap.independentParameters_.size(); ++i)
  {
    independentParameters_.shareParameter(getParameter(getParameterNameWithoutNamespace(ap.independentParameters_[i].getName())));
  }

  // Actualize the register with adequate pointers:
  for (auto it : ap.aliasListenersRegister_)
  {
    auto listener = shared_ptr<AliasParameterListener>(it.second->clone());
    listener->setParameterList(&getParameters_());
    aliasListenersRegister_[it.first] = listener;
    // Now correct parameters with appropriate pointers:
    for (unsigned int i = 0; i < getNumberOfParameters(); ++i)
    {
      if (getParameters_()[i].hasParameterListener(it.first))
      {
        getParameters_()[i].removeParameterListener(it.first);
        getParameters_()[i].addParameterListener(listener);
      }
    }
  }
}

AbstractParameterAliasable& AbstractParameterAliasable::operator=(const AbstractParameterAliasable& ap)
{
  AbstractParametrizable::operator=(ap);

  for (size_t i = 0; i < ap.independentParameters_.size(); i++)
  {
    independentParameters_.shareParameter(getParameter(getParameterNameWithoutNamespace(ap.independentParameters_[i].getName())));
  }

  // Actualize the register with adequate pointers:
  for (auto it : ap.aliasListenersRegister_)
  {
    auto listener = shared_ptr<AliasParameterListener>(it.second->clone());
    listener->setParameterList(&getParameters_());
    aliasListenersRegister_[it.first] = listener;
    // Now correct parameters with appropriate pointers:
    for (unsigned int i = 0; i < getNumberOfParameters(); ++i)
    {
      if (getParameters_()[i].hasParameterListener(it.first))
      {
        getParameters_()[i].removeParameterListener(it.first);
        getParameters_()[i].addParameterListener(listener);
      }
    }
  }
  return *this;
}

void AbstractParameterAliasable::aliasParameters(const std::string& p1, const std::string& p2)
{
  // In case this is the first time we call this method:
  if (getNumberOfParameters() > 0 && independentParameters_.size() == 0)
    independentParameters_.shareParameters(getParameters());

  if (!hasParameter(p1))
    throw ParameterNotFoundException("AbstractParameterAliasable::aliasParameters", p1);
  if (!hasParameter(p2))
    throw ParameterNotFoundException("AbstractParameterAliasable::aliasParameters", p2);
  if (!independentParameters_.hasParameter(getNamespace() + p2))
    throw Exception("AbstractParameterAliasable::aliasParameters. Parameter " + p2 + " does not exist in independent parameters. Perhaps it is already aliased to a parameter and can't be aliased twice.");

  string id = "__alias_" + p2 + "_to_" + p1;
  string idCheck = "__alias_" + p1 + "_to_" + p2;

  if (aliasListenersRegister_.find(idCheck) != aliasListenersRegister_.end())
    throw Exception("AbstractParameterAliasable::aliasParameters. Trying to alias parameter " + p2 + " to " + p1 + ", but parameter " + p1 + " is already aliased to parameter " + p2 + ".");
  Parameter* param1 = &getParameter_(p1);
  Parameter* param2 = &getParameter_(p2);

  if (!param1->hasConstraint())
  {
    if (param2->hasConstraint())
    {
      ApplicationTools::displayWarning("Aliasing parameter " + p2 + " to " + p1 + ". " + p1 + " gets the constraints of " + p2 + ": " + param2->getConstraint()->getDescription());
      param1->setConstraint(param2->getConstraint());
    }
  }
  else
  // We use a small trick here, we test the constraints on the basis of their string description (C++ does not provide a default operator==() :( ).
  if (param2->hasConstraint() && (param1->getConstraint()->getDescription() != param2->getConstraint()->getDescription()))
  {
    std::shared_ptr<ConstraintInterface> nc(*param2->getConstraint() & *param1->getConstraint());
    ApplicationTools::displayWarning("Aliasing parameter " + p2 + " to " + p1 + " with different constraints. They get the intersection of both constraints : " + nc->getDescription());

    param2->setConstraint(nc);
    param1->setConstraint(nc);
  }

  // Every thing seems ok, let's create the listener and register it:

  auto aliasListener = make_shared<AliasParameterListener>(id, getParameters().whichParameterHasName(getNamespace() + p2), &getParameters_(), p1);

  aliasListenersRegister_[id] = aliasListener;

  // Now we add it to the appropriate parameter, that is p1.
  param1->addParameterListener(aliasListener);

  // Finally we remove p2 from the list of independent parameters:
  independentParameters_.deleteParameter(getNamespace() + p2);
}

void AbstractParameterAliasable::aliasParameters(map<string, string>& unparsedParams, bool verbose)
{
  const ParameterList& pl = getParameters();
  ParameterList plpars;

  for (size_t i = 0; i < pl.size(); i++)
  {
    if (unparsedParams.find(pl[i].getName()) == unparsedParams.end())
      plpars.addParameter(pl[i].clone());
  }

  size_t unp_s = unparsedParams.size();
  while (unp_s != 0)
  {
    auto it = unparsedParams.begin();
    while (it != unparsedParams.end())
    {
      Parameter* pp = 0;
      try
      {
        pp = &plpars.parameter(it->second);
      }
      catch (ParameterNotFoundException& e)
      {
        if (!pl.hasParameter(it->second))
          throw ParameterNotFoundException("Unknown aliasing parameter", it->first + "->" + it->second);
        continue;
      }
      unique_ptr<Parameter> p2(pp->clone());
      p2->setName(it->first);
      plpars.addParameter(p2.release());
      plpars.parameter(it->first);
      aliasParameters(it->second, it->first);
      if (verbose)
        ApplicationTools::displayResult("Parameter alias found", it->first + " -> " + it->second + " = " + TextTools::toString(pp->getValue()));
      it = unparsedParams.erase(it);
    }

    if (unparsedParams.size() == unp_s)
      throw Exception("Error, there is a cycle in aliasing starting with " + unparsedParams.begin()->first);
    else
      unp_s = unparsedParams.size();
  }

  matchParametersValues(plpars);
}


void AbstractParameterAliasable::unaliasParameters(const std::string& p1, const std::string& p2)
{
  if (!hasParameter(p1))
    throw ParameterNotFoundException("AbstractParameterAliasable::unaliasParameters", p1);
  if (!hasParameter(p2))
    throw ParameterNotFoundException("AbstractParameterAliasable::unaliasParameters", p2);

  string id = "__alias_" + p2 + "_to_" + p1;
  auto it = aliasListenersRegister_.find(id);
  if (it == aliasListenersRegister_.end())
    throw Exception("AbstractParameterAliasable::unaliasParameters. Parameter " + p2 + " is not aliased to parameter " + p1 + ".");
  // Remove the listener:
  getParameter_(p1).removeParameterListener(id);
  aliasListenersRegister_.erase(it);
  // Finally we re-add p2 to the list of independent parameters:
  independentParameters_.shareParameter(getParameter(p2));
}

void AbstractParameterAliasable::setNamespace(const std::string& prefix)
{
  string currentName;

  // We modify all the listeners
  for (auto& it : aliasListenersRegister_)
  {
    currentName = it.second->getName();
    if (TextTools::startsWith(currentName, getNamespace()))
      it.second->rename(prefix + currentName.substr(getNamespace().size()));
    else
      it.second->rename(prefix + currentName);
  }

  // Finally we notify the mother class:
  AbstractParametrizable::setNamespace(prefix);
}

vector<string> AbstractParameterAliasable::getAlias(const string& name) const
{
  vector<string> aliases;

  for (const auto& it : aliasListenersRegister_)
  {
    if (it.second->getFrom() == name)
    {
      string alias = it.second->getAlias();
      aliases.push_back(alias);
      if (alias != name)
      {
        vector<string> chainAliases = getAlias(alias);
        VectorTools::append(aliases, chainAliases);
      }
    }
  }
  return aliases;
}


std::map<std::string, std::string> AbstractParameterAliasable::getAliases() const
{
  map<string, string> aliases;

  for (const auto& it : aliasListenersRegister_)
  {
    string name = it.second->getFrom();

    vector<string> alias = getAlias(name);

    for (auto a : alias)
    {
      aliases[a] = name;
    }
  }

  return aliases;
}

ParameterList AbstractParameterAliasable::getAliasedParameters(const ParameterList& pl) const
{
  ParameterList aliases;
  bool b = false;

  for (const auto& it : aliasListenersRegister_)
  {
    if ((pl.hasParameter(it.second->getFrom()) ||
        aliases.hasParameter(it.second->getFrom()))
        && !(aliases.hasParameter(it.second->getAlias()) ||
        pl.hasParameter(it.second->getAlias())))
    {
      b = true;
      aliases.addParameter(parameter(it.second->getAlias()));
    }
  }

  while (b)
  {
    b = false;

    for (const auto& it : aliasListenersRegister_)
    {
      if (aliases.hasParameter(it.second->getFrom())
          && !(aliases.hasParameter(it.second->getAlias()) ||
          pl.hasParameter(it.second->getAlias())))
      {
        b = true;
        aliases.addParameter(parameter(it.second->getAlias()));
      }
    }
  }

  return aliases;
}

string AbstractParameterAliasable::getFrom(const string& name) const
{
  string from = "";

  for (const auto& it : aliasListenersRegister_)
  {
    if (it.second->getName() == name)
    {
      from = it.second->getFrom();
      break;
    }
  }

  return from;
}


ParameterList AbstractParameterAliasable::getFromParameters(const ParameterList& pl) const
{
  ParameterList from;
  bool b = false;

  for (const auto& it : aliasListenersRegister_)
  {
    if ((pl.hasParameter(it.second->getAlias()) ||
        from.hasParameter(it.second->getAlias()))
        && !(from.hasParameter(it.second->getFrom()) ||
        pl.hasParameter(it.second->getFrom())))
    {
      b = true;
      from.addParameter(parameter(it.second->getFrom()));
    }
  }

  while (b)
  {
    b = false;

    for (const auto& it : aliasListenersRegister_)
    {
      if (from.hasParameter(it.second->getAlias())
          && !(from.hasParameter(it.second->getFrom()) ||
          pl.hasParameter(it.second->getFrom())))
      {
        b = true;
        from.addParameter(parameter(it.second->getFrom()));
      }
    }
  }

  return from;
}

// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_ABSTRACTPARAMETERALIASABLE_H
#define BPP_NUMERIC_ABSTRACTPARAMETERALIASABLE_H


#include "AbstractParametrizable.h"
#include "ParameterAliasable.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief Inner listener class used by AbstractParameterAliasable.
 */
class AliasParameterListener :
  public ParameterListener
{
private:
  std::string id_;
  size_t alias_;
  ParameterList* pl_;
  std::string name_;
  std::string from_;

public:
  AliasParameterListener(const std::string& id, size_t alias, ParameterList* pl, const std::string& from) :
    id_(id),
    alias_(alias),
    pl_(pl),
    name_(),
    from_(from)
  {
    // This allow us to check if the parameter position have changed at some point...
    name_ = (*pl_)[alias].getName();
  }

  AliasParameterListener(const AliasParameterListener& apl) :
    id_(apl.id_),
    alias_(apl.alias_),
    pl_(apl.pl_),
    name_(apl.name_),
    from_(apl.from_)
  {}

  AliasParameterListener& operator=(const AliasParameterListener& apl)
  {
    id_    = apl.id_;
    alias_ = apl.alias_;
    pl_    = apl.pl_;
    name_  = apl.name_;
    from_  = apl.from_;
    return *this;
  }

  AliasParameterListener* clone() const { return new AliasParameterListener(*this); }

public:
  const std::string& getId() const { return id_; }

  const std::string& getFrom() const { return from_; }

  void setParameterList(ParameterList* pl) { pl_ = pl; }

  void parameterNameChanged(ParameterEvent& event) {}

  void parameterValueChanged(ParameterEvent& event)
  {
    Parameter* p = &(*pl_)[alias_];
    if (p->getName() != name_)
      throw Exception("AbstractParameterAliasable::AliasParameterListener::parameterValueChanged. Error, aliased parameter have change, maybe because it was renamed, or a parameter was removed?");
    p->setValue(event.parameter()->Parameter::getValue());
  }

  void parameterConstraintChanged(ParameterEvent& event)
  {
    Parameter* p = &(*pl_)[alias_];
    if (p->getName() != name_)
      throw Exception("AbstractParameterAliasable::AliasParameterListener::parameterConstraintChanged. Error, aliased parameter have change, maybe because it was renamed, or a parameter was removed?");
    p->setConstraint(event.parameter()->getConstraint());
  }

  const std::string& getName() const { return name_; }

  void rename(const std::string& name) { name_ = name; }

  const std::string& getAlias() const { return (*pl_)[alias_].getName(); }
};

/**
 * @brief A partial implementation of the Parametrizable interface.
 *
 * Parameters are stored in a protected ParameterList object.
 *
 * The abstract fireParameterChanged() method is provided so that the derived class
 * know when a parameter has changed, and can be updated.
 * All methods call the corresponding method in ParameterList and then call the
 * fireParameterChanged() method.
 */
class AbstractParameterAliasable :
  public AbstractParametrizable,
  public virtual ParameterAliasable
{
private:
  mutable ParameterList independentParameters_;

  /**
   * Contains all parameter listeners for maintening alias relationships.
   * The registry will be updated appropriately upon cloning and deleting.
   */
  std::map<std::string, std::shared_ptr<AliasParameterListener> > aliasListenersRegister_;

public:
  AbstractParameterAliasable(const std::string& prefix) :
    AbstractParametrizable(prefix),
    independentParameters_(),
    aliasListenersRegister_()
  {}

  AbstractParameterAliasable(const AbstractParameterAliasable& ap);

  AbstractParameterAliasable& operator=(const AbstractParameterAliasable& ap);

  virtual ~AbstractParameterAliasable() {}

public:
  void setNamespace(const std::string& prefix);

  bool hasIndependentParameter(const std::string& name) const { return independentParameters_.hasParameter(getNamespace() + name); }

  const ParameterList& getIndependentParameters() const { return independentParameters_; }

  size_t getNumberOfIndependentParameters() const { return independentParameters_.size(); }

  /**
   * @brief alias the parameters.
   *
   * @param p1 name of the parameter to which the other parameter is aliased
   * @param p2 name of the aliased parameter, which will be deleted from independent parameters list
   */
  void aliasParameters(const std::string& p1, const std::string& p2);

  void unaliasParameters(const std::string& p1, const std::string& p2);

  /**
   * @brief alias the parameters following the links described in a
   * map, and update the object accordingly. Cycles in aliasing are
   * detected and forbidden.
   *
   * @param unparsedParams the map of the links : <A,B> matches for A->B aliasing.
   * @param verbose verbosity
   */
  void aliasParameters(std::map<std::string, std::string>& unparsedParams, bool verbose);

  /**
   * @brief Return the list of the names of the parameters that are
   * aliased (directly or not) to one of the parameters of the list.
   */
  ParameterList getAliasedParameters(const ParameterList& pl) const;

  /**
   * @brief Return the list of the names of the parameters from which
   * the parameters of the list are aliased  (directly or not).
   */
  ParameterList getFromParameters(const ParameterList& pl) const;


  /**
   * @return The list of names of the parameters that are aliased with a given parameter.
   * The implementation is recursive, which means that in the case of A->B->C, getalias(C) will return both A and B.
   * @param name The name of the parameter to look for.
   */
  virtual std::vector<std::string> getAlias(const std::string& name) const;

  /**
   * @return the map of the aliases.
   */
  virtual std::map<std::string, std::string> getAliases() const;

  /**
   * @return The name of the parameter from which a given parameter is aliased.
   * @param name The name of the parameter to look for.
   */
  std::string getFrom(const std::string& name) const;

  // void fireParameterChanged(const ParameterList& parameters)
  // {
  //   independentParameters_.matchParametersValues(getParameters());
  // }

protected:
  void addParameter_(Parameter* parameter)
  {
    AbstractParametrizable::addParameter_(parameter);
    independentParameters_.shareParameter(getParameter(getParameterNameWithoutNamespace(parameter->getName())));
  }

  void addParameters_(const ParameterList& parameters)
  {
    AbstractParametrizable::addParameters_(parameters);

    for (size_t i = 0; i < parameters.size(); ++i)
    {
      independentParameters_.shareParameter(getParameter(getParameterNameWithoutNamespace(parameters[i].getName())));
    }
  }

  void shareParameter_(const std::shared_ptr<Parameter>& parameter)
  {
    AbstractParametrizable::shareParameter_(parameter);
    independentParameters_.shareParameter(parameter);
  }

  void shareParameters_(const ParameterList& parameters)
  {
    AbstractParametrizable::shareParameters_(parameters);
    for (size_t i = 0; i < parameters.size(); ++i)
    {
      independentParameters_.shareParameter(getParameter(getParameterNameWithoutNamespace(parameters[i].getName())));
    }
  }

  void includeParameters_(const ParameterList& parameters)
  {
    AbstractParametrizable::includeParameters_(parameters);
    for (size_t i = 0; i < parameters.size(); ++i)
    {
      independentParameters_.shareParameter(getParameter(getParameterNameWithoutNamespace(parameters[i].getName())));
    }
  }


  void deleteParameter_(size_t index)
  {
    std::string name = getParameter_(index).getName();
    AbstractParametrizable::deleteParameter_(index);
    if (independentParameters_.hasParameter(name))
      independentParameters_.deleteParameter(name);
  }

  void deleteParameter_(std::string& name)
  {
    AbstractParametrizable::deleteParameter_(name);
    if (independentParameters_.hasParameter(name))
      independentParameters_.deleteParameter(name);
  }

  void deleteParameters_(const std::vector<std::string>& names)
  {
    std::string x;
    for (size_t i = 0; i < names.size(); ++i)
    {
      x = names[i];
      deleteParameter_(x);
    }
  }

  void resetParameters_()
  {
    AbstractParametrizable::resetParameters_();
    independentParameters_.reset();
  }
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_ABSTRACTPARAMETERALIASABLE_H

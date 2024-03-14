// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_PARAMETERALIASABLE_H
#define BPP_NUMERIC_PARAMETERALIASABLE_H


#include "ParameterExceptions.h"
#include "ParameterList.h"
#include "Parametrizable.h"

// From the STL:
#include <string>
#include <map>

namespace bpp
{
/**
 * @brief Extend the Parametrizable interface with support for parameter aliases.
 *
 * Parameter aliases allows several parameter to be constrained together, and
 * for instance, be jointly estimated.
 * The alias relationship is not symmetric:
 * @code
 * aliasParameters("a","b");
 * @endcode
 * results in the value of "b" being updated when a is modified, but a will not be updated upon modification of "a".
 * "b" will also be removed of the list of "independent" parameters.
 * Furthermore, a parameter can only be aliased with another one:
 * @code
 * aliasParameters("a","b");
 * aliasParameters("c","b"); //ERROR, throws an exception.
 * @endcode
 * However, several parameters can be aliased to the same one:
 * @code
 * aliasParameters("a","b");
 * aliasParameters("a","c");
 * @endcode
 * In this case, modifying "a" will automatically update the values of "b" and "c", and "b" and "c" are removed from the list of indepedent parameters.
 * Finally, parameters can be chained:
 * @code
 * aliasParameters("a","b");
 * aliasParameters("b","c");
 * @endcode
 * is equivallent to the previous example.
 */
class ParameterAliasable :
  public virtual Parametrizable
{
public:
  ParameterAliasable() {}
  virtual ~ParameterAliasable() {}

public:
  /**
   * @brief Get the number of independent parameters.
   *
   * @return The number of independent parameters.
   * If no parameters are aliased, this is equivalent to the getNumberOfParameters() method.
   */
  virtual size_t getNumberOfIndependentParameters() const = 0;

  /**
   * @brief Set two parameters as 'aliased'.
   *
   * The values of the two parameters will be synchronized, so that setting the value of one parameter will automatically set the value of the other one accordingly.
   * @param p1 Original parameter.
   * @param p2 Aliased parameter.
   * @throw ParameterNotFoundException if p1 or p2 do not correspond to existing parameters.
   * @throw Exception when trying to perform non-valid association.
   */
  virtual void aliasParameters(const std::string& p1, const std::string& p2) = 0;

  /**
   * @brief alias the parameters following the links described in a
   * map, and update the object accordingly. Cycles in aliasing are
   * detected and forbidden.
   *
   * @param unparsedParams the map of the links : <A,B> matches for A->B aliasing.
   * @param verbose verbosity
   *
   **/

  virtual void aliasParameters(std::map<std::string, std::string>& unparsedParams, bool verbose) = 0;

  /**
   * @brief Detach two parameters previously set as 'aliased'.
   *
   * The values of the two parameters will now be independent.
   * @param p1 Original parameter.
   * @param p2 Aliased parameter.
   * @throw ParameterNotFoundException if p1 or p2 do not correspond to existing parameters.
   * @throw Exception when trying to perform non-valid dissociation.
   */
  virtual void unaliasParameters(const std::string& p1, const std::string& p2)  = 0;

  /**
   * @brief Get the minimal list of parameters to set the model.
   *
   * If no parameters are aliased, this is the same a getParameters().
   *
   * @return A minimal set of parameters.
   */
  virtual const ParameterList& getIndependentParameters() const = 0;

  /**
   * @return The list of names of the parameters that are aliased with a given parameter.
   * Depending on the implementation, the function may be recursive or not...
   * @param name The name of the parameter to look for.
   */
  virtual std::vector<std::string> getAlias(const std::string& name) const = 0;

  /**
   * @return the map of the aliases.
   *
   **/

  virtual std::map<std::string, std::string> getAliases() const = 0;
};


/**
 * @brief A low-level implementation of the ParameterAliasable interface with void functions.
 *
 * @see Parameter, ParameterList, ParameterAliasable
 */
class ParameterAliasableAdapter :
  public ParametrizableAdapter
{
public:
  ParameterAliasableAdapter() {}
  virtual ~ParameterAliasableAdapter() {}

public:
  /**
   * @name The ParameterAliasable interface.
   *
   * @{
   */
  const ParameterList& getIndependentParameters() const { return getParameters(); }
  void aliasParameters(const std::string& p1, const std::string& p2) {}
  void unaliasParameters(const std::string& p1, const std::string& p2) {}
  unsigned int getNumberOfIndependentParameters() const { return 0; }
  std::vector<std::string> getAlias(const std::string& name) const { return std::vector<std::string>(); }
  std::map<std::string, std::string> getAliases() const { return std::map<std::string, std::string>(); }
  /** @} */
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_PARAMETERALIASABLE_H

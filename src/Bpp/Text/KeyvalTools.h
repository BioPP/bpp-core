// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_TEXT_KEYVALTOOLS_H
#define BPP_TEXT_KEYVALTOOLS_H


#include "../Exceptions.h"
#include "StringTokenizer.h"
#include "TextTools.h"

// From the STL:
#include <map>

namespace bpp
{
/**
 * @brief Exception thrown by the Keyval parser.
 */
class KeyvalException :
  public Exception
{
public:
  KeyvalException(const std::string& text) : Exception(text) {}
};

/**
 * @brief Tools to deal with the keyval syntax.
 *
 * This class contains method to deal with parameter=value syntax procedure.
 * A keyval procedure takes the form
 * @code
 * proc(p1=v1,p2=v2,p3=v3,etc)
 * @endcode
 * where 'p' are parameter names, and 'v' are the corresponding values.
 * These values can be nested keyval procedures.
 */
class KeyvalTools
{
public:
  KeyvalTools();
  virtual ~KeyvalTools();

public:
  /**
   * @brief Split a string into a key and a value (General purpose function).
   *
   * @param desc  [in]  A string descibing the keyval, with format key=val (space are considered normal character, that's up to you to deal with that afterward!).
   * @param key   [out] Will contain the text of the key.
   * @param val   [out] Will contain the text of the value.
   * @param split [in]  The delimiter. Default is '=' but ':' can be used.
   * @throw KeyvalException If the syntax describing the keyval is not correct.
   */

  static void singleKeyval(const std::string& desc, std::string& key, std::string& val, const std::string& split = "=");

  /**
   * @brief Split a string into several keys and corresponding values (General purpose function).
   *
   * @param desc [in]  A string descibing the keyval, with format key1=val1,key2=val2,etc (space are considered normal character, that's up to you to deal with that afterward!).
   * @param keyvals [out] Will contain the text of the keys and their corresponding values.
   * @param split [in] The keyval delimiter.the default is a coma, but a space character can be used for instance.
   * @param nested [in] Tell if nested keyval procedures are expected.
   * @throw KeyvalException If the syntax describing the keyval is not correct.
   */

  static void multipleKeyvals(const std::string& desc, std::map<std::string, std::string>& keyvals, const std::string& split = ",", bool nested = true);

  /**
   * @brief Change several keys to new corresponding values (General
   * purpose function).
   *
   * @param desc [in]  A string descibing the keyval, with format
   * key1=val1,key2=val2,etc (space are considered normal character,
   * that's up to you to deal with that afterwards!).
   * @param newkeyvals [in] contains the text of the keys to be changed
   * and their new corresponding values. If a key is not in desc, it
   * is not added.
   * @param split [in] The keyval delimiter. The default is a coma,
   * but a space character can be used for instance.
   * @param nested [in] Tell if nested keyval procedures are expected.
   * @return the string with the changed values.
   * @throw KeyvalException If the syntax describing the keyval is not correct.
   */

  static std::string changeKeyvals(const std::string& desc, const std::map<std::string, std::string>& newkeyvals, const std::string& split = ",", bool nested = true);

  /**
   * @brief Parse (not recursively) a procedure string.
   *
   * @param desc [in]  A string descibing the keyval procedure.
   * @param name [out] Outputs the name of the procedure.
   * @param args [out] Fills a map with all keys and values for parameters.
   * @throw KeyvalException If the description is invalid (one parenthesis is missing for instance).
   */
  static void parseProcedure(const std::string& desc, std::string& name, std::map<std::string, std::string>& args);
};
} // End of namespace bpp.
#endif // BPP_TEXT_KEYVALTOOLS_H

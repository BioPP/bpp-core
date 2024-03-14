// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_IOPARAMETRIZABLE_H
#define BPP_IO_IOPARAMETRIZABLE_H

#include <map>

#include "../Numeric/ParameterAliasable.h"
#include "IoFormat.h"
#include "OutputStream.h"

namespace bpp
{
/**
 * @brief General interface for parametrizable Output.
 */
class IOParametrizable :
  public virtual IOFormat
{
public:
  IOParametrizable() {}
  virtual ~IOParametrizable() {}

public:
  virtual const std::string getDataType() const { return "Parametrizable"; }
};

/**
 * @brief General interface writers.
 */
class OParametrizable :
  public virtual IOParametrizable
{
public:
  OParametrizable() {}
  virtual ~OParametrizable() {}

public:
  /**
   * @brief Write a Parametrizable to a stream.
   *
   * @param parametrizable A Parametrizable object;
   * @param out The output stream;
   * @param writtenNames is the vector of the written
   *        parameters so far [in, out];
   * @param printComma boolean if a comma should be written at the
   *        beginning of the description.
   */
  virtual void write(const Parametrizable& parametrizable,
                     OutputStream& out,
                     std::vector<std::string>& writtenNames,
                     bool printComma = false) const = 0;

  /**
   * @brief Write a ParameterAliasable to a stream.
   *
   * @param parametrizable A ParameterAliasable object;
   * @param out The output stream;
   * @param globalAliases parameters linked to global alias;
   * @param names the names of the parameters to be written;
   * @param writtenNames is the vector of the written
   *        parameters so far [in, out];
   * @param printLocalAliases boolean if local aliases should be written;
   * @param printComma boolean if a comma should be written at the
   *        beginning of the description.
   */
  virtual void write(const ParameterAliasable& parametrizable,
                     OutputStream& out,
                     std::map<std::string, std::string>& globalAliases,
                     const std::vector<std::string>& names,
                     std::vector<std::string>& writtenNames,
                     bool printLocalAliases = true,
                     bool printComma = false) const = 0;
};
} // end of namespace bpp.
#endif // BPP_IO_IOPARAMETRIZABLE_H

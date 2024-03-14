// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_BPPOPARAMETRIZABLEFORMAT_H
#define BPP_IO_BPPOPARAMETRIZABLEFORMAT_H


#include "IoParametrizable.h"

namespace bpp
{
/**
 * @brief Parametrizable output in BppO format.
 *
 * Writes a new parametrizable object according to BppO description
 * syntax (see the Bio++ Progam Suite manual for a detailed
 * description of this syntax).
 *
 */
class BppOParametrizableFormat :
  public virtual OParametrizable
{
public:
  BppOParametrizableFormat() {}
  virtual ~BppOParametrizableFormat() {}

public:
  const std::string getFormatName() const override { return "BppO"; }

  const std::string getFormatDescription() const override { return "Bpp Options format."; }

  void write(const Parametrizable& parametrizable,
             OutputStream& out,
             std::vector<std::string>& writtenNames,
             bool printComma = false) const override;

  void write(const ParameterAliasable& parametrizable,
             OutputStream& out,
             std::map<std::string, std::string>& globalAliases,
             const std::vector<std::string>& names,
             std::vector<std::string>& writtenNames,
             bool printLocalAliases = true,
             bool printComma = false) const override;
};
} // end of namespace bpp.
#endif // BPP_IO_BPPOPARAMETRIZABLEFORMAT_H

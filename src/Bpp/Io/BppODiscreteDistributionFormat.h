// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_BPPODISCRETEDISTRIBUTIONFORMAT_H
#define BPP_IO_BPPODISCRETEDISTRIBUTIONFORMAT_H


#include "IoDiscreteDistributionFactory.h"

namespace bpp
{
/**
 * @brief Discrete Distribution I/O in BppO format.
 *
 * Creates a new discrete distribution object according to
 * distribution description syntax (see the Bio++ Progam Suite
 * manual for a detailed description of this syntax).
 *
 */
class BppODiscreteDistributionFormat :
  public virtual IDiscreteDistribution,
  public virtual ODiscreteDistribution
{
protected:
  bool verbose_;
  std::map<std::string, std::string> unparsedArguments_;

public:
  BppODiscreteDistributionFormat(bool verbose = true) : verbose_(verbose), unparsedArguments_() {}
  virtual ~BppODiscreteDistributionFormat() {}

public:
  const std::string getFormatName() const { return "BppO"; }

  const std::string getFormatDescription() const { return "Bpp Options format."; }

  std::unique_ptr<DiscreteDistributionInterface> readDiscreteDistribution(const std::string& distDescription, bool parseArguments = true);

  const std::map<std::string, std::string>& getUnparsedArguments() const { return unparsedArguments_; }

  void writeDiscreteDistribution(
      const DiscreteDistributionInterface& dist,
      OutputStream& out,
      std::map<std::string, std::string>& globalAliases,
      std::vector<std::string>& writtenNames) const;

protected:
  /**
   * @brief Set parameter initial values of a given distribution according to options.
   *
   * Parameters actually depends on the distribution passed as argument.
   *
   * @param rDist The distribution to set up.
   * @throw Exception if an error occured.
   */
  void initialize_(DiscreteDistributionInterface& rDist);
};
} // end of namespace bpp.
#endif // BPP_IO_BPPODISCRETEDISTRIBUTIONFORMAT_H

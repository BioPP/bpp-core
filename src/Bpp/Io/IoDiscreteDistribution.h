// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_IODISCRETEDISTRIBUTION_H
#define BPP_IO_IODISCRETEDISTRIBUTION_H


#include "../Exceptions.h"
#include "../Numeric/Prob/DiscreteDistribution.h"
#include "IoFormat.h"
#include "OutputStream.h"

namespace bpp
{
/**
 * @brief General interface for model I/O.
 */
class IoDiscreteDistribution :
  public virtual IOFormat
{
public:
  IoDiscreteDistribution() {}
  virtual ~IoDiscreteDistribution() {}

public:
  virtual const std::string getDataType() const { return "Discrete Distribution"; }
};

/**
 * @brief General interface for distance matrix readers.
 */
class IDiscreteDistribution :
  public virtual IoDiscreteDistribution
{
public:
  IDiscreteDistribution() {}
  virtual ~IDiscreteDistribution() {}

public:
  /**
   * @brief Read a discrete distribution from a string.
   *
   * @param distrDescription A string describing the distribution in the format.
   * @param parseArguments Attempt to parse function arguments. If not, only store them and use default values instead.
   * @return A new DiscreteDistribution object according to options specified.
   * @throw Exception if an error occured.
   */
  virtual std::unique_ptr<DiscreteDistributionInterface> readDiscreteDistribution(
    const std::string& distrDescription,
    bool parseArguments = true) = 0;

  /**
   * @return The arguments and their unparsed values from the last call of the read function, if there are any.
   */
  virtual const std::map<std::string, std::string>& getUnparsedArguments() const = 0;
};

/**
 * @brief General interface writers.
 */
class ODiscreteDistribution :
  public virtual IoDiscreteDistribution
{
public:
  ODiscreteDistribution() {}
  virtual ~ODiscreteDistribution() {}

public:
  /**
   * @brief Write a discrete distribution to a stream.
   *
   * @param dist A discrete distribution object;
   * @param out The output stream;
   * @param globalAliases parameters linked to global alias.
   * @param writtenNames is the vector of the written
   *        parameters so far [in, out];
   * @throw Exception if an error occured.
   */
  virtual void writeDiscreteDistribution(
    const DiscreteDistributionInterface& dist,
    OutputStream& out,
    std::map<std::string, std::string>& globalAliases,
    std::vector<std::string>& writtenNames) const = 0;
};
} // end of namespace bpp.
#endif // BPP_IO_IODISCRETEDISTRIBUTION_H

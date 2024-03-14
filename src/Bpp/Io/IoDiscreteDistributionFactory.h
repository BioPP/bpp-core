// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_IODISCRETEDISTRIBUTIONFACTORY_H
#define BPP_IO_IODISCRETEDISTRIBUTIONFACTORY_H


#include "../Exceptions.h"
#include "../Numeric/Prob/DiscreteDistribution.h"
#include "IoDiscreteDistribution.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief Utilitary class for creating discrete distribution readers and
 * writers.
 */
class IoDiscreteDistributionFactory
{
public:
  static const std::string BPPO_FORMAT;

public:
  /**
   * @brief Creates a new factory object.
   *
   * Example:
   * @code
   * IDiscreteDistribution* distReader = IoDiscreteDistributionFactory().createReader(IoDiscreteDistributionFactory::BPP_FORMAT);
   * DiscreteDistribution* dist = distReader->read(...);
   * delete distReader;
   * @endcode
   */
  IoDiscreteDistributionFactory() {}
  virtual ~IoDiscreteDistributionFactory() {}

  /**
   * @brief Get a new dynamically created IDiscreteDistribution object.
   *
   * @param format The input file format.
   * @return A pointer toward a new IDiscreteDistribution object.
   * @throw Exception If the format name do not match any available format.
   */
  virtual IDiscreteDistribution* createReader(const std::string& format);

  /**
   * @brief Get a new dynamically created ODiscreteDistribution object.
   *
   * @param format The output file format.
   * @return A pointer toward a new ODiscreteDistribution object.
   * @throw Exception If the format name do not match any available format.
   */
  virtual ODiscreteDistribution* createWriter(const std::string& format);
};
} // end of namespace bpp.
#endif // BPP_IO_IODISCRETEDISTRIBUTIONFACTORY_H

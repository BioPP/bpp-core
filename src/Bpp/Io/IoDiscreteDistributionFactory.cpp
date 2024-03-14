// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "BppODiscreteDistributionFormat.h"

using namespace bpp;

const std::string IoDiscreteDistributionFactory::BPPO_FORMAT = "Bpp0";

IDiscreteDistribution* IoDiscreteDistributionFactory::createReader(const std::string& format)
{
  if (format == BPPO_FORMAT)
    return new BppODiscreteDistributionFormat();
  else
    throw Exception("Format " + format + " is not supported for input.");
}

ODiscreteDistribution* IoDiscreteDistributionFactory::createWriter(const std::string& format)
{
  if (format == BPPO_FORMAT)
    return new BppODiscreteDistributionFormat();
  else
    throw Exception("Format " + format + " is not supported for output.");
}

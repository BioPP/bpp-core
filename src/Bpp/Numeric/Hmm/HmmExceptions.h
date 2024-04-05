// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_HMM_HMMEXCEPTIONS_H
#define BPP_NUMERIC_HMM_HMMEXCEPTIONS_H


#include "../../Exceptions.h"

namespace bpp
{
/**
 * @brief Exception thrown when an unvalid state is requested.
 *
 * This class belongs to the HMM framework.
 *
 * @author Julien Dutheil
 */
class HmmBadStateException :
  public Exception
{
public:
  HmmBadStateException(const std::string& description) : Exception(description) {}
};


/**
 * @brief Exception thrown when an unvalid alphabet is specified.
 *
 * This class belongs to the HMM fra;ework.
 *
 * @author Julien Dutheil
 */
class HmmUnvalidAlphabetException :
  public Exception
{
public:
  HmmUnvalidAlphabetException(const std::string& description) : Exception(description) {}
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_HMM_HMMEXCEPTIONS_H

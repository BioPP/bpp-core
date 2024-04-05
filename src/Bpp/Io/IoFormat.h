// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_IOFORMAT_H
#define BPP_IO_IOFORMAT_H


#include "../Exceptions.h"

// From STL:
#include <string>

namespace bpp
{
/**
 * @brief The IOFormat interface.
 *
 * This is the most basal class of any format I/O implementation.
 */
class IOFormat
{
public:
  IOFormat() {}
  virtual ~IOFormat() {}

public:
  /**
   * @brief Get the type of data this format deals with.
   *
   * @return The type of data.
   */
  virtual const std::string getDataType() const = 0;

  /**
   * @brief Get the name of the file format.
   *
   * @return The name of the format implemented.
   */
  virtual const std::string getFormatName() const = 0;

  /**
   * @brief Get a description of the file format.
   *
   * @return A description of the format implemented.
   */
  virtual const std::string getFormatDescription() const = 0;
};
} // end of namespace bpp.
#endif // BPP_IO_IOFORMAT_H

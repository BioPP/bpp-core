// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_NUMERIC_TABLEEXCEPTIONS_H
#define BPP_NUMERIC_TABLEEXCEPTIONS_H


// #include "VectorTools.h"

// From Utils:
#include "../Exceptions.h"
#include "../Text/TextTools.h"

// From the STL:
#include <string>

namespace bpp
{
/**
 * @brief Exception thrown when a given name is not found is a DataTable object.
 */
class TableNameNotFoundException :
  public Exception
{
protected:
  std::string _name;

public:
  TableNameNotFoundException(const std::string& text, const std::string& name) :
    Exception("TableNameNotFoundException: " + name + ". " + text), _name(name) {}
  virtual ~TableNameNotFoundException() {}

public:
  std::string getName() const { return _name; }
};

/**
 * @brief Exception thrown when a given row name is not found is a DataTable object.
 */
class TableRowNameNotFoundException :
  public TableNameNotFoundException
{
public:
  TableRowNameNotFoundException(const std::string& text, const std::string& name) :
    TableNameNotFoundException("TableRowNameNotFoundException: " + name + ". " + text, name) {}
  virtual ~TableRowNameNotFoundException() {}
};

/**
 * @brief Exception thrown when a given column name is not found is a DataTable object.
 */
class TableColumnNameNotFoundException :
  public TableNameNotFoundException
{
public:
  TableColumnNameNotFoundException(const std::string& text, const std::string& name) :
    TableNameNotFoundException("TableColumnNameNotFoundException: " + name + ". " + text, name) {}
  virtual ~TableColumnNameNotFoundException() {}
};

/**
 * @brief Exception thrown when trying to retrieve a row by its name
 * and no row names have been specified.
 */
class NoTableRowNamesException :
  public Exception
{
public:
  NoTableRowNamesException(const std::string& text) :
    Exception("NoTableRowNamesException: " + text) {}
  virtual ~NoTableRowNamesException() {}
};

/**
 * @brief Exception thrown when trying to retrieve a column by its name
 * and no column names have been specified.
 */
class NoTableColumnNamesException :
  public Exception
{
public:
  NoTableColumnNamesException(const std::string& text) :
    Exception("NoTableColumnNamesException: " + text) {}
  virtual ~NoTableColumnNamesException() {}
};

/**
 * @brief General exception class dealing with row names.
 */
class TableRowNamesException :
  public Exception
{
public:
  TableRowNamesException(const std::string& text) :
    Exception("TableRowNamesException: " + text) {}
  virtual ~TableRowNamesException() {}
};

/**
 * @brief General exception class dealing with column names.
 */
class TableColumnNamesException :
  public Exception
{
public:
  TableColumnNamesException(const std::string& text) :
    Exception("TableColumnNamesException: " + text) {}
  virtual ~TableColumnNamesException() {}
};

/**
 * @brief Exception thrown when attempting to duplicate a row name.
 */
class DuplicatedTableRowNameException :
  public Exception
{
public:
  DuplicatedTableRowNameException(const std::string& text) :
    Exception("DuplicatedTableRowNameException: " + text) {}
  virtual ~DuplicatedTableRowNameException() {}
};

/**
 * @brief Excpetion thrown when attempting to duplicate a column name.
 */
class DuplicatedTableColumnNameException :
  public Exception
{
public:
  DuplicatedTableColumnNameException(const std::string& text) :
    Exception("DuplicatedTableColumnNameException: " + text) {}
  virtual ~DuplicatedTableColumnNameException() {}
};
} // end of namespace bpp.
#endif // BPP_NUMERIC_TABLEEXCEPTIONS_H

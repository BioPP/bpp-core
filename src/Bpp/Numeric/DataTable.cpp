// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../Io/FileTools.h"
#include "../Text/StringTokenizer.h"
#include "../Text/TextTools.h"
#include "DataTable.h"
#include "VectorTools.h"

#include <memory>

using namespace bpp;
using namespace std;

/******************************************************************************/

DataTable::DataTable(size_t nRow, size_t nCol) :
  nRow_(nRow),
  nCol_(nCol),
  data_(nCol),
  rowNames_(0),
  colNames_(0)
{
  for (size_t i = 0; i < nCol; i++)
  {
    data_[i].resize(nRow);
  }
}

DataTable::DataTable(size_t nCol) :
  nRow_(0),
  nCol_(nCol),
  data_(nCol),
  rowNames_(0),
  colNames_(0)
{}

DataTable::DataTable(size_t nRow, const std::vector<std::string>& colNames) :
  nRow_(nRow),
  nCol_(colNames.size()),
  data_(colNames.size()),
  rowNames_(0),
  colNames_(0)
{
  for (size_t i = 0; i < nCol_; i++)
  {
    data_[i].resize(nRow);
  }

  setColumnNames(colNames); // May throw an exception.
}

DataTable::DataTable(const std::vector<std::string>& colNames) :
  nRow_(0),
  nCol_(colNames.size()),
  data_(colNames.size()),
  rowNames_(0),
  colNames_(0)

{
  setColumnNames(colNames); // May throw an exception.
}

DataTable::DataTable(const DataTable& table) :
  nRow_(table.nRow_),
  nCol_(table.nCol_),
  data_(table.data_),
  rowNames_(0),
  colNames_(0)
{
  if (table.rowNames_.size())
    rowNames_ = table.rowNames_;
  if (table.colNames_.size())
    colNames_ = table.colNames_;
}

DataTable& DataTable::operator=(const DataTable& table)
{
  nRow_ = table.nRow_;
  nCol_ = table.nCol_;
  data_ = table.data_;
  if (table.rowNames_.size())
    rowNames_ = table.rowNames_;
  if (table.colNames_.size())
    colNames_ = table.colNames_;
  return *this;
}

/******************************************************************************/

DataTable::~DataTable()
{}

/******************************************************************************/
/*                             Cell access                                    */
/******************************************************************************/

string& DataTable::operator()(size_t rowIndex, size_t colIndex)
{
  if (colIndex >= nCol_)
    throw IndexOutOfBoundsException("DataTable::operator(size_t, size_t).", colIndex, 0, nCol_ - 1);
  if (rowIndex >= data_[colIndex].size())
    throw IndexOutOfBoundsException("DataTable::operator(size_t, size_t).", rowIndex, 0, data_[colIndex].size() - 1);
  return data_[colIndex][rowIndex];
}

const string& DataTable::operator()(size_t rowIndex, size_t colIndex) const
{
  if (colIndex >= nCol_)
    throw IndexOutOfBoundsException("DataTable::operator(size_t, size_t).", colIndex, 0, nCol_ - 1);
  if (rowIndex >= data_[colIndex].size())
    throw IndexOutOfBoundsException("DataTable::operator(size_t, size_t).", rowIndex, 0, data_[colIndex].size() - 1);
  return data_[colIndex][rowIndex];
}

/******************************************************************************/

string& DataTable::operator()(const string& rowName, const string& colName)
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::operator(const string &, const string &).");
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::operator(const string &, const string &).");
  try
  {
    size_t rowIndex = VectorTools::which(rowNames_, rowName);
    size_t colIndex = VectorTools::which(colNames_, colName);
    return (*this)(rowIndex, colIndex);
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableNameNotFoundException("DataTable::operator(const string &, const string &).", *ex.getElement());
  }
}

const string& DataTable::operator()(const string& rowName, const string& colName) const
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::operator(const string &, const string &).");
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::operator(const string &, const string &).");
  try
  {
    size_t rowIndex = VectorTools::which(rowNames_, rowName);
    size_t colIndex = VectorTools::which(colNames_, colName);
    return (*this)(rowIndex, colIndex);
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableNameNotFoundException("DataTable::operator(const string &, const string &).", *ex.getElement());
  }
}

/******************************************************************************/

string& DataTable::operator()(const string& rowName, size_t colIndex)
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::operator(const string &, size_t).");
  if (colIndex >= nCol_)
    throw IndexOutOfBoundsException("DataTable::operator(const string &, size_t).", colIndex, 0, nCol_ - 1);
  try
  {
    size_t rowIndex = VectorTools::which(rowNames_, rowName);
    return (*this)(rowIndex, colIndex);
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableNameNotFoundException("DataTable::operator(const string &, size_t).", *ex.getElement());
  }
}

const string& DataTable::operator()(const string& rowName, size_t colIndex) const
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::operator(const string &, size_t).");
  if (colIndex >= nCol_)
    throw IndexOutOfBoundsException("DataTable::operator(const string &, size_t).", colIndex, 0, nCol_ - 1);
  try
  {
    size_t rowIndex = VectorTools::which(rowNames_, rowName);
    return (*this)(rowIndex, colIndex);
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableNameNotFoundException("DataTable::operator(const string &, size_t).", *ex.getElement());
  }
}

/******************************************************************************/

string& DataTable::operator()(size_t rowIndex, const string& colName)
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::operator(size_t, const string &).");
  try
  {
    size_t colIndex = VectorTools::which(colNames_, colName);
    if (rowIndex >= data_[colIndex].size())
      throw IndexOutOfBoundsException("DataTable::operator(size_t, const string &).", rowIndex, 0, data_[colIndex].size() - 1);
    return (*this)(rowIndex, colIndex);
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableNameNotFoundException("DataTable::operator(const string &, const string &).", *ex.getElement());
  }
}

const string& DataTable::operator()(size_t rowIndex, const string& colName) const
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::operator(size_t, const string &).");
  try
  {
    size_t colIndex = VectorTools::which(colNames_, colName);
    if (rowIndex >= data_[colIndex].size())
      throw IndexOutOfBoundsException("DataTable::operator(size_t, const string &).", rowIndex, 0, data_[colIndex].size() - 1);
    return (*this)(rowIndex, colIndex);
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableNameNotFoundException("DataTable::operator(const string &, const string &).", *ex.getElement());
  }
}

/******************************************************************************/
/*                             Work with names                                */
/******************************************************************************/

void DataTable::setRowNames(const vector<string>& rowNames)
{
  if (!VectorTools::isUnique(rowNames))
  {
    throw DuplicatedTableRowNameException("DataTable::setRowNames(...). Row names must be unique.");
  }
  if (rowNames.size() != nRow_)
    throw DimensionException("DataTable::setRowNames.", rowNames.size(), nRow_);
  else
  {
    rowNames_ = rowNames;
  }
}

void DataTable::setRowName(size_t rowId, const string& rowName)
{
  if (VectorTools::contains(rowNames_, rowName))
  {
    throw DuplicatedTableRowNameException("DataTable::setRowName(...). New row name " + rowName + " already exists");
  }
  if (rowId >= nRow_)
    throw DimensionException("DataTable::setRowName.", rowId, nRow_);
  else
  {
    (rowNames_)[rowId] = rowName;
  }
}

vector<string> DataTable::getRowNames() const
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::getRowNames().");
  return rowNames_;
}

string DataTable::getRowName(size_t index) const
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::getRowName(size_t).");
  if (index >= nRow_)
    throw IndexOutOfBoundsException("DataTable::getRowName(size_t).", index, 0, nRow_ - 1);
  return (rowNames_)[index];
}

/******************************************************************************/

void DataTable::setColumnNames(const vector<string>& colNames)
{
  if (!VectorTools::isUnique(colNames))
    throw DuplicatedTableColumnNameException("DataTable::setColumnNames(...). Column names must be unique.");
  if (colNames.size() != nCol_)
    throw DimensionException("DataTable::setColumnNames.", colNames.size(), nCol_);
  else
  {
    colNames_ = colNames;
  }
}

vector<string> DataTable::getColumnNames() const
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::getColumnNames().");
  return colNames_;
}

string DataTable::getColumnName(size_t index) const
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::getColumnName(size_t).");
  if (index >= nCol_)
    throw IndexOutOfBoundsException("DataTable::getColumnName(size_t).", index, 0, nCol_ - 1);
  return (colNames_)[index];
}

/******************************************************************************/
/*                               Work on columns                              */
/******************************************************************************/

vector<string>& DataTable::getColumn(size_t index)
{
  if (index >= nCol_)
    throw IndexOutOfBoundsException("DataTable::getColumn(size_t).", index, 0, nCol_ - 1);
  return data_[index];
}

const vector<string>& DataTable::getColumn(size_t index) const
{
  if (index >= nCol_)
    throw IndexOutOfBoundsException("DataTable::getColumn(size_t).", index, 0, nCol_ - 1);
  return data_[index];
}

vector<string>& DataTable::getColumn(const string& colName)
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::getColumn(const string &).");
  try
  {
    size_t colIndex = VectorTools::which(colNames_, colName);
    return data_[colIndex];
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableColumnNameNotFoundException("DataTable::getColumn(const string &).", colName);
  }
}

const vector<string>& DataTable::getColumn(const string& colName) const
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::getColumn(const string &).");
  try
  {
    size_t colIndex = VectorTools::which(colNames_, colName);
    return data_[colIndex];
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableColumnNameNotFoundException("DataTable::getColumn(const string &).", colName);
  }
}

bool DataTable::hasColumn(const string& colName) const
{
  if (colNames_.size() == 0)
    return false;
  for (size_t i = 0; i < colNames_.size(); i++)
  {
    if ((colNames_)[i] == colName)
      return true;
  }
  return false;
}

void DataTable::deleteColumn(size_t index)
{
  if (index >= nCol_)
    throw IndexOutOfBoundsException("DataTable::deleteColumn(size_t).", index, 0, nCol_ - 1);
  data_.erase(data_.begin() + static_cast<ptrdiff_t>(index));
  if (colNames_.size() != 0)
    colNames_.erase(colNames_.begin() + static_cast<ptrdiff_t>(index));
  nCol_--;
}

void DataTable::deleteColumn(const string& colName)
{
  if (colNames_.size() == 0)
    throw NoTableColumnNamesException("DataTable::deleteColumn(const string &).");
  try
  {
    size_t colIndex = VectorTools::which(colNames_, colName);
    data_.erase(data_.begin() + static_cast<ptrdiff_t>(colIndex));
    colNames_.erase(colNames_.begin() + static_cast<ptrdiff_t>(colIndex));
    nCol_--;
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableColumnNameNotFoundException("DataTable::deleteColumn(const string &).", colName);
  }
}

void DataTable::addColumn(const vector<string>& newColumn)
{
  if (colNames_.size() != 0)
    throw TableColumnNamesException("DataTable::addColumn. Table has column names.");
  if (newColumn.size() != nRow_)
    throw DimensionException("DataTable::addColumn.", newColumn.size(), nRow_);
  data_.push_back(newColumn);
  nCol_++;
}

void DataTable::addColumn(const string& colName, const vector<string>& newColumn)
{
  if (colNames_.size() == 0)
  {
    if (nCol_ == 0)
      colNames_ = vector<string>(0);
    else
      throw NoTableColumnNamesException("DataTable::addColumn. Table has column names.");
  }
  if (newColumn.size() != nRow_)
    throw DimensionException("DataTable::addColumn.", newColumn.size(), nRow_);
  if (nCol_ > 0 && find(colNames_.begin(), colNames_.end(), colName) != colNames_.end())
    throw DuplicatedTableColumnNameException("DataTable::addColumn(const string &, const vector<string> &). Column names must be unique.");
  colNames_.push_back(colName);
  data_.push_back(newColumn);
  nCol_++;
}

/******************************************************************************/
/*                               Work on rows                                 */
/******************************************************************************/

vector<string> DataTable::getRow(size_t index) const
{
  if (index >= nRow_)
    throw IndexOutOfBoundsException("DataTable::getRow(size_t).", index, 0, nRow_ - 1);
  vector<string> row;
  for (size_t i = 0; i < nCol_; i++)
  {
    row.push_back(data_[i][index]);
  }
  return row;
}

vector<string> DataTable::getRow(const string& rowName) const
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::getRow(const string &).");
  try
  {
    size_t rowIndex = VectorTools::which(rowNames_, rowName);
    vector<string> row;
    for (size_t i = 0; i < nCol_; i++)
    {
      row.push_back(data_[i][rowIndex]);
    }
    return row;
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableRowNameNotFoundException("DataTable::getRow(const string &).", rowName);
  }
}

bool DataTable::hasRow(const string& rowName) const
{
  if (rowNames_.size() == 0)
    return false;
  for (size_t i = 0; i < rowNames_.size(); i++)
  {
    if ((rowNames_)[i] == rowName)
      return true;
  }
  return false;
}

void DataTable::deleteRow(size_t index)
{
  for (size_t j = 0; j < nCol_; j++)
  {
    vector<string>* column = &data_[j];
    if (index >= column->size())
      throw IndexOutOfBoundsException("DataTable::deleteRow(size_t).", index, 0, column->size() - 1);
    column->erase(column->begin() + static_cast<ptrdiff_t>(index));
  }
  if (rowNames_.size() != 0)
    rowNames_.erase(rowNames_.begin() + static_cast<ptrdiff_t>(index));
  nRow_--;
}

void DataTable::deleteRow(const string& rowName)
{
  if (rowNames_.size() == 0)
    throw NoTableRowNamesException("DataTable::deleteRow(const string &).");
  try
  {
    size_t rowIndex = VectorTools::which(rowNames_, rowName);
    for (size_t j = 0; j < nCol_; j++)
    {
      vector<string>* column = &data_[j];
      column->erase(column->begin() + static_cast<ptrdiff_t>(rowIndex));
    }
    rowNames_.erase(rowNames_.begin() + static_cast<ptrdiff_t>(rowIndex));
    nRow_--;
  }
  catch (ElementNotFoundException<string>& ex)
  {
    throw TableRowNameNotFoundException("DataTable::deleteRow(const string &).", rowName);
  }
}

void DataTable::addRow(const vector<string>& newRow)
{
  if (rowNames_.size() != 0)
    throw TableRowNamesException("DataTable::addRow. Table has row names.");
  if (newRow.size() != nCol_)
    throw DimensionException("DataTable::addRow.", newRow.size(), nCol_);
  for (size_t j = 0; j < nCol_; j++)
  {
    data_[j].push_back(newRow[j]);
  }
  nRow_++;
}

void DataTable::setRow(size_t rowIndex, const vector<string>& newRow)
{
  if (rowIndex >= nRow_)
    throw DimensionException("DataTable::setRow.", rowIndex, nRow_);
  if (newRow.size() != nCol_)
    throw DimensionException("DataTable::setRow.", newRow.size(), nCol_);

  for (size_t j = 0; j < nCol_; j++)
  {
    data_[j][rowIndex] = newRow[j];
  }
}

void DataTable::addRow(const string& rowName, const vector<string>& newRow)
{
  if (rowNames_.size() == 0)
  {
    if (nRow_ == 0)
      rowNames_ = vector<string>(0);
    else
      throw NoTableRowNamesException("DataTable::addRow. Table has row names.");
  }
  if (newRow.size() != nCol_)
    throw DimensionException("DataTable::addRow.", newRow.size(), nCol_);
  if (nRow_ > 0 && find(rowNames_.begin(), rowNames_.end(), rowName) != rowNames_.end())
    throw DuplicatedTableRowNameException("DataTable::addRow(const string &, const vector<string> &). Row names must be unique.");
  rowNames_.push_back(rowName);
  for (size_t j = 0; j < nCol_; j++)
  {
    data_[j].push_back(newRow[j]);
  }
  nRow_++;
}

/******************************************************************************/
/*                               Read from a CSV file                         */
/******************************************************************************/

unique_ptr<DataTable> DataTable::read(istream& in, const string& sep, bool header, int rowNames)
{
  string firstLine  = FileTools::getNextLine(in);
  const string sept(sep=="\\t"?"\t":sep);

  StringTokenizer st1(firstLine, sept, false, true);
  vector<string> row1(st1.getTokens().begin(), st1.getTokens().end());
  string secondLine = FileTools::getNextLine(in);
  StringTokenizer st2(secondLine, sept, false, true);
  vector<string> row2(st2.getTokens().begin(), st2.getTokens().end());
  size_t nCol = row1.size();
  bool hasRowNames;
  unique_ptr<DataTable> dt;
  if (row1.size() == row2.size())
  {
    dt = make_unique<DataTable>(nCol);
    if (header)
    { // Use first line as header.
      dt->setColumnNames(row1);
    }
    else
    {
      dt->addRow(row1);
    }
    dt->addRow(row2);
    hasRowNames = false;
  }
  else if (row1.size() == row2.size() - 1)
  {
    dt = make_unique<DataTable>(nCol);
    dt->setColumnNames(row1);
    string rowName = *row2.begin();
    dt->addRow(rowName, vector<string>(row2.begin() + 1, row2.end()));
    hasRowNames = true;
  }
  else
    throw DimensionException("DataTable::read(...). Row 2 has not the correct number of columns.", row2.size(), nCol);

  // Now read each line:
  string line = FileTools::getNextLine(in);
  while (!TextTools::isEmpty(line))
  {
    StringTokenizer st(line, sept, false, true);
    if (hasRowNames)
    {
      string rowName = *st.getTokens().begin();
      vector<string> row(st.getTokens().begin() + 1, st.getTokens().end());
      dt->addRow(rowName, row);
    }
    else
    {
      vector<string> row(st.getTokens().begin(), st.getTokens().end());
      dt->addRow(row);
    }
    line = FileTools::getNextLine(in);
  }

  // Row names:
  if (rowNames > -1)
  {
    if (static_cast<size_t>(rowNames) >= nCol)
      throw IndexOutOfBoundsException("DataTable::read(...). Invalid column specified for row names.", static_cast<size_t>(rowNames), 0, nCol - 1);
    vector<string> col = dt->getColumn(static_cast<size_t>(rowNames));
    dt->setRowNames(col);
    dt->deleteColumn(static_cast<size_t>(rowNames));
  }

  return dt;
}

/******************************************************************************/

void DataTable::write(const DataTable& data, ostream& out, const string& sep, bool alignHeaders)
{
  const string sept(sep=="\\t"?"\t":sep);

  size_t n = data.getNumberOfColumns();
  if (n == 0)
    return;
  if (data.hasColumnNames())
  { // Write header
    vector<string> colNames = data.getColumnNames();
    if (alignHeaders && data.hasRowNames())
      out << sept;
    out << colNames[0];
    for (size_t i = 1; i < n; i++)
    {
      out << sept << colNames[i];
    }
    out << endl;
  }
  // Now write each row:
  for (size_t i = 0; i < data.getNumberOfRows(); i++)
  {
    if (data.hasRowNames())
      out << data.getRowName(i) << sept;
    out << data(i, 0);
    for (size_t j = 1; j < n; j++)
    {
      out << sept << data(i, j);
    }
    out << endl;
  }
}

void DataTable::write(const DataTable& data, bpp::OutputStream& out, const string& sep, bool alignHeaders)
{
  const string sept(sep=="\\t"?"\t":sep);

  size_t n = data.getNumberOfColumns();
  if (n == 0)
    return;
  
  if (data.hasColumnNames())
  { // Write header
    vector<string> colNames = data.getColumnNames();
    if (alignHeaders && data.hasRowNames())
      out << sept;
    out << colNames[0];
    for (size_t i = 1; i < n; i++)
    {
      out << sept << colNames[i];
    }
    out.endLine();
  }
  // Now write each row:
  for (size_t i = 0; i < data.getNumberOfRows(); i++)
  {
    if (data.hasRowNames())
      out << data.getRowName(i) << sept;
    out << data(i, 0);
    for (size_t j = 1; j < n; j++)
    {
      out << sept << data(i, j);
    }
    out.endLine();
  }
}

/******************************************************************************/

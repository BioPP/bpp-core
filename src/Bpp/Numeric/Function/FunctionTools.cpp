// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include "../../App/ApplicationTools.h"
#include "FunctionTools.h"

using namespace bpp;

// From the STL;
#include <algorithm>
using namespace std;

void ParameterGrid::addDimension(const std::string& name, const Vdouble& values)
{
  if (find(names_.begin(), names_.end(), name) != names_.end())
    throw Exception("ParameterGrid::addDimension(). A dimension with name '" + name + "' already exists in the grid.");
  if (values.size() == 0)
    throw Exception("ParameterGrid::addDimension(). Empty vector given! The dimension should at least contain one point.");
  names_.push_back(name);
  grid_.push_back(values);
}

const Vdouble& ParameterGrid::getPointsForDimension(const std::string& name) const
{
  for (unsigned int i = 0; i < names_.size(); i++)
  {
    if (names_[i] == name)
      return grid_[i];
  }
  throw Exception("ParameterGrid::getPointsForDimension(). No dimension with name '" + name + "' was found in the grid.");
}

const Vdouble& ParameterGrid::getPointsForDimension(unsigned int i) const
{
  if (i >= names_.size())
    throw IndexOutOfBoundsException("ParameterGrid::getPointsForDimension().", i, 0, names_.size() - 1);
  return grid_[i];
}

size_t ParameterGrid::getTotalNumberOfPoints() const
{
  if (grid_.size() == 0)
    return 0;
  size_t n = 1;
  for (size_t i = 0; i < grid_.size(); i++)
  {
    n *= grid_[i].size();
  }
  return n;
}

shared_ptr<DataTable> FunctionTools::computeGrid(
    FunctionInterface& function,
    const ParameterGrid& grid)
{
  // Init stuff...
  size_t n = grid.getNumberOfDimensions();
  if (n == 0)
    return make_shared<DataTable>(0);
  ; // Empty data table returned.

  VVdouble points = grid.getPoints();

  // Get the parameter list. this may throw an exception if the grid does not
  // match the function parameters...
  auto parNames = grid.getDimensionNames();
  ParameterList pl = function.getParameters().createSubList(parNames);
  auto colNames = parNames;

  colNames.push_back("value");
  auto data = make_shared<DataTable>(colNames);

  for (unsigned int i = 0; i < n; i++)
  {
    pl.setParameterValue(grid.getDimensionName(i), grid.getPointsForDimension(i)[0]);
  }

  // Iterate over all dimensions:
  unsigned int currentDimension = 0;
  vector<unsigned int> currentPointInDimension(n);
  vector<string> row(n + 1);
  size_t nbPoints = grid.getTotalNumberOfPoints();
  ApplicationTools::displayMessage("Computing likelihood profile...");
  for (unsigned int i = 0; true; ++i)
  {
    ApplicationTools::displayGauge(i, nbPoints - 1, '=');
    // We start by adding the current point to the table:
    for (unsigned int j = 0; j < n; ++j)
    {
      row[j] = TextTools::toString(pl[j].getValue());
    }
    row[n] = TextTools::toString(function.f(pl), 12);
    data->addRow(row);

    // Now increment iterator:
    bool dimensionChanged = false;
    while (currentDimension < n && currentPointInDimension[currentDimension] == points[currentDimension].size() - 1)
    {
      currentDimension++;
      dimensionChanged = true;
    }
    // Stopping condition:
    if (currentDimension == n)
      break;

    currentPointInDimension[currentDimension]++;
    if (dimensionChanged)
    {
      for (unsigned int j = 0; j < currentDimension; j++)
      {
        currentPointInDimension[j] = 0;
      }
      currentDimension = 0;
    }

    // Set the new parameter value:
    for (unsigned int j = 0; j < points.size(); j++)
    {
      pl.setParameterValue(grid.getDimensionName(j), points[j][currentPointInDimension[j]]);
    }
  }
  ApplicationTools::displayMessage("\n");
  // and we are done:
  return data;
}

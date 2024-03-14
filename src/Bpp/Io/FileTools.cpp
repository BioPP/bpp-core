// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#include <sys/stat.h>

#include "../Text/TextTools.h"
#include "FileTools.h" // class's header file

using namespace bpp;
using namespace std;

/******************************************************************************/

char FileTools::DIR_SEP = '/';

/******************************************************************************/

bool FileTools::fileExists(const std::string& filename)
{
  ifstream file(filename.c_str());
  bool test = file.good(); // needed for CLang.
  file.close();
  return test;
}

/******************************************************************************/

bool FileTools::directoryExists(const std::string& path)
{
  ifstream file(path.c_str());
  bool test = file.good(); // needed for CLang.
  file.close();
  return test;
}

/******************************************************************************/

std::string FileTools::getFileName(const std::string& path, char dirSep)
{
  ptrdiff_t end = static_cast<ptrdiff_t>(path.find_last_of("."));
  ptrdiff_t begin = static_cast<ptrdiff_t>(path.find_last_of(dirSep) + 1);

  // Return an empty string if specified string isn't a path
  if (begin > end)
    return "";

  // Copy path and deletion of directories and extension
  string result(path);
  result.erase(result.begin() + end, result.end());
  result.erase(result.begin(), result.begin() + begin);

  // Send file name
  return result;
}

/******************************************************************************/

streampos FileTools::getFileSize(const std::string& filename)
{
  std::ifstream stream;
  streampos size;
  stream.open(filename.c_str(), std::ios::ate);
  size = stream.tellg();
  stream.close();
  return size;
}

/******************************************************************************/

std::string FileTools::getParent(const std::string& path, char dirSep)
{
  // Position of file name:
  ptrdiff_t begin = static_cast<ptrdiff_t>(path.find_last_of(dirSep));

  // Copy string and delte filename:
  string result(path);
  result.erase(result.begin() + begin, result.end());

  // Send directories
  return result;
}

/******************************************************************************/

std::string FileTools::getExtension(const std::string& path)
{
  size_t end = path.find_last_of(".");
  return path.substr(end + 1);
}

/******************************************************************************/

std::vector<std::string> FileTools::putStreamIntoVectorOfStrings(std::istream& input)
{
  vector<string> vs;
  string s = "";
  while (input)
  {
    getline(input, s, '\n');
    vs.push_back(s);
  }
  return vs;
}

/******************************************************************************/

std::string FileTools::getNextLine(std::istream& in)
{
  if (in.eof())
    return string("");
  string temp("");
  while (!in.eof() && TextTools::isEmpty(temp))
  {
    getline(in, temp, '\n');
  }
  return temp;
}

/******************************************************************************/

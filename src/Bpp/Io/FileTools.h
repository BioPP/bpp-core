// SPDX-FileCopyrightText: The Bio++ Development Group
//
// SPDX-License-Identifier: CECILL-2.1

#ifndef BPP_IO_FILETOOLS_H
#define BPP_IO_FILETOOLS_H


// From the STL:
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdio>
// #include <io.h>  // required for directoryExists method

namespace bpp
{
/**
 * @brief Some utilitary functions to deal with files.
 *
 * File paths default to unix, i.e. the directory seprator if '/'.
 * This character is stored in the variable 'DIR_SEP', and hence may be changed by
 * @code
 * FileTools::DIR_SEP='\\';
 * @endcode
 * for windows programs.
 *
 * Methods dealing with pathes have an optional dirSep argument, which default to DIR_SEP.
 */
class FileTools
{
public:
  static char DIR_SEP;

public:
  FileTools() {}
  virtual ~FileTools() {}

public:
  /**
   * @brief Tells if a file exist.
   *
   * @param filename The path toward the file.
   * @return True if the file exists.
   */
  static bool fileExists(const std::string& filename);

  /**
   * @brief Tells if a directory exist.
   *
   * NB: this is an alias for fileExists.
   *
   * @param path The path toward the file.
   * @return True if the file exists.
   */
  static bool directoryExists(const std::string& path);

  /**
   * @brief Get the name of of a file, without extension.
   *
   * Example:
   * FileTools::getFileName("/path/file.ext") returns "file".
   * This method is not recursive, hence
   * FileTools::getfileName("/home/me/archive.tar.gz") returns "archive".
     .   *
   * An empty string is returned if 'path' is not a valid path toward a file.
   *
   * @param path A file path.
   * @param dirSep The directory separator.
   * @return The filename of the file specified by path.
   */
  static std::string getFileName(const std::string& path, char dirSep = DIR_SEP);

  /**
   * @brief Get the size of a file.
   *
   * @param filename The path toward the file.
   * @return The size of the file.
   * @author Sylvain Gaillard
   */
  static std::streampos getFileSize(const std::string& filename);

  /**
   * @brief Get the path of the parent directry of the given file/dir.
   *
   * @param path A file/directory path.
   * @param dirSep The directory separator.
   * @return The path towar dthe parent directory.
   */
  static std::string getParent(const std::string& path, char dirSep = DIR_SEP);

  /**
   * @brief Get the extension of a file.
   *
   * Example:
   * FileTools::getExtension("/path/file.ext") returns "ext".
   * This method is not recursive, hence
   * FileTools::getExtension("/home/me/archive.tar.gz") returns "gz".
   *
   * @param path A file path.
   * @return The extension of the file specified by path.
   */
  static std::string getExtension(const std::string& path);

  /**
   * @brief Reads a stream and write each line in a vector.
   *
   * @param input Input stream.
   * @return A vector of strings.
   */
  static std::vector<std::string> putStreamIntoVectorOfStrings(std::istream& input);

  /**
   * @brief Get the next non-blanck line of a stream.
   *
   * @param in Input stream.
   */
  static std::string getNextLine(std::istream& in);
};
} // end of namespace bpp.
#endif // BPP_IO_FILETOOLS_H

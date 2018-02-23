# Documentation of the CMake system of Bio++
# Authors:
#   Francois Gindraud (2017)
# Created: 10/03/2017

# This file describes the CMake setup of bio++ (for this component and others).
# It includes important parts from cmake files and comments them.
# Not described: cpack, doxygen

####################################################################################################
# Main CMakeLists.txt
####################################################################################################

cmake_minimum_required (VERSION 2.8.11)
# 2.8.11 is required for:
# - clean target properties (link, include dir)

project (bpp-something CXX)
# Defines a lot of stuff including PROJECT_NAME which will contain "bpp-something"
# PROJECT_NAME is used to name:
# - the library
# - the CMake targets (${PROJECT_NAME}-static or ${PROJECT_NAME}-shared)
# - the CMake package
# Do not change it unless there is a good reason...

set (CMAKE_CXX_FLAGS "std=c++11 -Wall -Weffc++ -Wshadow -Wconversion")
# Define compile options to be used for all C++ targets.
# NOTES for the future:
# -> CMake >= 2.8.12 adds per target COMPILE_OPTIONS by using target_compile_options (<target> [PRIVATE|PUBLIC] <opt1> ... <optN>)
# -> CMake >= 3.1.x provides a CXX_STANDARD variable to set -std=...
# -> It also provides a "feature" property on targets which annotates, and auto selects the right -std=...

SET(${PROJECT_NAME}_VERSION_CURRENT "2")
SET(${PROJECT_NAME}_VERSION_REVISION "4")
SET(${PROJECT_NAME}_VERSION_AGE "0")
MATH(EXPR ${PROJECT_NAME}_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_CURRENT} - ${${PROJECT_NAME}_VERSION_AGE}")
SET(${PROJECT_NAME}_VERSION_MINOR ${${PROJECT_NAME}_VERSION_AGE})
SET(${PROJECT_NAME}_VERSION_PATCH ${${PROJECT_NAME}_VERSION_REVISION})
SET(${PROJECT_NAME}_VERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}.${${PROJECT_NAME}_VERSION_PATCH}")
# Libtool-like version number
# CURRENT:REVISION:AGE => file.so.(C-A).A.R
# current:  The most recent interface number that this library implements.
# revision: The implementation number of the current interface.
# age:      The difference between the newest and oldest interfaces that this
#           library implements.
# In other words, the library implements all the interface numbers in the
# range from number current - age to current.
# NOTE: unchanged from old cmake.

set (PROJECT_VERSION ${${PROJECT_NAME}_VERSION})
# Sets PROJECT_VERSION, which is used in package template files (version and config).
# NOTE: CMake >= 3.0 adds a "VERSION x.y.z" parameter to project() that fills these variables automatically (major/minor/...).

if (CMAKE_INSTALL_PREFIX)
  set (CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}" ${CMAKE_PREFIX_PATH})
endif (CMAKE_INSTALL_PREFIX)
include (GNUInstallDirs)
find_package (bpp-neededcomponent x.y.z REQUIRED)
# Includes other needed bpp components.
# They must be installed to standard paths, or to the target install path (hence the search path addition).
# If a version is provided, it will check that the found component is version-compatible with what we requested.
# -> Detects when we link to an old bpp version.
# Internally, this executes the package files of the needed components if they are found.
#
# Package files will include their targets file that define "imported targets".
# using target_link_libraries (current_target bpp-neededcomponent-static) will add link, include, and compile option flags to the whole compilation.
# For convenience package files also define the BPP_LIBS_SHARED and BPP_LIBS_STATIC variables.
# These variables hold a list of all imported shared or static library targets of bpp.
# 
# GNUInstallDirs is a standard CMake module that defines distribution-specific install paths.
# We use:
# - CMAKE_INSTALL_LIBDIR : system specific <prefix>/lib.
#   It automatically adds a '64' suffix if 64 bits, and an arch subdir on debian.
# - CMAKE_INSTALL_INCLUDEDIR : for homogeneity, in practice it is always <prefix>/include.

set (cmake-package-location ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
# Place where cmake package and target files will be placed at install.
# This is one of the paths that find_package will check, others are possible, see find_package().

include (CMakePackageConfigHelpers)
# Standard CMake module defining configure_package_config_file and write_basic_package_version_file.

configure_package_config_file (
  package.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/package.cmake
  INSTALL_DESTINATION ${cmake-package-location}
  )
# Generates the CMake package file from the template package.cmake.in
# It performs variable substitutions to fill the template with project name and versions.

write_basic_package_version_file (
  ${CMAKE_CURRENT_BINARY_DIR}/package-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )
# Generates a CMake package version file. Used by find_package to check version numbers.

install (FILES ${CMAKE_CURRENT_BINARY_DIR}/package.cmake DESTINATION ${cmake-package-location}
  RENAME ${PROJECT_NAME}-config.cmake)
install (FILES ${CMAKE_CURRENT_BINARY_DIR}/package-version.cmake DESTINATION ${cmake-package-location}
  RENAME ${PROJECT_NAME}-config-version.cmake)
# Install the cmake package files.
# They are renamed to the name convention required by CMake.

add_subdirectory (src)
# Define the library targets.

enable_testing ()
include (CTest)
if (BUILD_TESTING)
  add_subdirectory (test)
endif (BUILD_TESTING)
# Include tests.

####################################################################################################
# src/CMakeLists.txt
# Defines library targets (using PROJECT_NAME heavily, to be as generic as possible).
####################################################################################################

set (CPP_FILES
  Bpp/Something/do_something.cpp
  ...
  )
# File list (sorted to easily find duplicates or missing)

add_library (${PROJECT_NAME}-static STATIC ${CPP_FILES})
# Define the static lib

target_include_directories (${PROJECT_NAME}-static PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
  )
# Annotates the static lib target with include paths.
# These paths will be added as -I options:
# - during compilation of the target, by looking at CMAKE_CURRENT_SOURCE_DIR
# - during compilation of anything that links to this target, by looking at <install_dir>/include.
# This allow the code to reference headers by a path relative to src/ instead of using ../OtherDir/blah.h
# NOTE the "$<INSTALL_PREFIX>/" part is only needed to support 2.8.11 due to a bugged cmake check.

set_target_properties (${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
# Set target file name (default is the target name)

target_link_libraries (${PROJECT_NAME}-static ${BPP_LIBS_STATIC})
# "Links" to all bpp components included by find_package (adds everything, -I, -L -lbpp-comps...)
# (a manual list of bpp components targets also works here).

add_library (${PROJECT_NAME}-shared SHARED ${CPP_FILES})
target_include_directories (${PROJECT_NAME}-shared PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>
  )
set_target_properties (${PROJECT_NAME}-shared
  PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
  MACOSX_RPATH 1
  VERSION ${${PROJECT_NAME}_VERSION}
  SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
  )
target_link_libraries (${PROJECT_NAME}-shared ${BPP_LIBS_SHARED})
# Build the shared lib, using the same system

install (
  TARGETS ${PROJECT_NAME}-static ${PROJECT_NAME}-shared
  EXPORT ${PROJECT_NAME}-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
# Install libraries, while recording them under "${PROJECT_NAME}-targets"

install (
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bpp
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
  )
# Install headers (using the sub Bpp dir avoids looking into CMake temporary internal dirs in case of in tree build).

install (EXPORT ${PROJECT_NAME}-targets DESTINATION ${cmake-package-location})
# Install the target export file, that defines "imported targets" with properties for the two libs.

####################################################################################################
# bpp-something-config.cmake.in
# CMake package file template, will be expanded into a valid cmake file by configure_package_config_file()
####################################################################################################

@PACKAGE_INIT@
# Boilerplate from CMake (I do not think this is required in our case, but they say its better to use it).

if (NOT @PROJECT_NAME@_FOUND)
# Protection against double include of this package.

find_package (bpp-core @bpp-core_VERSION@ REQUIRED)
# Find dependencies.
# bpp-core_VERSION is a variable defined in main CMakeLists to the version of the found bpp-core component.
# @bpp-core_VERSION@ will be replaced by the version that was found.
# Thus, after installation, this package of bpp-something we require the same version.

include ("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@-targets.cmake")
# Include the imported targets

set (BPP_LIBS_STATIC "${BPP_LIBS_STATIC}" @PROJECT_NAME@-static)
set (BPP_LIBS_SHARED "${BPP_LIBS_SHARED}" @PROJECT_NAME@-shared)
# Append targets to convenient lists

get_property (static-location TARGET @PROJECT_NAME@-static PROPERTY LOCATION)
get_property (shared-location TARGET @PROJECT_NAME@-shared PROPERTY LOCATION)
get_property (header-location TARGET @PROJECT_NAME@-static PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
message (STATUS "@PROJECT_NAME@ @PROJECT_VERSION@ found:")
message (STATUS "  static lib: ${static-location}")
message (STATUS "  shared lib: ${shared-location}")
message (STATUS "  includes: ${header-location}")
unset (static-location)
unset (shared-location)
unset (header-location)
# Print information about this package when included (version, paths of libs and includes).

endif (NOT @PROJECT_NAME@_FOUND)

####################################################################################################
# test.CMakeLists.txt
# Performs tests
####################################################################################################

file (GLOB test_cpp_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
# Each .cpp file in test/ is considered to be a test.
# Globbing is more error prone than an explicit list, but tests are not critical.
# This allow tests to be added without having to modify the CMakelist (just re-run cmake).

foreach (test_cpp_file ${test_cpp_files})
  get_filename_component (test_name ${test_cpp_file} NAME_WE)
  add_executable (${test_name} ${test_cpp_file})
  # Each test is named after the cpp file without extension.

  target_link_libraries (${test_name} ${PROJECT_NAME}-shared)
  # Link to bpp-something shared library (pulls required -I -L -lbpp-*).

  add_test (
    NAME ${test_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${test_name}
    )
  # Add test to "make test".
  # The command must return EXIT_SUCCESS (usually 0) to be considered successful.
  # Test is run explicitely in CMAKE_CURRENT_SOURCE_DIR to allow out of tree builds (find data files).
endforeach (test_cpp_file)

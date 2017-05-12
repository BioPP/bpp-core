# Template project file to compile a program with Bio++ libraries
# Authors:
#   Francois Gindraud (2017)
# Created: 21/04/2017

# This file gives an example of how to compile your program using CMake and Bio++.
# Do not just copy the file as is, but use it as a guideline for your own project.
# If just copied and rename into place, some commands in section ADVANCED will interfere with the BASIC config.
# Remember that to be processed by CMake, the project file must be named "CMakeLists.txt".

################################### BASIC ####################################
# This is a basic configuration for users with Bio++ libraries installed in a standard system place (/usr/).

# These two lines are CMake boilerplate
cmake_minimum_required (VERSION 2.8.11) # Test of version. 2.8.11 is the oldest version supported by Bio++
project (myproject CXX) # The name is not critical

# Find Bio++ libraries and import their configurations.
find_package (bpp-core)
find_package (bpp-seq)

# Bio++ REQUIRES using at least the C++11 standard.
# This line specifies options passed to the compiler (you can add your own).
set (CMAKE_CXX_FLAGS "-std=c++11")

# Tells CMake to create a binary from dummy.cpp (replace with your .cpp file(s))
add_executable (myprogram dummy.cpp other.cpp)

# Link with the shared libraries of Bio++
target_link_libraries (myprogram ${BPP_LIBS_SHARED})

# You can then compile your program with:
# $ cmake .
# $ make

################################# ADVANCED ###################################
# Here we give some more advanced details

### Out of tree advice:
# CMake supports what is called "out of tree build":
# $ mkdir build
# $ cd build
# $ cmake <options> ..
# $ make
# In this form, you can easily delete all CMake stuff and generated files by deleting the build/ dir.
# It can be useful if you want to be sure to recompile from a clean state.

### find_package(bpp-<module>):
#
# If Bio++ is installed in a non standard path, CMake MUST be told to look there for it to find the Bio++ libraries.
# This can be done with: $ cmake -DCMAKE_PREFIX_PATH=<bpp-location> .
#
# find_package(bpp-<module>) defines, if successful:
# - a bpp-<module>-static symbol to link with the static library
# - a bpp-<module>-shared symbol to link with the shared library
# - variables BPP_LIBS_SHARED and BPP_LIBS_STATIC that includes the related symbols and symbols of Bio++ dependencies.
# The bpp-<module>-{static/shared} symbols also force CMake to link to their Bio++ dependencies.
# find_package of a a module also does a find_package of dependency Bio++ modules.
#
# So, for bpp-phyl (which depends on bpp-seq and bpp-core):
find_package (bpp-phyl)
# Is equivalent to:
find_package (bpp-core)
find_package (bpp-seq)
find_package (bpp-phyl)
# And the following target_link_libraries commands are equivalent:
target_link_libraries (myprogram bpp-phyl-shared)
target_link_libraries (myprogram bpp-core-shared bpp-seq-shared bpp-phyl-shared)
target_link_libraries (myprogram ${BPP_LIBS_SHARED})
# The short form relies on Bio++ inter module dependencies, but they are unlikely to change.

### Static libraries can be linked by using either:
target_link_libraries (myprogram bpp-phyl-static)
target_link_libraries (myprogram ${BPP_LIBS_STATIC})
# They make your binary program self contained (all Bio++ code is included with it).
# It can be given to people without having them install Bio++.
# However, the binary will be huge (10 to 100MB depending of which modules are used).
# Using `$ strip myprogram` can reduce the binary size.

### Shared libraries, RPATH and LD_LIBRARY_PATH:
# If you installed Bio++ to a NON STANDARD path and use the shared libraries, you might run into problems like:
# ./myprog: error while loading shared libraries: libbpp-seq.so.9: cannot open shared object file: No such file or directory
#
# The problem is that your executable must find the Bio++ shared library before starting.
# Libraries are normally searched in standard paths (/usr/lib).
# Two options exist to add a custom search path (to you Bio++ custom install path):
# - adding the path to the LD_LIBRARY_PATH environment variable.
# - adding the path as an RPATH entry in the executable 'myprog' itself.
#
# LD_LIBRARY_PATH:
# - does not require modifying the executable.
# - the executable can be used everywhere as long as the local Bio++ libs are in /usr/lib or in LD_LIBRARY_PATH.
# - LD_LIBRARY_PATH paths are used by all program to search for their libs (i.e. other commands in the shell).
#
# RPATH:
# - the executable runs without having to tweak LD_LIBRARY_PATH or anything else.
# - the executable includes hard-coded paths, so it CANNOT BE PUBLISHED as the paths are local to your system.
# - by default, the executable in the CMake build directory (after 'make') uses RPATH.
# - by default, "installed" executables (after 'make install') have no RPATH.
# - You can force the use of RPATH in installed executables with "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE"
# - if using RPATHs, consider compiling Bio++ with them by adding "-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE" during installation.
# - if you need an installed executable (to publish it without RPATH), add an install command:
install (TARGETS myprogram RUNTIME DESTINATION bin)
# And run `$ make install` to install it.
# You can select the location at the configuration step with -DCMAKE_INSTALL_PREFIX=<path>
# Or at install step with `$ make DESTDIR=<prefix> install`

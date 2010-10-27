# CMake script for bpp-core unit tests
# Author: Julien Dutheil
# Created: 27/10/2010

INCLUDE_DIRECTORIES("../src/")
ADD_EXECUTABLE(test_eigen test_eigen.cpp)
TARGET_LINK_LIBRARIES(test_eigen ${LIBS})

ADD_TEST(test_eigen "test_eigen")

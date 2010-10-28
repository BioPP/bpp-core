# CMake script for bpp-core unit tests
# Author: Julien Dutheil
# Created: 27/10/2010

INCLUDE_DIRECTORIES("../src/")
SET(LIBS "--static -L../src -lbpp-core")

ADD_EXECUTABLE(test_eigen test_eigen.cpp)
TARGET_LINK_LIBRARIES(test_eigen ${LIBS})
ADD_TEST(test_eigen "test_eigen")

ADD_EXECUTABLE(test_downhill test_downhill.cpp)
TARGET_LINK_LIBRARIES(test_downhill ${LIBS})
ADD_TEST(test_downhill "test_downhill")

ADD_EXECUTABLE(test_powell test_powell.cpp)
TARGET_LINK_LIBRARIES(test_powell ${LIBS})
ADD_TEST(test_powell "test_powell")

ADD_EXECUTABLE(test_gradient test_gradient.cpp)
TARGET_LINK_LIBRARIES(test_gradient ${LIBS})
ADD_TEST(test_gradient "test_gradient")


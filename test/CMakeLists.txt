# CMake script for bpp-core unit tests
# Author: Julien Dutheil
# Created: 27/10/2010

MACRO(TEST_FIND_LIBRARY OUTPUT_LIBS lib_name include_to_find)
  #start:
  FIND_PATH(${lib_name}_INCLUDE_DIR ${include_to_find})
  INCLUDE_DIRECTORIES(${${lib_name}_INCLUDE_DIR})

  SET(${lib_name}_NAMES ${lib_name} ${lib_name}.lib ${lib_name}.dll)
  FIND_LIBRARY(${lib_name}_LIBRARY NAMES ${${lib_name}_NAMES})
  IF(${lib_name}_LIBRARY)
    MESSAGE("-- Library ${lib_name} found here:")
    MESSAGE("   includes: ${${lib_name}_INCLUDE_DIR}")
    MESSAGE("   dynamic libraries: ${${lib_name}_LIBRARY}")
    MESSAGE(WARNING "Library ${lib_name} is already installed in the system tree. Test will be built against it. This may lead to unexpected results. You may want to do 'make install' before 'make test', or remove the installed version.")
    SET(${OUTPUT_LIBS} ${${OUTPUT_LIBS}} ${${lib_name}_LIBRARY})
  ELSE()
    INCLUDE_DIRECTORIES("../src/")
    SET(${OUTPUT_LIBS} "-L../src -lbpp-core" ${${OUTPUT_LIBS}})
  ENDIF()
ENDMACRO(TEST_FIND_LIBRARY)

#Find the bpp-core library library:
TEST_FIND_LIBRARY(LIBS bpp-core Bpp/Clonable.h)

ADD_EXECUTABLE(test_eigen test_eigen.cpp)
TARGET_LINK_LIBRARIES(test_eigen ${LIBS})
ADD_TEST(test_eigen "test_eigen")

ADD_EXECUTABLE(test_derivative1 test_derivative1.cpp)
TARGET_LINK_LIBRARIES(test_derivative1 ${LIBS})
ADD_TEST(test_derivative1 "test_derivative1")

ADD_EXECUTABLE(test_downhill test_downhill.cpp)
TARGET_LINK_LIBRARIES(test_downhill ${LIBS})
ADD_TEST(test_downhill "test_downhill")

ADD_EXECUTABLE(test_powell test_powell.cpp)
TARGET_LINK_LIBRARIES(test_powell ${LIBS})
ADD_TEST(test_powell "test_powell")

ADD_EXECUTABLE(test_gradient test_gradient.cpp)
TARGET_LINK_LIBRARIES(test_gradient ${LIBS})
ADD_TEST(test_gradient "test_gradient")

SET_PROPERTY(TEST test_eigen test_derivative1 test_downhill test_powell test_gradient PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=../src")

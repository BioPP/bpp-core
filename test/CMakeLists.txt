# CMake script for bpp-core unit tests
# Author: Julien Dutheil
# Created: 27/10/2010

MACRO(TEST_FIND_LIBRARY OUTPUT_LIBS lib_name include_to_find)
  #start:
  FIND_PATH(${lib_name}_INCLUDE_DIR ${include_to_find})

  SET(${lib_name}_NAMES ${lib_name} ${lib_name}.lib ${lib_name}.dll)
  FIND_LIBRARY(${lib_name}_LIBRARY NAMES ${${lib_name}_NAMES})
  IF(${lib_name}_LIBRARY)
    MESSAGE("-- Library ${lib_name} found here:")
    MESSAGE("   includes: ${${lib_name}_INCLUDE_DIR}")
    MESSAGE("   dynamic libraries: ${${lib_name}_LIBRARY}")
    MESSAGE(WARNING "Library ${lib_name} is already installed in the system tree. Test will be built against it. This may lead to unexpected results. You may want to do 'make install' before 'make test', or remove the installed version.")
  ELSE()
    SET(${lib_name}_LIBRARY "-L../src -lbpp-core")
    SET(${lib_name}_INCLUDE_DIR "../src/")
  ENDIF()
  INCLUDE_DIRECTORIES(${${lib_name}_INCLUDE_DIR})
  SET(${OUTPUT_LIBS} ${${OUTPUT_LIBS}} ${${lib_name}_LIBRARY})
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

ADD_EXECUTABLE(test_bfgs test_bfgs.cpp)
TARGET_LINK_LIBRARIES(test_bfgs ${LIBS})
ADD_TEST(test_bfgs "test_bfgs")

ADD_EXECUTABLE(test_stats test_stats.cpp)
TARGET_LINK_LIBRARIES(test_stats ${LIBS})
ADD_TEST(test_stats "test_stats")

ADD_EXECUTABLE(test_mva test_mva.cpp)
TARGET_LINK_LIBRARIES(test_mva ${LIBS})
ADD_TEST(test_mva "test_mva")

IF(UNIX)
  SET_PROPERTY(TEST test_eigen test_derivative1 test_downhill test_powell test_gradient test_bfgs test_stats test_mva PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=$ENV{LD_LIBRARY_PATH}:../src")
ENDIF()

IF(APPLE)
  SET_PROPERTY(TEST test_eigen test_derivative1 test_downhill test_powell test_gradient test_bfgs test_stats test_mva PROPERTY ENVIRONMENT "DYLD_LIBRARY_PATH=$ENV{DYLD_LIBRARY_PATH}:../src")
ENDIF()

IF(WIN32)
  SET(ENV{PATH} "$ENV{PATH};..\\src")
ENDIF()


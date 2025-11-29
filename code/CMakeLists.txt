cmake_minimum_required(VERSION 3.16)

# Set paths
set(PATH_CORE ${PATH_CODE}/core CACHE INTERNAL "")

include(${PATH_CMAKE}/ADD_GTEST.cmake)

# GET_COMPILER_VERSION(COMPILER_VERSION)
# # Add to cache to include in build options
# set(COMPILER_VERSION ${COMPILER_VERSION} CACHE INTERNAL "")

# Add to cache to include in build options
set(CMAKE_DEBUG_POSTFIX _dbg CACHE INTERNAL "")
set(CMAKE_RELWITHDEBINFO_POSTFIX _reldbg CACHE INTERNAL "")

if(NOT DEFINED ${CMAKE_CXX_STANDARD})
   set(CMAKE_CXX_STANDARD 20 CACHE INTERNAL "")
else()
   set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD} CACHE INTERNAL "")
endif()
message(STATUS "CMAKE_CXX_STANDARD (${CMAKE_CXX_STANDARD})")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")
message(STATUS "CMAKE_CXX_STANDARD_REQUIRED (${CMAKE_CXX_STANDARD_REQUIRED})")
set(CMAKE_CXX_EXTENSIONS OFF CACHE INTERNAL "")
message(STATUS "CMAKE_CXX_EXTENSIONS (${CMAKE_CXX_EXTENSIONS})")

set(CMAKE_POSITION_INDEPENDENT_CODE_SOURCE "input")
if("${CMAKE_POSITION_INDEPENDENT_CODE}" STREQUAL "")
   set(CMAKE_POSITION_INDEPENDENT_CODE ON)
   set(CMAKE_POSITION_INDEPENDENT_CODE_SOURCE "default")
endif()
message(STATUS "CMAKE_POSITION_INDEPENDENT_CODE (${CMAKE_POSITION_INDEPENDENT_CODE}) set by (${CMAKE_POSITION_INDEPENDENT_CODE_SOURCE})")

# Threads
find_package(Threads REQUIRED)
#set(Threads_VERSION ${Threads_VERSION} CACHE INTERNAL "")
#message(STATUS "Threads_VERSION (${Threads_VERSION})")

# PkgConfig - Not required
set(PKG_CONFIG_PATH_EV $ENV{PKG_CONFIG_PATH})
if(NOT PKG_CONFIG_PATH_EV)
   set(PKG_CONFIG_PATH_EV "/usr/local/lib64/pkgconfig")
   message(STATUS "PKG_CONFIG_PATH EV not defined, using (${PKG_CONFIG_PATH_EV})")
   set(ENV{PKG_CONFIG_PATH} ${PKG_CONFIG_PATH_EV})
endif()
message(STATUS "PKG_CONFIG_PATH EV defined as (${PKG_CONFIG_PATH_EV})")
unset(PKGCONFIG_FOUND CACHE)
find_package(PkgConfig QUIET)
set(FIND_PACKAGE_MESSAGE_DETAILS_PkgConfig ${FIND_PACKAGE_MESSAGE_DETAILS_PkgConfig} CACHE INTERNAL "")
message(STATUS "FIND_PACKAGE_MESSAGE_DETAILS_PkgConfig (${FIND_PACKAGE_MESSAGE_DETAILS_PkgConfig})")
if(NOT PKGCONFIG_FOUND)
   set(PKGCONFIG_FOUND FALSE CACHE INTERNAL "")
else()
   set(PKGCONFIG_FOUND ${PKGCONFIG_FOUND} CACHE INTERNAL "")
endif()
message(STATUS "PKGCONFIG_FOUND (${PKGCONFIG_FOUND})")

set(CUDAToolkit_VERSION_FIND 11.2 CACHE INTERNAL "")
message(STATUS "CUDAToolkit_VERSION_FIND (${CUDAToolkit_VERSION_FIND})")
set(CMAKE_CUDA_ARCHITECTURES 70-real 75 CACHE INTERNAL "")
message(STATUS "CMAKE_CUDA_ARCHITECTURES (${CMAKE_CUDA_ARCHITECTURES})")
set(CUDAToolkit_ENABLED_OPTION_DEFAULT "OFF" CACHE INTERNAL "")
message(STATUS "CUDAToolkit_ENABLED_OPTION_DEFAULT (${CUDAToolkit_ENABLED_OPTION_DEFAULT})")
option(BPM_CUDAToolkit_ENABLED "Enable use of 'CUDAToolkit'" ${CUDAToolkit_ENABLED_OPTION_DEFAULT})
message(STATUS "BPM_CUDAToolkit_ENABLED (${BPM_CUDAToolkit_ENABLED})")
unset(CUDAToolkit_FOUND CACHE)
if(BPM_CUDAToolkit_ENABLED)
   set(CUDAToolkit_ENABLED TRUE CACHE INTERNAL "")
   find_package(CUDAToolkit ${CUDAToolkit_VERSION_FIND} QUIET)
else()
   set(CUDAToolkit_ENABLED FALSE CACHE INTERNAL "")
endif()
if(NOT CUDAToolkit_FOUND)
   set(CUDAToolkit_FOUND FALSE CACHE INTERNAL "")
else()
   set(CUDAToolkit_FOUND ${CUDAToolkit_FOUND} CACHE INTERNAL "")
   enable_language(CUDA)
endif()
message(STATUS "CUDAToolkit_ENABLED (${CUDAToolkit_ENABLED})")
message(STATUS "CUDAToolkit_FOUND (${CUDAToolkit_FOUND})")
set(CUDAToolkit_VERSION "${CUDAToolkit_VERSION}" CACHE INTERNAL "")
message(STATUS "CUDAToolkit_VERSION (${CUDAToolkit_VERSION})")

# VTUNE - Not required
set(VTUNE_ENABLED_OPTION_DEFAULT "OFF" CACHE INTERNAL "")
message(STATUS "VTUNE_ENABLED_OPTION_DEFAULT (${VTUNE_ENABLED_OPTION_DEFAULT})")
option(BPM_VTUNE_ENABLED "Enable use of 'VTUNE'" ${VTUNE_ENABLED_OPTION_DEFAULT})
message(STATUS "BPM_VTUNE_ENABLED (${BPM_VTUNE_ENABLED})")
unset(VTUNE_FOUND CACHE)
if(BPM_VTUNE_ENABLED)
   set(VTUNE_ENABLED TRUE CACHE INTERNAL "")
   find_package(VTUNE QUIET)
else()
   set(VTUNE_ENABLED FALSE CACHE INTERNAL "")
endif()
if(NOT VTUNE_FOUND)
   set(VTUNE_FOUND FALSE CACHE INTERNAL "")
else()
   set(VTUNE_FOUND ${VTUNE_FOUND} CACHE INTERNAL "")
endif()
message(STATUS "VTUNE_ENABLED (${VTUNE_ENABLED})")
message(STATUS "VTUNE_FOUND (${VTUNE_FOUND})")

# OpenMP - Not required
set(OpenMP_ENABLED_OPTION_DEFAULT "ON" CACHE INTERNAL "")
message(STATUS "OpenMP_ENABLED_OPTION_DEFAULT (${OpenMP_ENABLED_OPTION_DEFAULT})")
option(BPM_OpenMP_ENABLED "Enable use of 'OpenMP'" ${OpenMP_ENABLED_OPTION_DEFAULT})
message(STATUS "BPM_OpenMP_ENABLED (${BPM_OpenMP_ENABLED})")
unset(OpenMP_FOUND CACHE)
if(BPM_OpenMP_ENABLED)
   set(OpenMP_ENABLED TRUE CACHE INTERNAL "")
   find_package(OpenMP QUIET)
else()
   set(OpenMP_ENABLED FALSE CACHE INTERNAL "")
endif()
if(NOT OpenMP_CXX_FOUND)
   set(OpenMP_CXX_FOUND FALSE CACHE INTERNAL "")
else()
   set(OpenMP_CXX_FOUND ${OpenMP_CXX_FOUND} CACHE INTERNAL "")
endif()
message(STATUS "OpenMP_ENABLED (${OpenMP_ENABLED})")
message(STATUS "OpenMP_CXX_FOUND (${OpenMP_CXX_FOUND})")

# MKL - Not required
set(MKL_ENABLED_OPTION_DEFAULT "OFF" CACHE INTERNAL "")
message(STATUS "MKL_ENABLED_OPTION_DEFAULT (${MKL_ENABLED_OPTION_DEFAULT})")
option(BPM_MKL_ENABLED "Enable use of 'MKL'" ${MKL_ENABLED_OPTION_DEFAULT})
message(STATUS "BPM_MKL_ENABLED (${BPM_MKL_ENABLED})")
unset(MKL_FOUND CACHE)
if(BPM_MKL_ENABLED)
   find_package(MKL QUIET)
endif()
if(NOT MKL_FOUND)
   set(MKL_ENABLED FALSE CACHE INTERNAL "")
   set(MKL_FOUND FALSE CACHE INTERNAL "")
else()
   set(MKL_ENABLED TRUE CACHE INTERNAL "")
   set(MKL_FOUND ${MKL_FOUND} CACHE INTERNAL "")
endif()
message(STATUS "MKL_ENABLED (${MKL_ENABLED})")
message(STATUS "MKL_FOUND (${MKL_FOUND})")

# find_package(Doxygen QUIET)
# message(STATUS "DOXYGEN_FOUND (${DOXYGEN_FOUND})")
# if(DOXYGEN_FOUND)
#    message(STATUS "Adding custom target for Doxygen")

#    # set input and output files
#    set(DOXYGEN_IN ${PATH_DOXYGEN}/Doxyfile.in)
#    set(DOXYGEN_OUT doxygen/Doxyfile)

#    # configure the doxygen file
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#    add_custom_target(doxygen
#       COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#       COMMENT "Generating API documentation with Doxygen"
#       VERBATIM
#    )
# endif()

# Set up unit testing with gtest
if(BUILD_TESTS)
   if(CMAKE_CXX_STANDARD STREQUAL 98)  # All the other options the var takes are 11 or newer.
      message(FATAL_ERROR "GoogleTest requires C++11 or newer. You may specify the standard on the cmake command line with -DCMAKE_CXX_STANDARD=11, or disable the unit tests with -DBUILD_TESTS=OFF.")
   endif()
   find_package(GTest MODULE REQUIRED)
   set(CMAKE_VERSION_GTEST_DISC "3.10.0")
   if(NOT ${CMAKE_VERSION} VERSION_LESS ${CMAKE_VERSION_GTEST_DISC})
      set(GTEST_DISCOVERY 1)
      include(GoogleTest) # Provides gtest_discover_tests. CMake 3.10+.
   else()
      message("WARNING: CMAKE version (${CMAKE_VERSION}) must be greater than or equal to (${CMAKE_VERSION_GTEST_DISC}) for gtest_discover_tests.")
   endif()
endif()
message(STATUS "GTest_FOUND (${GTest_FOUND})")
message(STATUS "GTest_VERSION (${GTest_VERSION})")

add_subdirectory(${PATH_CORE})

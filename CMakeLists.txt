cmake_minimum_required(VERSION 3.16)

# CMAKE_ASM_COMPILER:FILEPATH=/usr/bin/cc
# CMAKE_ASM_COMPILER_AR:FILEPATH=/usr/bin/gcc-ar
# CMAKE_ASM_COMPILER_RANLIB:FILEPATH=/usr/bin/gcc-ranlib
# CMAKE_CXX_COMPILER:FILEPATH=/usr/bin/c++
# CMAKE_CXX_COMPILER_AR:FILEPATH=/usr/bin/gcc-ar-8
# CMAKE_CXX_COMPILER_RANLIB:FILEPATH=/usr/bin/gcc-ranlib-8
# CMAKE_C_COMPILER:FILEPATH=/usr/bin/cc
# CMAKE_C_COMPILER_AR:FILEPATH=/usr/bin/gcc-ar-8
# CMAKE_C_COMPILER_RANLIB:FILEPATH=/usr/bin/gcc-ranlib-8

# set(CMAKE_ASM_COMPILER "/opt/gcc-10.1.0/bin/gcc-10.1" CACHE INTERNAL "")
# set(CMAKE_ASM_COMPILER_AR "/opt/gcc-10.1.0/bin/gcc-ar-10.1" CACHE INTERNAL "")
# set(CMAKE_ASM_COMPILER_RANLIB "/opt/gcc-10.1.0/bin/gcc-ranlib-10.1" CACHE INTERNAL "")
# set(CMAKE_CXX_COMPILER "/opt/gcc-10.1.0/bin/g++-10.1" CACHE INTERNAL "")
# set(CMAKE_CXX_COMPILER_AR "/opt/gcc-10.1.0/bin/gcc-ar-10.1" CACHE INTERNAL "")
# set(CMAKE_CXX_COMPILER_RANLIB "/opt/gcc-10.1.0/bin/gcc-ranlib-10.1" CACHE INTERNAL "")
# set(CMAKE_C_COMPILER "/opt/gcc-10.1.0/bin/gcc-10.1" CACHE INTERNAL "")
# set(CMAKE_C_COMPILER_AR "/opt/gcc-10.1.0/bin/gcc-ar-10.1" CACHE INTERNAL "")
# set(CMAKE_C_COMPILER_RANLIB "/opt/gcc-10.1.0/bin/gcc-ranlib-10.1" CACHE INTERNAL "")

set(CMAKE_CXX_COMPILER "/opt/gcc-10.1.0/bin/g++-10.1" CACHE INTERNAL "")
message(STATUS "CMAKE_CXX_COMPILER (${CMAKE_CXX_COMPILER})")
# Tell the compiler where the system libraries are
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib/arm-linux-gnueabihf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/arm-linux-gnueabihf")
# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
# export LIBRARY_PATH=/usr/lib/arm-linux-gnueabihf:$LIBRARY_PATH
# export CPATH=/usr/include/arm-linux-gnueabihf:$CPATH
# export C_INCLUDE_PATH=/usr/include/arm-linux-gnueabihf:$C_INCLUDE_PATH
# export CPLUS_INCLUDE_PATH=/usr/include/arm-linux-gnueabihf:$CPLUS_INCLUDE_PATH
# sudo ln -s /usr/lib/arm-linux-gnueabihf/crt1.o /usr/lib/crt1.o
# sudo ln -s /usr/lib/arm-linux-gnueabihf/crti.o /usr/lib/crti.o
# sudo ln -s /usr/lib/arm-linux-gnueabihf/crtn.o /usr/lib/crtn.o

# Name of the project is bpm.
# The variables bpm_SOURCE_DIR and bpm_BINARY_DIR are set with this command.
# Version number is set here manually.
# The variables bpm_VERSION, bpm_VERSION_MAJOR, bpm_VERSION_MINOR, bpm_VERSION_PATCH, bpm_VERSION_TWEAK are set here.
cmake_policy(SET CMP0048 NEW)
set(PROJECT_VERSION 1.0.0.0)
project(bpm
   VERSION ${PROJECT_VERSION}
   LANGUAGES C CXX ASM
)

string(TIMESTAMP TS "%Y-%m-%d.%H%M")

# Output compile commands
message(STATUS "Setting compile command export to ON")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set paths
set(PATH_CMAKE ${PROJECT_SOURCE_DIR}/cmake CACHE INTERNAL "")
set(PATH_CODE ${PROJECT_SOURCE_DIR}/code CACHE INTERNAL "")

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${PATH_CMAKE}")
if(NOT DEFINED CMAKE_BUILD_TYPE OR "${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Release)
  message(STATUS "CMAKE_BUILD_TYPE not defined, setting to default (${CMAKE_BUILD_TYPE})")
endif()
# Set installation path if not set via CMAKE_INSTALL_PREFIX
if(CMAKE_INSTALL_PREFIX STREQUAL "/usr/local")
   set(CMAKE_INSTALL_PREFIX "./${TS}-${PROJECT_NAME}_BUILD_$ENV{USER}" CACHE PATH "cmake install prefix" FORCE)
endif()
# Set name of build link type used to differentiate shared builds with static builds
set(BUILD_LINK_TYPE_SHARED "shared")
set(BUILD_LINK_TYPE_STATIC "static")
if(BUILD_SHARED_LIBS)
   set(BUILD_LINK_TYPE ${BUILD_LINK_TYPE_SHARED})
else()
   set(BUILD_LINK_TYPE ${BUILD_LINK_TYPE_STATIC})
endif()

set(CMAKE_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} CACHE PATH "cmake install dir")
set(INSTALL_EXPORT_TARGETS ${PROJECT_NAME}-targets-${BUILD_LINK_TYPE} CACHE PATH "install export targets")
set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME} CACHE PATH "include install dir")
set(TMPL_INSTALL_DIR tmpl/${PROJECT_NAME} CACHE PATH "template source install dir")
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME} CACHE PATH "lib install dir")
set(BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME} CACHE PATH "bin install dir")
set(CONFIG_INSTALL_DIR config/${PROJECT_NAME} CACHE PATH "config install dir")

message(STATUS "CMAKE_CXX_COMPILER set to (${CMAKE_CXX_COMPILER})")
message(STATUS "CMAKE_CXX_COMPILER_VERSION set to (${CMAKE_CXX_COMPILER_VERSION})")
message(STATUS "CMAKE_SYSTEM_PROCESSOR set to (${CMAKE_SYSTEM_PROCESSOR})")
message(STATUS "CMAKE_MODULE_PATH set to (${CMAKE_MODULE_PATH})")
message(STATUS "CMAKE_PREFIX_PATH set to (${CMAKE_PREFIX_PATH})")
message(STATUS "CMAKE_BUILD_TYPE set to (${CMAKE_BUILD_TYPE})")
message(STATUS "CMAKE_INSTALL_PREFIX set to (${CMAKE_INSTALL_PREFIX})")
message(STATUS "CMAKE_INSTALL_DIR set to (${CMAKE_INSTALL_DIR})")
message(STATUS "BUILD_SHARED_LIBS set to (${BUILD_SHARED_LIBS})")
message(STATUS "BUILD_LINK_TYPE set to (${BUILD_LINK_TYPE})")
message(STATUS "INSTALL_EXPORT_TARGETS set to (${INSTALL_EXPORT_TARGETS})")
message(STATUS "INCLUDE_INSTALL_DIR set to (${INCLUDE_INSTALL_DIR})")
message(STATUS "TMPL_INSTALL_DIR set to (${TMPL_INSTALL_DIR})")
message(STATUS "LIB_INSTALL_DIR set to (${LIB_INSTALL_DIR})")
message(STATUS "BIN_INSTALL_DIR set to (${BIN_INSTALL_DIR})")
message(STATUS "CONFIG_INSTALL_DIR set to (${CONFIG_INSTALL_DIR})")

set(BASH_NAME bash)
find_program(BASH_EXE ${BASH_NAME})
if(BASH_EXE)
   message(STATUS "BASH_NAME (${BASH_NAME}) found, BASH_EXE set to (${BASH_EXE})")
else()
   message(STATUS "BASH_NAME (${BASH_NAME}) NOT found")
endif()

option(BUILD_TESTS "Build unit tests" OFF)
message(STATUS "BUILD_TESTS set to ${BUILD_TESTS}")
if(BUILD_TESTS)
   include(CTest)
   set(USING_CTEST_ENV_VAR "USING_CTEST=TRUE" CACHE INTERNAL "")
   enable_testing() # Enables testing for this directory and below. Provides ctest integration.
endif()

add_subdirectory(${PATH_CODE})

###################################
# START INSTALL via 'make install'
###################################

# # Set the versions for installed libraries
# get_all_library_targets(LIBRARY_TARGETS ${CMAKE_CURRENT_LIST_DIR})
# #message(STATUS "LIBRARY_TARGETS (${LIBRARY_TARGETS})")
# foreach(LIBRARY_TARGET ${LIBRARY_TARGETS})
#    set_target_properties(${LIBRARY_TARGET}
#       PROPERTIES
#          OUTPUT_NAME ${LIBRARY_TARGET}
#       VERSION
#          ${PROJECT_VERSION}
#       SOVERSION
#          ${PROJECT_VERSION_MAJOR}
#    )
# endforeach()

include(CMakePackageConfigHelpers)

# Set the version number. It will use what is set in the "project" command at the top of this
# file. NOTE the COMPATIBILITY setting!
set(CMAKE_CONFIG_VERSION_FILE "${PROJECT_NAME}ConfigVersion.cmake")
write_basic_package_version_file(${CMAKE_CONFIG_VERSION_FILE}
   COMPATIBILITY SameMajorVersion
)

# Install all targets.
set(CMAKE_TARGETS_FILE "${PROJECT_NAME}Targets-${BUILD_LINK_TYPE}.cmake")
install(EXPORT ${INSTALL_EXPORT_TARGETS}
   FILE ${CMAKE_TARGETS_FILE}
   NAMESPACE ${PROJECT_NAME}::
   DESTINATION ${CMAKE_INSTALL_DIR}
)

set(CMAKE_CONFIG_FILE "${PROJECT_NAME}Config.cmake")
configure_package_config_file(${PATH_CMAKE}/${CMAKE_CONFIG_FILE}.in ${CMAKE_CONFIG_FILE}
   INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}
   PATH_VARS INCLUDE_INSTALL_DIR
)

# Capture build configuration settings (i.e. many variables set here are exported to this file and on
# to the install so application customers can see how this was configured, and adapt their own build
# to match.)
set(CMAKE_BUILD_OPTIONS_FILE "${PROJECT_NAME}BuildOptions.cmake")
configure_file(${PATH_CMAKE}/${CMAKE_BUILD_OPTIONS_FILE}.in ${CMAKE_BUILD_OPTIONS_FILE} @ONLY)

# Install the build config and the version files.
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIG_FILE}"
              "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CONFIG_VERSION_FILE}"
              "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_OPTIONS_FILE}"
   DESTINATION ${CMAKE_INSTALL_DIR}
)

install(DIRECTORY "${PATH_CMAKE}/"
   DESTINATION ${CMAKE_INSTALL_DIR}
   FILES_MATCHING PATTERN "*.cmake"
)

install(DIRECTORY "${PATH_CMAKE}/"
   DESTINATION ${CMAKE_INSTALL_DIR}
   FILES_MATCHING PATTERN "*.bash"
   PERMISSIONS
      OWNER_READ OWNER_EXECUTE
      GROUP_READ GROUP_EXECUTE
      WORLD_READ WORLD_EXECUTE
)

# Exporting from the build tree.
export(EXPORT ${INSTALL_EXPORT_TARGETS}
   FILE ${CMAKE_TARGETS_FILE}
   NAMESPACE ${PROJECT_NAME}::
)

# Register package in user's package registry
export(PACKAGE ${PROJECT_NAME})

###################################
# END INSTALL
###################################

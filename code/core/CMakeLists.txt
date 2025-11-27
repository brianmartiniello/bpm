cmake_minimum_required(VERSION 3.16)

set(PATH_CORE_INCL ${PATH_CORE}/include CACHE INTERNAL "")
set(PATH_CORE_TMPL ${PATH_CORE}/tmpl CACHE INTERNAL "")
set(PATH_CORE_SRC ${PATH_CORE}/src CACHE INTERNAL "")
set(PATH_CORE_CONFIG ${PATH_CORE}/config CACHE INTERNAL "")
set(PATH_CORE_TEST ${PATH_CORE}/test CACHE INTERNAL "")

##############################################
##############################################

set(TARGET_LIBS_PREFIX "bpm_" CACHE INTERNAL "")

set(TARGET_LIBS_CORE_PREFIX_INITIAL ${TARGET_LIBS_PREFIX}core CACHE INTERNAL "")
set(TARGET_LIBS_CORE_PREFIX ${TARGET_LIBS_CORE_PREFIX_INITIAL}_ CACHE INTERNAL "")

##############################################
## START - CORE COMMON LIBRARY
##############################################

set(TARGET_LIB_CORE_CMN
   ${TARGET_LIBS_CORE_PREFIX}common
)

set(TARGET_LIB
   ${TARGET_LIB_CORE_CMN}
)

unset(SRC_FILES)
unset(LINK_LIBS)
unset(COMPILER_DEFS)

set(VERSION_SRC_FILE ${PATH_CORE_SRC}/Version.cxx)
configure_file(${VERSION_SRC_FILE}.in ${VERSION_SRC_FILE})

set(SRC_FILES
   ${PATH_CORE_SRC}/CoreUtilities.cxx
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_SRC}/Version.cxx
)

# set(LINK_LIBS
# )

# set(COMPILER_DEFS
# )

# if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|i.86)$" AND
#     NOT "${BUILD_LINK_TYPE}" STREQUAL "${BUILD_LINK_TYPE_SHARED}")
#     message(STATUS "Building x86 assembly code")
#     list(APPEND
#        SRC_FILES
#        ${PATH_CORE_SRC}/asm/TODO.s
#     )
#     list(APPEND
#        COMPILER_DEFS
#        BPM_ASM_ENABLED
#     )
# else()
#     message(WARNING "Skipping x86 assembly on non-x86 architecture (${CMAKE_SYSTEM_PROCESSOR}) or link type not shared (${BUILD_LINK_TYPE_SHARED})")
# endif()

# message("TARGET_LIB (${TARGET_LIB})")
# message("SRC_FILES (${SRC_FILES})")
# message("LINK_LIBS (${LINK_LIBS})")
# message("COMPILER_DEFS (${COMPILER_DEFS})")

add_library(${TARGET_LIB})

set_target_properties(${TARGET_LIB}
   PROPERTIES
      DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
      RELWITHDEBINFO_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX}
      POSITION_INDEPENDENT_CODE ON
)

# target_compile_options(${TARGET_LIB}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_sources(${TARGET_LIB}
   PRIVATE
      ${SRC_FILES}
)

target_include_directories(${TARGET_LIB}
   PRIVATE
      $<BUILD_INTERFACE:${PATH_CORE_TMPL}>
      $<BUILD_INTERFACE:${PATH_CORE_INCL}>
)

target_link_libraries(${TARGET_LIB}
   PRIVATE
      ${LINK_LIBS}
      Threads::Threads
)

if(COMPILER_DEFS)
   target_compile_definitions(${TARGET_LIB}
      PRIVATE
         ${COMPILER_DEFS}
   )
endif()

install(
   TARGETS
      ${TARGET_LIB}
   EXPORT
      ${INSTALL_EXPORT_TARGETS}
   DESTINATION
      ${LIB_INSTALL_DIR}
)

##############################################
## END - CORE COMMON LIBRARY
##############################################

##############################################
## START - CORE LIBRARIES
##############################################

set(TARGET_LIBS_CORE
   ${TARGET_LIBS_CORE_PREFIX}proper
   ${TARGET_LIBS_CORE_PREFIX}stub
)

foreach(TARGET_LIB ${TARGET_LIBS_CORE})
   unset(SRC_FILES)
   unset(LINK_LIBS)
   unset(COMPILER_DEFS)
   unset(INCLUDE_DIRS)

   if("${TARGET_LIB}" STREQUAL "${TARGET_LIBS_CORE_PREFIX}proper")
      set(SRC_FILES
         ${PATH_CORE_SRC}/Empty.cxx
         ${PATH_CORE_SRC}/Graph.cxx
      )
      if(CUDAToolkit_FOUND)
         list(APPEND
            SRC_FILES
            ${PATH_CORE_SRC}/TODO.cxx
         )
      endif()
      if(MKL_FOUND)
         list(APPEND
            SRC_FILES
            ${PATH_CORE_SRC}/TODO.cxx
         )
      endif()
      # set(LINK_LIBS
      # )
      # set(COMPILER_DEFS
      # )
      # set(INCLUDE_DIRS
      # )
   elseif("${TARGET_LIB}" STREQUAL "${TARGET_LIBS_CORE_PREFIX}stub")
      set(SRC_FILES
         ${PATH_CORE_SRC}/test/EmptyStub.cxx
         ${PATH_CORE_SRC}/test/GraphStub.cxx
      )
      if(CUDAToolkit_FOUND)
         list(APPEND
            SRC_FILES
            ${PATH_CORE_SRC}/test/TODO.cxx
         )
      endif()
      if(MKL_FOUND)
         list(APPEND
            SRC_FILES
            ${PATH_CORE_SRC}/test/TODO.cxx
         )
      endif()
      # set(LINK_LIBS
      # )
      # set(COMPILER_DEFS
      # )
      # set(INCLUDE_DIRS
      # )
   else()
      message(FATAL_ERROR "Target name (${TARGET_LIB}) unsupported")
   endif()

   if(CUDAToolkit_FOUND)
      list(APPEND
         LINK_LIBS
         CUDA::cudart
      )
      list(APPEND
         COMPILER_DEFS
         BPM_CUDA_ENABLED
      )
   endif()

   if(OpenMP_CXX_FOUND)
      list(APPEND
         LINK_LIBS
         OpenMP::OpenMP_CXX
      )
      list(APPEND
         COMPILER_DEFS
         BPM_OPENMP_ENABLED
      )
   endif()

   if(MKL_FOUND)
      list(APPEND
         LINK_LIBS
         MKL::MKL
      )
      list(APPEND
         COMPILER_DEFS
         BPM_MKL_ENABLED
      )
   endif()

   # message("TARGET_LIB (${TARGET_LIB})")
   # message("SRC_FILES (${SRC_FILES})")
   # message("LINK_LIBS (${LINK_LIBS})")
   # message("COMPILER_DEFS (${COMPILER_DEFS})")
   # message("INCLUDE_DIRS (${INCLUDE_DIRS})")

   add_library(${TARGET_LIB})

   set_target_properties(${TARGET_LIB}
      PROPERTIES
         DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
         RELWITHDEBINFO_POSTFIX ${CMAKE_RELWITHDEBINFO_POSTFIX}
         POSITION_INDEPENDENT_CODE ON
   )

   # target_compile_options(${TARGET_LIB}
   #    PRIVATE
   #       ${PLATFORM_COMPILE_OPTIONS}
   # )

   target_sources(${TARGET_LIB}
      PRIVATE
         ${SRC_FILES}
   )

   if("${TARGET_LIB}" MATCHES "stub")
      target_include_directories(${TARGET_LIB}
         PUBLIC
            $<INSTALL_INTERFACE:${TMPL_INSTALL_DIR}/bpm/core/test/>
            $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/bpm/core/test/>
            $<BUILD_INTERFACE:${PATH_CORE_TMPL}/bpm/core/test/>
            $<BUILD_INTERFACE:${PATH_CORE_INCL}/bpm/core/test/>
      )
   endif()

   target_include_directories(${TARGET_LIB}
      PUBLIC
         $<INSTALL_INTERFACE:${TMPL_INSTALL_DIR}>
         $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
         $<BUILD_INTERFACE:${PATH_CORE_TMPL}>
         $<BUILD_INTERFACE:${PATH_CORE_INCL}>
   )

   target_link_libraries(${TARGET_LIB}
      PUBLIC
         ${LINK_LIBS}
         ${TARGET_LIBS_CORE_PREFIX}common
         Threads::Threads
   )

   if(COMPILER_DEFS)
      target_compile_definitions(${TARGET_LIB}
         PUBLIC
            ${COMPILER_DEFS}
      )
   endif()

   install(
      TARGETS
         ${TARGET_LIB}
      EXPORT
         ${INSTALL_EXPORT_TARGETS}
      DESTINATION
         ${LIB_INSTALL_DIR}
   )
endforeach()

# Interface library
set(TARGET_LIB_CORE_INTERFACE ${TARGET_LIBS_CORE_PREFIX_INITIAL})
add_library(${TARGET_LIB_CORE_INTERFACE}
   INTERFACE
)
target_link_libraries(${TARGET_LIB_CORE_INTERFACE}
   INTERFACE
      ${TARGET_LIBS_CORE_PREFIX}proper
)
install(
   TARGETS
      ${TARGET_LIB_CORE_INTERFACE}
   EXPORT
      ${INSTALL_EXPORT_TARGETS}
   DESTINATION
      ${LIB_INSTALL_DIR}
)

##############################################
## END - CORE LIBRARIES
##############################################

##############################################
##############################################

# Install the headers
install(DIRECTORY ${PATH_CORE_INCL}/
   DESTINATION ${INCLUDE_INSTALL_DIR}
   FILES_MATCHING
   PATTERN "*.hxx"
   PATTERN "*.hpp"
   PATTERN "*.h"
)
install(DIRECTORY ${PATH_CORE_TMPL}/
   DESTINATION ${TMPL_INSTALL_DIR}
   FILES_MATCHING
   PATTERN "*.hxx"
   PATTERN "*.h"
)

# Install the config files
install(DIRECTORY ${PATH_CORE_CONFIG}/
   DESTINATION ${CONFIG_INSTALL_DIR}
)

##############################################
##############################################

if(GTest_FOUND)
   add_subdirectory(${PATH_CORE_TEST})
endif()

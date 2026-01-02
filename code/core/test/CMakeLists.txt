cmake_minimum_required(VERSION 3.16)

set(PATH_CORE_TEST_BENCHMARK ${PATH_CORE_TEST}/benchmark CACHE INTERNAL "")
set(PATH_CORE_TEST_BW ${PATH_CORE_TEST}/bw CACHE INTERNAL "")
set(PATH_CORE_TEST_CONCEPTS ${PATH_CORE_TEST}/concepts CACHE INTERNAL "")
set(PATH_CORE_TEST_OMP ${PATH_CORE_TEST}/omp CACHE INTERNAL "")
set(PATH_CORE_TEST_UT ${PATH_CORE_TEST}/ut CACHE INTERNAL "")

# Set ctest variables
set(CORE_TEST_LABEL ${TARGET_LIBS_CORE_PREFIX}test)
set(CORE_TEST_ENV_VARS
   ${USING_CTEST_ENV_VAR}
)

cmake_host_system_information(RESULT host_name QUERY HOSTNAME)
cmake_policy(SET CMP0057 NEW) # Support IN_LIST

set(INCLUDE_DIRS
   ${PATH_CORE_TMPL}
   ${PATH_CORE_INCL}
)
set(LINK_LIBS
   atomic
   GTest::GTest
   benchmark::benchmark
   # Threads::Threads
   pthread
)
set(COMPILER_DEFS)

##############################################
##############################################

set(TestVersion_MAIN TestVersion)

add_executable(${TestVersion_MAIN}
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_SRC}/Version.cxx
   ${PATH_CORE_TEST_UT}/TestVersion.cxx
)

# target_compile_options(${TestVersion_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestVersion_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestVersion_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestVersion_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestVersion_MAIN} XML)
set_tests_properties(${TestVersion_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

set(TestGraph_MAIN TestGraph)

add_executable(${TestGraph_MAIN}
   ${PATH_CORE_SRC}/Graph.cxx
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_TEST_UT}/TestGraph.cxx
)

# target_compile_options(${TestGraph_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestGraph_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestGraph_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestGraph_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestGraph_MAIN} XML)
set_tests_properties(${TestGraph_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

if(OpenMP_CXX_FOUND)
   set(TestOpenMP_MAIN TestOpenMP)

   add_executable(${TestOpenMP_MAIN}
      ${PATH_CORE_SRC}/Logger.cxx
      ${PATH_CORE_SRC}/Timer.cxx
      ${PATH_CORE_TEST_OMP}/TestOpenMP.cxx
   )

   # target_compile_options(${TestOpenMP_MAIN}
   #    PRIVATE
   #       ${PLATFORM_COMPILE_OPTIONS}
   # )

   target_include_directories(${TestOpenMP_MAIN}
      PRIVATE
         ${INCLUDE_DIRS}
   )

   target_link_libraries(${TestOpenMP_MAIN}
      PRIVATE
         ${LINK_LIBS}
         OpenMP::OpenMP_CXX
   )

   target_compile_definitions(${TestOpenMP_MAIN}
      PRIVATE
         ${COMPILER_DEFS}
         BPM_OPENMP_ENABLED
   )

   ADD_GTEST(${TestOpenMP_MAIN} XML)
   set_tests_properties(${TestOpenMP_MAIN}
      PROPERTIES
         LABELS
            ${CORE_TEST_LABEL}
         ENVIRONMENT
            "${CORE_TEST_ENV_VARS}"
   )
endif()

##############################################
##############################################

set(TestCoreUtilities_MAIN TestCoreUtilities)

add_executable(${TestCoreUtilities_MAIN}
   ${PATH_CORE_SRC}/CoreUtilities.cxx
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_TEST_UT}/TestCoreUtilities.cxx
)

# target_compile_options(${TestCoreUtilities_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestCoreUtilities_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestCoreUtilities_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestCoreUtilities_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestCoreUtilities_MAIN} XML)
set_tests_properties(${TestCoreUtilities_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

set(TestSharedCountVector_MAIN TestSharedCountVector)

add_executable(${TestSharedCountVector_MAIN}
   ${PATH_CORE_SRC}/CoreTypes.cxx
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_SRC}/SharedCountVector.cxx
   ${PATH_CORE_SRC}/Timer.cxx
   ${PATH_CORE_TEST_UT}/TestSharedCountVector.cxx
)

# target_compile_options(${TestSharedCountVector_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestSharedCountVector_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestSharedCountVector_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestSharedCountVector_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestSharedCountVector_MAIN} XML)
set_tests_properties(${TestSharedCountVector_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

set(TestScopedGuard_MAIN TestScopedGuard)

add_executable(${TestScopedGuard_MAIN}
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_TEST_UT}/TestScopedGuard.cxx
)

# target_compile_options(${TestScopedGuard_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestScopedGuard_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestScopedGuard_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestScopedGuard_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestScopedGuard_MAIN} XML)
set_tests_properties(${TestScopedGuard_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

set(TestBW_MAIN TestBW)

add_executable(${TestBW_MAIN}
   ${PATH_CORE_TEST_BW}/TestBW.cxx
)

# target_compile_options(${TestBW_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

# target_include_directories(${TestBW_MAIN}
#    PRIVATE
#       ${INCLUDE_DIRS}
# )

target_link_libraries(${TestBW_MAIN}
   PRIVATE
      bpm_core
)

# target_compile_definitions(${TestBW_MAIN}
#    PRIVATE
#       ${COMPILER_DEFS}
# )

# ADD_GTEST(${TestBW_MAIN} XML)
# set_tests_properties(${TestBW_MAIN}
#    PROPERTIES
#       LABELS
#          ${CORE_TEST_LABEL}
#       ENVIRONMENT
#          "${CORE_TEST_ENV_VARS}"
# )
set(TestBW_MAIN_THREADS 4)
set(TestBW_MAIN_BYTES 1024)
set(TestBW_MAIN_INTENSITY 1000)
set(TestBW_MAIN_MODE memory)
add_test(NAME ${TestBW_MAIN}
   COMMAND ${TestBW_MAIN} ${TestBW_MAIN_THREADS} ${TestBW_MAIN_BYTES} ${TestBW_MAIN_INTENSITY} ${TestBW_MAIN_MODE}
)

set(TestBW_MAIN_PY "${TestBW_MAIN}.py")
set(TestBW_MAIN_PY_SRC_PATH "${PATH_CORE_TEST_BW}/${TestBW_MAIN_PY}")
set(TestBW_MAIN_PY_DEST_PATH "${CMAKE_CURRENT_BINARY_DIR}/${TestBW_MAIN_PY}")
add_custom_command(
   TARGET ${TestBW_MAIN}
   POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy ${TestBW_MAIN_PY_SRC_PATH} ${TestBW_MAIN_PY_DEST_PATH}
   COMMENT "Copying (${TestBW_MAIN_PY}) to build directory"
   VERBATIM
)
# add_custom_command(
#    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/dist/${TestBW_MAIN_PY}"
#    COMMAND ${Python3_EXECUTABLE} -m PyInstaller --onefile --name ${TestBW_MAIN_PY} ${TestBW_MAIN_PY_SRC_PATH}
#    DEPENDS ${TestBW_MAIN_PY_SRC_PATH}
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#    COMMENT "Bundling Python script into an executable (${TestBW_MAIN_PY})..."
# )
# add_custom_target(${TestBW_MAIN_PY} ALL
#    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/dist/${TestBW_MAIN_PY}"
# )

##############################################
##############################################

set(TestGuardBenchmark_MAIN TestGuardBenchmark)

add_executable(${TestGuardBenchmark_MAIN}
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_TEST_BENCHMARK}/TestGuard.cxx
)

# target_compile_options(${TestGuardBenchmark_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestGuardBenchmark_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestGuardBenchmark_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestGuardBenchmark_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestGuardBenchmark_MAIN} "")
set_tests_properties(${TestGuardBenchmark_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

set(TestDerivedFromConcept_MAIN TestDerivedFromConcept)

add_executable(${TestDerivedFromConcept_MAIN}
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_TEST_CONCEPTS}/TestDerivedFrom.cxx
)

# target_compile_options(${TestDerivedFromConcept_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestDerivedFromConcept_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestDerivedFromConcept_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestDerivedFromConcept_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestDerivedFromConcept_MAIN} "")
set_tests_properties(${TestDerivedFromConcept_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

set(TestThread_MAIN TestThread)

add_executable(${TestThread_MAIN}
   ${PATH_CORE_SRC}/Logger.cxx
   ${PATH_CORE_SRC}/Thread.cxx
   ${PATH_CORE_TEST_UT}/TestThread.cxx
)

# target_compile_options(${TestThread_MAIN}
#    PRIVATE
#       ${PLATFORM_COMPILE_OPTIONS}
# )

target_include_directories(${TestThread_MAIN}
   PRIVATE
      ${INCLUDE_DIRS}
)

target_link_libraries(${TestThread_MAIN}
   PRIVATE
      ${LINK_LIBS}
)

target_compile_definitions(${TestThread_MAIN}
   PRIVATE
      ${COMPILER_DEFS}
)

ADD_GTEST(${TestThread_MAIN} "")
set_tests_properties(${TestThread_MAIN}
   PROPERTIES
      LABELS
         ${CORE_TEST_LABEL}
      ENVIRONMENT
         "${CORE_TEST_ENV_VARS}"
)

##############################################
##############################################

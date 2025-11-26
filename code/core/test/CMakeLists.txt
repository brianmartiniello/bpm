cmake_minimum_required(VERSION 3.16)

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
   GTest::GTest
)
set(COMPILER_DEFS)

##############################################
##############################################

set(TestVersion_MAIN TestVersion)

add_executable(${TestVersion_MAIN}
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

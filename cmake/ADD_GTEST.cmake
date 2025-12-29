macro(ADD_GTEST TEST_NAME FORMAT)

set(LOG_HEADER "Macro ADD_GTEST: TEST_NAME (${TEST_NAME}) - ")

set_target_properties(${TEST_NAME}
   PROPERTIES
      DEBUG_POSTFIX
         ${CMAKE_DEBUG_POSTFIX}
      RELWITHDEBINFO_POSTFIX
         ${CMAKE_RELWITHDEBINFO_POSTFIX}
)

get_target_property(TARGET_DIR "${TEST_NAME}" BINARY_DIR)
set(TEST_EXE "${TARGET_DIR}/${TEST_NAME}")
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
if("${BUILD_TYPE}" STREQUAL "DEBUG")
   set(TEST_EXE "${TEST_EXE}${CMAKE_DEBUG_POSTFIX}")
elseif("${BUILD_TYPE}" STREQUAL "RELWITHDEBINFO")
   set(TEST_EXE "${TEST_EXE}${CMAKE_RELWITHDEBINFO_POSTFIX}")
endif()

set(TEST_OPTS)
if(NOT "${FORMAT}" STREQUAL "")
   string(TOLOWER ${FORMAT} FORMAT_LOWERCASE)

   # Check if the format is valid
   list(APPEND VALID_FORMATS "xml" "json")
   list(FIND VALID_FORMATS "${FORMAT_LOWERCASE}" index)
   if (${index} EQUAL -1)
      message(FATAL_ERROR "${LOG_HEADER}Unsupported format (${FORMAT}), valid (${VALID_FORMATS})")
   endif()

   list(APPEND TEST_OPTS "--gtest_output=${FORMAT_LOWERCASE}:${TEST_EXE}_gtest_log.${FORMAT_LOWERCASE}")
endif()

message(STATUS "${LOG_HEADER}Adding test command (${TEST_EXE} ${TEST_OPTS})")

add_test(NAME ${TEST_NAME}
   COMMAND ${TEST_EXE} ${TEST_OPTS}
)

endmacro(ADD_GTEST)
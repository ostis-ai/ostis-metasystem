make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME identifiers_module_test_starter
    DEPENDS sc-agents-common sc-builder-lib identifiers
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_ROOT}/sc-tools/sc-builder/src
)
message("test")
message(${SC_MEMORY_SRC}/tests/sc-memory/_test)
message(${SC_MACHINE_ROOT}/sc-tools/sc-builder/src)
add_definitions(-DIDENTIFIERS_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME identifiers_module_test_starter
    DEPENDS sc-agents-common identifiers sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_ROOT}/sc-tools/sc-builder/src ${SC_MEMORY_SRC}/sc-memory
)

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

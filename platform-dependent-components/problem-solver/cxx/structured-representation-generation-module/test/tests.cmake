add_definitions(-DSTRUCTURED_REPRESENTATION_GENERATION_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME structured-representation-generation-test-starter
    DEPENDS sc-memory sc-core sc-builder-lib structured-representation-generation
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${SC_TOOLS_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(structured-representation-generation-test-starter)
endif()

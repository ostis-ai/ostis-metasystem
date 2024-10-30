make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME identifiers-module-test-starter
    DEPENDS sc-memory sc-core sc-builder-lib identifiers-module
    INCLUDES ${SC_TOOLS_SRC} ${SC_MEMORY_SRC} ${SC_BUILDER_PATH}
)

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(identifiers-module-test-starter)
endif()

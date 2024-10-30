make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME sections-module-test-starter
    DEPENDS sc-memory sc-core sc-builder-lib sections-module
    INCLUDES ${SC_MEMORY_SRC} ${SC_TOOLS_SRC} ${SC_BUILDER_PATH}
)

add_definitions(-DSUBJECT_DOMAIN_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sections-module-test-starter)
endif()

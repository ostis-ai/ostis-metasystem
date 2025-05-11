make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME verification-module-test-starter
    DEPENDS sc-machine::sc-builder-lib verification-module
    INCLUDES ${SC_MEMORY_SRC} ${SC_TOOLS_SRC} ${SC_BUILDER_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(verification-module-test-starter)
endif()

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME sections-module-test-starter
    DEPENDS sc-machine::sc-builder-lib sections-module
    INCLUDES ${SECTIONS_MODULE_SRC}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sections-module-test-starter)
endif()

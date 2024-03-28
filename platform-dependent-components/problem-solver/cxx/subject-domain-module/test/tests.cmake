make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME subject-domain-module-test-starter
    DEPENDS sc-core sc-agents-common subjectDomainModule sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_TOOLS_SRC}/sc-builder/src ${SC_MEMORY_SRC}/sc-memory
)

add_definitions(-DSUBJECT_DOMAIN_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(subject-domain-module-test-starter)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH}/ostis-ps-tests)

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
        NAME natural_language_processing_test_starter
        DEPENDS naturalLanguageProcessingModule sc-builder-lib sc-core
        INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_MACHINE_PATH}/sc-tools/sc-builder/src)

add_definitions(-DNATURAL_LANGUAGE_PROCESSING_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

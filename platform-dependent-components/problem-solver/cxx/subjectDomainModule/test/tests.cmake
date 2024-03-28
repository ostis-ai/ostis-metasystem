make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
		NAME subj_domain_module_test_starter
		DEPENDS subjDomainModule sc-builder-lib
		INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/..)

add_definitions(-DSUBJ_DOMAIN_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

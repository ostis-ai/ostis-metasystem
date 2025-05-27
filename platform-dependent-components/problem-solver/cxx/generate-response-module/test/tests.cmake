
make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME generate-response-module-test-starter
    DEPENDS sc-memory sc-core sc-builder-lib generate_response_test_agents generate-response-module
    # INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_TOOLS_SRC}/sc-builder/src ${SC_MEMORY_SRC}/sc-memory
    INCLUDES ${SC_MEMORY_INCLUDE} ${SC_BUILDER_INCLUDE}
)

file(GLOB GENERATE_RESPONSE_TEST_AGENTS_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/agent/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/agent/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/keynodes/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/keynodes/*.hpp"
)
add_library(generate_response_test_agents ${GENERATE_RESPONSE_TEST_AGENTS_SOURCE})
target_link_libraries(
    generate_response_test_agents
    # generate_response
    generate-response-module
    sc-agents-common
    sc-memory)
target_include_directories(generate-response-module
    PUBLIC ${GENERATE_RESPONSE_TEST_AGENTS_SOURCE}
    PUBLIC ${SC_MEMORY_INCLUDE}
    PUBLIC ${SC_CORE_INCLUDE}
    PUBLIC ${SC_AGENTS_COMMON_INCLUDE})

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
add_definitions(-DGENERATE_RESPONSE_TEMPLATE_PATH="${CMAKE_SOURCE_DIR}/sc-models/problem-solver/generate_response/templates/")

if(${SC_CLANG_FORMAT_CODE})
target_clangformat_setup(generate-response-module-test-starter)
endif()

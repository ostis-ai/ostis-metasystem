file(GLOB GENERATE_RESPONSE_TEST_AGENTS_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/agent/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/agent/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/keynodes/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/keynodes/*.hpp"
)
add_library(generate_response_test_agents ${GENERATE_RESPONSE_TEST_AGENTS_SOURCE})
target_link_libraries(
    generate_response_test_agents
    generate_response
    sc-agents-common
)

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME generate_response_module_test_starter
    DEPENDS generate_response_test_agents sc-builder-lib
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_TOOLS_SRC}/sc-builder/src ${SC_MEMORY_SRC}/sc-memory
)

add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
add_definitions(-DGENERATE_RESPONSE_TEMPLATE_PATH="${CMAKE_SOURCE_DIR}/sc-models/problem-solver/generate_response/templates/")

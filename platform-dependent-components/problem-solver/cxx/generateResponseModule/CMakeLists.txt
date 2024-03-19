set(GENERATE_RESPONSE_MODULE_SRC "${PROBLEM_SOLVER_PATH}/cxx/generateResponseModule")

set(SOURCES
    "agent/GenerateResponseAgent.cpp"
    "agent/GenerateResponseAgent.hpp"
    "keynodes/Keynodes.cpp"
    "keynodes/Keynodes.hpp"
    "GenerateResponseModule.cpp"
    "GenerateResponseModule.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
)

add_library(generate_response SHARED ${SOURCES})
target_link_libraries(generate_response sc-memory sc-agents-common)

sc_codegen_ex(generate_response ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(generate_response)
endif()

add_definitions(-DGENERATE_RESPONSE_MODULE_PATH="${CMAKE_CURRENT_LIST_DIR}/")
if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

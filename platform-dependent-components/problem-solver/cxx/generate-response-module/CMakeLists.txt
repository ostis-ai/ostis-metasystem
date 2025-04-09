set(GENERATE_RESPONSE_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(HEADERS
        "agent/generate-response-agent.hpp"
        "constants/generate_response_constants.hpp"
        "keynodes/generate_response_keynodes.hpp"
        "generate_response_module.hpp"
)

set(SOURCES
        "agent/generate-response-agent.cpp"
        "constants/generate_response_constants.cpp"
        "generate_response_module.cpp"
)

add_library(generate-response-module SHARED ${HEADERS} ${SOURCES})

target_link_libraries(generate-response-module
        LINK_PUBLIC sc-memory
        LINK_PUBLIC sc-agents-common
        LINK_PUBLIC sc-core)
target_include_directories(generate-response-module
        PUBLIC ${GENERATE_RESPONSE_MODULE_SRC}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(generate-response-module)
endif()

add_definitions(-DGENERATE_RESPONSE_MODULE_PATH="${CMAKE_CURRENT_LIST_DIR}/")
if(${SC_BUILD_TESTS})
    include(${GENERATE_RESPONSE_MODULE_SRC}/test/tests.cmake)
endif()

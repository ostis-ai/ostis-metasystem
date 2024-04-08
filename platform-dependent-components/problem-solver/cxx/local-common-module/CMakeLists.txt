set(SOURCES
    "http/sc_http_request.cpp"
    "http/sc_http_response.cpp"
    "http/sc_http.cpp"
    "keynodes/LocalKeynodes.cpp"
)

set(HEADERS
    "http/sc_http_request.hpp"
    "http/sc_http_response.hpp"
    "http/sc_http.hpp"
    "keynodes/LocalKeynodes.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC})

add_library(local-common SHARED ${SOURCES} ${HEADERS})

target_link_libraries(local-common sc-memory sc-agents-common)

sc_codegen_ex(local-common ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

# if(${SC_BUILD_TESTS})
#     include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
# endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(local-common)
endif()

set(SOURCES
    "LocalCommonModule.cpp"
    "http/sc_http_request.cpp"
    "http/sc_http_response.cpp"
    "http/sc_http.cpp"
    "keynodes/LocalKeynodes.cpp"
)

set(HEADERS
    "LocalCommonModule.hpp"
    "http/sc_http_request.hpp"
    "http/sc_http_response.hpp"
    "http/sc_http.hpp"
    "keynodes/LocalKeynodes.hpp"
)

add_library(local-common SHARED ${SOURCES} ${HEADERS})

find_package(CURL REQUIRED)

target_include_directories(local-common
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_KPM_SRC}
    PUBLIC ${SC_TOOLS_SRC}
    PUBLIC ${CURL_INCLUDE_DIRS}
)

target_link_libraries(local-common sc-memory sc-agents-common ${CURL_LIBRARIES})

sc_codegen_ex(local-common ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

# if(${SC_BUILD_TESTS})
#     include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
# endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(local-common)
endif()

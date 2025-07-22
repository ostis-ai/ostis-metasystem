set(SOURCES
    "LocalCommonModule.cpp"
    "http/sc_http_request.cpp"
    "http/sc_http_response.cpp"
    "http/sc_http.cpp"
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

target_link_libraries(local-common
        LINK_PUBLIC sc-machine::sc-memory
        LINK_PUBLIC sc-machine::sc-agents-common
        LINK_PUBLIC sc-machine::sc-core
        ${CURL_LIBRARIES})
target_include_directories(local-common
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC ${SC_MEMORY_INCLUDE}
    PUBLIC ${SC_KPM_SRC}
    PUBLIC ${SC_TOOLS_SRC}
    PUBLIC ${SC_CORE_INCLUDE}
    PUBLIC ${CURL_INCLUDE_DIRS}
)
# target_include_directories(generate-response-module
#         PUBLIC ${GENERATE_RESPONSE_MODULE_SRC}
#         PUBLIC ${SC_MEMORY_INCLUDE}
#         PUBLIC ${SC_CORE_INCLUDE}
#         PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
# )

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(local-common)
endif()

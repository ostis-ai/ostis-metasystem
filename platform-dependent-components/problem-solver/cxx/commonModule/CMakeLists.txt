set(HEADERS
    "utils/SetUtils.hpp")

set(SOURCES
    "utils/SetUtils.cpp")

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC})

add_library(commonModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(commonModule sc-memory sc-agents-common)

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(commonModule)
endif ()

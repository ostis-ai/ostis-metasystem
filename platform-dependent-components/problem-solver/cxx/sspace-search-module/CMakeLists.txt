file(GLOB SOURCES CONFIGURE_DEPENDS
    "*.c" "*.h"
    "*/*.c" "*/*.h"
)

add_library(sspace-search-module SHARED ${SOURCES})
target_link_libraries(sspace-search-module LINK_PUBLIC sc-kpm-common)
target_include_directories(sspace-search-module
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/.."
    PUBLIC "${SC_MEMORY_SRC}"
    PUBLIC "${SC_KPM_SRC}"
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sspace-search-module)
endif()

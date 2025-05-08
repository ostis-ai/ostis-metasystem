file(GLOB SOURCES CONFIGURE_DEPENDS "*.c" "*.h" "*/*.c" "*/*.h")

add_library(utils-module SHARED ${SOURCES})
target_link_libraries(utils-module
    LINK_PRIVATE sc-memory
    LINK_PRIVATE sc-kpm-common
#    LINK_PRIVATE sc-search
)

target_include_directories(utils-module
    PUBLIC "${SC_MEMORY_SRC}"
    PUBLIC "${SC_KPM_SRC}"
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(utils-module)
endif()

set(STRUCTURE_TRANSLATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# Collect all files recursively
file(GLOB_RECURSE ALL_SOURCES ${ROOT_DIR}/*.cpp)
file(GLOB_RECURSE ALL_HEADERS ${ROOT_DIR}/*.hpp)
# Exclude files in the 'test' folder
file(GLOB_RECURSE TEST_FILES ${ROOT_DIR}/test/*)
list(REMOVE_ITEM ALL_SOURCES ${TEST_FILES})
list(REMOVE_ITEM ALL_HEADERS ${TEST_FILES})
# Set the SOURCE variable
set(SOURCES ${ALL_SOURCES})
set(HEADERS ${ALL_HEADERS})

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        ${SC_MEMORY_SRC}
        ${SC_KPM_SRC}
        ${SC_TOOLS_SRC})

add_library(structure-translation-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(structure-translation-module
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
)

target_include_directories(structure-translation-module
    PUBLIC ${STRUCTURE_TRANSLATION_MODULE_SRC}
    PUBLIC ${SC_MEMORY_INCLUDE}
    PUBLIC ${SC_CORE_INCLUDE}
    PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
    PUBLIC ${SC_BUILDER_INCLUDE}
)
set_target_properties(structure-translation-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if (${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif ()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(structure-translation-module)
endif()

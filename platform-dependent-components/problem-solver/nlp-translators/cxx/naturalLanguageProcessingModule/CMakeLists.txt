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

add_library(naturalLanguageProcessingModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(naturalLanguageProcessingModule sc-memory sc-agents-common)

sc_codegen_ex(naturalLanguageProcessingModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if (${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif ()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(naturalLanguageProcessingModule)
endif()

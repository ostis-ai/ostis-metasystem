cmake_minimum_required(VERSION 3.11)
project(ostis-metasystem)
set(CMAKE_CXX_STANDARD 17)

set(SC_MACHINE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ostis-web-platform/sc-machine")
include(${SC_MACHINE_PATH}/macro/macros.cmake)

option(SC_BUILD_TESTS "Flag to build unit tests" ON)

code_coverage(SC_COVERAGE "Flag to generate coverage report" OFF "-g -O0 --coverage")
option(SC_CLANG_FORMAT_CODE "Flag to add clangformat and clangformat_check targets" OFF)
sanitizer(SC_USE_SANITIZER "Build with specified sanitizer" "none")

if(NOT SC_BIN_PATH)
    set(SC_BIN_PATH "${CMAKE_BINARY_DIR}/../bin")
endif()
set(SC_EXTENSIONS_DIRECTORY "${SC_BIN_PATH}/extensions")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SC_BIN_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
link_directories(${SC_BIN_PATH} ${SC_EXTENSIONS_DIRECTORY})

set(CMAKE_MODULE_PATH "${SC_MACHINE_PATH}/cmake")
include("${CMAKE_MODULE_PATH}/codegen.cmake")

if(${SC_CLANG_FORMAT_CODE})
    include(${CMAKE_MODULE_PATH}/ClangFormat.cmake)
endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -std=c++17")
    if (${SC_USE_SANITIZER} STREQUAL "none" AND NOT ${SC_COVERAGE})
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
    if(${SC_USE_SANITIZER} STREQUAL "none" AND NOT ${SC_COVERAGE})
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3") ## Optimize
    endif()
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -s") ## Strip binary
elseif (NOT ${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

set(SC_MEMORY_SRC "${SC_MACHINE_PATH}/sc-memory/")
set(SC_KPM_SRC "${SC_MACHINE_PATH}/sc-kpm/")
set(SC_TOOLS_SRC "${SC_MACHINE_PATH}/sc-tools/")

include("${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake")
sc_target_dependencies()

# for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
endforeach(OUTPUT_CONFIG)

if(${SC_BUILD_TESTS})
    include(${CMAKE_MODULE_PATH}/tests.cmake)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
endif()

add_subdirectory(platform-dependent-components/problem-solver)

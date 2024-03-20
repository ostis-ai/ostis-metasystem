cmake_minimum_required(VERSION 3.11.0)
project(ostis-metasystem)
set(CMAKE_CXX_STANDARD 17)

option(SC_BUILD_TESTS "Flag to build unit tests" OFF)

if(NOT SC_BIN_PATH)
    set(SC_BIN_PATH "${CMAKE_BINARY_DIR}/../bin")
endif()
set(SC_EXTENSIONS_DIRECTORY "${SC_BIN_PATH}/extensions")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})
link_directories(${SC_BIN_PATH} ${SC_EXTENSIONS_DIRECTORY})

set(SC_MACHINE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ostis-web-platform/sc-machine")
include(${SC_MACHINE_PATH}/CMakeLists.txt)

if(${SC_BUILD_TESTS})
    include(${CMAKE_MODULE_PATH}/tests.cmake)
endif()

if(${WIN32})
    message(SEND_ERROR "ostis-metasystem isn't supported on Windows OS.")
endif(${WIN32})

# for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUT_CONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${SC_BIN_PATH})
endforeach(OUTPUT_CONFIG)

add_subdirectory(platform-dependent-components/problem-solver)

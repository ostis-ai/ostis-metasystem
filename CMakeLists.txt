project(ostis-metasystem)
cmake_minimum_required(VERSION 2.8.12)

set(PLATFORM_PATH "${CMAKE_CURRENT_LIST_DIR}/ostis-web-platform")
set(PROBLEM_SOLVER_PATH "${CMAKE_CURRENT_LIST_DIR}/platform-dependent-components/problem-solver")
set(SC_MACHINE_PATH "${PLATFORM_PATH}/sc-machine")
include("${SC_MACHINE_PATH}/CMakeLists.txt")
project_version(METASYSTEM_PROJECT "0.0.0")

# platform doesn't support multi-extensions. Have to put project binaries to sc-machine extensions
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin/extensions")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

# for multi-config builds (e.g. msvc)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUT_CONFIG} output_required_filesUT_CONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} ${SC_EXTENSIONS_DIRECTORY})
endforeach(OUTPUT_CONFIG CMAKE_CONFIGURATION_TYPES)

add_subdirectory(platform-dependent-components/problem-solver)

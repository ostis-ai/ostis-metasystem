set(HEADERS
    "subject_domain_module.hpp"
    "agent/get_decomposition_agent.hpp"
    "agent/add_section_agent.hpp"
    "agent/remove_section_agent.hpp"
    "builder/sections_builder.hpp"
    "constants/sections_aliases.hpp"
    "generator/sections_generator.hpp"
    "keynodes/sections_keynodes.hpp"
    "utils/set_utils.hpp"
    "utils/sections_utils.hpp")

set(SOURCES
    "subject_domain_module.cpp"
    "agent/get_decomposition_agent.cpp"
    "agent/add_section_agent.cpp"
    "agent/remove_section_agent.cpp"
    "builder/sections_builder.cpp"
    "constants/sections_aliases.cpp"
    "generator/sections_generator.cpp"
    "keynodes/sections_keynodes.cpp"
    "utils/set_utils.cpp"
    "utils/sections_utils.cpp")

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_COMMON_MODULE_SRC}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC})


find_package(nlohmann_json 3.2.0 REQUIRED)
add_library(sections-module SHARED ${SOURCES} ${HEADERS})
target_link_libraries(sections-module sc-memory sc-agents-common nlohmann_json::nlohmann_json)

sc_codegen_ex(sections-module ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if (${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif ()

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sections-module)
endif ()

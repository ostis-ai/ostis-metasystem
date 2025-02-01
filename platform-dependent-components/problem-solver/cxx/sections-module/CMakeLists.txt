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
    "utils/sections_utils.hpp"
)

set(SOURCES
    "subject_domain_module.cpp"
    "agent/get_decomposition_agent.cpp"
    "agent/add_section_agent.cpp"
    "agent/remove_section_agent.cpp"
    "builder/sections_builder.cpp"
    "constants/sections_aliases.cpp"
    "generator/sections_generator.cpp"
    "utils/set_utils.cpp"
    "utils/sections_utils.cpp"
)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(sections-module SHARED ${HEADERS} ${SOURCES})
target_link_libraries(sections-module
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
    LINK_PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(sections-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sections-module)
endif()

if(${SC_BUILD_TESTS})
    set(SECTIONS_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()

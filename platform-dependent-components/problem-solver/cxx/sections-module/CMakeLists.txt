set(SECTIONS_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

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

add_library(sections-module SHARED ${HEADERS} ${SOURCES})
target_link_libraries(sections-module
    LINK_PUBLIC sc-memory
    LINK_PUBLIC sc-agents-common
    LINK_PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(sections-module
    PUBLIC ${SECTIONS_MODULE_SRC}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_CORE_PATH}
    PUBLIC ${SC_AGENTS_COMMON_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(sections-module)
endif()

if(${SC_BUILD_TESTS})
    include(${SECTIONS_MODULE_SRC}/test/tests.cmake)
endif()

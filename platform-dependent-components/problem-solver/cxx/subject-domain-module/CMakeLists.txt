set(HEADERS
    "subject_domain_module.hpp"
    "agent/get_decomposition_agent.hpp"
    "agent/add_section_agent.hpp"
    "agent/remove_section_agent.hpp"
    "builder/subject_domain_builder.hpp"
    "constants/subject_domain_aliases.hpp"
    "constants/sc_search_aliases.hpp"
    "generator/subject_domain_generator.hpp"
    "keynodes/subject_domain_keynodes.hpp"
    "utils/set_utils.hpp"
    "utils/subject_domain_utils.hpp")

set(SOURCES
    "subject_domain_module.cpp"
    "agent/get_decomposition_agent.cpp"
    "agent/add_section_agent.cpp"
    "agent/remove_section_agent.cpp"
    "builder/subject_domain_builder.cpp"
    "constants/subject_domain_aliases.cpp"
    "constants/sc_search_aliases.cpp"
    "generator/subject_domain_generator.cpp"
    "keynodes/subject_domain_keynodes.cpp"
    "utils/set_utils.cpp"
    "utils/subject_domain_utils.cpp")

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_COMMON_MODULE_SRC}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC})


find_package(nlohmann_json 3.2.0 REQUIRED)
add_library(subjectDomainModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(subjectDomainModule sc-memory sc-agents-common nlohmann_json::nlohmann_json)

sc_codegen_ex(subjectDomainModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if (${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif ()

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(subjectDomainModule)
endif ()

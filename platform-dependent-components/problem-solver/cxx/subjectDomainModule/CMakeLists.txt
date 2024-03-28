set(HEADERS
    "SubjectDomainModule.hpp"
    "agent/GetDecompositionAgent.hpp"
    "agent/AddSectionAgent.hpp"
    "agent/RemoveSectionAgent.hpp"
    "builder/SubjDomainBuilder.hpp"
    "constants/SubjDomainAliases.hpp"
    "constants/ScSearchAliases.hpp"
    "generator/SubjDomainGenerator.hpp"
    "keynodes/Keynodes.hpp"
    "utils/SubjDomainUtils.hpp")

set(SOURCES
    "SubjectDomainModule.cpp"
    "agent/GetDecompositionAgent.cpp"
    "agent/AddSectionAgent.cpp"
    "agent/RemoveSectionAgent.cpp"
    "builder/SubjDomainBuilder.cpp"
    "constants/SubjDomainAliases.cpp"
    "constants/ScSearchAliases.cpp"
    "generator/SubjDomainGenerator.cpp"
    "keynodes/Keynodes.cpp"
    "utils/SubjDomainUtils.cpp")

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_COMMON_MODULE_SRC}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC})


find_package(nlohmann_json 3.2.0 REQUIRED)
add_library(subjDomainModule SHARED ${SOURCES} ${HEADERS})
target_link_libraries(subjDomainModule sc-memory sc-agents-common commonModule nlohmann_json::nlohmann_json)

sc_codegen_ex(subjDomainModule ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if (${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif ()

if (${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(subjDomainModule)
endif ()

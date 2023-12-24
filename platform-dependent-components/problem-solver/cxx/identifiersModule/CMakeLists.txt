set(IDENTIFIERS_MODULE_SRC "${PROBLEM_SOLVER_PATH}/cxx/identifiersModule")

set(SOURCES
        "agent/TranslateMainSystemIdtfsFromScToFileAgent.cpp" "agent/TranslateMainSystemIdtfsFromScToFileAgent.hpp"
        "keynodes/IdentifiersKeynodes.cpp" "keynodes/IdentifiersKeynodes.hpp"
        "IdentifiersModule.cpp" "IdentifiersModule.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
)

add_library(identifiers SHARED ${SOURCES})
target_link_libraries(identifiers sc-memory sc-agents-common)

sc_codegen_ex(identifiers ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(identifiers)
endif()

add_definitions(-DIDENTIFIERS_MODULE_PATH="${CMAKE_CURRENT_LIST_DIR}/")

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()
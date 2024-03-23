set(IDENTIFIERS_MODULE_SRC "${PROBLEM_SOLVER_PATH}/cxx/identifiers-module")

set(SOURCES
    "agent/translate_main_system_idtfs_from_sc_to_file_agent.cpp"
    "agent/translate_main_system_idtfs_from_sc_to_file_agent.hpp"
    "keynodes/identifiers_keynodes.cpp"
    "keynodes/identifiers_keynodes.hpp"
    "identifiers_module.cpp"
    "identifiers_module.hpp"
)

add_library(identifiers SHARED ${SOURCES})
target_link_libraries(identifiers
    LINK_PUBLIC sc-memory
    LINK_PUBLIC sc-agents-common
)
target_include_directories(identifiers
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_KPM_SRC}
)

sc_codegen_ex(identifiers ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(identifiers)
endif()

add_definitions(-DIDENTIFIERS_MODULE_PATH="${CMAKE_CURRENT_LIST_DIR}/")
if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

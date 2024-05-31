set(STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCES
    "structured_representation_generation_module.hpp"
    "structured_representation_generation_module.cpp"

    "agent/structured_representation_generation_agent.hpp"
    "agent/structured_representation_generation_agent.cpp"
    "agent/generation_rule_search_agent.hpp"
    "agent/generation_rule_search_agent.cpp"

    "constants/structured_representation_generation_constants.hpp"

    "generator/structured_representation_generator.hpp"
    "generator/structured_representation_generator.cpp"

    "keynodes/structured_representation_generation_keynodes.hpp"

    "manager/structured_representation_generation_manager.hpp"
    "manager/structured_representation_generation_manager.cpp"

    "utils/template_utils.hpp"
    "utils/template_utils.cpp"
)

add_library(structured-representation-generation SHARED ${SOURCES})
target_link_libraries(structured-representation-generation
    LINK_PUBLIC sc-memory 
    LINK_PUBLIC sc-agents-common
)
include_directories(structured-representation-generation
    PUBLIC ${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_KPM_SRC}
)

sc_codegen_ex(structured-representation-generation ${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC} ${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(structured-representation-generation)
endif()

add_definitions(-DSTRUCTURED_REPRESENTATION_GENERATION_MODULE_PATH="${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC}/")
# if(${SC_BUILD_TESTS})
#     include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
# endif()

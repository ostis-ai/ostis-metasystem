set(VERIFICATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(HEADERS
        "agent/check-duplicate-constructions-agent.hpp"
        "agent/not-enough-specified-objects-search-agent.hpp"
        "constants/verification_constants.hpp"
        "keynodes/verification_keynodes.hpp"
        "searcher/check_duplicate_constructions_searcher.hpp"
        "generator/verification_generator.hpp"
        "verification_module.hpp"
)

set(SOURCES
        "agent/check-duplicate-constructions-agent.cpp"
        "agent/not-enough-specified-objects-search-agent.cpp"
        "constants/verification_constants.cpp"
        "searcher/check_duplicate_constructions_searcher.cpp"
        "generator/verification_generator.cpp"
        "verification_module.cpp"
)

add_library(verification-module SHARED ${HEADERS} ${SOURCES})
target_link_libraries(verification-module
        LINK_PUBLIC sc-memory
        LINK_PUBLIC sc-agents-common
        LINK_PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(verification-module
        PUBLIC ${VERIFICATION_MODULE_SRC}
        PUBLIC ${SC_MEMORY_SRC}
        PUBLIC ${SC_CORE_PATH}
        PUBLIC ${SC_AGENTS_COMMON_PATH}
        PUBLIC ${SC_KPM_SRC}
        PUBLIC ${SC_BUILDER_PATH}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(verification-module)
endif()

if(${SC_BUILD_TESTS})
    include(${VERIFICATION_MODULE_SRC}/test/tests.cmake)
endif()

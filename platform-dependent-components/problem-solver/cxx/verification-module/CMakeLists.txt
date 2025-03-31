set(VERIFICATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(HEADERS
        "agent/check-duplicate-constructions-agent.hpp"
        "config/config.hpp"
        "constants/verification_constants.hpp"
        "keynodes/verification_keynodes.hpp"
        "searcher/check_duplicate_constructions_searcher.hpp"
        "handler/verification_result_file_handler.hpp"
        "manager/duplications_check_manager.hpp"
        "logger/verification_result_logger.hpp"
        "utils/search_utils.hpp"
        "verification_module.hpp"
)

set(SOURCES
        "agent/check-duplicate-constructions-agent.cpp"
        "config/config.cpp"
        "constants/verification_constants.cpp"
        "searcher/check_duplicate_constructions_searcher.cpp"
        "handler/verification_result_file_handler.cpp"
        "manager/duplications_check_manager.cpp"
        "logger/verification_result_logger.cpp"
        "utils/search_utils.cpp"
        "verification_module.cpp"
)

add_library(verification-module SHARED ${HEADERS} ${SOURCES})

target_link_libraries(verification-module
        LINK_PUBLIC sc-memory
        LINK_PUBLIC sc-agents-common)
target_include_directories(verification-module
        PUBLIC ${VERIFICATION_MODULE_SRC}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
        PUBLIC ${SC_KPM_INCLUDE}
        PUBLIC ${SC_BUILDER_INCLUDE}
        PUBLIC ${SC_CONFIG_INCLUDE}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(verification-module)
endif()

if(${SC_BUILD_TESTS})
    include(${VERIFICATION_MODULE_SRC}/test/tests.cmake)
endif()
set(HEADERS
        "agent/check-duplicate-constructions-agent.hpp"
        "config/config.hpp"
        "constants/verification_constants.hpp"
        "keynodes/verification_keynodes.hpp"
        "handler/verification_result_file_handler.hpp"
        "manager/duplications_check_manager.hpp"
        "logger/verification_result_logger.hpp"
        "utils/search_utils.hpp"
        "utils/identifier_utils.hpp"
        "utils/container_utils.hpp"
        "verification_module.hpp"
)

set(SOURCES
        "agent/check-duplicate-constructions-agent.cpp"
        "config/config.cpp"
        "constants/verification_constants.cpp"
        "handler/verification_result_file_handler.cpp"
        "manager/duplications_check_manager.cpp"
        "logger/verification_result_logger.cpp"
        "utils/search_utils.cpp"
        "utils/identifier_utils.cpp"
        "utils/container_utils.cpp"
        "verification_module.cpp"
)

add_library(verification-module SHARED ${HEADERS} ${SOURCES})

target_link_libraries(verification-module
        LINK_PUBLIC sc-machine::sc-memory
        LINK_PUBLIC sc-machine::sc-config
        LINK_PUBLIC sc-machine::sc-agents-common)
target_include_directories(verification-module
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)
set_target_properties(verification-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(verification-module)
endif()

if(${SC_BUILD_TESTS})
    set(VERIFICATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")
    include(${VERIFICATION_MODULE_SRC}/test/tests.cmake)
endif()

set(SEARCH_ANSWER_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(HEADERS
        "agent/search-properties-agent.hpp"
        "constants/search_answer_constants.hpp"
        "keynodes/search_answer_keynodes.hpp"
        "search_answer_module.hpp"
)

set(SOURCES
        "agent/search-properties-agent.cpp"
        "constants/search_answer_constants.cpp"
        "search_answer_module.cpp"
)

add_library(search-answer-module SHARED ${HEADERS} ${SOURCES})

target_link_libraries(search-answer-module
        LINK_PUBLIC sc-machine::sc-memory
        LINK_PUBLIC sc-machine::sc-agents-common)
target_include_directories(search-answer-module
        PUBLIC ${SEARCH_ANSWER_MODULE_SRC}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
        PUBLIC ${SC_BUILDER_INCLUDE}
)
set_target_properties(search-answer-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(search-answer-module)
endif()

if(${SC_BUILD_TESTS})
    include(${SEARCH_ANSWER_MODULE_SRC}/test/tests.cmake)
endif()

# ./../sc-machine/build/Release/bin/sc-builder -i repo.path -o kb.bin --clear

# ../sc-machine/build/Release/bin/sc-machine -s kb.bin -c ostis-metasystem.ini -e "../sc-machine/build/Release/lib/extensions;build/Release/lib/extensions;../ostis-ps-lib/non-atomic-action-interpreter-module/build/Release/lib/extensions"
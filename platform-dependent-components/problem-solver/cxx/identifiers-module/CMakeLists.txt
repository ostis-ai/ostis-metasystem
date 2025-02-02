set(HEADERS
    "agent/translate_main_system_idtfs_from_sc_to_file_agent.hpp"
    "keynodes/identifiers_keynodes.hpp"
    "identifiers_module.hpp"
)

set(SOURCES
    "agent/translate_main_system_idtfs_from_sc_to_file_agent.cpp"
    "identifiers_module.cpp"
)

add_library(identifiers-module SHARED ${SOURCES})
target_link_libraries(identifiers-module
    LINK_PUBLIC sc-machine::sc-memory
    LINK_PUBLIC sc-machine::sc-agents-common
)
target_include_directories(identifiers-module
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
set_target_properties(identifiers-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(identifiers-module)
endif()

if(${SC_BUILD_TESTS})
    set(IDENTIFIERS_MODULE_SRC ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(test)
endif()

set(MESSAGE_CLASSIFICATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(SC_MACHINE_DIR "${CMAKE_SOURCE_DIR}/ostis-web-platform/sc-machine")
set(SC_CONFIG_INCLUDE_DIR "${SC_MACHINE_DIR}/sc-tools/sc-config/include")
set(SC_CONFIG_UTILS_INCLUDE_DIR "${SC_TOOLS_SRC}/sc-config-utils")

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_CONFIG_INCLUDE_DIR}
    ${SC_CONFIG_UTILS_INCLUDE_DIR}
    ${SC_TOOLS_SRC}/sc-config-utils
    ${LOCAL_COMMON_MODULE_PATH}
    ${COMMON_MODULE_PATH}
    ${GLIB2_INCLUDE_DIRS}
)

set(SOURCES
    "message_classification_module.cpp"
    "agent/message-topic-classification-agent.cpp"
    "classifier/MessageTopicClassifier.cpp"
    "client/WitAiClient.cpp"
    "manager/MessageTopicClassificationManager.cpp"
    "constants/MessageClassificationConstants.cpp"
    "searcher/MessageSearcher.cpp"
)

set(HEADERS
    "message_classification_module.hpp"
    "keynodes/message_classification_keynodes.hpp"
    "agent/message-topic-classification-agent.hpp"
    "classifier/MessageTopicClassifier.hpp"
    "client/WitAiClient.hpp"
    "keynodes/message_keynodes.hpp"
    "keynodes/message_classification_keynodes.hpp"
    "manager/MessageTopicClassificationManager.hpp"
    "client/WitAiClientInterface.hpp"
    "constants/MessageClassificationConstants.hpp"
    "searcher/MessageSearcher.hpp"
)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 REQUIRED glib-2.0)
pkg_search_module(GLIB2_MODULE REQUIRED gmodule-2.0)



add_library(message-classification-module SHARED ${SOURCES} ${HEADERS})

target_link_libraries(message-classification-module
        LINK_PUBLIC local-common
        LINK_PUBLIC ps-common-lib::common-utils
        LINK_PUBLIC sc-machine::sc-memory
        LINK_PUBLIC sc-machine::sc-agents-common
        LINK_PUBLIC sc-machine::sc-config)
target_include_directories(message-classification-module
        PUBLIC ${MESSAGE_CLASSIFICATION_MODULE_SRC}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
        PUBLIC ${SC_CONFIG_INCLUDE_DIR}
)
set_target_properties(message-classification-module PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${SC_EXTENSIONS_DIRECTORY})

add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${CMAKE_SOURCE_DIR}/ostis-metasystem.ini")

# if(${SC_BUILD_TESTS})
#     include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
# endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-classification-module)
endif()

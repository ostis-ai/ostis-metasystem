set(MESSAGE_CLASSIFICATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCES
    "message_classification_module.cpp"
    "keynodes/message_classification_keynodes.cpp"
    "agent/message-topic-classification-agent.cpp"
    "classifier/MessageTopicClassifier.cpp"
    "client/WitAiClient.cpp"
    "keynodes/message_keynodes.cpp"
    "keynodes/MessageKeynodes.cpp"
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
    "keynodes/MessageKeynodes.hpp"
    "manager/MessageTopicClassificationManager.hpp"
    "client/WitAiClientInterface.hpp"
    "constants/MessageClassificationConstants.hpp"
    "searcher/MessageSearcher.hpp"
)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB2 REQUIRED glib-2.0)
pkg_search_module(GLIB2_MODULE REQUIRED gmodule-2.0)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
    ${SC_TOOLS_SRC}/sc-config-utils
    ${LOCAL_COMMON_MODULE_PATH}
    ${COMMON_MODULE_PATH}
    ${GLIB2_INCLUDE_DIRS}
)

add_library(message-classification-module SHARED ${SOURCES} ${HEADERS})

target_link_libraries(message-classification-module
        LINK_PUBLIC local-common
        LINK_PUBLIC common-ps-lib
        LINK_PUBLIC sc-memory
        LINK_PUBLIC sc-agents-common
        LINK_PUBLIC sc-config-utils)
target_include_directories(message-classification-module
        PUBLIC ${MESSAGE_CLASSIFICATION_MODULE_SRC}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
)

add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${CMAKE_SOURCE_DIR}/ostis-metasystem.ini")

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-classification-module)
endif()

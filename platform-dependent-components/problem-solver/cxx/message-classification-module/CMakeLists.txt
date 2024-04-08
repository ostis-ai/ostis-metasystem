set(SOURCES
    "MessageClassificationModule.cpp"
    "keynodes/MessageClassificationKeynodes.cpp"
    "agent/MessageTopicClassificationAgent.cpp"
    "classifier/MessageTopicClassifier.cpp"
    "client/WitAiClient.cpp"
    "keynodes/MessageClassificationKeynodes.cpp"
    "keynodes/MessageKeynodes.cpp"
    "manager/MessageTopicClassificationManager.cpp"
    "constants/MessageClassificationConstants.cpp"
    "searcher/MessageSearcher.cpp"
)

set(HEADERS
    "MessageClassificationModule.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
    "agent/MessageTopicClassificationAgent.hpp"
    "classifier/MessageTopicClassifier.hpp"
    "client/WitAiClient.hpp"
    "keynodes/MessageClassificationKeynodes.hpp"
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

add_library(messageClassification SHARED ${SOURCES} ${HEADERS})
target_link_libraries(
    messageClassification
    local-common
    common
    sc-memory
    sc-agents-common
    sc-config-utils
)

sc_codegen_ex(messageClassification ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

add_definitions(-DMESSAGE_CLASSIFICATION_CONFIG_PATH="${CMAKE_CURRENT_LIST_DIR}/ostis-metasystem.ini")

if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(messageClassification)
endif()

file(GLOB MESSAGE_REPLY_TEST_AGENTS_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/agent/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/agent/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/../keynodes/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/../keynodes/*.hpp"
)
add_library(message_reply_test_agents ${MESSAGE_REPLY_TEST_AGENTS_SOURCE})
target_link_libraries(
    message_reply_test_agents
    message-reply
    sc-agents-common
)

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message_reply_module_test_starter
    DEPENDS sc-builder-lib message_reply_test_agents message-reply sc-core
    INCLUDES ${SC_MEMORY_SRC}/tests/sc-memory/_test ${CMAKE_CURRENT_LIST_DIR}/.. ${SC_TOOLS_SRC}/sc-builder/src ${SC_MEMORY_SRC}/sc-memory ${SC_MACHINE_ROOT}/sc-tools
)

add_definitions(-DMESSAGE_REPLY_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message_reply_module_test_starter)
endif()
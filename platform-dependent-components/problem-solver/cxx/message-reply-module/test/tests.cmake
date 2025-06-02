# make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
# NAME message-reply-module-test-starter
# DEPENDS sc-memory sc-core sc-builder-lib message-reply-module
# INCLUDES ${SC_MEMORY_INCLUDE} ${SC_BUILDER_INCLUDE}
# )

make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
    NAME message-reply-module-test-starter
    DEPENDS sc-memory sc-core sc-builder-lib  message_reply_test_agents message-reply-module
    INCLUDES ${SC_MEMORY_INCLUDE} ${SC_BUILDER_INCLUDE}
)

file(GLOB MESSAGE_REPLY_TEST_AGENTS_SOURCE
    "${CMAKE_CURRENT_LIST_DIR}/agent/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/agent/*.hpp"
    "${CMAKE_CURRENT_LIST_DIR}/../keynodes/*.cpp" "${CMAKE_CURRENT_LIST_DIR}/../keynodes/*.hpp"
)
add_library(message_reply_test_agents ${MESSAGE_REPLY_TEST_AGENTS_SOURCE})
target_link_libraries(
    message_reply_test_agents
    message-reply-module
    sc-memory
    sc-agents-common
)
target_include_directories(message-reply-module
        PUBLIC ${MESSAGE_REPLY_TEST_AGENTS_SOURCE}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
)


add_definitions(-DMODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")
if(${SC_CLANG_FORMAT_CODE})
target_clangformat_setup(message-reply-module-test-starter)
endif()
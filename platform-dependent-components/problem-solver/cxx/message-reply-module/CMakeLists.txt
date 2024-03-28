set(IDENTIFIERS_MODULE_SRC "${PROBLEM_SOLVER_PATH}/cxx/message-reply-module")

set(SOURCES
    "agent/MessageReplyAgent.cpp"
    "agent/MessageReplyAgent.hpp"
    "keynodes/MessageReplyKeynodes.cpp"
    "keynodes/MessageReplyKeynodes.hpp"
    "MessageReplyModule.cpp"
    "MessageReplyModule.hpp"
)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${SC_MEMORY_SRC}
    ${SC_KPM_SRC}
)

add_library(message-reply SHARED ${SOURCES})
target_link_libraries(message-reply sc-memory sc-agents-common)

sc_codegen_ex(message-reply ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-reply)
endif()

add_definitions(-DIDENTIFIERS_MODULE_PATH="${CMAKE_CURRENT_LIST_DIR}/")
if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

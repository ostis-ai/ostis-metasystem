set(MESSAGE_REPLY_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCES
    "agent/MessageReplyAgent.cpp"
    "agent/MessageReplyAgent.hpp"
    "keynodes/MessageReplyKeynodes.cpp"
    "keynodes/MessageReplyKeynodes.hpp"
    "MessageReplyModule.cpp"
    "MessageReplyModule.hpp"
)

add_library(message-reply SHARED ${SOURCES})
target_link_libraries(message-reply 
    LINK_PUBLIC sc-memory 
    LINK_PUBLIC sc-agents-common
)
include_directories(message-reply
    PUBLIC ${MESSAGE_REPLY_MODULE_SRC}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_KPM_SRC}
)

sc_codegen_ex(message-reply ${MESSAGE_REPLY_MODULE_SRC} ${MESSAGE_REPLY_MODULE_SRC}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-reply)
endif()

add_definitions(-DMESSAGE_REPLY_MODULE_PATH="${MESSAGE_REPLY_MODULE_SRC}/")
# if(${SC_BUILD_TESTS})
#     include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
# endif()

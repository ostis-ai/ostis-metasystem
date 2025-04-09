set(MESSAGE_REPLY_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(HEADERS
        "agent/message-reply-agent.hpp"
        "keynodes/message_reply_keynodes.hpp"
        "message_reply_module.hpp"
)

set(SOURCES
        "agent/message-reply-agent.cpp"
        "message_reply_module.cpp"
)

add_library(message-reply-module SHARED ${HEADERS} ${SOURCES})

target_link_libraries(message-reply-module
    LINK_PUBLIC sc-memory 
    LINK_PUBLIC sc-agents-common
)
target_include_directories(message-reply-module
    PUBLIC ${MESSAGE_REPLY_MODULE_SRC}
    PUBLIC ${SC_MEMORY_INCLUDE}
    PUBLIC ${SC_CORE_INCLUDE}
    PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(message-reply-module)
>>>>>>> 0e7c22e (update ask-ai)
endif()

add_definitions(-DMESSAGE_REPLY_MODULE_PATH="${MESSAGE_REPLY_MODULE_SRC}/")
if(${SC_BUILD_TESTS})
    include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
endif()

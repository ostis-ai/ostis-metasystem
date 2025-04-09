set(LANGUAGE_RECOGNITION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(HEADERS
        "agent/language-recognition-agent.hpp"
        "keynodes/language_recognition_keynodes.hpp"
        "language_recognition_module.hpp"
)

set(SOURCES
        "agent/language-recognition-agent.cpp"
        "language_recognition_module.cpp"
)

add_library(language-recognition-module SHARED ${HEADERS} ${SOURCES})

target_link_libraries(language-recognition-module
        LINK_PUBLIC sc-memory
        LINK_PUBLIC sc-agents-common)
target_include_directories(language-recognition-module
        PUBLIC ${LANGUAGE_RECOGNITION_MODULE_SRC}
        PUBLIC ${SC_MEMORY_INCLUDE}
        PUBLIC ${SC_CORE_INCLUDE}
        PUBLIC ${SC_AGENTS_COMMON_INCLUDE}
)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(language-recognition-module)
endif()

if(${SC_BUILD_TESTS})
    include(${LANGUAGE_RECOGNITION_MODULE_SRC}/test/tests.cmake)
endif()

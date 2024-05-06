set(STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCES
    "StructuredRepresentationGenerationModule.cpp"
    "StructuredRepresentationGenerationModule.hpp"
)

add_library(structured-representation-generation SHARED ${SOURCES})
target_link_libraries(structured-representation-generation
    LINK_PUBLIC sc-memory 
    LINK_PUBLIC sc-agents-common
)
include_directories(structured-representation-generation
    PUBLIC ${MESSAGE_REPLY_MODULE_SRC}
    PUBLIC ${SC_MEMORY_SRC}
    PUBLIC ${SC_KPM_SRC}
)

sc_codegen_ex(structured-representation-generation ${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC} ${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC}/generated)

if(${SC_CLANG_FORMAT_CODE})
    target_clangformat_setup(structured-representation-generation)
endif()

add_definitions(-DSTRUCTURED_REPRESENTATION_GENERATION_MODULE_PATH="${STRUCTURED_REPRESENTATION_GENERATION_MODULE_SRC}/")
# if(${SC_BUILD_TESTS})
#     include(${CMAKE_CURRENT_LIST_DIR}/test/tests.cmake)
# endif()

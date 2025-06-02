make_tests_from_folder(${CMAKE_CURRENT_LIST_DIR}/units
NAME message_topic_classification_tests_starter
DEPENDS messageClassification sc-builder-lib sc-core sc-memory message-classification-module
INCLUDES ${SC_MEMORY_INCLUDE} ${SC_BUILDER_INCLUDE}
)

add_definitions(-DMESSAGE_TOPIC_CLASSIFICATION_MODULE_TEST_SRC_PATH="${CMAKE_CURRENT_LIST_DIR}")

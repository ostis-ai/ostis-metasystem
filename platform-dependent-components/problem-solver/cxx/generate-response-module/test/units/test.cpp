#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-memory/sc_keynodes.hpp"
#include "sc-memory/utils/sc_log.hpp"

#include "agent/generate-response-agent.hpp"
#include "test/agent/OneParameterTestAgent.hpp"
#include "test/agent/ZeroParameterTestAgent.hpp"
#include "test/keynodes/TestKeynodes.hpp"
#include "keynodes/generate_response_keynodes.hpp"

#include <filesystem>


using namespace generateResponseModule;
using AgentTest = ScMemoryTest;

namespace ModuleTest
{
ScsLoader loader;
const std::string TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";
const std::string TEMPLATES_DIR_PATH = GENERATE_RESPONSE_TEMPLATE_PATH;

const int WAIT_TIME = 1000;

// using GenerateRsponseAgentTest = ScMemoryTest;

void loadScsFile(ScAgentContext & context, std::string const & filename)
{
    loader.loadScsFile(context, TEST_FILES_DIR_PATH + filename);
}

void initialize(ScAgentContext & context)
{
    context.SubscribeAgent<generateResponseModule::GenerateResponseAgent>();

    for (auto const & file : std::filesystem::directory_iterator(TEMPLATES_DIR_PATH))
    {
        loader.loadScsFile(context, file.path());
    }
    loadScsFile(context, "baseConcepts.scs");
}

void shutdown(ScAgentContext & context)
{
    context.UnsubscribeAgent<generateResponseModule::GenerateResponseAgent>();
}

TEST_F(AgentTest, OneParameterAgentTest)
{
    ScAgentContext & context = *m_ctx;

    loadScsFile(context, "oneParameterTest.scs");

    initialize(context);


    context.SubscribeAgent<generateResponseModuleTest::OneParameterTestAgent>();

    ScAddr const testActionNode = context.SearchElementBySystemIdentifier("one_param_action_node");
    ScAddr const message = context.SearchElementBySystemIdentifier("message");
    ScAddr const answer = context.SearchElementBySystemIdentifier("_answer");
    
    ScAction testAction = context.ConvertToAction(testActionNode);

    EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
    SC_LOG_DEBUG("there");
    EXPECT_TRUE(testAction.IsFinishedSuccessfully());

    SC_LOG_DEBUG("here");

    ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, GenerateResponseKeynodes::nrel_reply_structure);

    EXPECT_TRUE(messageAnswer.IsValid());

    ScAddr testEntity = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

    EXPECT_TRUE(context.HelperGetSystemIdtf(testEntity) == "test_entity");

    EXPECT_TRUE(context.HelperCheckEdge(answer, messageAnswer, ScType::EdgeAccessConstPosTemp));
    
    context.UnsubscribeAgent<generateResponseModuleTest::OneParameterTestAgent>();
    shutdown(context);
}

TEST_F(AgentTest, ZeroParameterTestAgent)
{
    ScAgentContext & context = *m_ctx;
    loadScsFile(context, "zeroParameterTest.scs");

    initialize(context);

    context.SubscribeAgent<generateResponseModuleTest::ZeroParameterTestAgent>();

    ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("zero_param_action_node");
    ScAddr const & message = context.SearchElementBySystemIdentifier("message");
    ScAddr const & answer = context.SearchElementBySystemIdentifier("_answer");
    
    ScAction testAction = context.ConvertToAction(testActionNode);

    EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
    EXPECT_TRUE(testAction.IsFinishedSuccessfully());

    ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, generateResponseModule::GenerateResponseKeynodes::nrel_reply_structure);

    EXPECT_TRUE(messageAnswer.IsValid());

    ScAddr testSuccess = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

    ScAddr correctTestSuccess = context.HelperFindBySystemIdtf("test_success");
    EXPECT_TRUE(correctTestSuccess.IsValid() && testSuccess == correctTestSuccess);

    EXPECT_TRUE(context.HelperCheckEdge(answer, messageAnswer, ScType::EdgeAccessConstPosTemp));

    context.UnsubscribeAgent<generateResponseModuleTest::ZeroParameterTestAgent>();
    shutdown(context);
}
}

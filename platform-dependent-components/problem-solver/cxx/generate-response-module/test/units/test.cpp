#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>

#include "agent/GenerateResponseAgent.hpp"
#include "test/agent/OneParameterTestAgent.hpp"
#include "test/agent/ZeroParameterTestAgent.hpp"
#include "test/keynodes/TestKeynodes.hpp"
#include "keynodes/Keynodes.hpp"

#include <filesystem>

using namespace generateResponseModule;

namespace generateResponseModuleTest
{
    const std::string TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";
    const std::string TEMPLATES_DIR_PATH = GENERATE_RESPONSE_TEMPLATE_PATH;

    const int WAIT_TIME = 1000;

    using GenerateRsponseAgentTest = ScMemoryTest;

    ScsLoader loader;
    void loadScsFile(ScMemoryContext & context, std::string const & filename)
    {
        loader.loadScsFile(context, TEST_FILES_DIR_PATH + filename);
    }

    void initialize(ScMemoryContext & context)
    {

        context.SubscribeAgent<generateResponseModule::GenerateResponseAgent>();

        for (auto const & file : std::filesystem::directory_iterator(TEMPLATES_DIR_PATH))
        {
            loader.loadScsFile(context, file.path());
        }
        loadScsFile(context, "baseConcepts.scs");
    }

    void shutdown()
    {
        context.UnsubscribeAgent<generateResponseModule::GenerateResponseAgent>();
    }

    TEST_F(GenerateRsponseAgentTest, OneParameterAgentTest)
    {
        ScMemoryContext & context = *m_ctx;

        loadScsFile(context, "oneParameterTest.scs");

        initialize(context);

        context.SubscribeAgent<OneParameterTestAgent>();

        ScAddr const testActionNode = context.SearchElementBySystemIdentifier("one_param_action_node");
        ScAddr const message = context.SearchElementBySystemIdentifier("message");
        ScAddr const answer = context.SearchElementBySystemIdentifier("_answer");
        
        ScAction testAction = context.ConvertToAction(testActionNode);

        EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
        EXPECT_TRUE(testAction.IsFinishedSuccessfully());

        ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, GenerateResponseKeynodes::nrel_reply_structure);

        EXPECT_TRUE(messageAnswer.IsValid());

        ScAddr testEntity = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

        EXPECT_TRUE(context.HelperGetSystemIdtf(testEntity) == "test_entity");

        EXPECT_TRUE(context.HelperCheckEdge(answer, messageAnswer, ScType::EdgeAccessConstPosTemp));
        
        context.UnsubscribeAgent<OneParameterTestAgent>();
        shutdown();
    }

    TEST_F(GenerateRsponseAgentTest, ZeroParameterTestAgent)
    {
        ScMemoryContext & context = *m_ctx;
        loadScsFile(context, "zeroParameterTest.scs");

        initialize(context);

        context.SubscribeAgent<ZeroParameterTestAgent>();

        ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("zero_param_action_node");
        ScAddr const & message = context.SearchElementBySystemIdentifier("message");
        ScAddr const & answer = context.SearchElementBySystemIdentifier("_answer");
        
        ScAction testAction = context.ConvertToAction(testActionNode);

        EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
        EXPECT_TRUE(testAction.IsFinishedSuccessfully());

        ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, generateResponseModule::Keynodes::nrel_reply_structure);

        EXPECT_TRUE(messageAnswer.IsValid());

        ScAddr testSuccess = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

        ScAddr correctTestSuccess = context.HelperFindBySystemIdtf("test_success");
        EXPECT_TRUE(correctTestSuccess.IsValid() && testSuccess == correctTestSuccess);

        EXPECT_TRUE(context.HelperCheckEdge(answer, messageAnswer, ScType::EdgeAccessConstPosTemp));

        context.UnsubscribeAgent<ZeroParameterTestAgent>();
        shutdown();
    }
}

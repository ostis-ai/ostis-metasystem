#include "scs_loader.hpp"
#include "sc_test.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "agent/GenerateResponseAgent.hpp"
#include "test/agent/OneParameterTestAgent.hpp"
#include "test/agent/ZeroParameterTestAgent.hpp"
#include "test/keynodes/TestKeynodes.hpp"
#include "keynodes/Keynodes.hpp"

namespace generateResponseModuleTest
{
    const std::string TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";

    const int WAIT_TIME = 1000;

    using AgentTest = ScMemoryTest;

    ScsLoader loader;
    void loadScsFile(ScMemoryContext & context, std::string const & filename)
    {
        loader.loadScsFile(context, TEST_FILES_DIR_PATH + filename);
    }

    void initialize()
    {
        scAgentsCommon::CoreKeynodes::InitGlobal();
        generateResponseModule::Keynodes::InitGlobal();
        TestKeynodes::InitGlobal();
    }

    TEST_F(AgentTest, OneParameterAgentTest)
    {
        ScMemoryContext & context = *m_ctx;
        loadScsFile(context, "baseConcepts.scs");
        loadScsFile(context, "oneParameterTest.scs");

        ScAgentInit(true);
        initialize();

        SC_AGENT_REGISTER(OneParameterTestAgent);
        SC_AGENT_REGISTER(generateResponseModule::GenerateResponseAgent);

        ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");
        ScAddr const & message = context.HelperFindBySystemIdtf("message");
        ScAddr const & answer = context.HelperFindBySystemIdtf("_answer");
        
        utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME);

        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_successfully,
                testActionNode,
                ScType::EdgeAccessConstPosPerm));

        ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, generateResponseModule::Keynodes::nrel_response);

        EXPECT_TRUE(messageAnswer.IsValid());

        ScAddr test_entity = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

        EXPECT_TRUE(context.HelperGetSystemIdtf(test_entity) == "test_entity");

        EXPECT_TRUE(context.HelperCheckEdge(answer, messageAnswer, ScType::EdgeAccessConstPosTemp));
        
        SC_AGENT_UNREGISTER(OneParameterTestAgent);
    }

    TEST_F(AgentTest, ZeroParameterTestAgent)
    {
        ScMemoryContext & context = *m_ctx;
        loadScsFile(context, "baseConcepts.scs");
        loadScsFile(context, "zeroParameterTest.scs");

        ScAgentInit(true);
        initialize();

        SC_AGENT_REGISTER(ZeroParameterTestAgent);
        SC_AGENT_REGISTER(generateResponseModule::GenerateResponseAgent);

        ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");
        ScAddr const & message = context.HelperFindBySystemIdtf("message");
        ScAddr const & answer = context.HelperFindBySystemIdtf("_answer");
        
        utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME);

        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_successfully,
                testActionNode,
                ScType::EdgeAccessConstPosPerm));

        ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, generateResponseModule::Keynodes::nrel_response);

        EXPECT_TRUE(messageAnswer.IsValid());

        ScAddr test_success = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

        EXPECT_TRUE(context.HelperGetSystemIdtf(test_success) == "test_success");

        EXPECT_TRUE(context.HelperCheckEdge(answer, messageAnswer, ScType::EdgeAccessConstPosTemp));

        SC_AGENT_UNREGISTER(OneParameterTestAgent);
    }

    TEST_F(AgentTest, InvalidCallTest)
    {
        ScMemoryContext & context = *m_ctx;
        loadScsFile(context, "baseConcepts.scs");
        loadScsFile(context, "invalidTest.scs");

        ScAgentInit(true);
        initialize();

        SC_AGENT_REGISTER(OneParameterTestAgent);
        SC_AGENT_REGISTER(generateResponseModule::GenerateResponseAgent);

        ScAddr const & testActionNode1 = context.HelperFindBySystemIdtf("test_action_node1");
        ScAddr const & message1 = context.HelperFindBySystemIdtf("message1");
        utils::AgentUtils::applyAction(&context, testActionNode1, WAIT_TIME);
        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully,
                testActionNode1,
                ScType::EdgeAccessConstPosPerm));
        ScAddr messageAnswer1 = utils::IteratorUtils::getAnyByOutRelation(&context, message1, generateResponseModule::Keynodes::nrel_response);
        EXPECT_FALSE(messageAnswer1.IsValid());


        ScAddr const & testActionNode2 = context.HelperFindBySystemIdtf("test_action_node2");
        ScAddr const & message2 = context.HelperFindBySystemIdtf("message2");
        utils::AgentUtils::applyAction(&context, testActionNode2, WAIT_TIME);
        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully,
                testActionNode2,
                ScType::EdgeAccessConstPosPerm));
        ScAddr messageAnswer2 = utils::IteratorUtils::getAnyByOutRelation(&context, message1, generateResponseModule::Keynodes::nrel_response);
        EXPECT_FALSE(messageAnswer2.IsValid());


        ScAddr const & testActionNode3 = context.HelperFindBySystemIdtf("test_action_node3");
        utils::AgentUtils::applyAction(&context, testActionNode3, WAIT_TIME);
        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully,
                testActionNode3,
                ScType::EdgeAccessConstPosPerm));
        SC_AGENT_UNREGISTER(OneParameterTestAgent);
    }
}
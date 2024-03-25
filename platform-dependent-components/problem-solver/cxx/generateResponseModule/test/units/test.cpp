#include "scs_loader.hpp"
#include "sc_test.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "agent/GenerateResponseAgent.hpp"
#include "test/agent/OneParameterTestAgent.hpp"
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
        
        utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME);

        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_successfully,
                testActionNode,
                ScType::EdgeAccessConstPosPerm));

        ScAddr messageAnswer = utils::IteratorUtils::getAnyByOutRelation(&context, message, generateResponseModule::Keynodes::nrel_response);

        EXPECT_TRUE(messageAnswer.IsValid());

        ScAddr test_entity = utils::IteratorUtils::getAnyFromSet(&context, messageAnswer);

        EXPECT_TRUE(context.HelperGetSystemIdtf(test_entity) == "test_entity");
        
        SC_AGENT_UNREGISTER(OneParameterTestAgent);
    }
}
#include "scs_loader.hpp"
#include "sc_wait.hpp"
#include "sc_test.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "keynodes/IdentifiersKeynodes.hpp"
#include "agent/TranslateMainSystemIdtfsFromScToFileAgent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

using namespace identifiersModule;

namespace ModuleTest
{
    ScsLoader loader;
    const std::string TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";

    const int WAIT_TIME = 5900;

    using AgentTest = ScMemoryTest;

    void initialize()
    {
      scAgentsCommon::CoreKeynodes::InitGlobal();
      IdentifiersKeynodes::InitGlobal();
    }

    void shutdown(){SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)}

    bool waitAction(ScMemoryContext * context, ScAddr const & actionAddr, int const & waitTime)
    {
        auto check = [](ScAddr const & listenAddr, ScAddr const & edgeAddr, ScAddr const & otherAddr) {
            return otherAddr == scAgentsCommon::CoreKeynodes::question_finished;
        };
        ScWaitCondition<ScEventAddInputEdge> waiter(*context, actionAddr, SC_WAIT_CHECK(check));
        return waiter.Wait(waitTime);
    }

    std::string getFileContent()
    {
        char fileName[] = IDENTIFIERS_MODULE_PATH "identifiers.txt";
        std::ifstream file(fileName);
        std::string fileContent = "";
        std::string line;
        try {
            if (file.is_open()) {
                while (std::getline(file, line)) {
                    fileContent += line;
                }
                file.close();
                std::remove(fileName);
            } else {
                SC_THROW_EXCEPTION(utils::ScException, "Cant find idtfs file");
            }
        }
        catch (utils::ScException & exception)
        {
            SC_LOG_ERROR(exception.Description());
        }

        return fileContent;
    }

    bool isSubstringOfFile(std::vector<std::string> input, std::string const content)
    {
        for(auto const & el : input) {
            if (content.find(el) == std::string::npos) {
                return false;
            }
        }
        return true;
    }

    bool isNotSubstringOfFile(std::vector<std::string> input, std::string const & content)
    {
        for(auto const & el : input) {
            if (content.find(el) != std::string::npos) {
                return false;
            }
        }
        return true;
    }

    TEST_F(AgentTest, VoidTest)
    {
        ScMemoryContext & context = *m_ctx;

        loader.loadScsFile(context, TEST_FILES_DIR_PATH + "voidTest.scs");
        ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

        ScAgentInit(true);
        initialize();

        SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

        context.CreateEdge(
                ScType::EdgeAccessConstPosPerm,
                scAgentsCommon::CoreKeynodes::question_initiated,
                testActionNode);

        waitAction(&context, testActionNode, WAIT_TIME);

        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_successfully,
                testActionNode,
                ScType::EdgeAccessConstPosPerm));

        bool result = false;

        std::ifstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
        std::string const file_content = getFileContent();

        if (file_content.empty()){
            result = true;
        }

        EXPECT_TRUE(result);

        SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
    }

    TEST_F(AgentTest, CorrectTest)
    {
        ScMemoryContext & context = *m_ctx;

        loader.loadScsFile(context, TEST_FILES_DIR_PATH + "correctTest.scs");
        ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

        ScAgentInit(true);
        initialize();

        SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

        context.CreateEdge(
                ScType::EdgeAccessConstPosPerm,
                scAgentsCommon::CoreKeynodes::question_initiated,
                testActionNode);

        waitAction(&context, testActionNode, WAIT_TIME);

        EXPECT_TRUE(context.HelperCheckEdge(
                scAgentsCommon::CoreKeynodes::question_finished_successfully,
                testActionNode,
                ScType::EdgeAccessConstPosPerm));

        std::string const & space = "{\"space\", \"sc_node_class\"}";
        std::string const & knowledge = "{\"знание\", {\"knowledge\", \"sc_node_class\"} }";
        std::string const & variable = "{\"переменная\", {\"no_name\", \"sc_node\"} }";
        std::string const & query = "{\"очередь\", {\"query\", \"sc_node\"} }";

        std::vector<std::string> const true_input {knowledge, variable, query};
        std::vector<std::string> const false_input {space};

        std::ifstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
        std::string const file_content = getFileContent();

        bool const true_result = isSubstringOfFile(true_input, file_content);
        bool const false_result = isNotSubstringOfFile(false_input, file_content);

        EXPECT_TRUE(false_result && true_result);
        SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
    }

    TEST_F(AgentTest, IncorrectTest)
    {
            ScMemoryContext & context = *m_ctx;

            loader.loadScsFile(context, TEST_FILES_DIR_PATH + "incorrectTest.scs");
            ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

            ScAgentInit(true);
            initialize();

            SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

            context.CreateEdge(
                    ScType::EdgeAccessConstPosPerm,
                    scAgentsCommon::CoreKeynodes::question_initiated,
                    testActionNode);

            waitAction(&context, testActionNode, WAIT_TIME);

            EXPECT_TRUE(context.HelperCheckEdge(
                    scAgentsCommon::CoreKeynodes::question_finished_successfully,
                    testActionNode,
                    ScType::EdgeAccessConstPosPerm));

            std::ifstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
            std::string const file_content = getFileContent();

            std::string const incorrectVariable = "{\"переменная\", {\"no_name\", \"sc_node\"} }";
            std::string const correctVariable = "{\"Переменная\", {\"No_name\", \"sc_node\"} }";
            std::string const knowledge = "{\"знание\", {\"knowledge\", \"sc_node_class\"} }";
            std::string const space = "{\"пространство\", {\"space\", \"sc_node_class\"} }";
            std::string const query = "{\"очередь\", {\"query\", \"sc_node\"} }";

            std::vector<std::string> false_input = {knowledge, space, query, incorrectVariable};
            std::vector<std::string> true_input = {correctVariable};

            bool const true_result = isSubstringOfFile(true_input, file_content);
            bool const false_result = isNotSubstringOfFile(false_input, file_content);

            EXPECT_TRUE(false_result && true_result);

            SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
        }

}  // namespace ModuleTest

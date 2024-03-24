#include "scs_loader.hpp"
#include "sc-memory/sc_wait.hpp"
#include "sc_test.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "keynodes/identifiers_keynodes.hpp"
#include "agent/translate_main_system_idtfs_from_sc_to_file_agent.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

using namespace identifiersModule;
using AgentTest = ScMemoryTest;

namespace moduleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/test-structures/";

int const WAIT_TIME = 1000;

void InitializeKeynodes()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  IdentifiersKeynodes::InitGlobal();
}

std::string GetFileContent()
{
  char fileName[] = IDENTIFIERS_MODULE_PATH "identifiers.txt";
  std::ifstream file(fileName);
  std::stringstream fileContent;
  std::string line;
  try
  {
    if (file.is_open())
    {
      while (std::getline(file, line))
        fileContent << line;
      file.close();
      std::remove(fileName);
    }
    else
    {
      SC_THROW_EXCEPTION(utils::ScException, "Cant find file with identifiers");
    }
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
  }
  return fileContent.str();
}

bool IsSubstringOfFile(std::vector<std::string> const & input, std::string const & content)
{
  return std::all_of(
      std::begin(input),
      std::end(input),
      [&](std::string const & el)
      {
        return content.find(el) != std::string::npos;
      });
}

TEST_F(AgentTest, VoidTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "void_test.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  InitializeKeynodes();

  SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));

  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  std::string const fileContent = GetFileContent();

  EXPECT_TRUE(fileContent.empty());

  SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
}

TEST_F(AgentTest, CorrectTest)
{
  ScMemoryContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "correct_test.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  InitializeKeynodes();
  SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));

  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  std::string const & space = R"({"space", "sc_node_class"})";
  std::string const & knowledge = R"({"знание", {"knowledge", "sc_node_class"}})";
  std::string const & variable = R"({"переменная", {"no_name", "sc_node"}})";
  std::string const & query = R"({"очередь", {"query", "sc_node"}})";

  std::vector<std::string> const trueInput{knowledge, variable, query};
  std::vector<std::string> const falseInput{space};

  std::string const fileContent = GetFileContent();

  bool const trueResult = IsSubstringOfFile(trueInput, fileContent);
  bool const falseResult = IsSubstringOfFile(falseInput, fileContent);

  EXPECT_TRUE(!falseResult && trueResult);
  SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
}

TEST_F(AgentTest, IncorrectTest)
{
  ScMemoryContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "incorrect_test.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  InitializeKeynodes();
  SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));

  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  std::string const fileContent = GetFileContent();

  std::string const & incorrectVariable = R"({"переменная", {"no_name", "sc_node"}})";
  std::string const & correctVariable = R"({"Переменная", {"No_name", "sc_node"}})";
  std::string const & knowledge = R"({"знание", {"knowledge", "sc_node_class"}})";
  std::string const & space = R"({"пространство", {"space", "sc_node_class"}})";
  std::string const & query = R"({"очередь", {"query", "sc_node"}})";

  std::vector<std::string> const falseInput = {knowledge, space, query, incorrectVariable};
  std::vector<std::string> const trueInput = {correctVariable};

  bool const trueResult = IsSubstringOfFile(trueInput, fileContent);
  bool const falseResult = IsSubstringOfFile(falseInput, fileContent);

  EXPECT_TRUE(!falseResult && trueResult);

  SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
}

TEST_F(AgentTest, ManySystemIdtfsTest)
{
  ScMemoryContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "many_system_idtfs_test.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  InitializeKeynodes();
  SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));

  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
}

}  // namespace moduleTest

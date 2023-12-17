//#include "scs_loader.hpp"
//#include "sc_test.hpp"
//#include "scs_loader.hpp"

#include "agent/TranslateMainSystemIdtfsFromScToFileAgent.hpp"
#include "keynodes/IdentifiersKeynodes.hpp"
#include "tests/sc-memory/_test/sc_test.hpp"
#include "../../../../ostis-web-platform/sc-machine/sc-tools/sc-builder/src/scs_loader.hpp"


using namespace identifiersModule;
using IdentifiersTest = ScMemoryTest;

namespace testTranslateMainSystemIdtfsFromScToFileAgent
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = IDENTIFIERS_MODULE_PATH "test/TestsStructures/";

void initializeClasses()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  IdentifiersKeynodes::InitGlobal();
}

TEST_F(IdentifiersTest, TranslateIdentifiersCorrectTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "correctTest.scs");
  ScAddr const & testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAgentInit(true);
  initializeClasses();

  SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent)

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
      testActionNode);

  // Add logic for checking agent output
  EXPECT_TRUE(false);

  SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
}

} // testTranslateMainSystemIdtfsFromScToFileAgent

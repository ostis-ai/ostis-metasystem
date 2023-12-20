//#include "scs_loader.hpp"
//#include "sc_test.hpp"

#include "agent/TranslateMainSystemIdtfsFromScToFileAgent.hpp"
#include "keynodes/IdentifiersKeynodes.hpp"
#include "tests/sc-memory/_test/sc_test.hpp"
#include "../../../../ostis-web-platform/sc-machine/sc-tools/sc-builder/src/scs_loader.hpp"


using namespace identifiersModule;

namespace testTranslateMainSystemIdtfsFromScToFileAgent
{
    ScsLoader loader;
    std::string const TEST_FILES_DIR_PATH = IDENTIFIERS_MODULE_PATH "test/TestsStructures/";
    using IdentifiersTest = ScMemoryTest;

    void initializeClasses()
    {
      scAgentsCommon::CoreKeynodes::InitGlobal();
      IdentifiersKeynodes::InitGlobal();
    };


   TEST_F(IdentifiersTest, correctVariant)
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

      std::ifstream file("../../identifiers.txt");
      std::string const & knowledge = "{\"знание\", {\"knowledge\", \"sc_node_class\"} },\n";
      std::string const & space = "{\"пространство\", {\"space\", \"sc_node_class\"} }";

      bool result = false;
      std::string file_content;

      if (file){
          file >> file_content;
          if (file_content.find(knowledge+space)){
              result = true;
              SC_LOG_DEBUG(knowledge+space);
          }
      }

      EXPECT_TRUE(result);

      SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent)
    };

} // testTranslateMainSystemIdtfsFromScToFileAgent

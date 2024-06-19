/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */


#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "sc-memory/kpm/sc_agent.hpp"

#include "agent/structured_representation_generation_agent.hpp"
#include "keynodes/structured_representation_generation_keynodes.hpp"

using namespace structuredRepresentationGenerationModule;
namespace structuredRepresenationGenerationTest
{
std::string const TEST_FILES_DIR_PATH = STRUCTURED_REPRESENTATION_GENERATION_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using StructuredRepresentationGenerationTest = ScMemoryTest;

TEST_F(StructuredRepresentationGenerationTest, succesfull_generate_structured_representation)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  StructuredRepresentationGenerationKeynodes::InitGlobal();
  SC_AGENT_REGISTER(StructuredRepresentationGenerationAgent)

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "templates.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "structured_representation_test.scs");

  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  utils::AgentUtils::applyAction(&context, testActionNode, 1000);
  ScIterator5Ptr it5 = context.Iterator5(
      testActionNode,
      ScType::EdgeDCommon,
      ScType::Unknown,
      ScType::EdgeAccessConstPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_answer);
  EXPECT_TRUE(it5->Next());

  ScIterator3Ptr it3 = context.Iterator3(it5->Get(2), ScType::EdgeAccessConstPosPerm, ScType::Unknown);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.IsElement(it3->Get(2)));

  SC_AGENT_UNREGISTER(StructuredRepresentationGenerationAgent)
}

}

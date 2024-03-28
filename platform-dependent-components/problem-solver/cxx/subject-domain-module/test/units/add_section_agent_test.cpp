/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/add_section_agent.hpp"
#include "keynodes/subject_domain_keynodes.hpp"
#include "utils/subject_domain_utils.hpp"

#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>

using namespace subjectDomainModule;

namespace subjDomainTest
{
std::string const TEST_FILES_DIR_PATH = SUBJECT_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using AddSectionDomainTest = ScMemoryTest;

TEST_F(AddSectionDomainTest, successful_add_section_to_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SubjectDomainKeynodes::InitGlobal();
  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");
  ScAddr parentSectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  ScAddr decompositionTupleAddr = subject_domain_utils::GetSectionDecompositionTuple(&context, parentSectionAddr);
  size_t decompositionSize = utils::CommonUtils::getSetPower(&context, decompositionTupleAddr);

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
  EXPECT_EQ(utils::CommonUtils::getSetPower(&context, decompositionTupleAddr) - decompositionSize, 1u);

  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(AddSectionDomainTest, successful_add_section_empty_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SubjectDomainKeynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node2");
  ScAddr parentSectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME);
  ScIterator5Ptr it5 = context.Iterator5(
      testActionNode,
      ScType::EdgeDCommon,
      ScType::Unknown,
      ScType::EdgeAccessConstPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_answer);
  EXPECT_TRUE(it5->Next());

  ScIterator3Ptr it3 = context.Iterator3(it5->Get(2), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.IsElement(it3->Get(2)));
  ScAddr decompositionTupleAddr = subject_domain_utils::GetSectionDecompositionTuple(&context, parentSectionAddr);
  EXPECT_EQ(utils::CommonUtils::getSetPower(&context, decompositionTupleAddr), 1u);

  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(AddSectionDomainTest, add_section_invalid_parameters_1)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SubjectDomainKeynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node3");

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(AddSectionDomainTest, add_section_invalid_parameters_3)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SubjectDomainKeynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node4");

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(AddSectionDomainTest, add_section_without_parent)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SubjectDomainKeynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node5");
  ScAddr sectionNameAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));
  ScIterator5Ptr it5 = context.Iterator5(
      testActionNode,
      ScType::EdgeDCommon,
      ScType::Unknown,
      ScType::EdgeAccessConstPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_answer);
  EXPECT_TRUE(it5->Next());

  ScIterator3Ptr it3 = context.Iterator3(it5->Get(2), ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.IsElement(it3->Get(2)));
  std::string sectionName;
  context.GetLinkContent(sectionNameAddr, sectionName);
  ScAddr sectionAddr = subject_domain_utils::FindSectionByName(&context, sectionName);
  EXPECT_TRUE(context.IsElement(sectionAddr));
  EXPECT_EQ(it3->Get(2), sectionAddr);

  SC_AGENT_UNREGISTER(AddSectionAgent)
}
}  // namespace subjDomainTest

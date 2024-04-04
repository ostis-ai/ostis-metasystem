/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/remove_section_agent.hpp"
#include "constants/sections_aliases.hpp"
#include "utils/sections_utils.hpp"

#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "sc-memory/kpm/sc_agent.hpp"

#include <vector>

using namespace sectionsModule;

namespace subjDomainTest
{

const std::string TEST_FILES_DIR_PATH = SUBJECT_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using RemoveSectionTest = ScMemoryTest;

TEST_F(RemoveSectionTest, successful_remove_section_from_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SectionsKeynodes::InitGlobal();
  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");
  ScAddr sectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr parentSectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  ScAddr decompositionTupleAddr = sections_utils::GetSectionDecompositionTuple(&context, parentSectionAddr);
  size_t decompositionSize = utils::CommonUtils::getSetPower(&context, decompositionTupleAddr);

  context.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);
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

  EXPECT_TRUE(context.HelperCheckEdge(SectionsKeynodes::removed_section, sectionAddr, ScType::EdgeAccessConstPosPerm));
  EXPECT_EQ(decompositionSize - utils::CommonUtils::getSetPower(&context, decompositionTupleAddr), 1u);

  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}

TEST_F(RemoveSectionTest, successful_remove_section_from_decomposition_2)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SectionsKeynodes::InitGlobal();

  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node2");
  ScAddr sectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr parentSectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  context.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);
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
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      ScType::NodeVarTuple >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeDCommonVar,
      parentSectionAddr,
      ScType::EdgeAccessVarPosPerm,
      SectionsKeynodes::nrel_entity_decomposition);
  ScTemplateSearchResult searchResult;
  context.HelperSearchTemplate(scTemplate, searchResult);
  EXPECT_TRUE(searchResult.IsEmpty());
  EXPECT_TRUE(context.HelperCheckEdge(SectionsKeynodes::removed_section, sectionAddr, ScType::EdgeAccessConstPosPerm));
  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}

TEST_F(RemoveSectionTest, remove_section_invalid_parameters_1)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SectionsKeynodes::InitGlobal();

  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node3");

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_unsuccessfully, testActionNode, ScType::EdgeAccessConstPosPerm));

  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}

TEST_F(RemoveSectionTest, remove_section_without_parent)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  SectionsKeynodes::InitGlobal();

  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node4");
  ScAddr sectionAddr =
      utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(SectionsKeynodes::removed_section, sectionAddr, ScType::EdgeAccessConstPosPerm));
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      ScType::NodeVarTuple >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeDCommonVar,
      ScType::NodeVar,
      ScType::EdgeAccessVarPosPerm,
      SectionsKeynodes::nrel_entity_decomposition);
  scTemplate.Triple(sections_aliases::DECOMPOSITION_TUPLE, ScType::EdgeAccessVarPosPerm, sectionAddr);
  ScTemplateSearchResult searchResult;
  context.HelperSearchTemplate(scTemplate, searchResult);
  EXPECT_TRUE(searchResult.IsEmpty());

  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}
}  // namespace subjDomainTest

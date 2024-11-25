/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/remove_section_agent.hpp"
#include "constants/sections_aliases.hpp"
#include "utils/sections_utils.hpp"

#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include <sc-memory/sc_agent.hpp>
#include <vector>

using namespace sectionsModule;

namespace subjDomainTest
{

const std::string TEST_FILES_DIR_PATH = SUBJECT_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using RemoveSectionTest = ScMemoryTest;

TEST_F(RemoveSectionTest, successful_remove_section_from_decomposition)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  ScAddr sectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_1);
  ScAddr parentSectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_2);
  ScAddr decompositionTupleAddr = sections_utils::GetSectionDecompositionTuple(&context, parentSectionAddr);
  size_t decompositionSize = utils::CommonUtils::getSetPower(&context, decompositionTupleAddr);

  context.SubscribeAgent<sectionsModule::RemoveSectionAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  ScStructure result = testAction.GetResult();

  ScIterator3Ptr it3 = context.CreateIterator3(result, ScType::ConstPermPosArc, ScType::Unknown);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.CheckConnector(SectionsKeynodes::removed_section, sectionAddr, ScType::ConstPermPosArc));
  EXPECT_EQ(decompositionSize - utils::CommonUtils::getSetPower(&context, decompositionTupleAddr), 1u);

  context.UnsubscribeAgent<sectionsModule::RemoveSectionAgent>();
}

TEST_F(RemoveSectionTest, successful_remove_section_from_decomposition_2)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node2");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::RemoveSectionAgent>();

  ScAddr sectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_1);
  ScAddr parentSectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_2);

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));

  ScStructure result = testAction.GetResult();

  ScIterator3Ptr it3 = context.CreateIterator3(result, ScType::ConstPermPosArc, ScType::ConstNode);
  EXPECT_TRUE(it3->Next());
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      ScType::VarNodeTuple >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarCommonArc,
      parentSectionAddr,
      ScType::VarPermPosArc,
      SectionsKeynodes::nrel_entity_decomposition);
  ScTemplateSearchResult searchResult;
  context.SearchByTemplate(scTemplate, searchResult);
  EXPECT_TRUE(searchResult.IsEmpty());
  EXPECT_TRUE(context.CheckConnector(SectionsKeynodes::removed_section, sectionAddr, ScType::ConstPermPosArc));

  context.UnsubscribeAgent<sectionsModule::RemoveSectionAgent>();
}

TEST_F(RemoveSectionTest, remove_section_invalid_parameters_1)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node3");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::RemoveSectionAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());

  context.UnsubscribeAgent<sectionsModule::RemoveSectionAgent>();
}

TEST_F(RemoveSectionTest, remove_section_without_parent)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");

  ScAddr testActionNode = context.SearchElementBySystemIdentifier("test_action_node4");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::RemoveSectionAgent>();

  ScAddr sectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_1);

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));

  EXPECT_TRUE(context.CheckConnector(SectionsKeynodes::removed_section, sectionAddr, ScType::ConstPermPosArc));
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      ScType::VarNodeTuple >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarCommonArc,
      ScType::VarNode,
      ScType::VarPermPosArc,
      SectionsKeynodes::nrel_entity_decomposition);
  scTemplate.Triple(sections_aliases::DECOMPOSITION_TUPLE, ScType::VarPermPosArc, sectionAddr);
  ScTemplateSearchResult searchResult;
  context.SearchByTemplate(scTemplate, searchResult);
  EXPECT_TRUE(searchResult.IsEmpty());

  context.UnsubscribeAgent<sectionsModule::RemoveSectionAgent>();
}
}  // namespace subjDomainTest

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/add_section_agent.hpp"
#include "keynodes/sections_keynodes.hpp"
#include "utils/sections_utils.hpp"

#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>

using namespace sectionsModule;

namespace subjDomainTest
{
std::string const TEST_FILES_DIR_PATH = SUBJECT_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using AddSectionTest = ScMemoryTest;

TEST_F(AddSectionTest, successful_add_section_to_decomposition)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");

  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  ScAddr parentSectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_2);
  ScAddr decompositionTupleAddr = sections_utils::GetSectionDecompositionTuple(&context, parentSectionAddr);
  size_t decompositionSize = utils::CommonUtils::getSetPower(&context, decompositionTupleAddr);

  context.SubscribeAgent<sectionsModule::AddSectionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  ScStructure result = testAction.GetResult();

  ScIterator3Ptr it3 = context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::Unknown);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.IsElement(it3->Get(2)));
  EXPECT_EQ(utils::CommonUtils::getSetPower(&context, decompositionTupleAddr) - decompositionSize, 1u);

  context.UnsubscribeAgent<sectionsModule::AddSectionAgent>();
}

TEST_F(AddSectionTest, successful_add_section_empty_decomposition)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");

  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node2");
  ScAddr parentSectionAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_2);

  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::AddSectionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);

  ScStructure result = testAction.GetResult();

  ScIterator3Ptr it3 = context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.IsElement(it3->Get(2)));
  ScAddr decompositionTupleAddr = sections_utils::GetSectionDecompositionTuple(&context, parentSectionAddr);
  EXPECT_EQ(utils::CommonUtils::getSetPower(&context, decompositionTupleAddr), 1u);

  context.UnsubscribeAgent<sectionsModule::AddSectionAgent>();
}

TEST_F(AddSectionTest, add_section_invalid_parameters_1)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node3");

  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::AddSectionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  context.UnsubscribeAgent<sectionsModule::AddSectionAgent>();
}

TEST_F(AddSectionTest, add_section_invalid_parameters_3)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node4");

  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::AddSectionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  EXPECT_TRUE(testAction.IsFinishedUnsuccessfully());
  context.UnsubscribeAgent<sectionsModule::AddSectionAgent>();
}

TEST_F(AddSectionTest, add_section_without_parent)
{
  ScAgentContext & context = *m_ctx;

  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node5");
  ScAddr sectionNameAddr = utils::IteratorUtils::getAnyByOutRelation(&context, testActionNode, ScKeynodes::rrel_1);
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::AddSectionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  ScStructure result = testAction.GetResult();

  ScIterator3Ptr it3 = context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);
  EXPECT_TRUE(it3->Next());

  EXPECT_TRUE(context.IsElement(it3->Get(2)));
  std::string sectionName;
  context.GetLinkContent(sectionNameAddr, sectionName);
  ScAddr sectionAddr = sections_utils::FindSectionByName(&context, sectionName);
  EXPECT_TRUE(context.IsElement(sectionAddr));
  EXPECT_EQ(it3->Get(2), sectionAddr);

  context.UnsubscribeAgent<sectionsModule::AddSectionAgent>();
}
}  // namespace subjDomainTest

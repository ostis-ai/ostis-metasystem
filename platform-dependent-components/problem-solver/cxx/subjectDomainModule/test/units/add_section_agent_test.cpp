/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#include "agent/AddSectionAgent.hpp"
#include "keynodes/Keynodes.hpp"
#include "utils/SubjDomainUtils.hpp"

#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>

using namespace subjDomainModule;

namespace subjDomainTest
{
const std::string TEST_FILES_DIR_PATH = SUBJ_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

const int WAIT_TIME = 1000;

using SubjDomainTest = ScMemoryTest;

TEST_F(SubjDomainTest, successful_add_section_to_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();
  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");
  ScAddr parentSectionAddr = utils::IteratorUtils::getAnyByOutRelation(
        &context,
        testActionNode,
        scAgentsCommon::CoreKeynodes::rrel_2
  );
  ScAddr decompositionTupleAddr = SubjDomainUtils::getSectionDecompositionTuple(&context, parentSectionAddr);
  int decompositionSize = (int) utils::CommonUtils::getSetPower(&context, decompositionTupleAddr);

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  utils::AgentUtils::waitAgentResult(&context, testActionNode, 1000);
  ScIterator5Ptr it5 = context.Iterator5(
        testActionNode,
        ScType::EdgeDCommon,
        ScType::Unknown,
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_answer
        );
  if (it5->Next())
  {
    ScIterator3Ptr it3 = context.Iterator3(
          it5->Get(2),
          ScType::EdgeAccessConstPosPerm,
          ScType::Unknown
          );
    if (it3->Next())
    {
      EXPECT_TRUE(context.IsElement(it3->Get(2)));
      EXPECT_EQ((int) utils::CommonUtils::getSetPower(&context, decompositionTupleAddr) - decompositionSize, 1);
    }
  }
  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(SubjDomainTest, successful_add_section_empty_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node2");
  ScAddr parentSectionAddr = utils::IteratorUtils::getAnyByOutRelation(
        &context,
        testActionNode,
        scAgentsCommon::CoreKeynodes::rrel_2
  );

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  utils::AgentUtils::waitAgentResult(&context, testActionNode, WAIT_TIME);
  ScIterator5Ptr it5 = context.Iterator5(
        testActionNode,
        ScType::EdgeDCommon,
        ScType::Unknown,
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_answer
  );
  if (it5->Next())
  {
    ScIterator3Ptr it3 = context.Iterator3(
          it5->Get(2),
          ScType::EdgeAccessConstPosPerm,
          ScType::NodeConst
    );
    if (it3->Next())
    {
      EXPECT_TRUE(context.IsElement(it3->Get(2)));
      ScAddr decompositionTupleAddr = SubjDomainUtils::getSectionDecompositionTuple(&context, parentSectionAddr);
      EXPECT_EQ((int) utils::CommonUtils::getSetPower(&context, decompositionTupleAddr), 1);
    }
  }
  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(SubjDomainTest, add_section_invalid_parameters_1)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node3");

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  EXPECT_FALSE(utils::AgentUtils::waitAgentResult(&context, testActionNode, WAIT_TIME));
  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(SubjDomainTest, add_section_invalid_parameters_3)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node4");

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  EXPECT_FALSE(utils::AgentUtils::waitAgentResult(&context, testActionNode, WAIT_TIME));
  SC_AGENT_UNREGISTER(AddSectionAgent)
}

TEST_F(SubjDomainTest, add_section_without_parent)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(AddSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_add_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node5");
  ScAddr sectionNameAddr = utils::IteratorUtils::getAnyByOutRelation(
        &context,
        testActionNode,
        scAgentsCommon::CoreKeynodes::rrel_1
  );

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  EXPECT_TRUE(utils::AgentUtils::waitAgentResult(&context, testActionNode, WAIT_TIME));
  ScIterator5Ptr it5 = context.Iterator5(
        testActionNode,
        ScType::EdgeDCommon,
        ScType::Unknown,
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_answer
  );
  if (it5->Next())
  {
    ScIterator3Ptr it3 = context.Iterator3(
          it5->Get(2),
          ScType::EdgeAccessConstPosPerm,
          ScType::NodeConst
    );
    if (it3->Next())
    {
      EXPECT_TRUE(context.IsElement(it3->Get(2)));
      std::string sectionName;
      context.GetLinkContent(sectionNameAddr, sectionName);
      ScAddr sectionAddr = SubjDomainUtils::findSectionByName(&context, sectionName);
      EXPECT_TRUE(context.IsElement(sectionAddr));
      EXPECT_EQ(it3->Get(2), sectionAddr);
    }
  }
  SC_AGENT_UNREGISTER(AddSectionAgent)
}
}

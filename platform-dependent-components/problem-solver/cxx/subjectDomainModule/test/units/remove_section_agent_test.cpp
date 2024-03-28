/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#include "agent/RemoveSectionAgent.hpp"
#include "constants/SubjDomainAliases.hpp"
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

TEST_F(SubjDomainTest, successful_remove_section_from_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();
  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");
  ScAddr sectionAddr = utils::IteratorUtils::getAnyByOutRelation(
        &context,
        testActionNode,
        scAgentsCommon::CoreKeynodes::rrel_1
  );
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
  EXPECT_TRUE(it5->Next());
  ScIterator3Ptr it3 = context.Iterator3(
        it5->Get(2),
        ScType::EdgeAccessConstPosPerm,
        ScType::Unknown
  );
  if (it3->Next())
  {
    EXPECT_TRUE(context.HelperCheckEdge(Keynodes::removed_section, sectionAddr, ScType::EdgeAccessConstPosPerm));
    EXPECT_EQ(decompositionSize - (int) utils::CommonUtils::getSetPower(&context, decompositionTupleAddr), 1);
  }
  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}

TEST_F(SubjDomainTest, successful_remove_section_from_decomposition_2)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node2");
  ScAddr sectionAddr = utils::IteratorUtils::getAnyByOutRelation(
        &context,
        testActionNode,
        scAgentsCommon::CoreKeynodes::rrel_1
  );
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
  EXPECT_TRUE(it5->Next());
  ScIterator3Ptr it3 = context.Iterator3(
        it5->Get(2),
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConst
  );
  EXPECT_TRUE(it3->Next());
  ScTemplate scTemplate;
  scTemplate.Quintuple(
        ScType::NodeVarTuple >> SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeDCommonVar,
        parentSectionAddr,
        ScType::EdgeAccessVarPosPerm,
        Keynodes::nrel_entity_decomposition
  );
  ScTemplateSearchResult searchResult;
  context.HelperSearchTemplate(scTemplate, searchResult);
  EXPECT_TRUE(searchResult.IsEmpty());
  EXPECT_TRUE(context.HelperCheckEdge(Keynodes::removed_section, sectionAddr, ScType::EdgeAccessConstPosPerm));
  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}

TEST_F(SubjDomainTest, remove_section_invalid_parameters_1)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node3");

  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  EXPECT_FALSE(utils::AgentUtils::waitAgentResult(&context, testActionNode, WAIT_TIME));
  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}

TEST_F(SubjDomainTest, remove_section_without_parent)
{
  ScMemoryContext & context = *m_ctx;
  scAgentsCommon::CoreKeynodes::InitGlobal();
  Keynodes::InitGlobal();

  SC_AGENT_REGISTER(RemoveSectionAgent)
  ScsLoader loader;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_remove_section.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node4");
  ScAddr sectionAddr = utils::IteratorUtils::getAnyByOutRelation(
        &context,
        testActionNode,
        scAgentsCommon::CoreKeynodes::rrel_1
  );
  context.CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        scAgentsCommon::CoreKeynodes::question_initiated,
        testActionNode);
  EXPECT_TRUE(utils::AgentUtils::waitAgentResult(&context, testActionNode, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(Keynodes::removed_section, sectionAddr, ScType::EdgeAccessConstPosPerm));
  ScTemplate scTemplate;
  scTemplate.Quintuple(
        ScType::NodeVarTuple >> SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeDCommonVar,
        ScType::NodeVar,
        ScType::EdgeAccessVarPosPerm,
        Keynodes::nrel_entity_decomposition
  );
  scTemplate.Triple(
        SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeAccessVarPosPerm,
        sectionAddr
  );
  ScTemplateSearchResult searchResult;
  context.HelperSearchTemplate(scTemplate, searchResult);
  EXPECT_TRUE(searchResult.IsEmpty());

  SC_AGENT_UNREGISTER(RemoveSectionAgent)
}
}

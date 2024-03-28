/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/get_decomposition_agent.hpp"
#include "keynodes/subject_domain_keynodes.hpp"

#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc-memory/utils/sc_base64.hpp"

#include <nlohmann/json.hpp>

#include <vector>

using namespace subjectDomainModule;
using json = nlohmann::json;

namespace subjDomainTest
{

json GetDecompositionIdList(json const &, int level = 1);

json GetTestJSON(int level = 1);

ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = SUBJECT_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using AddSectionDomainTest = ScMemoryTest;

void initialize()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  subject_domain_keynodes::InitGlobal();

  SC_AGENT_REGISTER(GetDecompositionAgent)
}

void shutdown(){SC_AGENT_UNREGISTER(GetDecompositionAgent)}

TEST_F(AddSectionDomainTest, successful_decomposition)
{
  ScMemoryContext & context = *m_ctx;
  initialize();
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_successful_decomposition.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME);
  ScIterator5Ptr it5 = context.Iterator5(
      testActionNode,
      ScType::EdgeDCommon,
      ScType::Unknown,
      ScType::EdgeAccessConstPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_answer);
  if (it5->Next())
  {
    ScIterator3Ptr it3 = context.Iterator3(it5->Get(2), ScType::EdgeAccessConstPosPerm, ScType::Link);
    if (it3->Next())
    {
      std::string decompositionContent;
      context.GetLinkContent(it3->Get(2), decompositionContent);
      json decomposition = json::parse(decompositionContent);
      json temp;
      std::string idtf = decomposition.begin().value()["idtf"];
      temp[idtf] = GetDecompositionIdList(decomposition.begin().value()["decomposition"]);
      json testJson = GetTestJSON();
      EXPECT_EQ(temp, GetTestJSON());
    }
  }

  shutdown();
}

TEST_F(AddSectionDomainTest, successful_decomposition_with_level)
{
  ScMemoryContext & context = *m_ctx;
  initialize();
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_successful_decomposition.scs");
  ScAddr testActionNode = context.HelperFindBySystemIdtf("test_action_node2");

  context.CreateEdge(ScType::EdgeAccessConstPosPerm, scAgentsCommon::CoreKeynodes::question_initiated, testActionNode);
  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, testActionNode, WAIT_TIME));
  ScIterator5Ptr it5 = context.Iterator5(
      testActionNode,
      ScType::EdgeDCommon,
      ScType::Unknown,
      ScType::EdgeAccessConstPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_answer);
  if (it5->Next())
  {
    ScIterator3Ptr it3 = context.Iterator3(it5->Get(2), ScType::EdgeAccessConstPosPerm, ScType::Link);
    if (it3->Next())
    {
      std::string decompositionText;
      context.GetLinkContent(it3->Get(2), decompositionText);
      json decomposition = json::parse(decompositionText);
      json temp;
      std::string idtf = decomposition.begin().value()["idtf"];
      temp[idtf] = GetDecompositionIdList(decomposition.begin().value()["decomposition"], 2);
      json testJson = GetTestJSON(2);
      EXPECT_EQ(temp, GetTestJSON(2));
    }
  }

  shutdown();
}

json GetDecompositionIdList(json const & answerDecomposition, int level)
{
  json decomposition;
  for (auto element : answerDecomposition)
  {
    std::string idtf = element["idtf"];
    if (level != 1)
      decomposition[idtf] = GetDecompositionIdList(element["decomposition"], level - 1);
    else
      decomposition[idtf] = {};
  }
  return decomposition;
}

json GetTestJSON(int level)
{
  json result;
  result["База знаний IMS"] = {
      {"context_technology_OSTIS_and_Metasystem_IMS_in_the_Global_knowledge_base", {}},
      {"section_OSTIS_technology", {}},
      {"section_project_OSTIS_perspectives_current_state_history_of_evolution_and_use", {}}};
  if (level == 2)
  {
    for (auto it = result["База знаний IMS"].begin(); it != result["База знаний IMS"].end(); it++)
    {
      result["База знаний IMS"][it.key()] = {
          {"doc_IMS", {}},
          {"history_and_current_processes_of_use_IMS", {}},
          {"section_project_IMS_history_and_development_programme", {}}};
    }
  }
  return result;
}
}  // namespace subjDomainTest

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/get_decomposition_agent.hpp"
#include "keynodes/sections_keynodes.hpp"

#include "sc-builder/src/scs_loader.hpp"

#include "sc_test.hpp"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include <nlohmann/json.hpp>

#include <vector>

using namespace sectionsModule;
using json = nlohmann::json;

namespace subjDomainTest
{

json GetDecompositionIdList(json const &, int level = 1);

json GetTestJSON(int level = 1);

ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = SUBJECT_DOMAIN_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using GetSectionDecompositionTest = ScMemoryTest;

TEST_F(GetSectionDecompositionTest, successful_decomposition)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_successful_decomposition.scs");
  ScAddr const testActionNode = context.HelperFindBySystemIdtf("test_action_node");

  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::GetDecompositionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  ScStructure result = testAction.GetResult();


  ScIterator3Ptr const it3 = context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::Link);
  EXPECT_TRUE(it3->Next());
  std::string decompositionContent;
  context.GetLinkContent(it3->Get(2), decompositionContent);
  json decomposition = json::parse(decompositionContent);
  json temp;
  std::string idtf = decomposition.begin().value()["idtf"];
  temp[idtf] = GetDecompositionIdList(decomposition.begin().value()["decomposition"]);
  json testJson = GetTestJSON();
  EXPECT_EQ(temp, GetTestJSON());

  context.UnsubscribeAgent<sectionsModule::GetDecompositionAgent>();
}

TEST_F(GetSectionDecompositionTest, successful_decomposition_with_level)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_successful_decomposition.scs");
  ScAddr const testActionNode = context.HelperFindBySystemIdtf("test_action_node2");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<sectionsModule::GetDecompositionAgent>();

  testAction.InitiateAndWait(WAIT_TIME);
  ScStructure result = testAction.GetResult();

  ScIterator3Ptr it3 = context.Iterator3(result, ScType::EdgeAccessConstPosPerm, ScType::Link);

  EXPECT_TRUE(it3->Next());
  std::string decompositionText;
  context.GetLinkContent(it3->Get(2), decompositionText);
  json decomposition = json::parse(decompositionText);
  json temp;
  std::string idtf = decomposition.begin().value()["idtf"];
  temp[idtf] = GetDecompositionIdList(decomposition.begin().value()["decomposition"], 2);
  json testJson = GetTestJSON(2);
  EXPECT_EQ(temp, GetTestJSON(2));

  context.UnsubscribeAgent<sectionsModule::GetDecompositionAgent>();
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
  result["База знаний IMS"] = {{"section_2", {}}, {"section_1", {}}, {"section_3", {}}};
  if (level == 2)
  {
    for (auto it = result["База знаний IMS"].begin(); it != result["База знаний IMS"].end(); it++)
    {
      result["База знаний IMS"][it.key()] = {{"section_1_1", {}}, {"section_1_2", {}}, {"section_1_3", {}}};
    }
  }
  return result;
}
}  // namespace subjDomainTest

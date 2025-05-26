/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-builder/scs_loader.hpp>

#include <sc-memory/test/sc_test.hpp>
#include "sc-memory/sc_addr.hpp"

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/search_answer_keynodes.hpp"
#include "agent/search-properties-agent.hpp"

using namespace searchAnswerModule;
using AgentTest = ScMemoryTest;

namespace ModuleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

TEST_F(AgentTest, RelationWithPropertiesTest)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "relation_with_properties.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<searchAnswerModule::SearchPropertiesAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAddr const & relationAddr = context.SearchElementBySystemIdentifier("relation");

  ScAddr answerStruct = utils::IteratorUtils::getAnyByOutRelation(& context, testAction, ScKeynodes::nrel_result);
  
  ScTemplate propertiesTemplate;
    propertiesTemplate.Triple(
        answerStruct,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "set_node");
    propertiesTemplate.Quintuple(
        relationAddr,
        ScType::EdgeDCommonVar,
        "set_node",
        ScType::EdgeAccessVarPosPerm,
        searchAnswerModule::SearchAnswerKeynodes::nrel_properties);
    ScTemplateSearchResult result;
    context.HelperSearchTemplate(propertiesTemplate, result);

    EXPECT_TRUE(!result.IsEmpty());

    ScAddr setNode = result["set_node"];

    ScAddr propertyLink;
    std::string property;
    std::vector<std::string> propertiesVector;

    ScIterator3Ptr const & propertiesIterator = context.CreateIterator3(setNode, ScType::ConstPermPosArc, ScType::NodeLink);
    while (propertiesIterator->Next())
    {
        propertyLink = propertiesIterator->Get(2);
        context.GetLinkContent(propertyLink, property);
        propertiesVector.push_back(property);
    }

    EXPECT_TRUE(propertiesVector.size() == 5);

    EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "бинарное") != propertiesVector.end());
    EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "ориентированное") != propertiesVector.end());
    EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "антирефлексивное") != propertiesVector.end());
    EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "асимметричное") != propertiesVector.end());
    EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "антитранзитивное") != propertiesVector.end());

  context.UnsubscribeAgent<searchAnswerModule::SearchPropertiesAgent>();
}

TEST_F(AgentTest, NoPropertiesEnTest)
{
    ScAgentContext & context = *m_ctx;
    loader.loadScsFile(context, TEST_FILES_DIR_PATH + "no_properties_en.scs");
    ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
    ScAction testAction = context.ConvertToAction(testActionNode);
  
    context.SubscribeAgent<searchAnswerModule::SearchPropertiesAgent>();
  
    EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
    EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  
    ScAddr const & relationAddr = context.SearchElementBySystemIdentifier("relation");
  
    ScAddr answerStruct = utils::IteratorUtils::getAnyByOutRelation(& context, testAction, ScKeynodes::nrel_result);
    
    ScTemplate propertiesTemplate;
      propertiesTemplate.Triple(
          answerStruct,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "set_node");
      propertiesTemplate.Quintuple(
          relationAddr,
          ScType::EdgeDCommonVar,
          "set_node",
          ScType::EdgeAccessVarPosPerm,
          searchAnswerModule::SearchAnswerKeynodes::nrel_properties);
      ScTemplateSearchResult result;
      context.HelperSearchTemplate(propertiesTemplate, result);
    
      EXPECT_TRUE(!result.IsEmpty());
  
      ScAddr setNode = result["set_node"];
  
      ScAddr propertyLink;
      std::string property;
      std::vector<std::string> propertiesVector;
  
      ScIterator3Ptr const & propertiesIterator = context.CreateIterator3(setNode, ScType::ConstPermPosArc, ScType::NodeLink);
      while (propertiesIterator->Next())
      {
        propertyLink = propertiesIterator->Get(2);
        context.GetLinkContent(propertyLink, property);
        propertiesVector.push_back(property);
      }

      EXPECT_TRUE(propertiesVector.size() == 1);
  
      EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "none") != propertiesVector.end());
  
    context.UnsubscribeAgent<searchAnswerModule::SearchPropertiesAgent>();
}

TEST_F(AgentTest, NoPropertiesRuTest)
{
    ScAgentContext & context = *m_ctx;
    loader.loadScsFile(context, TEST_FILES_DIR_PATH + "no_properties_ru.scs");
    ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
    ScAction testAction = context.ConvertToAction(testActionNode);
  
    context.SubscribeAgent<searchAnswerModule::SearchPropertiesAgent>();
  
    EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
    EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  
    ScAddr const & relationAddr = context.SearchElementBySystemIdentifier("relation");
  
    ScAddr answerStruct = utils::IteratorUtils::getAnyByOutRelation(& context, testAction, ScKeynodes::nrel_result);
    
    ScTemplate propertiesTemplate;
      propertiesTemplate.Triple(
          answerStruct,
          ScType::EdgeAccessVarPosPerm,
          ScType::NodeVar >> "set_node");
      propertiesTemplate.Quintuple(
          relationAddr,
          ScType::EdgeDCommonVar,
          "set_node",
          ScType::EdgeAccessVarPosPerm,
          searchAnswerModule::SearchAnswerKeynodes::nrel_properties);
      ScTemplateSearchResult result;
      context.HelperSearchTemplate(propertiesTemplate, result);
    
      EXPECT_TRUE(!result.IsEmpty());
  
      ScAddr setNode = result["set_node"];
  
      ScAddr propertyLink;
      std::string property;
      std::vector<std::string> propertiesVector;
  
      ScIterator3Ptr const & propertiesIterator = context.CreateIterator3(setNode, ScType::ConstPermPosArc, ScType::NodeLink);
      while (propertiesIterator->Next())
      {
        propertyLink = propertiesIterator->Get(2);
        context.GetLinkContent(propertyLink, property);
        propertiesVector.push_back(property);
      }

      EXPECT_TRUE(propertiesVector.size() == 1);
  
      EXPECT_TRUE(std::find(propertiesVector.begin(), propertiesVector.end(), "нет") != propertiesVector.end());
  
    context.UnsubscribeAgent<searchAnswerModule::SearchPropertiesAgent>();
}

}  // namespace SearchAnswerModuleTest

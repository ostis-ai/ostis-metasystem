/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-builder/scs_loader.hpp>

#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/message_reply_keynodes.hpp"
#include "agent/message-reply-agent.hpp"

#include "test/agent/GenerateReplyMessageAgent.hpp"
 
using namespace messageReplyModule;
using AgentTest = ScMemoryTest;

namespace ModuleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";
const int WAIT_TIME = 5000;

using MessageReplyAgentTest = ScMemoryTest;

bool generatedMessageIsValid(ScMemoryContext * context, ScAddr const & textLinkAddr)
{
  ScTemplate scTemplate;
  scTemplate.Triple(
      messageReplyModule::MessageReplyKeynodes::concept_message,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVar >> "_user_message");
  scTemplate.Quintuple(
      ScType::NodeVar >> "_translation_node",
      ScType::EdgeDCommonVar,
      "_user_message",
      ScType::EdgeAccessVarPosPerm,
      messageReplyModule::MessageReplyKeynodes::nrel_sc_text_translation);
  scTemplate.Triple(
      "_translation_node",
      ScType::EdgeAccessVarPosPerm,
      textLinkAddr);
  ScTemplateSearchResult searchResult;
  context->HelperSearchTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}

TEST_F(AgentTest, messageProcessingWithTextLinkSuccessful)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context,TEST_FILES_DIR_PATH + "replyMessageAgentTextLinkTestStructure.scs");
  //initialize();
  //SC_AGENT_REGISTER(GenerateReplyMessageAgent)
  

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction test_action = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();
  
  EXPECT_TRUE(test_action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(test_action.IsFinishedSuccessfully());

  EXPECT_TRUE(generatedMessageIsValid(&context, utils::IteratorUtils::getAnyByOutRelation(
          &context,
          test_action_node,
          ScKeynodes::rrel_1)));

  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.unsubscribeAgent<messageReplyModule::GenerateReplyMessageAgent>();
}


TEST_F(MessageReplyAgentTest, argumentIsNotALink)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context,TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureFirstArgumentIsNotALink.scs");
  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<messageReplyModule::GenerateReplyMessageAgent>();

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  context.CreateEdge(
      ScType::EdgeAccessConstPosPerm,
      ScKeynodes::action_initiated,
      test_action_node);

  ScAction test_action = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(test_action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(test_action.IsFinishedUnsuccessfully());

  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.unsubscribeAgent<GenerateReplyMessageAgent>();
}

TEST_F(MessageReplyAgentTest, linkSpecifiedIncorrectly)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context,TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureWithIncorrectlySpecifiedLink.scs");
  
  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<messageReplyModule::GenerateReplyMessageAgent>();

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  context.CreateEdge(
      ScType::EdgeAccessConstPosPerm,
      ScKeynodes::action_initiated,
      test_action_node);

  ScAction test_action = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(test_action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(test_action.IsFinishedUnsuccessfully());

  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.unsubscribeAgent<messageReplyModule::GenerateReplyMessageAgent>();
}

}//namespace messageReplyModuleTest

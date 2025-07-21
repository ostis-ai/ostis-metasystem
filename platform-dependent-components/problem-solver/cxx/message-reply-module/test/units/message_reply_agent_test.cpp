/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-builder/scs_loader.hpp>

#include <sc-memory/test/sc_test.hpp>
#include "sc-memory/utils/sc_logger.hpp"

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

// using MessageReplyAgentTest = ScMemoryTest;

bool generatedMessageIsValid(ScMemoryContext * context, ScAddr const & textLinkAddr)
{
  ScTemplate scTemplate;
  scTemplate.Triple(
      messageReplyModule::MessageReplyKeynodes::concept_message,
      ScType::VarPermPosArc,
      ScType::VarNode >> "_user_message");
  scTemplate.Quintuple(
      ScType::VarNode >> "_translation_node",
      ScType::VarCommonArc,
      "_user_message",
      ScType::VarPermPosArc,
      messageReplyModule::MessageReplyKeynodes::nrel_sc_text_translation);
  scTemplate.Triple(
      "_translation_node",
      ScType::VarPermPosArc,
      textLinkAddr);
  ScTemplateSearchResult searchResult;
  context->SearchByTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}

TEST_F(AgentTest, messageProcessingWithTextLinkSuccessful)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "replyMessageAgentTextLinkTestStructure.scs");

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction test_action = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();
  
  EXPECT_TRUE(test_action.InitiateAndWait(WAIT_TIME));
  SC_LOG_DEBUG("5.5");
  EXPECT_TRUE(test_action.IsFinishedSuccessfully());


  SC_LOG_DEBUG("6");

  EXPECT_TRUE(generatedMessageIsValid(&context, utils::IteratorUtils::getAnyByOutRelation(
          &context,
          test_action,
          ScKeynodes::rrel_1)));


  SC_LOG_DEBUG("6");

  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.UnsubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();
}


TEST_F(AgentTest, argumentIsNotALink)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context,TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureFirstArgumentIsNotALink.scs");
  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");

  ScAction test_action = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(test_action.InitiateAndWait(WAIT_TIME));

  EXPECT_TRUE(test_action.IsFinishedWithError());

  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.UnsubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();
}

TEST_F(AgentTest, linkSpecifiedIncorrectly)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context,TEST_FILES_DIR_PATH + "replyMessageAgentTestStructureWithIncorrectlySpecifiedLink.scs");
  
  context.SubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.SubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(test_action_node.IsValid());

  ScAction test_action = context.ConvertToAction(test_action_node);
  EXPECT_TRUE(test_action.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(test_action.IsFinishedWithError());

  context.UnsubscribeAgent<messageReplyModule::MessageReplyAgent>();
  context.UnsubscribeAgent<messageReplyModuleTest::GenerateReplyMessageAgent>();
}

}//namespace messageReplyModuleTest

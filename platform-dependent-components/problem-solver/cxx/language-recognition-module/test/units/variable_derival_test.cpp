/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-builder/scs_loader.hpp>

#include <sc-memory/test/sc_test.hpp>
#include "sc-memory/sc_keynodes.hpp"

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "keynodes/language_recognition_keynodes.hpp"
#include "agent/language-recognition-agent.hpp"

using namespace languageRecognitionModule;
using AgentTest = ScMemoryTest;

namespace ModuleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

TEST_F(AgentTest, EnglishMessageTest)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "english_message.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<languageRecognitionModule::LanguageRecognitionAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAddr const & message = context.SearchElementBySystemIdentifier("message");
  
  EXPECT_TRUE(context.HelperCheckEdge(LanguageRecognitionKeynodes::lang_en, message, ScType::EdgeAccessConstPosPerm));
  
  context.UnsubscribeAgent<languageRecognitionModule::LanguageRecognitionAgent>();
}

TEST_F(AgentTest, RussianMessageTest)
{
  ScAgentContext & context = *m_ctx;
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "russian_message.scs");
  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_action_node");
  ScAction testAction = context.ConvertToAction(testActionNode);

  context.SubscribeAgent<languageRecognitionModule::LanguageRecognitionAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());

  ScAddr const & message = context.SearchElementBySystemIdentifier("message");
  
  EXPECT_TRUE(context.HelperCheckEdge(ScKeynodes::lang_ru, message, ScType::EdgeAccessConstPosPerm));
  
  context.UnsubscribeAgent<languageRecognitionModule::LanguageRecognitionAgent>();
}

}  // namespace LanguageRecognitionTest

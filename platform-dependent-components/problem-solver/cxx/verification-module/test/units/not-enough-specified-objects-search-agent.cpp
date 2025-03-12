/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>

#include "keynodes/verification_keynodes.hpp"
#include "agent/not-enough-specified-objects-search-agent.hpp"

using namespace verificationModule;

namespace VerificationModuleTestA
{
std::string const TEST_PATH = VERIFICATION_MODULE_TEST_SRC_PATH "/testStructures/notEnoughSpecifiedObjects/";
int const WAIT_TIME = 1000;

using SearchNotEnoughSpecifiedObjectsAgentTest = ScMemoryTest;

TEST_F(SearchNotEnoughSpecifiedObjectsAgentTest, TestA)
{
  ScAgentContext & context = *m_ctx;
  ScsLoader loader;

  loader.loadScsFile(context, TEST_PATH + "templates.scs");
  loader.loadScsFile(context, TEST_PATH + "notEnoughSpecifiedObjects.scs");
  loader.loadScsFile(context, TEST_PATH + "action.scs");
  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(context.IsElement(test_action_node));

  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<verificationModule::SearchNotEnoughSpecifiedObjectsAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<verificationModule::SearchNotEnoughSpecifiedObjectsAgent>();
}

}  // namespace VerificationModuleTestA

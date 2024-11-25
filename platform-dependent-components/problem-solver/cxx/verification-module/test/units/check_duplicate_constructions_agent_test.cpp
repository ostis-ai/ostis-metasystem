/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <gtest/gtest.h>
#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>

#include "keynodes/verification_keynodes.hpp"
#include "agent/check-duplicate-constructions-agent.hpp"

using namespace verificationModule;

namespace VerificationModuleTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = VERIFICATION_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using CheckDuplicateConstructionsAgentTest = ScMemoryTest;

TEST_F(CheckDuplicateConstructionsAgentTest, DoesntHaveComponentTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "doesntHaveComponent.scs");
  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_action_node");
  EXPECT_TRUE(context.IsElement(test_action_node));

  ScAction testAction = context.ConvertToAction(test_action_node);

  context.SubscribeAgent<verificationModule::CheckDuplicateConstructionsAgent>();

  EXPECT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  EXPECT_TRUE(testAction.IsFinishedSuccessfully());
  context.UnsubscribeAgent<verificationModule::CheckDuplicateConstructionsAgent>();
}

}  // namespace VerificationModuleTest

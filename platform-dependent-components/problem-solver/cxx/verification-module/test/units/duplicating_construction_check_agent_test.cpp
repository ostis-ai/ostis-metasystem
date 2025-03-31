/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>

#include "duplicating_construction_check_agent_test.hpp"

#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>

#include "manager/duplications_check_manager.hpp"
#include "keynodes/verification_keynodes.hpp"

using namespace verificationModule;

namespace VerificationModuleTest
{
TEST_F(DuplicatingConstructionCheckAgentTest, NoMembersTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "duplicationTestCommonConstructions.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "noMembers.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  EXPECT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  //checks
}

//AccessArcsWithoutRelationsTest
//CommonArcsWithoutRelationsTest
//DuplicatingRoleRelationsTest
//MultipleArcsUsageTest
//EqualSetQuasybinariesTest
//MultipleSingularRelationsTest

//subjectDomainsSearch

}  // namespace VerificationModuleTest

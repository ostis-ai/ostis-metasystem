/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "duplicating_construction_check_agent_test.hpp"

#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"

#include <vector>
#include <regex>

#include "utils/identifier_utils.hpp"
#include "manager/duplications_check_manager.hpp"
#include "keynodes/verification_keynodes.hpp"
#include "agent/check-duplicate-constructions-agent.hpp"

using namespace verificationModule;

void DuplicatingConstructionCheckAgentTest::compareResultFiles(
    std::string const & generatedFileName,
    std::string const & referenceFileName)
{
  std::ifstream generatedFileStream(generatedFileName);
  std::ifstream referenceFileStream(referenceFileName);

  if (!referenceFileStream.is_open() || !generatedFileStream.is_open())
    FAIL();

  std::regex datetimeRegex("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
  std::string const & datetimeReplacement = "[DATETIME]";

  std::string referenceLine, generatedLine;

  std::getline(referenceFileStream, referenceLine);
  std::getline(generatedFileStream, generatedLine);

  // first string contain datetime
  std::string const & normalizedReferenceLine = std::regex_replace(referenceLine, datetimeRegex, datetimeReplacement);
  std::string const & normalizedGeneratedLine = std::regex_replace(generatedLine, datetimeRegex, datetimeReplacement);
  if (normalizedReferenceLine != normalizedGeneratedLine)
    FAIL();

  while (std::getline(referenceFileStream, referenceLine) && std::getline(generatedFileStream, generatedLine))
  {
    if (referenceLine != generatedLine)
      FAIL();
  }

  if (std::getline(referenceFileStream, referenceLine))
    FAIL();

  if (std::getline(generatedFileStream, generatedLine))
    FAIL();
}

namespace VerificationModuleTest
{
TEST_F(DuplicatingConstructionCheckAgentTest, NoMembersTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "no_members.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_FALSE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  ASSERT_TRUE(checkResult.elemtnsCheckResults.empty());
}

TEST_F(DuplicatingConstructionCheckAgentTest, MembershipArcsWithoutRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "membership_arcs_without_relations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_TRUE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1u);

  ElementDuplicationsCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");
  ASSERT_TRUE(elementCheckResult.errorsDescriptions.empty());
  ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1u);

  std::string const & warningDescription = elementCheckResult.warningDescriptions.front();

  ASSERT_EQ(
      warningDescription,
      "Found multiple membership arcs to object3. Duplication is possible if it was not intended as a multiset.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, CommonArcsWithoutRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "common_arcs_without_relations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_TRUE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1u);

  ElementDuplicationsCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");
  ASSERT_TRUE(elementCheckResult.errorsDescriptions.empty());
  ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1u);

  std::string const & warningDescription = elementCheckResult.warningDescriptions.front();

  ASSERT_EQ(
      warningDescription,
      "Found common arcs to object3 not belonging to any relations. Possible incorrect construction.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, DuplicatingNonRoleRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "duplicate_non_role_relations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_TRUE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1u);

  ElementDuplicationsCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");
  ASSERT_TRUE(elementCheckResult.warningDescriptions.empty());
  ASSERT_EQ(elementCheckResult.errorsDescriptions.size(), 1u);

  std::string const & errorDescription = elementCheckResult.errorsDescriptions.front();

  ASSERT_EQ(errorDescription, "Found duplication of relation nrel_r1 to object3");
}

TEST_F(DuplicatingConstructionCheckAgentTest, DuplicatingRoleRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "duplicate_role_relations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_TRUE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1u);

  ElementDuplicationsCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");

  ASSERT_EQ(elementCheckResult.errorsDescriptions.size(), 1u);
  std::string const & errorDescription = elementCheckResult.errorsDescriptions.front();
  ASSERT_EQ(errorDescription, "Found duplicate relation rrel_r3 to object3");

  ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1u);
  std::string const & warningDescription = elementCheckResult.warningDescriptions.front();
  ASSERT_EQ(
      warningDescription,
      "Found multiple membership arcs to object3. Duplication is possible if it was not intended as a multiset.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, EqualSetQuasybinariesTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "equal_set_quasybinaries.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_TRUE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  std::list<ElementDuplicationsCheckResult> const & elementsCheckResults = checkResult.elemtnsCheckResults;
  ASSERT_EQ(elementsCheckResults.size(), 3u);

  bool checkResultForObject1Exist = false;
  bool checkResultForObject2Exist = false;
  bool checkResultForObject3Exist = false;
  for (auto const & elementCheckResult : elementsCheckResults)
  {
    if (elementCheckResult.elementIdtf == "object1")
    {
      checkResultForObject1Exist = true;

      std::string errorDescription = elementCheckResult.errorsDescriptions.front();
      ASSERT_EQ(errorDescription, "Two or more nrel_quasibinary_r5's tuples are equal. Likely duplication.");
    }
    else if (elementCheckResult.elementIdtf == "object2")
    {
      checkResultForObject2Exist = true;

      ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1u);

      std::string warningDescription = elementCheckResult.warningDescriptions.front();
      ASSERT_EQ(warningDescription, "Found duplication of nrel_quasibinary_r6's tuple elements.");
    }
    else if (elementCheckResult.elementIdtf == "object3")
    {
      checkResultForObject3Exist = true;

      std::string errorDescription = elementCheckResult.errorsDescriptions.front();
      ASSERT_EQ(errorDescription, "Two or more nrel_quasibinary_r7's tuples are equal. Likely duplication.");
    }
    else
      FAIL();
  }

  ASSERT_TRUE(checkResultForObject1Exist);
  ASSERT_TRUE(checkResultForObject2Exist);
  ASSERT_TRUE(checkResultForObject3Exist);
}

TEST_F(DuplicatingConstructionCheckAgentTest, MultipleSingularRelationsWithSubjDomainSearchTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "multiple_singular_relations_test.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "subject_domains.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetDuplicationsCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  ASSERT_TRUE(duplicationsCheckManager.CheckSetElementsDuplications(testClass, checkResult));

  ASSERT_EQ(checkResult.setIdtf, "test_class");
  ASSERT_EQ(checkResult.subjectDomainsContainingAsMaximumClass.size(), 1u);
  ASSERT_EQ(checkResult.subjectDomainsContainingAsMaximumClass.front(), "subj_dom_1");

  ASSERT_EQ(checkResult.subjectDomainsContainingAsNotMaximumClass.size(), 2u);
  std::string firstSubjDom = checkResult.subjectDomainsContainingAsNotMaximumClass.front();
  std::string secondSubjDom = checkResult.subjectDomainsContainingAsNotMaximumClass.back();

  ASSERT_TRUE(firstSubjDom != secondSubjDom);
  ASSERT_TRUE(firstSubjDom == "subj_dom_2" || firstSubjDom == "subj_dom_3");
  ASSERT_TRUE(secondSubjDom == "subj_dom_2" || secondSubjDom == "subj_dom_3");

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1u);

  ElementDuplicationsCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");

  ASSERT_EQ(elementCheckResult.errorsDescriptions.size(), 2u);
  std::string const & firstErrorDescription = elementCheckResult.errorsDescriptions.front();
  std::string const & secondErrorDescription = elementCheckResult.errorsDescriptions.back();

  std::string const & firstExpectedErrorDescription =
      "Duplicate relation nrel_singular_r8 from object1. Expected only one outgoing relation pair per element.";
  std::string const & secondExpectedErrorDescription =
      "Duplicate relation rrel_singular_r10 from object1. Expected only one outgoing relation pair per element.";

  // Descriptions order is not defined
  ASSERT_TRUE(firstErrorDescription != secondErrorDescription);
  ASSERT_TRUE(
      (firstErrorDescription == firstExpectedErrorDescription)
      || (firstErrorDescription == secondExpectedErrorDescription));
  ASSERT_TRUE(
      (secondErrorDescription == firstExpectedErrorDescription)
      || (secondErrorDescription == secondExpectedErrorDescription));
}

TEST_F(DuplicatingConstructionCheckAgentTest, ComplexAgentCallWithArgument)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_class_main_idtf.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "multiple_duplications.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_action_with_argument.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "subject_domains.scs");

  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_verification_action_node");
  ASSERT_TRUE(context.IsElement(testActionNode));

  ScAction testAction = context.ConvertToAction(testActionNode);
  context.SubscribeAgent<verificationModule::CheckDuplicateConstructionsAgent>();

  ASSERT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  ASSERT_TRUE(testAction.IsFinishedSuccessfully());

  compareResultFiles(
      GENERATED_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class (test class)",
      REFERENCE_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class_example.txt");
}

TEST_F(DuplicatingConstructionCheckAgentTest, ComplexAgentCallWithoutArgument)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elements_types.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_class_main_idtf.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "multiple_duplications.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "test_action_without_argument.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "subject_domains.scs");

  ScAddr const & testActionNode = context.SearchElementBySystemIdentifier("test_verification_action_node");
  ASSERT_TRUE(context.IsElement(testActionNode));

  ScAction testAction = context.ConvertToAction(testActionNode);
  context.SubscribeAgent<verificationModule::CheckDuplicateConstructionsAgent>();

  ASSERT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  ASSERT_TRUE(testAction.IsFinishedSuccessfully());

  compareResultFiles(
      GENERATED_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class (test class)",
      REFERENCE_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class_example.txt");
}

TEST_F(DuplicatingConstructionCheckAgentTest, GetUniqueIdentifierTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "unique_identifiers_test.scs");

  ScAddr const & nodesWithIdtfsSet = context.SearchElementBySystemIdentifier("node_with_system_id");
  ASSERT_TRUE(context.IsElement(nodesWithIdtfsSet));
  ScAddr const & nodeWithIdtf = utils::IteratorUtils::getAnyFromSet(&context, nodesWithIdtfsSet);
  ASSERT_TRUE(context.IsElement(nodeWithIdtf));

  std::string identifierOfNodeWithSystemId =
      verificationModule::IdentifierUtils::GetUniqueIdentifier(&context, nodeWithIdtf);
  ASSERT_EQ(identifierOfNodeWithSystemId, "test_node");

  ScAddr const & nodesWithoutIdtfsSet = context.SearchElementBySystemIdentifier("node_without_system_id");
  ASSERT_TRUE(context.IsElement(nodesWithoutIdtfsSet));
  ScAddr const & nodeWithoutIdtf = utils::IteratorUtils::getAnyFromSet(&context, nodesWithoutIdtfsSet);
  ASSERT_TRUE(context.IsElement(nodeWithoutIdtf));

  std::string identifierOfNodeWithoutSystemId =
      verificationModule::IdentifierUtils::GetUniqueIdentifier(&context, nodeWithoutIdtf);
  ASSERT_EQ(identifierOfNodeWithoutSystemId, std::to_string(nodeWithoutIdtf.Hash()));
}

}  // namespace VerificationModuleTest

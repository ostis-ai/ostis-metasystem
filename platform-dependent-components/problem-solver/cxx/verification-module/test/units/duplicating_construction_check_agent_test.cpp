/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "duplicating_construction_check_agent_test.hpp"

#include <sc-memory/sc_agent.hpp>
#include "sc-memory/utils/sc_base64.hpp"

#include <vector>
#include <regex>

#include "manager/duplications_check_manager.hpp"
#include "keynodes/verification_keynodes.hpp"
#include "agent/check-duplicate-constructions-agent.hpp"

using namespace verificationModule;

void DuplicatingConstructionCheckAgentTest::compareResultFiles(
    std::string const & generatedFileName, std::string const & referenceFileName)
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
  std::string const & normalizedReferenceLine
      = std::regex_replace(referenceLine, datetimeRegex, datetimeReplacement);
  std::string const & normalizedGeneratedLine
      = std::regex_replace(generatedLine, datetimeRegex, datetimeReplacement);
  if (normalizedReferenceLine != normalizedGeneratedLine)
    FAIL();

  while (std::getline(referenceFileStream, referenceLine) && std::getline(generatedFileStream, generatedLine)) {
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

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "noMembers.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  ASSERT_TRUE(checkResult.elemtnsCheckResults.empty());
}

TEST_F(DuplicatingConstructionCheckAgentTest, AccessArcsWithoutRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "accessArcsWithoutRelations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1);

  ElementCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");
  ASSERT_TRUE(elementCheckResult.errorsDescriptions.empty());
  ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1);

  std::string const & warningDescription = elementCheckResult.warningDescriptions.front();

  ASSERT_EQ(
      warningDescription,
      "Found multiple access arcs to object3. Possible duplication if it was not meant "
      "as multiset.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, CommonArcsWithoutRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "commonArcsWithoutRelations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1);

  ElementCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");
  ASSERT_TRUE(elementCheckResult.errorsDescriptions.empty());
  ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1);

  std::string const & warningDescription = elementCheckResult.warningDescriptions.front();

  ASSERT_EQ(
      warningDescription,
      "Found common arcs to object3 not belonging to any relations. Possible incorrect construction.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, DuplicatingNonRoleRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "duplicatingNonRoleRelations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1);

  ElementCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");
  ASSERT_TRUE(elementCheckResult.warningDescriptions.empty());
  ASSERT_EQ(elementCheckResult.errorsDescriptions.size(), 1);

  std::string const & errorDescription = elementCheckResult.errorsDescriptions.front();

  ASSERT_EQ(errorDescription, "Found duplication of relation nrel_r1 to object3");
}

TEST_F(DuplicatingConstructionCheckAgentTest, DuplicatingRoleRelationsTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "duplicatingRoleRelations.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1);

  ElementCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");

  ASSERT_EQ(elementCheckResult.errorsDescriptions.size(), 1);
  std::string const & errorDescription = elementCheckResult.errorsDescriptions.front();
  ASSERT_EQ(errorDescription, "Found duplication of relation rrel_r3 to object3");

  ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1);
  std::string const & warningDescription = elementCheckResult.warningDescriptions.front();
  ASSERT_EQ(
      warningDescription,
      "Found multiple access arcs to object3. Possible duplication if it was not meant as multiset.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, EqualSetQuasybinariesTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "equalSetQuasybinaries.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  std::list<ElementCheckResult> const & elementsCheckResults = checkResult.elemtnsCheckResults;
  ASSERT_EQ(elementsCheckResults.size(), 3);

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

      ASSERT_EQ(elementCheckResult.warningDescriptions.size(), 1);

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

  ASSERT_TRUE(checkResultForObject1Exist && checkResultForObject2Exist && checkResultForObject3Exist);
}

TEST_F(DuplicatingConstructionCheckAgentTest, MultipleSingularRelationsWithSubjDomainSearchTest)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "multipleSingularRelationsTest.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "subjectDomains.scs");

  ScAddr const & testClass = context.SearchElementBySystemIdentifier("test_class");
  ASSERT_TRUE(context.IsElement(testClass));

  SetCheckResult checkResult;
  DuplicationsCheckManager duplicationsCheckManager(&context);
  duplicationsCheckManager.checkSetElementsDuplications(testClass, checkResult);

  ASSERT_EQ(checkResult.setIdtf, "test_class");
  ASSERT_EQ(checkResult.subjectDomainContainingAsMaximumClass, "subj_dom_1");

  ASSERT_EQ(checkResult.subjectDomainsContainingAsNotMaximumClass.size(), 2);
  std::string firstSubjDom = checkResult.subjectDomainsContainingAsNotMaximumClass.front();
  std::string secondSubjDom = checkResult.subjectDomainsContainingAsNotMaximumClass.back();

  ASSERT_TRUE(firstSubjDom != secondSubjDom);
  ASSERT_TRUE(firstSubjDom == "subj_dom_2" || firstSubjDom == "subj_dom_3");
  ASSERT_TRUE(secondSubjDom == "subj_dom_2" || secondSubjDom == "subj_dom_3");

  ASSERT_EQ(checkResult.elemtnsCheckResults.size(), 1);

  ElementCheckResult const & elementCheckResult = checkResult.elemtnsCheckResults.front();

  ASSERT_EQ(elementCheckResult.elementIdtf, "object1");

  ASSERT_EQ(elementCheckResult.errorsDescriptions.size(), 1);
  std::string const & errorDescription = elementCheckResult.errorsDescriptions.front();
  ASSERT_EQ(
      errorDescription,
      "Duplicating relation nrel_singular_r8 from object1. Expected only one outgoing relation pair per element.");
}

TEST_F(DuplicatingConstructionCheckAgentTest, ComplexAgentCallWithArgument)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "multipleDuplications.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testActionWithArgument.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "subjectDomains.scs");

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_verification_action_node");
  ASSERT_TRUE(context.IsElement(test_action_node));

  ScAction testAction = context.ConvertToAction(test_action_node);
  context.SubscribeAgent<verificationModule::CheckDuplicateConstructionsAgent>();

  ASSERT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  ASSERT_TRUE(testAction.IsFinishedSuccessfully());

  compareResultFiles(
      GENERATED_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class",
      REFERENCE_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class_example.txt");
}

TEST_F(DuplicatingConstructionCheckAgentTest, ComplexAgentCallWithoutArgument)
{
  ScAgentContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "elementsTypes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "multipleDuplications.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testActionWithoutArgument.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "subjectDomains.scs");

  ScAddr const & test_action_node = context.SearchElementBySystemIdentifier("test_verification_action_node");
  ASSERT_TRUE(context.IsElement(test_action_node));

  ScAction testAction = context.ConvertToAction(test_action_node);
  context.SubscribeAgent<verificationModule::CheckDuplicateConstructionsAgent>();

  ASSERT_TRUE(testAction.InitiateAndWait(WAIT_TIME));
  ASSERT_TRUE(testAction.IsFinishedSuccessfully());

  compareResultFiles(
      GENERATED_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class",
      REFERENCE_RESULT_FILES_PATH + "duplications_check_result_file_for_test_class_example.txt");
}

}  // namespace VerificationModuleTest

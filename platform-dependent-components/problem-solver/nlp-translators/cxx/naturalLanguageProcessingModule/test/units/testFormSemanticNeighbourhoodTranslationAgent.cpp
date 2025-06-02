#include "scs_loader.hpp"

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/TranslationKeynodes.hpp"

#include "agent/FormSemanticNeighbourhoodTranslationAgent.hpp"

#include "translator/SemanticNeighbourhoodTranslatorSet.hpp"
#include "translator/SemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelInNodeSemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelInLinkSemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"
#include "translator/FromParameterSemanticNeighbourhoodTranslator.hpp"
#include "translator/FromConceptSemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelFromNodeSemanticNeighbourhoodTranslator.hpp"
#include "translator/NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"

#include "sc-memory/kpm/sc_agent.hpp"
#include "sc_test.hpp"
#include "translator/SemanticNeighbourhoodTranslatorSet.hpp"

#include <algorithm>

using namespace naturalLanguageProcessingModule;

namespace testFormSemanticNeighbourhoodTranslationAgent
{
ScsLoader loader;
std::string const & TEST_FILES_DIR_PATH = NATURAL_LANGUAGE_PROCESSING_MODULE_TEST_SRC_PATH "/testStructures/FormSemanticNeighbourhoodTranslation/";
std::string const & TEST_QUESTION_NODE_ALIAS = "test_question_node";
int const WAIT_TIME = 3000;
size_t const MAX_TRANSLATIONS = 30;

using FormSemanticNeighbourhoodTest = ScMemoryTest;

void initializeClasses()
{
  scAgentsCommon::CoreKeynodes::InitGlobal();
  TranslationKeynodes::InitGlobal();
  ScAgentInit(true);
  SC_AGENT_REGISTER(FormSemanticNeighbourhoodTranslationAgent);
}

void deinitializeClasses()
{
  SC_AGENT_UNREGISTER(FormSemanticNeighbourhoodTranslationAgent);
}

TEST_F(FormSemanticNeighbourhoodTest, TestSetFillingWithPointers)
{
  ScMemoryContext & context = *m_ctx;
  auto translator1 = new NrelInLinkSemanticNeighbourhoodTranslator(&context);
  auto translator2 = new NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator(&context);
  auto translator3 = new NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator(&context);
  auto translator4 = new NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(&context);
  auto translator5 = new NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(&context);
  auto translator6 = new NrelInNodeSemanticNeighbourhoodTranslator(&context);
  auto translator7 = new NrelFromNodeSemanticNeighbourhoodTranslator(&context);
  auto translator8 = new FromParameterSemanticNeighbourhoodTranslator(&context);
  auto translator9 = new FromConceptSemanticNeighbourhoodTranslator(&context);

  std::set<SemanticNeighbourhoodTranslator *, SemanticNeighbourhoodTranslatorCmp> translators;
  translators.insert(translator1);
  translators.insert(translator2);
  translators.insert(translator3);
  translators.insert(translator4);
  translators.insert(translator5);
  translators.insert(translator6);
  translators.insert(translator7);
  translators.insert(translator8);
  translators.insert(translator9);
  translators.insert(translator9);
  translators.insert(translator8);
  translators.insert(translator7);
  translators.insert(translator6);
  translators.insert(translator5);
  translators.insert(translator4);
  translators.insert(translator3);
  translators.insert(translator2);
  translators.insert(translator1);
  EXPECT_EQ(translators.size(), 9u);
  for (const auto & item: translators)
    delete item;
}

TEST_F(FormSemanticNeighbourhoodTest, TestFromConceptSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testFromConceptSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new FromConceptSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "is apple"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "is fruit"), answer.cend());
  EXPECT_EQ(std::find(answer.cbegin(), answer.cend(), "is red"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestFromParameterSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testFromParameterSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new FromParameterSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 1u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "color is red"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelInLinkSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelInLinkSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelInLinkSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 1u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "name Arnold"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelInQuasybinaryLinkSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelInQuasybinaryLinkSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "synonyms question node"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "synonyms node with question"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "decomposition question 1"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "decomposition question 2"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelInQuasybinaryNodeSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelInQuasybinaryNodeSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "parents first parent"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "parents second parent"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "parts decomposition big part"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "parts decomposition little part"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelInNodeSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelInNodeSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelInNodeSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "likes pizza"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "likes to eat pizza"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestNrelFromNodeSemanticNeighbourhoodTranslator)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testNrelFromNodeSemanticNeighbourhoodTranslator.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new NrelFromNodeSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 2u);
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "parent kid1"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "parent kid2"), answer.cend());

  deinitializeClasses();
  delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestAllSemanticNeighbourhoodTranslators)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testAllSemanticNeighbourhoodTranslators.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);
  auto translator1 = new NrelInLinkSemanticNeighbourhoodTranslator(&context);
  auto translator2 = new NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator(&context);
  auto translator3 = new NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator(&context);
  auto translator4 = new NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(&context);
  auto translator5 = new NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(&context);
  auto translator6 = new NrelInNodeSemanticNeighbourhoodTranslator(&context);
  auto translator7 = new NrelFromNodeSemanticNeighbourhoodTranslator(&context);
  auto translator8 = new FromParameterSemanticNeighbourhoodTranslator(&context);
  auto translator9 = new FromConceptSemanticNeighbourhoodTranslator(&context);

  std::set<SemanticNeighbourhoodTranslator *, SemanticNeighbourhoodTranslatorCmp> translators;
  translators.insert(translator1);
  translators.insert(translator2);
  translators.insert(translator3);
  translators.insert(translator4);
  translators.insert(translator5);
  translators.insert(translator6);
  translators.insert(translator7);
  translators.insert(translator8);
  translators.insert(translator9);

  std::set<std::string> answer;

  for (const auto & translator : translators)
  {
    std::vector<std::string> const & translations = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});
    std::copy(translations.begin(), translations.end(), std::inserter(answer, answer.cend()));
  }

  EXPECT_EQ(answer.size(), 16u);

  EXPECT_NE(answer.find("is apple"), answer.cend());
  EXPECT_NE(answer.find("is fruit"), answer.cend());

  EXPECT_NE(answer.find("color is red"), answer.cend());

  EXPECT_NE(answer.find("name Arnold"), answer.cend());

  EXPECT_NE(answer.find("synonyms question node"), answer.cend());
  EXPECT_NE(answer.find("synonyms node with question"), answer.cend());

  EXPECT_NE(answer.find("decomposition question 1"), answer.cend());
  EXPECT_NE(answer.find("decomposition question 2"), answer.cend());

  EXPECT_NE(answer.find("parents first parent"), answer.cend());
  EXPECT_NE(answer.find("parents second parent"), answer.cend());

  EXPECT_NE(answer.find("parts decomposition big part"), answer.cend());
  EXPECT_NE(answer.find("parts decomposition little part"), answer.cend());

  EXPECT_NE(answer.find("likes pizza"), answer.cend());
  EXPECT_NE(answer.find("likes to eat pizza"), answer.cend());

  EXPECT_NE(answer.find("parent kid1"), answer.cend());
  EXPECT_NE(answer.find("parent kid2"), answer.cend());

  deinitializeClasses();
  for (const auto & translator : translators)
    delete translator;
}

TEST_F(FormSemanticNeighbourhoodTest, TestAllSemanticNeighbourhoodTranslatorSetgetSemanticNeighbourhoods)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testAllSemanticNeighbourhoodTranslators.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);
  SemanticNeighbourhoodTranslatorSet SemanticNeighbourhoodTranslatorSet(&context);


  std::set<std::string> answer;
  auto const & translationsVector = SemanticNeighbourhoodTranslatorSet.getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {});
  std::copy(translationsVector.cbegin(), translationsVector.cend(), std::inserter(answer, answer.cend()));

  EXPECT_EQ(answer.size(), 16u);

  EXPECT_NE(answer.find("is apple"), answer.cend());
  EXPECT_NE(answer.find("is fruit"), answer.cend());

  EXPECT_NE(answer.find("color is red"), answer.cend());

  EXPECT_NE(answer.find("name Arnold"), answer.cend());

  EXPECT_NE(answer.find("synonyms question node"), answer.cend());
  EXPECT_NE(answer.find("synonyms node with question"), answer.cend());

  EXPECT_NE(answer.find("decomposition question 1"), answer.cend());
  EXPECT_NE(answer.find("decomposition question 2"), answer.cend());

  EXPECT_NE(answer.find("parents first parent"), answer.cend());
  EXPECT_NE(answer.find("parents second parent"), answer.cend());

  EXPECT_NE(answer.find("parts decomposition big part"), answer.cend());
  EXPECT_NE(answer.find("parts decomposition little part"), answer.cend());

  EXPECT_NE(answer.find("likes pizza"), answer.cend());
  EXPECT_NE(answer.find("likes to eat pizza"), answer.cend());

  EXPECT_NE(answer.find("parent kid1"), answer.cend());
  EXPECT_NE(answer.find("parent kid2"), answer.cend());

  deinitializeClasses();
}

TEST_F(FormSemanticNeighbourhoodTest, TestAllSemanticNeighbourhoodTranslatorSetgetSemanticNeighbourhoodsNodes)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testAllSemanticNeighbourhoodTranslators.scs");
  ScAddr const & ignored_keynodes = context.HelperResolveSystemIdtf("translation_ignored_keynodes");
  ScAddr const & nrel_system_identifier = context.HelperFindBySystemIdtf("nrel_system_identifier");
  context.CreateEdge(ScType::EdgeAccessConstPosPerm, ignored_keynodes, nrel_system_identifier);
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);
  SemanticNeighbourhoodTranslatorSet SemanticNeighbourhoodTranslatorSet(&context);


  ScAddrVector answer;
  auto const & translationsNodesList = SemanticNeighbourhoodTranslatorSet.getSemanticNeighbourhoodsElements(test_question_node, {});
  for (const auto & item : translationsNodesList)
    answer.insert(answer.cend(), item.cbegin(), item.cend());

  EXPECT_EQ(answer.size(), 92u);

  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("concept_apple")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("concept_red")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("concept_fruit")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("arnold_link")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_name")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("question_node_link")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("node_with_question_link")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_synonyms")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("question_1_link")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("question_2_link")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_decomposition")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("parent1")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("parent2")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_parents")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("part1")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("part2")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_parts_decomposition")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("pizza")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_likes_to_eat")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_likes")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("kid1")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("kid2")), answer.cend());
  EXPECT_NE(std::find(answer.begin(), answer.end(), context.HelperFindBySystemIdtf("nrel_parent")), answer.cend());
  deinitializeClasses();
}

TEST_F(FormSemanticNeighbourhoodTest, TestAllSemanticNeighbourhoodAgent)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testAgentOnAllTranslations.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);
  EXPECT_TRUE(test_question_node.IsValid());

  EXPECT_TRUE(utils::AgentUtils::applyAction(&context, test_question_node, WAIT_TIME));
  EXPECT_TRUE(context.HelperCheckEdge(
      scAgentsCommon::CoreKeynodes::question_finished_successfully,
      test_question_node,
      ScType::EdgeAccessConstPosPerm));
  ScAddr const & answer = utils::IteratorUtils::getAnyByOutRelation(&context, test_question_node, scAgentsCommon::CoreKeynodes::nrel_answer);
  ASSERT_TRUE(answer.IsValid());
  ScAddr const & answerLink = utils::IteratorUtils::getAnyFromSet(&context, answer);
  ASSERT_TRUE(context.GetElementType(answerLink).IsLink());

  deinitializeClasses();
}

TEST_F(FormSemanticNeighbourhoodTest, TestAllSemanticNeighbourhoodTranslatorSetgetSemanticNeighbourhoodsOnlyNrels)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testAllSemanticNeighbourhoodTranslators.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);
  SemanticNeighbourhoodTranslatorSet SemanticNeighbourhoodTranslatorSet(&context);

  std::set<std::string> answer;
  ScAddrVector const & norolesVector = utils::IteratorUtils::getAllWithType(&context, context.HelperFindBySystemIdtf("everything"), ScType::NodeConstNoRole);
  ScAddrSet const & norolesSet = {norolesVector.cbegin(), norolesVector.cend()};
  auto const & translationsVector = SemanticNeighbourhoodTranslatorSet.getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, norolesSet);
  std::copy(translationsVector.cbegin(), translationsVector.cend(), std::inserter(answer, answer.cend()));

  EXPECT_EQ(answer.size(), 13u);

  EXPECT_NE(answer.find("name Arnold"), answer.cend());

  EXPECT_NE(answer.find("synonyms question node"), answer.cend());
  EXPECT_NE(answer.find("synonyms node with question"), answer.cend());

  EXPECT_NE(answer.find("decomposition question 1"), answer.cend());
  EXPECT_NE(answer.find("decomposition question 2"), answer.cend());

  EXPECT_NE(answer.find("parents first parent"), answer.cend());
  EXPECT_NE(answer.find("parents second parent"), answer.cend());

  EXPECT_NE(answer.find("parts decomposition big part"), answer.cend());
  EXPECT_NE(answer.find("parts decomposition little part"), answer.cend());

  EXPECT_NE(answer.find("likes pizza"), answer.cend());
  EXPECT_NE(answer.find("likes to eat pizza"), answer.cend());

  EXPECT_NE(answer.find("parent kid1"), answer.cend());
  EXPECT_NE(answer.find("parent kid2"), answer.cend());
  deinitializeClasses();
}

TEST_F(FormSemanticNeighbourhoodTest, TestAllSemanticNeighbourhoodTranslatorSetgetSemanticNeighbourhoodsOnlyClasses)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testAllSemanticNeighbourhoodTranslators.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);
  SemanticNeighbourhoodTranslatorSet SemanticNeighbourhoodTranslatorSet(&context);

  std::set<std::string> answer;
  ScAddrVector const & norolesVector = utils::IteratorUtils::getAllWithType(&context, context.HelperFindBySystemIdtf("everything"), ScType::NodeConstClass);
  ScAddrSet const & norolesSet = {norolesVector.cbegin(), norolesVector.cend()};
  auto const & translationsVector = SemanticNeighbourhoodTranslatorSet.getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, norolesSet);
  std::copy(translationsVector.cbegin(), translationsVector.cend(), std::inserter(answer, answer.cend()));

  EXPECT_EQ(answer.size(), 3u);

  EXPECT_NE(answer.find("is apple"), answer.cend());
  EXPECT_NE(answer.find("is fruit"), answer.cend());

  EXPECT_NE(answer.find("color is red"), answer.cend());

  deinitializeClasses();
}

TEST_F(FormSemanticNeighbourhoodTest, TestIgnoredKeynodes)
{
  ScMemoryContext & context = *m_ctx;
  initializeClasses();

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "testIgnoredKeynodes.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "ignoredKeynodes.scs");
  ScAddr test_question_node = context.HelperFindBySystemIdtf(TEST_QUESTION_NODE_ALIAS);

  auto translator = new FromConceptSemanticNeighbourhoodTranslator(&context);
  auto const & answer = translator->getSemanticNeighbourhoods(test_question_node, MAX_TRANSLATIONS, {}, {});

  EXPECT_EQ(answer.size(), 1u);
  EXPECT_EQ(std::find(answer.cbegin(), answer.cend(), "is apple"), answer.cend());
  EXPECT_NE(std::find(answer.cbegin(), answer.cend(), "is fruit"), answer.cend());
  EXPECT_EQ(std::find(answer.cbegin(), answer.cend(), "is red"), answer.cend());

  deinitializeClasses();
  delete translator;
}

}  // namespace testFormStaticTranslationAgent

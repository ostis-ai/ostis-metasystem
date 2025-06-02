#include <sc-builder/scs_loader.hpp>
#include <sc-memory/test/sc_test.hpp>

#include <sc-agents-common/utils/CommonUtils.hpp>

#include "agent/message-topic-classification-agent.hpp"
#include "keynodes/message_classification_keynodes.hpp"
#include "keynodes/Keynodes.hpp"
#include "keynodes/LocalKeynodes.hpp"
#include "WitAiCkientMock.hpp"

using namespace messageClassificationModule;

namespace messageTopicClassificationTest
{
ScsLoader loader;
std::string const TEST_FILES_DIR_PATH = MESSAGE_TOPIC_CLASSIFICATION_MODULE_TEST_SRC_PATH "/testStructures/";

int const WAIT_TIME = 1000;

using MessageTopicClassificationTest = ScMemoryTest;

TEST_F(MessageTopicClassificationTest, connectionTest)
{
  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":{},"intents":[{"confidence":0.8592,"id":"4251337931554570","name":"start_greeting"}],
              "text":"Hello.","traits":{"wit$sentiment":[{"confidence":0.65,"id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c",
              "value":"positive"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(json::parse(witResponse)));
  json response = client->getWitResponse("Привет Максиму из лета.");

  EXPECT_FALSE(response.empty());

  delete client;
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithoutEntityTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hello_message.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");

  ScAddr greetingMessageClass = context.SearchElementBySystemIdentifier("concept_greeting_message");
  ScAddr messageAddr = context.SearchElementBySystemIdentifier("message");
  EXPECT_TRUE(greetingMessageClass.IsValid());
  EXPECT_TRUE(messageAddr.IsValid());

  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":{},"intents":[{"confidence":0.8592,"id":"4251337931554570","name":"start_greeting"}],
              "text":"Hello.","traits":{"wit$sentiment":[{"confidence":0.65,"id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c",
              "value":"positive"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);
  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);
  EXPECT_FALSE(messageClassificationItems.empty());

  bool isMessageClassified = context.HelperCheckEdge(greetingMessageClass, messageAddr, ScType::EdgeAccessConstPosPerm);
  EXPECT_TRUE(isMessageClassified);
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithEntityTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hello_message_with_entity.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");

  ScAddr greetingMessageClass = context.SearchElementBySystemIdentifier("concept_greeting_message");
  ScAddr messageAddr = context.SearchElementBySystemIdentifier("message");
  ScAddr entityAddr = context.SearchElementBySystemIdentifier("maksim");
  ScAddr entityRoleAddr = context.SearchElementBySystemIdentifier("rrel_contact");

  EXPECT_TRUE(greetingMessageClass.IsValid());
  EXPECT_TRUE(messageAddr.IsValid());
  EXPECT_TRUE(entityAddr.IsValid());
  EXPECT_TRUE(entityRoleAddr.IsValid());

  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":{"wit$contact:contact":[{"body":"Максим","confidence":0.9059,"end":12,"entities":{},
              "id":"210261890968181","name":"wit$contact", "role":"contact","start":6,"suggested":true,
              "type":"value","value":"Максим"}]},"intents":[{"confidence":0.9994,"id":"4251337931554570",
              "name":"start_greeting"}],"text":"Hello Maksim","traits":{"wit$sentiment":[{"confidence":0.6224,
              "id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c","value":"neutral"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(nlohmann::json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);

  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);

  EXPECT_FALSE(messageClassificationItems.empty());

  ScTemplate classificationTemplate;
  classificationTemplate.Triple(greetingMessageClass, ScType::EdgeAccessVarPosPerm, messageAddr);
  classificationTemplate.Quintuple(
      messageAddr, ScType::EdgeAccessVarPosPerm, entityAddr, ScType::EdgeAccessVarPosPerm, entityRoleAddr);

  ScTemplateSearchResult classificationTemplateResult;
  context.HelperSearchTemplate(classificationTemplate, classificationTemplateResult);
  EXPECT_TRUE(classificationTemplateResult.Size() == 1);
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithTwoEntitiesTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hello_message_with_two_entities.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");

  ScAddr weatherMessageClass = context.SearchElementBySystemIdentifier("concept_general_message_about_weather");
  ScAddr neutralMessageClass = context.SearchElementBySystemIdentifier("concept_message_with_neutral_emotional_coloring");
  ScAddr messageAddr = context.SearchElementBySystemIdentifier("message");
  ScAddr entityContactAddr = context.SearchElementBySystemIdentifier("maksim");
  ScAddr entitySeasonAddr = context.SearchElementBySystemIdentifier("summer");
  ScAddr rrelContactAddr = context.SearchElementBySystemIdentifier("rrel_contact");
  ScAddr rrelSeasonAddr = context.SearchElementBySystemIdentifier("rrel_season");

  EXPECT_TRUE(weatherMessageClass.IsValid());
  EXPECT_TRUE(messageAddr.IsValid());
  EXPECT_TRUE(entityContactAddr.IsValid());
  EXPECT_TRUE(entitySeasonAddr.IsValid());
  EXPECT_TRUE(rrelContactAddr.IsValid());
  EXPECT_TRUE(rrelSeasonAddr.IsValid());

  auto * client = new WitAiClientMock();
  std::string witResponse =
      R"({"entities":
              {"season:season":[{"body":"лето","confidence":0.9995,"end":24,"entities":{},"id":"530932871575541",
              "name":"season","role":"season","start":18,"type":"value","value":"лето"}],"wit$contact:contact":
              [{"body":"Максим","confidence":0.9356,"end":12,"entities":{},"id":"210261890968181","name":"wit$contact",
              "role":"contact","start":6,"suggested":true,"type":"value","value":"Максим"}]},"intents":
              [{"confidence":0.6678,"id":"607208973598157","name":"weather"}],"text":"Привет Максиму из лета.",
              "traits":{"wit$sentiment":[{"confidence":0.5628,"id":"5ac2b50a-44e4-466e-9d49-bad6bd40092c",
              "value":"neutral"}]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(nlohmann::json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);

  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);

  EXPECT_FALSE(messageClassificationItems.empty());

  ScTemplate classificationTemplate;
  classificationTemplate.Triple(weatherMessageClass, ScType::EdgeAccessVarPosPerm, messageAddr);
  classificationTemplate.Triple(neutralMessageClass, ScType::EdgeAccessVarPosPerm, messageAddr);
  classificationTemplate.Quintuple(
      messageAddr, 
      ScType::EdgeAccessVarPosPerm, 
      entityContactAddr, 
      ScType::EdgeAccessVarPosPerm, 
      rrelContactAddr);
  classificationTemplate.Quintuple(
      messageAddr, 
      ScType::EdgeAccessVarPosPerm, 
      entitySeasonAddr, 
      ScType::EdgeAccessVarPosPerm, 
      rrelSeasonAddr);

  ScTemplateSearchResult classificationTemplateResult;
  context.HelperSearchTemplate(classificationTemplate, classificationTemplateResult);

  EXPECT_TRUE(classificationTemplateResult.Size() == 1);
}

TEST_F(MessageTopicClassificationTest, classifyMessageWithTwoEntitiesSameRoleTest)
{
  ScMemoryContext & context = *m_ctx;

  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "hobby_message_with_two_same_role_entities.scs");
  loader.loadScsFile(context, TEST_FILES_DIR_PATH + "wit_concepts.scs");

  ScAddr messageAddr = context.SearchElementBySystemIdentifier("message");
  ScAddr hobbyAddr = context.SearchElementBySystemIdentifier("concept_hobby");
  ScAddr theatreAddr = context.SearchElementBySystemIdentifier("concept_theatre");
  ScAddr rrelEntityAddr = context.SearchElementBySystemIdentifier("rrel_entity");

  EXPECT_TRUE(messageAddr.IsValid());
  EXPECT_TRUE(hobbyAddr.IsValid());
  EXPECT_TRUE(theatreAddr.IsValid());
  EXPECT_TRUE(rrelEntityAddr.IsValid());

  auto * client = new WitAiClientMock();
  // Four entities with the same role "rrel_entity", some entities are duplicated. Expected to get two unique entities
  std::string witResponse =
      R"({"entities": {"rrel_entity:rrel_entity":
    [
     {"body":"хобби","confidence":1,"end":27,"entities":{},"id":"454915576488202",
     "name":"rrel_entity","role":"rrel_entity","start":22,"type":"value","value":"хобби"},
     {"body":"театр","confidence":1,"end":38,"entities":{},"id":"454915576488202",
     "name":"rrel_entity","role":"rrel_entity","start":33,"type":"value","value":"театр"},
     {"body":"хобби","confidence":0.9616397948452406,"end":49,"entities":{},"id":"454915576488202",
     "name":"rrel_entity","role":"rrel_entity","start":39,"suggested":true,"type":"value","value":"хобби"},
     {"body":"театр","confidence":0.9967311536328475,"end":69,"entities":{},"id":"454915576488202",
     "name":"rrel_entity","role":"rrel_entity","start":58,"suggested":true,"type":"value","value":"театр"}
    ]}})";
  EXPECT_CALL(*client, getWitResponse(testing::_))
      .Times(testing::Exactly(1))
      .WillOnce(testing::Return(nlohmann::json::parse(witResponse)));

  MessageTopicClassifier classifier(&context, client);

  ScAddrVector messageClassificationItems = classifier.classifyMessage(messageAddr);

  EXPECT_FALSE(messageClassificationItems.empty());

  ScTemplate entitiesTemplate;
  entitiesTemplate.Quintuple(
      messageAddr, ScType::EdgeAccessVarPosPerm, hobbyAddr, ScType::EdgeAccessVarPosPerm, rrelEntityAddr);
  entitiesTemplate.Quintuple(
      messageAddr, ScType::EdgeAccessVarPosPerm, theatreAddr, ScType::EdgeAccessVarPosPerm, rrelEntityAddr);

  ScTemplateSearchResult classificationTemplateResult;
  context.HelperSearchTemplate(entitiesTemplate, classificationTemplateResult);

  SC_LOG_DEBUG(classificationTemplateResult.Size());
  EXPECT_TRUE(classificationTemplateResult.Size() == 1);
}

}  // namespace messageTopicClassificationTest

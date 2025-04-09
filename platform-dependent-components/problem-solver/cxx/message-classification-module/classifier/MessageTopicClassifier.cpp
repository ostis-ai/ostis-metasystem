#include <sc-agents-common/utils/IteratorUtils.hpp>

#include "constants/MessageClassificationConstants.hpp"
#include "keynodes/message_classification_keynodes.hpp"
#include "searcher/MessageSearcher.hpp"

#include "MessageTopicClassifier.hpp"

namespace messageClassificationModule
{
MessageTopicClassifier::MessageTopicClassifier(ScMemoryContext * context, WitAiClientInterface * client)
  : context(context)
  , client(client)
{
  messageSearcher = std::make_unique<MessageSearcher>(this->context);
}

ScAddrVector MessageTopicClassifier::classifyMessage(ScAddr const & messageAddr)
{
  ScAddrVector messageClassificationElements = {messageAddr};

  std::string const messageText = getMessageText(messageAddr);

  json const witResponse = client->getWitResponse(messageText);
  SC_LOG_INFO(witResponse);

  ScAddrVector const messageIntentElements = getMessageIntentClass(messageAddr, witResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageIntentElements.cbegin(), messageIntentElements.cend());

  ScAddrVector const messageTraitElements = getMessageTraitClass(messageAddr, witResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageTraitElements.cbegin(), messageTraitElements.cend());

  ScAddrVector const messageEntitiesElements = getMessageEntity(messageAddr, witResponse);
  messageClassificationElements.insert(
      messageClassificationElements.cend(), messageEntitiesElements.cbegin(), messageEntitiesElements.cend());

  return messageClassificationElements;
}

std::string MessageTopicClassifier::getMessageText(ScAddr const & messageAddr)
{
  ScAddr const messageLink = messageSearcher->getMessageLink(messageAddr);
  if (!messageLink.IsValid())
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "MessageTopicClassifier: Message link is not found.");
  }
  std::string linkContent;
  if (context->GetLinkContent(messageLink, linkContent))
  {
    return linkContent;
  }
  return {};
}

ScAddrVector MessageTopicClassifier::getMessageIntentClass(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageIntentCLassElements;

  std::string const messageIntent = getMessageIntent(witResponse);
  if (messageIntent.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        MessageClassificationKeynodes::concept_not_classified_by_intent_message,
        messageAddr);
    messageIntentCLassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_intent_message);
    messageIntentCLassElements.push_back(messageIntentCLassEdge);
    return messageIntentCLassElements;
  }

  ScIterator3Ptr const possibleIntentIterator = context->CreateIterator3(
      MessageClassificationKeynodes::concept_intent_possible_class,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstClass);

  std::vector<std::string> witAiIdtfs;
  ScAddr possibleMessageCLass;
  while (possibleIntentIterator->Next())
  {
    possibleMessageCLass = possibleIntentIterator->Get(2);
    witAiIdtfs = getWitAiIdtfs(possibleMessageCLass);

    for (std::string const & witAiIdtf : witAiIdtfs)
    {
      if (messageIntent == witAiIdtf)
      {
        SC_LOG_DEBUG(
            "MessageTopicClassifier: found " + context->HelperGetSystemIdtf(possibleMessageCLass) + " intent class");
        ScAddr messageIntentCLassEdge =
            context->CreateEdge(ScType::EdgeAccessConstPosPerm, possibleMessageCLass, messageAddr);
        messageIntentCLassElements.push_back(possibleMessageCLass);
        messageIntentCLassElements.push_back(messageIntentCLassEdge);
        return messageIntentCLassElements;
      }
    }
  }

  return messageIntentCLassElements;
}

std::string MessageTopicClassifier::getMessageIntent(json const & witResponse)
{
  std::string messageIntent;
  try
  {
    messageIntent = witResponse.at(WitAiConstants::intents).at(0).at(WitAiConstants::name);
  }
  catch (...)
  {
    SC_LOG_WARNING("MessageTopicClassifier: Message intent class is not found.");
  }

  return messageIntent;
}

std::vector<std::string> MessageTopicClassifier::getWitAiIdtfs(ScAddr const & messageClass)
{
  ScAddrVector witAiIdtfAddrs =
      utils::IteratorUtils::getAllByOutRelation(context, messageClass, MessageClassificationKeynodes::nrel_wit_ai_idtf);
  std::vector<std::string> witAiIdtfs;
  for (ScAddr const & witAiIdtfAddr : witAiIdtfAddrs)
  {
    std::string linkContent;
    if (context->GetLinkContent(witAiIdtfAddr, linkContent))
    {
      witAiIdtfs.push_back(linkContent);
    }
  }

  return witAiIdtfs;
}

ScAddrVector MessageTopicClassifier::getMessageTraitClass(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageTraitClassElements;

  json const messageTrait = getMessageTrait(witResponse);
  if (messageTrait.empty())
  {
    ScAddr const & messageIntentCLassEdge = context->CreateEdge(
        ScType::EdgeAccessConstPosPerm,
        MessageClassificationKeynodes::concept_not_classified_by_trait_message,
        messageAddr);
    messageTraitClassElements.push_back(MessageClassificationKeynodes::concept_not_classified_by_trait_message);
    messageTraitClassElements.push_back(messageIntentCLassEdge);
    return messageTraitClassElements;
  }

  ScIterator3Ptr possibleTraitIterator = context->CreateIterator3(
      MessageClassificationKeynodes::concept_trait_possible_class,
      ScType::EdgeAccessConstPosPerm,
      ScType::NodeConstClass);

  messageTraitClassElements =
      processTraits(possibleTraitIterator, messageTrait, messageTraitClassElements, messageAddr);

  return messageTraitClassElements;
}

json MessageTopicClassifier::getMessageTrait(json const & witResponse)
{
  json messageIntent;
  try
  {
    messageIntent = witResponse.at(WitAiConstants::traits);
  }
  catch (...)
  {
    SC_LOG_WARNING("MessageTopicClassifier: Message trait class is not found.");
  }

  return messageIntent;
}

void MessageTopicClassifier::buildTraitTemplate(ScTemplate & traitTemplate, ScAddr const & possibleMessageCLass)
{
  traitTemplate.Quintuple(
      possibleMessageCLass,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> MessageClassificationAliasConstants::traitIncludedClassLinkAlias,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
  traitTemplate.Quintuple(
      ScType::NodeVarClass >> MessageClassificationAliasConstants::setOfTraitsClassAlias,
      ScType::EdgeDCommonVar,
      possibleMessageCLass,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::nrel_inclusion);
  traitTemplate.Quintuple(
      MessageClassificationAliasConstants::setOfTraitsClassAlias,
      ScType::EdgeDCommonVar,
      ScType::LinkVar >> MessageClassificationAliasConstants::setOfTraitsClassLinkAlias,
      ScType::EdgeAccessVarPosPerm,
      MessageClassificationKeynodes::nrel_wit_ai_idtf);
}

ScAddrVector MessageTopicClassifier::processTraits(
    ScIterator3Ptr & possibleTraitIterator,
    json const & messageTrait,
    ScAddrVector & messageTraitClassElements,
    ScAddr const & messageAddr)
{
  std::string traitWitIdtf;
  std::string setOfTraitsWitIdtf;
  std::string traitClassIdtf;

  ScTemplate traitTemplate;
  ScAddr possibleMessageCLass;

  while (possibleTraitIterator->Next())
  {
    possibleMessageCLass = possibleTraitIterator->Get(2);
    buildTraitTemplate(traitTemplate, possibleMessageCLass);

    ScTemplateSearchResult traitTemplateResult;
    context->HelperSearchTemplate(traitTemplate, traitTemplateResult);
    traitTemplate.Clear();

    if (traitTemplateResult.Size() == 1)
    {
      ScAddr setOfTraitsLink = traitTemplateResult[0][MessageClassificationAliasConstants::setOfTraitsClassLinkAlias];
      ScAddr traitLink = traitTemplateResult[0][MessageClassificationAliasConstants::traitIncludedClassLinkAlias];

      std::string traitWitIdtf;
      std::string setOfTraitsWitIdtf;
      context->GetLinkContent(traitLink, traitWitIdtf);
      context->GetLinkContent(setOfTraitsLink, setOfTraitsWitIdtf);
      traitClassIdtf = messageTrait.at(setOfTraitsWitIdtf).at(0).at(WitAiConstants::value);

      if (traitClassIdtf == traitWitIdtf)
      {
        SC_LOG_DEBUG(
            "MessageTopicClassifier: found " + context->HelperGetSystemIdtf(possibleMessageCLass) + " trait class");
        ScAddr messageTraitClassEdge =
            context->CreateEdge(ScType::EdgeAccessConstPosPerm, possibleMessageCLass, messageAddr);
        messageTraitClassElements.push_back(possibleMessageCLass);
        messageTraitClassElements.push_back(messageTraitClassEdge);

        return messageTraitClassElements;
      }
    }
  }
  return messageTraitClassElements;
}

ScAddrVector MessageTopicClassifier::getMessageEntity(ScAddr const & messageAddr, json const & witResponse)
{
  ScAddrVector messageEntitiesElements;

  json const messageEntity = getMessageEntities(witResponse);
  if (!messageEntity.empty())
  {
    ScIterator3Ptr possibleEntityIterator = context->CreateIterator3(
        MessageClassificationKeynodes::concept_entity_possible_class,
        ScType::EdgeAccessConstPosPerm,
        ScType::NodeConstClass);

    messageEntitiesElements = processEntities(possibleEntityIterator, messageEntity, messageAddr);
  }

  return messageEntitiesElements;
}

json MessageTopicClassifier::getMessageEntities(json const & witResponse)
{
  json messageEntity;
  try
  {
    messageEntity = witResponse.at(WitAiConstants::entities);
  }
  catch (...)
  {
    SC_LOG_WARNING("MessageTopicClassifier: Message entities are not found.");
  }

  return messageEntity;
}

ScAddr MessageTopicClassifier::findEntityByIdtf(std::string const & idtf, ScType const & entityType)
{
  ScAddrVector links = context->FindLinksByContent(idtf);
  for (ScAddr const & link : links)
  {
    for (ScAddr const & idtfRelation : relationsToIdtf)
    {
      ScIterator5Ptr entityByIdtfIterator =
          context->CreateIterator5(entityType, ScType::EdgeDCommonConst, link, ScType::EdgeAccessConstPosPerm, idtfRelation);
      if (entityByIdtfIterator->Next())
      {
        return entityByIdtfIterator->Get(0);
      }
    }
  };

  return {};
}

bool MessageTopicClassifier::processAsFoundEntity(
    std::string const & entityIdtf,
    ScAddr const & entityRoleAddr,
    ScAddr const & messageAddr,
    ScAddrVector & result)
{
  ScAddr entityAddr = findEntityByIdtf(entityIdtf);

  if (!entityAddr.IsValid() || !entityRoleAddr.IsValid())
    return false;

  ScAddr const messageEntityEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, messageAddr, entityAddr);
  ScAddr const messageEntityRoleEdge =
      context->CreateEdge(ScType::EdgeAccessConstPosPerm, entityRoleAddr, messageEntityEdge);

  result.insert(result.end(), {entityAddr, messageEntityEdge, entityRoleAddr, messageEntityRoleEdge});
  return true;
}

void MessageTopicClassifier::processAsNotFoundEntity(
    std::string const & entityIdtf,
    ScAddr const & entityRoleAddr,
    ScAddr const & messageAddr,
    ScAddrVector & result)
{
  ScAddr const createdEntity = context->CreateLink();
  context->SetLinkContent(createdEntity, entityIdtf);
  // ScAddr const createdEntityEdge =
  //     context->CreateEdge(ScType::EdgeAccessConstPosPerm, commonModule::LocalKeynodes::lang_en, createdEntity);
  ScAddr const messageEntityEdge = context->CreateEdge(ScType::EdgeAccessConstPosPerm, messageAddr, createdEntity);
  ScAddr const messageEntityRoleEdge =
      context->CreateEdge(ScType::EdgeAccessConstPosPerm, entityRoleAddr, messageEntityEdge);

  result.insert(result.end(), {createdEntity, messageEntityEdge, messageEntityRoleEdge, entityRoleAddr});
}

ScAddrVector MessageTopicClassifier::processEntities(
    ScIterator3Ptr & possibleEntityIterator,
    json const & messageEntity,
    ScAddr const & messageAddr)
{
  ScAddrVector messageEntitiesElements;

  for (auto const & [key, value] : messageEntity.items())
  {
    std::string entityRoleIdtf = key.substr(key.find(':') + 1);
    ScAddr entityRoleAddr = findEntityByIdtf(entityRoleIdtf, ScType::NodeConstRole);
    if (!entityRoleAddr.IsValid())
    {
      SC_LOG_WARNING("MessageTopicClassifier: not found " << entityRoleIdtf << " relation");
      entityRoleAddr = context->HelperResolveSystemIdtf(entityRoleIdtf, ScType::NodeConstRole);
      SC_LOG_DEBUG("MessageTopicClassifier: role relation " << entityRoleIdtf << " created");
    }

    std::set<std::string> processedIdtfs;

    for (auto const & valueItem : value)
    {
      std::string entityIdtf = valueItem.at(WitAiConstants::value);

      if (processedIdtfs.find(entityIdtf) != processedIdtfs.end())
        continue;
      processedIdtfs.insert(entityIdtf);

      bool isEntityFound = processAsFoundEntity(entityIdtf, entityRoleAddr, messageAddr, messageEntitiesElements);
      if (!isEntityFound)
      {
        SC_LOG_DEBUG("MessageTopicClassifier: not found " << entityIdtf << " entity with role " << entityRoleIdtf);
        processAsNotFoundEntity(entityIdtf, entityRoleAddr, messageAddr, messageEntitiesElements);

        SC_LOG_DEBUG(
            "MessageTopicClassifier: generated " << entityIdtf << " entity with " << entityRoleIdtf << " role");
        continue;
      }
      SC_LOG_DEBUG("MessageTopicClassifier: found " << entityIdtf << " entity with role " << entityRoleIdtf);
    }
  }

  return messageEntitiesElements;
}

}  // namespace messageClassificationModule

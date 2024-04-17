#pragma once

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

#include "client/WitAiClientInterface.hpp"
#include "searcher/MessageSearcher.hpp"
#include "keynodes/MessageClassificationKeynodes.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

namespace messageClassificationModule
{
class MessageTopicClassifier
{
public:
  explicit MessageTopicClassifier(ScMemoryContext * context, WitAiClientInterface * client);

  ScAddrVector classifyMessage(ScAddr const & messageAddr);

private:
  ScMemoryContext * context;

  std::unique_ptr<MessageSearcher> messageSearcher;

  std::unique_ptr<WitAiClientInterface> client;

  ScAddrVector const relationsToIdtf = {
      scAgentsCommon::CoreKeynodes::nrel_idtf,
      scAgentsCommon::CoreKeynodes::nrel_main_idtf,
      scAgentsCommon::CoreKeynodes::nrel_system_identifier,
      MessageClassificationKeynodes::nrel_wit_ai_idtf};

  std::string getMessageText(ScAddr const & messageAddr);

  ScAddrVector getMessageIntentClass(ScAddr const & messageAddr, json const & witResponse);

  static std::string getMessageIntent(json const & witResponse);

  std::vector<std::string> getWitAiIdtfs(ScAddr const & messageClass);

  ScAddrVector getMessageTraitClass(ScAddr const & messageClass, json const & witResponse);

  static json getMessageTrait(json const & witResponse);

  static void buildTraitTemplate(ScTemplate & traitTemplate, ScAddr const & possibleMessageCLass);

  ScAddrVector processTraits(
      ScIterator3Ptr & possibleTraitIterator,
      json const & messageTrait,
      ScAddrVector & messageTraitClassElements,
      ScAddr const & messageAddr);

  ScAddrVector getMessageEntity(ScAddr const & messageAddr, json const & witResponse);

  static json getMessageEntities(json const & witResponse);

  ScAddrVector processEntities(
      ScIterator3Ptr & possibleEntityIterator,
      json const & messageEntity,
      ScAddr const & messageAddr);

  ScAddr findEntityByIdtf(std::string const & idtf, ScType const & entityType = ScType::Unknown);

  bool processAsFoundEntity(
      std::string const & entityIdtf,
      ScAddr const & entityRoleAddr,
      ScAddr const & messageAddr,
      ScAddrVector & result);

  void processAsNotFoundEntity(
      std::string const & entityIdtf,
      ScAddr const & entityRoleAddr,
      ScAddr const & messageAddr,
      ScAddrVector & result);
};

}  // namespace messageClassificationModule

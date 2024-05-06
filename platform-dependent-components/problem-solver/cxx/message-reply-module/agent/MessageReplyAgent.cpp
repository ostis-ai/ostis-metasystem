#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "keynodes/MessageReplyKeynodes.hpp"
#include "MessageReplyAgent.hpp"

using namespace messageReplyModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(MessageReplyAgent)
{
  ScAddr actionAddr = otherAddr;
  if (!checkActionClass(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("MessageReplyAgent started");

  ScAddr linkAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, actionAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr processingProgramAddr = getMessageProcessingProgram();
  if (!processingProgramAddr.IsValid())
  {
    SC_LOG_ERROR("Message processing program not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  if (!linkIsValid(linkAddr))
  {
    SC_LOG_ERROR("Message link not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  ScAddr messageAddr;
  try
  {
    messageAddr = generateMessage(linkAddr);
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description() << exception.Message());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }
  ScAddrVector argsVector = {processingProgramAddr, generateNonAtomicActionArgsSet(messageAddr)};

  ScAddr answerAddr;
  if (!utils::AgentUtils::applyAction(
          &m_memoryCtx, MessageReplyKeynodes::action_interpret_non_atomic_action, argsVector, WAIT_TIME))
  {
    SC_LOG_ERROR("Action wait time expired or action not finished successfully");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  try
  {
    answerAddr = generateAnswer(messageAddr);
    utils::GenerationUtils::generateRelationBetween(&m_memoryCtx, actionAddr, answerAddr, scAgentsCommon::CoreKeynodes::nrel_answer);
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description() << exception.Message());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  SC_LOG_DEBUG("MessageReplyAgent finished");
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}

bool MessageReplyAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(
      MessageReplyKeynodes::action_reply_to_message, actionAddr, ScType::EdgeAccessConstPosPerm);
}

ScAddr MessageReplyAgent::getMessageProcessingProgram()
{
  return MessageReplyKeynodes::message_processing_program;
}

ScAddr MessageReplyAgent::generateMessage(ScAddr const & linkAddr)
{
  std::string const USER_MESSAGE_ALIAS = "_user_message";
  std::string const TRANSLATION_NODE_ALIAS = "_translation_node";

  ScTemplate userMessageTemplate;
  userMessageTemplate.Triple(
      MessageReplyKeynodes::concept_message, ScType::EdgeAccessVarPosPerm, ScType::NodeVar >> USER_MESSAGE_ALIAS);
  userMessageTemplate.Quintuple(
      ScType::NodeVar >> TRANSLATION_NODE_ALIAS,
      ScType::EdgeDCommonVar,
      USER_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      CoreKeynodes::nrel_sc_text_translation);
  userMessageTemplate.Triple(TRANSLATION_NODE_ALIAS, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateGenResult templateGenResult;
  if (!m_memoryCtx.HelperGenTemplate(userMessageTemplate, templateGenResult))
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Unable to generate message");
  }

  return templateGenResult[USER_MESSAGE_ALIAS];
}

ScAddr MessageReplyAgent::generateNonAtomicActionArgsSet(ScAddr const & messageAddr)
{
  std::string const ARGS_SET_ALIAS = "_args_set";

  ScTemplate argsSetTemplate;
  argsSetTemplate.Quintuple(
      ScType::NodeVar >> ARGS_SET_ALIAS,
      ScType::EdgeAccessVarPosPerm,
      messageAddr,
      ScType::EdgeAccessVarPosPerm,
      scAgentsCommon::CoreKeynodes::rrel_1);
  ScTemplateGenResult templateGenResult;
  if (!m_memoryCtx.HelperGenTemplate(argsSetTemplate, templateGenResult))
  {
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Unable to generate arguments set for interpreter agent action");
  }
  return templateGenResult[ARGS_SET_ALIAS];
}

ScAddr MessageReplyAgent::generateAnswer(ScAddr const & messageAddr)
{
  std::string const REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS = "_reply_message_relation_pair_arc";
  std::string const REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS = "_reply_message_relation_access_arc";
  std::string const REPLY_MESSAGE_ALIAS = "_reply_message";
  std::string const ANSWER_ALIAS = "_answer";

  ScTemplate replySearchTemplate;
  replySearchTemplate.Quintuple(
      messageAddr,
      ScType::EdgeDCommonVar >> REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS,
      ScType::NodeVar >> REPLY_MESSAGE_ALIAS, // TODO: check why there was LinkVar
      ScType::EdgeAccessVarPosPerm >> REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS,
      MessageReplyKeynodes::nrel_reply);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(replySearchTemplate, searchResult);
  if (searchResult.Size() != 1)
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidState, "Reply message not generated.");
  }

  ScTemplate answerGenerationTemplate;
  answerGenerationTemplate.Triple(ScType::NodeVarStruct >> ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, messageAddr);
  answerGenerationTemplate.Triple(
      ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_RELATION_PAIR_ARC_ALIAS]);
  answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_ALIAS]);
  answerGenerationTemplate.Triple(
      ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, searchResult[0][REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS]);
  answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, MessageReplyKeynodes::nrel_reply);

  ScAddrVector classes;
  ScAddrVector classesArcs;
  ScIterator3Ptr classesIt = m_memoryCtx.Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, messageAddr);

  while (classesIt->Next())
  {
    classes.emplace_back(classesIt->Get(0));
    classesArcs.emplace_back(classesIt->Get(1));
  }

  for (size_t i = 0; i < classes.size(); i++)
  {
    answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, classes.at(i));
    answerGenerationTemplate.Triple(ANSWER_ALIAS, ScType::EdgeAccessVarPosPerm, classesArcs.at(i));
  }

  ScTemplateGenResult templateGenResult;
  if (!m_memoryCtx.HelperGenTemplate(answerGenerationTemplate, templateGenResult))
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidState, "Unable to generate answer.");
  }
  return templateGenResult[ANSWER_ALIAS];
}

bool MessageReplyAgent::linkIsValid(ScAddr const & linkAddr)
{
  if (!utils::CommonUtils::checkType(&m_memoryCtx, linkAddr, ScType::LinkConst))
  {
    SC_LOG_ERROR("LinkAddr is not LinkConst.");
    return false;
  }

  ScIterator3Ptr const conceptTextFileIterator = m_memoryCtx.Iterator3(
      MessageReplyKeynodes::concept_text_file, ScType::EdgeAccessConstPosPerm, linkAddr);
  if (!conceptTextFileIterator->Next())
  {
    SC_LOG_ERROR("LinkAddr doesn't belong concept_text_file class.");
    return false;
  }
  if (conceptTextFileIterator->Next())
  {
    SC_LOG_ERROR("LinkAddr belongs concept_text_file class multiple times.");
    return false;
  }

  SC_LOG_DEBUG("Text link is found");
  return true;
}

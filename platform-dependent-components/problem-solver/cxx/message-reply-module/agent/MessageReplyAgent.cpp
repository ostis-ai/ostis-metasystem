#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
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
  catch (std::runtime_error & exception)
  {
    SC_LOG_ERROR(exception.what());
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

    ScTemplate replySearchTemplate;
    replySearchTemplate.Quintuple(
        messageAddr,
        ScType::EdgeDCommonVar,
        ScType::NodeVar >> "_reply_message",
        ScType::EdgeAccessVarPosPerm,
        MessageReplyKeynodes::nrel_reply);
    ScTemplateSearchResult searchResult;
    m_memoryCtx.HelperSearchTemplate(replySearchTemplate, searchResult);

    ScTemplate answerGenTemplate;
    answerGenTemplate.Quintuple(
        actionAddr,
        ScType::EdgeDCommonVar,
        answerAddr,
        ScType::EdgeAccessVarPosPerm,
        scAgentsCommon::CoreKeynodes::nrel_answer);
    ScTemplateGenResult genResult;
    m_memoryCtx.HelperGenTemplate(answerGenTemplate, genResult);
    if (genResult.Size() != 1) 
      throw std::runtime_error("");
  }
  catch (std::runtime_error & exception)
  {
    SC_LOG_ERROR(exception.what());
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
    throw std::runtime_error("Unable to generate message");
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
    throw std::runtime_error("Unable to generate arguments set for interpreter agent action");
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
      ScType::LinkVar >> REPLY_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm >> REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS,
      MessageReplyKeynodes::nrel_reply);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(replySearchTemplate, searchResult);
  if (searchResult.Size() != 1)
  {
    throw std::runtime_error("Reply message not generated.");
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
    throw std::runtime_error("Unable to generate answer.");
  }
  return templateGenResult[ANSWER_ALIAS];
}

bool MessageReplyAgent::linkIsValid(ScAddr const & linkAddr)
{
  if (utils::CommonUtils::checkType(&m_memoryCtx, linkAddr, ScType::LinkConst))
  {
    bool isTextValid = textLinkIsValid(linkAddr);
    if (isTextValid)
    {
      SC_LOG_DEBUG("Text link is found");
      return true;
    }
  }
  return false;
}

bool MessageReplyAgent::textLinkIsValid(ScAddr const & linkAddr)
{
  ScTemplate textLinkTemplate;
  textLinkTemplate.Triple(MessageReplyKeynodes::concept_text_file, ScType::EdgeAccessVarPosPerm, linkAddr);

  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(textLinkTemplate, searchResult);
  return searchResult.Size() == 1;
}

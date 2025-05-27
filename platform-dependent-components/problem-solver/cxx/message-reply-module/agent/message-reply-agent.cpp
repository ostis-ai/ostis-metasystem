/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
 
#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "sc-memory/sc_action.hpp"
#include "sc-memory/sc_addr.hpp"

#include "sc-memory/sc_keynodes.hpp"
#include "keynodes/message_reply_keynodes.hpp"
#include "message-reply-agent.hpp"

using namespace messageReplyModule;

ScResult MessageReplyAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [linkAddr] = action.GetArguments<1>();

  SC_LOG_DEBUG("MessageReplyAgent started");

  ScAddr processingProgramAddr = getMessageProcessingProgram();
  if (!processingProgramAddr.IsValid())
  {
    SC_LOG_ERROR("Message processing program not found.");
    return action.FinishWithError();
  }
  if (!linkIsValid(linkAddr))
  {
    SC_LOG_ERROR("Message link not found.");
    return action.FinishWithError();
  }

  ScAddr messageAddr;
  try
  {
    messageAddr = generateMessage(linkAddr);
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description() << exception.Message());
    return action.FinishWithError();
  }
  //ScAddrVector argsVector = {processingProgramAddr, generateNonAtomicActionArgsSet(messageAddr)};

  SC_LOG_DEBUG("here");

  ScAddr answerAddr;
  // ScAction nonAtomicAction = m_context.GenerateAction(MessageReplyKeynodes::action_interpret_non_atomic_action);
  ScAction nonAtomicAction = m_context.GenerateAction(MessageReplyKeynodes::action_interpret_non_atomic_action);
  SC_LOG_DEBUG("te");
  nonAtomicAction.SetArguments(processingProgramAddr, generateNonAtomicActionArgsSet(messageAddr));
  SC_LOG_DEBUG("ty");
  if (!nonAtomicAction.InitiateAndWait(WAIT_TIME))
  {
    SC_LOG_ERROR("Action wait time expired or action not finished successfully");
    return action.FinishWithError();
  }


  SC_LOG_DEBUG("there");

  try
  {
    answerAddr = generateAnswer(messageAddr);
    utils::GenerationUtils::generateRelationBetween(&m_context, action, answerAddr, ScKeynodes::nrel_result);
  }
  catch (utils::ScException & exception)
  {
    SC_LOG_ERROR(exception.Description() << exception.Message());
    return action.FinishWithError();
  }

  SC_LOG_DEBUG("MessageReplyAgent finished");
  return action.FinishSuccessfully();
}

bool MessageReplyAgent::checkActionClass(ScAddr const & actionAddr)
{
  return m_context.CheckConnector(MessageReplyKeynodes::action_reply_to_message, actionAddr, ScType::EdgeAccessConstPosPerm);
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
      MessageReplyKeynodes::nrel_sc_text_translation);
  userMessageTemplate.Triple(TRANSLATION_NODE_ALIAS, ScType::EdgeAccessVarPosPerm, linkAddr);
  ScTemplateGenResult templateGenResult;
  if (!m_context.HelperGenTemplate(userMessageTemplate, templateGenResult))
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
      ScKeynodes::rrel_1);
  ScTemplateGenResult templateGenResult;
  if (!m_context.HelperGenTemplate(argsSetTemplate, templateGenResult))
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
      ScType::LinkVar >> REPLY_MESSAGE_ALIAS,
      ScType::EdgeAccessVarPosPerm >> REPLY_MESSAGE_RELATION_ACCESS_ARC_ALIAS,
      MessageReplyKeynodes::nrel_reply);
  ScTemplateSearchResult searchResult;
  m_context.HelperSearchTemplate(replySearchTemplate, searchResult);
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
  ScIterator3Ptr classesIt = m_context.CreateIterator3(ScType::NodeConstClass, ScType::ConstPermPosArc, messageAddr);

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
  if (!m_context.HelperGenTemplate(answerGenerationTemplate, templateGenResult))
  {
    SC_THROW_EXCEPTION(utils::ExceptionInvalidState, "Unable to generate answer.");
  }
  return templateGenResult[ANSWER_ALIAS];
}

bool MessageReplyAgent::linkIsValid(ScAddr const & linkAddr)
{
  if (!utils::CommonUtils::checkType(&m_context, linkAddr, ScType::LinkConst))
  {
    SC_LOG_ERROR("LinkAddr is not LinkConst.");
    return false;
  }

  ScIterator3Ptr const conceptTextFileIterator = m_context.CreateIterator3(
      MessageReplyKeynodes::concept_text_file, ScType::ConstPermPosArc, linkAddr);
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

ScAddr MessageReplyAgent::GetActionClass() const
{
  return MessageReplyKeynodes::action_reply_to_message;
}

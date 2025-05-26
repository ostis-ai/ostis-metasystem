/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-memory/sc_keynodes.hpp"

#include "keynodes/message_reply_keynodes.hpp"
#include "GenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

ScResult GenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  if (!m_context.HelperCheckEdge(
        messageReplyModule::MessageReplyKeynodes::action_interpret_non_atomic_action, action, ScType::EdgeAccessConstPosPerm))
  {
      return action.FinishSuccessfully();
  }
  SC_LOG_DEBUG("GenerateMessageReplyAgent started");
  if(!actionIsValid(action))
  {
      return action.FinishSuccessfully();
  }

  ScAddr argsSet = utils::IteratorUtils::getAnyByOutRelation(
        & m_context,
        action,
        ScKeynodes::rrel_2);
  ScAddr messageAddr = utils::IteratorUtils::getAnyByOutRelation(
        & m_context,
        argsSet,
        ScKeynodes::rrel_1);

  ScTemplate scTemplate;
  scTemplate.Quintuple(
      messageAddr,
      ScType::EdgeDCommonVar,
      ScType::NodeVar,
      ScType::EdgeAccessVarPosPerm,
      messageReplyModule::MessageReplyKeynodes::nrel_reply);
  ScTemplateParams templateParams;
  ScTemplateGenResult templateGenResult;
  m_context.HelperGenTemplate(scTemplate, templateGenResult, templateParams);
  SC_LOG_DEBUG("GenerateMessageReplyAgent finished");
  return action.FinishSuccessfully();
}

bool GenerateReplyMessageAgent::actionIsValid(const ScAddr & actionAddr)
{
  ScTemplate scTemplate;
  scTemplate.Quintuple(
        actionAddr,
        ScType::EdgeAccessVarPosPerm,
        messageReplyModule::MessageReplyKeynodes::message_processing_program,
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  scTemplate.Quintuple(
        actionAddr,
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_args_set",
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_2);
  scTemplate.Quintuple(
        "_args_set",
        ScType::EdgeAccessVarPosPerm,
        ScType::NodeVar >> "_message",
        ScType::EdgeAccessVarPosPerm,
        ScKeynodes::rrel_1);
  scTemplate.Triple(
        messageReplyModule::MessageReplyKeynodes::concept_message,
        ScType::EdgeAccessVarPosPerm,
        "_message");
  ScTemplateSearchResult searchResult;
  m_context.HelperSearchTemplate(scTemplate, searchResult);
  return searchResult.Size() == 1;
}



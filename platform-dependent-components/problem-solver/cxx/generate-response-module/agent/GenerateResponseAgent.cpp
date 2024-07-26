/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "keynodes/Keynodes.hpp"
#include "constants/Constants.hpp"

#include "GenerateResponseAgent.hpp"

using namespace generateResponseModule;

SC_AGENT_IMPLEMENTATION(GenerateResponseAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG(Constants::generateAnswerAgentClassName + " started");

  ScAddr const & messageAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, otherAddr, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr const & answerAddr =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, otherAddr, scAgentsCommon::CoreKeynodes::rrel_2);

  ScAddr messageAnswer;
  ScAddr const actionToApply = createActionNode(messageAddr);
  try
  {
    validateAddrWithInvalidParamException(messageAddr, Constants::messageAddrParamName);
    validateAddrWithInvalidParamException(answerAddr, Constants::answerAddrParamName);
    messageAnswer = utils::AgentUtils::applyActionAndGetResultIfExists(&m_memoryCtx, actionToApply);
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(Constants::generateAnswerAgentClassName << ": " << exception.Message());
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, false);
    return SC_RESULT_ERROR;
  }

  attachAnswer(messageAnswer, messageAddr, answerAddr);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  SC_LOG_DEBUG(Constants::generateAnswerAgentClassName << " finished");
  return SC_RESULT_OK;
}

void GenerateResponseAgent::validateAddrWithInvalidParamException(ScAddr const & addr, std::string const & paramName)
{
  SC_CHECK_PARAM(addr, "parameter '" + paramName + "' is not valid");
}

bool GenerateResponseAgent::checkAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(Keynodes::action_generate_response, actionAddr, ScType::EdgeAccessConstPosPerm);
}

void GenerateResponseAgent::attachAnswer(
    ScAddr const & messageAnswer,
    ScAddr const & messageAddr,
    ScAddr const & answerAddr)
{
  utils::GenerationUtils::generateRelationBetween(
      &m_memoryCtx, messageAddr, messageAnswer, Keynodes::nrel_reply_structure);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, answerAddr, messageAnswer);
}

ScAddr GenerateResponseAgent::createActionNode(ScAddr const & message)
{
  ScAddr const actionClass = FindResponseActionClass(message);
  ScAddr const actionNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionClass, actionNode);

  ScAddrSet mappedRelations;

  processParamsFromMessage(message, actionClass, actionNode, mappedRelations);

  return actionNode;
}

ScAddr GenerateResponseAgent::FindResponseActionClass(ScAddr const & message)
{
  ScAddr actionClass;

  ScTemplate responseActionTemplate;
  responseActionTemplate.Triple(
      Keynodes::concept_intent_possible_class,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVarClass >> Constants::messageClassVarName);
  responseActionTemplate.Quintuple(
      Constants::messageClassVarName,
      ScType::EdgeDCommonVar,
      ScType::NodeVar >> Constants::actionClassVarName,
      ScType::EdgeAccessVarPosPerm,
      Keynodes::nrel_response_action);
  responseActionTemplate.Triple(Constants::messageClassVarName, ScType::EdgeAccessVarPosPerm, message);

  m_memoryCtx.HelperSmartSearchTemplate(
      responseActionTemplate,
      [&actionClass](ScTemplateResultItem const & resultItem) -> ScTemplateSearchRequest
      {
        actionClass = resultItem[Constants::actionClassVarName];
        return ScTemplateSearchRequest::STOP;
      });

  if (!m_memoryCtx.IsElement(actionClass))
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "response action class not found");

  return actionClass;
}

void GenerateResponseAgent::processParamsFromMessage(
    ScAddr const & message,
    ScAddr const & action,
    ScAddr const & actionNode,
    ScAddrSet & mappedRelations)
{
  ScTemplateParams params;
  params.Add(Constants::messageVarName, message);
  params.Add(Constants::actionClassVarName, action);

  SC_LOG_DEBUG(m_memoryCtx.HelperGetSystemIdtf(action));

  ScTemplate templ;
  m_memoryCtx.HelperBuildTemplate(
      templ, m_memoryCtx.HelperFindBySystemIdtf(Constants::roleRelationMappingTemplateName), params);

  m_memoryCtx.HelperSearchTemplate(
      templ,
      [this, &actionNode, &mappedRelations](ScTemplateResultItem const & resultItem)
      {
        ScAddr const param = resultItem[Constants::paramVarName];
        ScAddr const targetRole = resultItem[Constants::targetRoleRelationVarName];

        ScAddr actionNodeToParamEdge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionNode, param);
        m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, targetRole, actionNodeToParamEdge);

        mappedRelations.insert(targetRole);
      });
}

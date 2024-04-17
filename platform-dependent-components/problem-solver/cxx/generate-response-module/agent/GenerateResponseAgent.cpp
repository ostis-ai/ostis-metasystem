/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
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
  try
  {
    validateAddrWithInvalidParamException(messageAddr, Constants::messageAddrParamName);
    validateAddrWithInvalidParamException(answerAddr, Constants::answerAddrParamName);
    messageAnswer = utils::AgentUtils::applyActionAndGetResultIfExists(&m_memoryCtx, createActionNode(messageAddr));
  }
  catch (utils::ScException const & ex)
  {
    finishWorkWithMessage(ex.Message(), actionAddr, false);
    return SC_RESULT_ERROR;
  };

  bool isSuccess = attachAnswer(messageAnswer, messageAddr, answerAddr);

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, isSuccess);
  SC_LOG_DEBUG(Constants::generateAnswerAgentClassName + " finished");
  return SC_RESULT_OK;
}

void GenerateResponseAgent::validateAddrWithItemNotFoundException(ScAddr const & addr, std::string const & message)
{
  if (addr.IsValid())
    return;

  SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, message);
}

void GenerateResponseAgent::validateAddrWithInvalidParamException(ScAddr const & addr, std::string const & paramName)
{
  SC_CHECK_PARAM(addr, "parameter '" + paramName + "' is not valid");
}

void GenerateResponseAgent::finishWorkWithMessage(
    std::string const & message,
    ScAddr const & questionNode,
    bool isSuccess)
{
  SC_LOG_ERROR(Constants::generateAnswerAgentClassName + ": " + message);
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, isSuccess);
}

bool GenerateResponseAgent::checkAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(Keynodes::action_generate_response, actionAddr, ScType::EdgeAccessConstPosPerm);
}

bool GenerateResponseAgent::attachAnswer(
    ScAddr const & messageAnswer,
    ScAddr const & messageAddr,
    ScAddr const & answerAddr)
{
  if (!messageAnswer.IsValid())
    return false;

  ScAddr const & edgeBetweenMessageAndMessageAnswer =
      m_memoryCtx.CreateEdge(ScType::EdgeDCommonConst, messageAddr, messageAnswer);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::nrel_reply, edgeBetweenMessageAndMessageAnswer);

  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, answerAddr, messageAnswer);

  return true;
}

ScAddr GenerateResponseAgent::createActionNode(ScAddr const & message)
{
  ScAddr const action = findResponseAction(message);

  ScAddr const actionNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, action, actionNode);

  ScAddrSet mappedRelations;

  processParamsFromMessage(message, action, actionNode, mappedRelations);

  processParamsWithDefaultArgValue(action, actionNode, mappedRelations);

  return actionNode;
}

ScAddr GenerateResponseAgent::findResponseAction(ScAddr const & message)
{
  ScTemplateParams params;
  params.Add(Constants::messageVarName, message);
  ScTemplate templ;
  m_memoryCtx.HelperBuildTemplate(
      templ, m_memoryCtx.HelperFindBySystemIdtf(Constants::findResponseActionTemplateName), params);

  ScAddr action;

  m_memoryCtx.HelperSmartSearchTemplate(
      templ,
      [&action](ScTemplateResultItem const & resultItem)
      {
        action = resultItem[Constants::actionVarName];
        return ScTemplateSearchRequest::STOP;
      });

  if (!action.IsValid())
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "response action class not found");

  return action;
}

void GenerateResponseAgent::processParamsFromMessage(
    ScAddr const & message,
    ScAddr const & action,
    ScAddr const & actionNode,
    ScAddrSet & mappedRelations)
{
  ScTemplateParams params;
  params.Add(Constants::messageVarName, message);
  params.Add(Constants::actionVarName, action);

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

void GenerateResponseAgent::processParamsWithDefaultArgValue(
    ScAddr const & action,
    ScAddr const & actionNode,
    ScAddrSet const & mappedRelations)
{
  ScTemplateParams params;
  params.Add(Constants::actionVarName, action);

  ScTemplate templ;
  m_memoryCtx.HelperBuildTemplate(
      templ, m_memoryCtx.HelperFindBySystemIdtf(Constants::roleRelationDefaultArgValueTemplateName), params);

  m_memoryCtx.HelperSearchTemplate(
      templ,
      [this, &actionNode, &mappedRelations](ScTemplateResultItem const & resultItem)
      {
        ScAddr const defaultArg = resultItem[Constants::defaultArgVarName];
        ScAddr const relation = resultItem[Constants::rrelWithDefaultArgVarName];

        if (mappedRelations.find(relation) != mappedRelations.end())
          return;

        ScAddr actionNodeToParamEdge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, actionNode, defaultArg);
        m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, relation, actionNodeToParamEdge);
      });
}

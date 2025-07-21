/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/GenerationUtils.hpp>
#include <sc-agents-common/utils/CommonUtils.hpp>
#include "sc-memory/sc_addr.hpp"

#include "keynodes/generate_response_keynodes.hpp"
#include "constants/generate_response_constants.hpp"

#include "generate-response-agent.hpp"

using namespace generateResponseModule;
 
 
ScResult GenerateResponseAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{

  SC_LOG_DEBUG(GenerateResponseConstants::generateAnswerAgentClassName + " started");

  auto [messageAddr, answerAddr] = action.GetArguments<2>();

  ScAddr messageAnswer;
  ScAction actionToApply = createActionNode(messageAddr);
  try
  {
    validateAddrWithInvalidParamException(messageAddr, GenerateResponseConstants::messageAddrParamName);
    validateAddrWithInvalidParamException(answerAddr, GenerateResponseConstants::answerAddrParamName);
    actionToApply.InitiateAndWait();
    messageAnswer = actionToApply.GetResult();
  }
  catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(GenerateResponseConstants::generateAnswerAgentClassName << ": " << exception.Message());
    return action.FinishWithError();
  }

  attachAnswer(messageAnswer, messageAddr, answerAddr);

  SC_LOG_DEBUG(GenerateResponseConstants::generateAnswerAgentClassName << " finished");
  return action.FinishSuccessfully();
}

void GenerateResponseAgent::validateAddrWithInvalidParamException(ScAddr const & addr, std::string const & paramName)
{
  SC_CHECK_PARAM(addr, "parameter '" + paramName + "' is not valid");
}

bool GenerateResponseAgent::checkAction(ScAddr const & actionAddr)
{
  return m_context.CheckConnector(GenerateResponseKeynodes::action_generate_response, actionAddr, ScType::ConstPermPosArc);
}

void GenerateResponseAgent::attachAnswer(
    ScAddr const & messageAnswer,
    ScAddr const & messageAddr,
    ScAddr const & answerAddr)
{
  utils::GenerationUtils::generateRelationBetween(
      &m_context, messageAddr, messageAnswer, GenerateResponseKeynodes::nrel_reply_structure);
      m_context.GenerateConnector(ScType::ConstPermPosArc, answerAddr, messageAnswer);
}

ScAction GenerateResponseAgent::createActionNode(ScAddr const & message)
{
  ScAddr const actionClass = FindResponseActionClass(message);

  ScAction actionNode = m_context.GenerateAction(actionClass);

  ScAddrSet mappedRelations;

  processParamsFromMessage(message, actionClass, actionNode, mappedRelations);

  return actionNode;
}

ScAddr GenerateResponseAgent::FindResponseActionClass(ScAddr const & message)
{
  ScAddr actionClass;

  ScTemplate responseActionTemplate;
  responseActionTemplate.Triple(
    GenerateResponseKeynodes::concept_intent_possible_class,
      ScType::VarPermPosArc,
      ScType::VarNodeClass >> GenerateResponseConstants::messageClassVarName);
  responseActionTemplate.Quintuple(
    GenerateResponseConstants::messageClassVarName,
      ScType::VarCommonArc,
      ScType::VarNode >> GenerateResponseConstants::actionClassVarName,
      ScType::VarPermPosArc,
      GenerateResponseKeynodes::nrel_response_action);
  responseActionTemplate.Triple(GenerateResponseConstants::messageClassVarName, ScType::VarPermPosArc, message);

  m_context.SearchByTemplateInterruptibly(
      responseActionTemplate,
      [&actionClass](ScTemplateResultItem const & resultItem) -> ScTemplateSearchRequest
      {
        actionClass = resultItem[GenerateResponseConstants::actionClassVarName];
        return ScTemplateSearchRequest::STOP;
      });

  if (!m_context.IsElement(actionClass))
    SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "response action class not found");

    SC_LOG_INFO("\n\n\n\nFound action " << m_context.GetElementSystemIdentifier(actionClass));
  return actionClass;
}

void GenerateResponseAgent::processParamsFromMessage(
    ScAddr const & message,
    ScAddr const & action,
    ScAddr const & actionNode,
    ScAddrSet & mappedRelations)
{
  ScTemplateParams params;
  params.Add(GenerateResponseConstants::messageVarName, message);
  params.Add(GenerateResponseConstants::actionClassVarName, action);

  SC_LOG_DEBUG(m_context.GetElementSystemIdentifier(action));

  ScTemplate templ;
  m_context.BuildTemplate(
      templ, m_context.SearchElementBySystemIdentifier(GenerateResponseConstants::roleRelationMappingTemplateName), params);

      m_context.SearchByTemplate(
      templ,
      [this, &actionNode, &mappedRelations](ScTemplateResultItem const & resultItem)
      {
        ScAddr const param = resultItem[GenerateResponseConstants::paramVarName];
        ScAddr const targetRole = resultItem[GenerateResponseConstants::targetRoleRelationVarName];

        ScAddr actionNodeToParamEdge = m_context.GenerateConnector(ScType::ConstPermPosArc, actionNode, param);
        m_context.GenerateConnector(ScType::ConstPermPosArc, targetRole, actionNodeToParamEdge);

        mappedRelations.insert(targetRole);
      });
}

ScAddr GenerateResponseAgent::GetActionClass() const
{
  return GenerateResponseKeynodes::action_generate_response;
}


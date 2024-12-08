/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "constants/verification_constants.hpp"
#include "keynodes/verification_keynodes.hpp"

#include "check-synonym-objects-agent.hpp"

using namespace utils;

namespace verificationModule
{

ScResult CheckSynonymObjectsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [classAddr] = action.GetArguments<1>();
  if (!m_context.IsElement(classAddr))
  {
    SC_AGENT_LOG_ERROR("CheckSynonymObjectsAgent: class not found.");
    return action.FinishUnsuccessfully();
  }

  SC_LOG_DEBUG("CheckSynonymObjectsAgent started");

  ScAddr actionClass;
  if (m_context.checktype(classAddr, ScType::sc_node_role_relation) || m_context.checktype(classAddr, ScType::sc_node_non_role_relation))
    actionClass = VerificationKeynodes::action_check_synonym_relations;
  else if (m_context.checktype(classAddr, ScType::sc_node_class))
    actionClass = VerificationKeynodes::action_check_synonym_classes;

  try
  {
    ScAddr const actionNode = m_context.CreateNode(ScType::NodeConst);
    m_context.CreateEdge(ScType::EdgeAccessConstPosPerm, actionClass, actionNode);
    m_context.generateRelationBetween(actionNode, classAddr, ScKeynodes::rrel_1)

    messageAnswer = utils::AgentUtils::applyActionAndGetResultIfExists(&m_memoryCtx, actionNode);


    return action.FinishSuccessfully();
  }
  catch (utils::ScException const & exception)
  {
    return action.FinishUnsuccessfully();
  }
}
}
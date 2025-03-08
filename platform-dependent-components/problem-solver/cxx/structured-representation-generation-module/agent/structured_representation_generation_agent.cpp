/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "keynodes/structured_representation_generation_keynodes.hpp"

#include "structured_representation_generation_agent.hpp"

using namespace structuredRepresentationGenerationModule;

SC_AGENT_IMPLEMENTATION(StructuredRepresentationGenerationAgent)
{
  ScAddr const questionNode = otherAddr;
  if (!CheckActionClass(questionNode))
    return SC_RESULT_OK;
  InitFields();

  SC_LOG_DEBUG(GetClassNameForLog() + " started");

  ScAddr const structureForProcessing =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!m_memoryCtx.IsElement(structureForProcessing))
  {
    SC_LOG_ERROR(GetClassNameForLog() + ": structure for structurisation not found");
    return SC_RESULT_ERROR;
  }

  ScAddr processingRule =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  if (!m_memoryCtx.IsElement(processingRule))
  {
    processingRule = utils::AgentUtils::applyActionAndGetResultIfExists(
        &m_memoryCtx,
        StructuredRepresentationGenerationKeynodes::action_search_rule_for_generation,
        {structureForProcessing});
    if (!m_memoryCtx.IsElement(processingRule))
    {
      SC_LOG_ERROR(GetClassNameForLog() + ": structurisation rule not found");
      return SC_RESULT_ERROR;
    }
  }

  ScAddrVector answerVector;

  try
  {
    answerVector = manager->Manage({structureForProcessing, processingRule});
  }
  catch (utils::ScException const & ex)
  {
    SC_LOG_ERROR(GetClassNameForLog() + ": " + ex.Message());
    return SC_RESULT_ERROR;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, answerVector);

  SC_LOG_DEBUG(GetClassNameForLog() + " finished");
  return SC_RESULT_OK;
}

bool StructuredRepresentationGenerationAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      StructuredRepresentationGenerationKeynodes::action_generate_structured_representation,
      actionNode,
      ScType::EdgeAccessConstPosPerm);
}

std::string StructuredRepresentationGenerationAgent::GetClassNameForLog()
{
  static std::string const className = "StructuredRepresentationGenerationAgent";
  return className;
}

void StructuredRepresentationGenerationAgent::InitFields()
{
  manager = std::make_unique<StructuredRepresentationGenerationManager>(&m_memoryCtx);
}

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "constants/structured_representation_generation_constants.hpp"
#include "keynodes/structured_representation_generation_keynodes.hpp"

#include "generation_rule_search_agent.hpp"

using namespace structuredRepresentationGenerationModule;

SC_AGENT_IMPLEMENTATION(GenerationRuleSearchAgent)
{
  ScAddr const questionNode = otherAddr;
  if (!CheckActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_DEBUG(GetClassNameForLog() + " started");

  ScAddr structNode =
      utils::IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  ScTemplate templ;
  BuildRuleSearchingTemplate(structNode, templ);

  ScAddr ruleNode;
  m_memoryCtx.HelperSmartSearchTemplate(
      templ,
      [&ruleNode](ScTemplateResultItem const & resultItem)
      {
        ruleNode = resultItem[StructuredRepresentationGenerationConstants::GENERATION_RULE_VAR_NAME];
        return ScTemplateSearchRequest::STOP;
      });

  if (m_memoryCtx.IsElement(ruleNode))
  {
    SC_LOG_DEBUG(GetClassNameForLog() + ": rule node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
  }
  else
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, {ruleNode});

  SC_LOG_DEBUG(GetClassNameForLog() + " finished");
  return SC_RESULT_OK;
}

bool GenerationRuleSearchAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      StructuredRepresentationGenerationKeynodes::action_search_rule_for_generation,
      actionNode,
      ScType::EdgeAccessConstPosPerm);
}

void GenerationRuleSearchAgent::BuildRuleSearchingTemplate(ScAddr const & structureNode, ScTemplate & templ)
{
  templ.Clear();
  templ.Triple(ScType::NodeVarClass, ScType::EdgeAccessVarPosPerm, structureNode);
  templ.Quintuple(
      structureNode,
      ScType::EdgeAccessVarPosPerm,
      ScType::NodeVarTuple >> StructuredRepresentationGenerationConstants::GENERATION_RULE_VAR_NAME,
      ScType::EdgeAccessVarPosPerm,
      StructuredRepresentationGenerationKeynodes::nrel_generation_rule);
}

std::string GenerationRuleSearchAgent::GetClassNameForLog()
{
  static std::string const className = "GenerationRuleSearchAgent";
  return className;
}

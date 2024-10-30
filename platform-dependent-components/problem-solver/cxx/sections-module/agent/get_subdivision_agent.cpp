/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-core/sc-store/sc-container/sc_struct_node.h"
#include "sc-memory/utils/sc_log.hpp"

#include "keynodes/sections_keynodes.hpp"

#include "get_subdivision_agent.hpp"
#include "constants/sections_aliases.hpp"

using namespace utils;

namespace sectionsModule
{
SC_AGENT_IMPLEMENTATION(GetSubdivisionAgent)
{
  ScAddr const questionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!CheckActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_INFO("GetSubdivisionAgent started");

  ScAddr const conceptAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);

  if (!m_memoryCtx.IsElement(conceptAddr))
  {
    SC_LOG_ERROR("GetSubdivisionAgent: concept node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  ScAddrVector subdivision = GetSubdivision(conceptAddr, SectionsKeynodes::nrel_subdividing);
  if (subdivision.size() == 2)
    subdivision = GetSubdivision(conceptAddr, SectionsKeynodes::nrel_section_decomposition);

  ScAddrVector answerElements = {subdivision};

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, answerElements, true);
  SC_LOG_INFO("GetSubdivisionAgent finished");
  return SC_RESULT_OK;
}

bool GetSubdivisionAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      SectionsKeynodes::action_get_subdivision, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddrVector GetSubdivisionAgent::GetSubdivision(ScAddr const & conceptAddr, ScAddr const & subdivisionAddr)
{
  ScAddrVector subdivision;
        subdivision.push_back(conceptAddr);
        subdivision.push_back(subdivisionAddr);

  ScTemplate subdivisionTemplate;
  subdivisionTemplate.Quintuple(
      conceptAddr,
      ScType::EdgeDCommonVar >> sections_aliases::SUBDIVISION_EDGE,
      ScType::NodeVar >> sections_aliases::SUBDIVISION_TUPLE,
      ScType::EdgeAccessVarPosPerm >> sections_aliases::EDGE,
      SectionsKeynodes::nrel_subdividing);
  ScTemplateSearchResult result;
  m_memoryCtx.HelperSearchTemplate(subdivisionTemplate, result);
  for (size_t i = 0; i < result.Size(); i++)
  {
    ScAddr tupleNode = result[i][sections_aliases::SUBDIVISION_TUPLE];
    subdivision.push_back(tupleNode);
    ScAddr subdivisionEdge = result[i][sections_aliases::SUBDIVISION_EDGE];
    subdivision.push_back(subdivisionEdge);
    ScAddr edge = result[i][sections_aliases::EDGE];
    subdivision.push_back(edge);

    auto const & subdivisionIterator = m_memoryCtx.Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

      while (subdivisionIterator->Next())
      {
        ScAddr const & arc = subdivisionIterator->Get(1);
        ScAddr const & subconcept = subdivisionIterator->Get(2);
        subdivision.push_back(arc);
        subdivision.push_back(subconcept);
      }
  }
  return subdivision;
}
}  // namespace sectionsModule

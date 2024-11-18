/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-memory/sc_addr.hpp"
#include "sc-memory/utils/sc_log.hpp"

#include "keynodes/sections_keynodes.hpp"

#include "get_subdivision_agent.hpp"

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

  ScAddr subdivisionRelationAddr;

  ScAddrVector sectionClasses = {sectionsModule::SectionsKeynodes::not_enough_formed_structure,
sectionsModule::SectionsKeynodes::removed_section,
sectionsModule::SectionsKeynodes::section,
sectionsModule::SectionsKeynodes::atomic_section,
sectionsModule::SectionsKeynodes::non_atomic_section};

  auto const & classIterator = m_memoryCtx.Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, conceptAddr);
  ScAddr conceptClass = classIterator->Get(0);

  if (std::find(sectionClasses.begin(), sectionClasses.end(), conceptClass)!=sectionClasses.end()) 
    subdivisionRelationAddr = SectionsKeynodes::nrel_section_decomposition;
  else subdivisionRelationAddr = SectionsKeynodes::nrel_subdividing;

  ScAddrVector subdivision = GetSubdivision(conceptAddr, subdivisionRelationAddr);

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

ScAddrVector GetSubdivisionAgent::GetSubdivision(ScAddr const & conceptAddr, ScAddr const & subdivisionRelationAddr)
{
  SC_LOG_DEBUG("func started");
  ScAddrVector subdivision;
        subdivision.push_back(conceptAddr);
        subdivision.push_back(subdivisionRelationAddr);

  auto const & tupleIterator = m_memoryCtx.Iterator5(conceptAddr, ScType::EdgeDCommonConst, ScType::NodeConst, ScType::EdgeAccessConstPosPerm, subdivisionRelationAddr);
  while (tupleIterator->Next())
  {
  SC_LOG_DEBUG("first cycle");
    ScAddr tupleNode = tupleIterator->Get(2);
    subdivision.push_back(tupleNode);
    ScAddr subdivisionEdge = tupleIterator->Get(3);
    subdivision.push_back(subdivisionEdge);
    ScAddr edge = tupleIterator->Get(1);
    subdivision.push_back(edge);

    auto const & subdivisionIterator = m_memoryCtx.Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::NodeConst);

      while (subdivisionIterator->Next())
      {
        SC_LOG_DEBUG("second cycle");
        ScAddr const & arc = subdivisionIterator->Get(1);
        ScAddr const & subconcept = subdivisionIterator->Get(2);
        subdivision.push_back(arc);
        subdivision.push_back(subconcept);
      }

  SC_LOG_DEBUG("func finished");
  }
  return subdivision;
}
}  // namespace sectionsModule

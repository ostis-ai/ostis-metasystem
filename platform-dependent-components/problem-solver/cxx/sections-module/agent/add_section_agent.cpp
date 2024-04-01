/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "utils/set_utils.hpp"

#include "constants/sections_aliases.hpp"
#include "generator/sections_generator.hpp"
#include "keynodes/sections_keynodes.hpp"
#include "utils/sections_utils.hpp"

#include "add_section_agent.hpp"

using namespace scAgentsCommon;
using namespace utils;

namespace sectionsModule
{
SC_AGENT_IMPLEMENTATION(AddSectionAgent)
{
  ScAddr const & questionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!CheckActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_DEBUG("AddSectionAgent started");

  ScAddr sectionNameAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr parentSectionAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);
  ScAddr langAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_3);

  if (!m_memoryCtx.IsElement(sectionNameAddr))
  {
    SC_LOG_ERROR("AddSectionAgent: section identifier link not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  if (!m_memoryCtx.IsElement(langAddr))
  {
    SC_LOG_ERROR("AddSectionAgent: lang node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }

  std::string sectionName;
  m_memoryCtx.GetLinkContent(sectionNameAddr, sectionName);
  SC_LOG_DEBUG("AddSectionAgent: new section name is \"" + sectionName + "\".");
  ScAddr sectionAddr;
  if (m_memoryCtx.IsElement(parentSectionAddr))
    sectionAddr = GenerateSection(sectionName, parentSectionAddr, langAddr);
  else
    sectionAddr = GenerateSection(sectionName, langAddr);

  if (!m_memoryCtx.IsElement(sectionAddr))
  {
    SC_LOG_ERROR("AddSectionAgent: section is not generated.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, (ScAddrVector){sectionAddr}, true);
  SC_LOG_DEBUG("AddSectionAgent finished");
  return SC_RESULT_OK;
}

bool AddSectionAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
        SectionsKeynodes::action_add_section, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddr AddSectionAgent::GenerateSection(
    std::string const & sectionName,
    ScAddr const & parentSection,
    ScAddr const & lang)
{
  ScAddr decompositionTuple = sections_utils::GetSectionDecompositionTuple(&m_memoryCtx, parentSection);
  if (!m_memoryCtx.IsElement(decompositionTuple))
    return {};

  ScAddr newSection = sections_utils::FindSectionByName(&m_memoryCtx, sectionName);
  if (!m_memoryCtx.IsElement(newSection))
    newSection = sections_generator::GenerateSection(&m_memoryCtx, sectionName, lang, true);
  else
  {
    if (m_memoryCtx.HelperCheckEdge(decompositionTuple, newSection, ScType::EdgeAccessConstPosPerm)
        || parentSection == newSection)
      return {};
  }

  ScAddr lastSection = sections_utils::GetLastSubSection(&m_memoryCtx, decompositionTuple);
  ScAddr newSectionArc = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, decompositionTuple, newSection);
  if (m_memoryCtx.IsElement(lastSection))
  {
    SC_LOG_DEBUG(
        "AddSectionAgent: last section system idtf is \"" + m_memoryCtx.HelperGetSystemIdtf(lastSection) + "\".");
    ScIterator5Ptr lastSectionIterator = m_memoryCtx.Iterator5(
          decompositionTuple,
          ScType::EdgeAccessConstPosPerm,
          lastSection,
          ScType::EdgeAccessConstPosTemp,
          SectionsKeynodes::rrel_last);
    if (lastSectionIterator->Next())
    {
      ScAddr previousSectionArc = lastSectionIterator->Get(1);
      ScAddr previousSectionTempArc = lastSectionIterator->Get(3);
      m_memoryCtx.EraseElement(previousSectionTempArc);

      utils::GenerationUtils::generateRelationBetween(
          &m_memoryCtx, previousSectionArc, newSectionArc, CoreKeynodes::nrel_basic_sequence);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, SectionsKeynodes::rrel_last, newSectionArc);
    }
  }
  else
  {
    SC_LOG_DEBUG("AddSectionAgent: added section is new.");
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::rrel_1, newSectionArc);
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, SectionsKeynodes::rrel_last, newSectionArc);
  }

  sectionsModule::SetUtils::AddToSets(&m_memoryCtx, parentSection, { SectionsKeynodes::non_atomic_section});
  sectionsModule::SetUtils::RemoveFromSets(&m_memoryCtx, parentSection, { SectionsKeynodes::atomic_section});
  sectionsModule::SetUtils::RemoveFromSets(&m_memoryCtx, newSection, { SectionsKeynodes::removed_section});
  return newSection;
}

ScAddr AddSectionAgent::GenerateSection(std::string const & sectionName, ScAddr const & lang)
{
  ScAddr section = sections_utils::FindSectionByName(&m_memoryCtx, sectionName);
  if (!m_memoryCtx.IsElement(section))
    section = sections_generator::GenerateSection(&m_memoryCtx, sectionName, lang, true);
  else
    sectionsModule::SetUtils::RemoveFromSets(&m_memoryCtx, section, { SectionsKeynodes::removed_section});
  return section;
}

}  // namespace subjectDomainModule

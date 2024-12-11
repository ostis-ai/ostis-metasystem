/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "remove_section_agent.hpp"

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/GenerationUtils.hpp>

#include "builder/sections_builder.hpp"
#include "constants/sections_aliases.hpp"
#include "keynodes/sections_keynodes.hpp"
#include "utils/set_utils.hpp"

using namespace utils;

namespace sectionsModule
{
ScResult RemoveSectionAgent::DoProgram(ScAction & action)
{
  ScAddr sectionAddr = IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_1);
  ScAddr parentSectionAddr = IteratorUtils::getAnyByOutRelation(&m_context, action, ScKeynodes::rrel_2);

  if (!m_context.IsElement(sectionAddr))
  {
    SC_AGENT_LOG_ERROR("Section node not found.");
    return action.FinishUnsuccessfully();
  }
  bool isSuccess = m_context.IsElement(parentSectionAddr) ? RemoveSection(sectionAddr, parentSectionAddr)
                                                          : RemoveSection(sectionAddr);
  if (!isSuccess)
  {
    SC_AGENT_LOG_ERROR("Error in the section deletion.");
    return action.FinishUnsuccessfully();
  }

  ScStructure result = m_context.GenerateStructure();
  result << sectionAddr;
  action.SetResult(result);
  return action.FinishSuccessfully();
}

ScAddr RemoveSectionAgent::GetActionClass() const
{
  return SectionsKeynodes::action_remove_section;
}

bool RemoveSectionAgent::RemoveSection(ScAddr const & section, ScAddr const & parentSection)
{
  SC_AGENT_LOG_DEBUG("Section system idtf is " + m_context.GetElementSystemIdentifier(section) + ".");
  SC_AGENT_LOG_DEBUG("Parent section system idtf is " + m_context.GetElementSystemIdentifier(parentSection) + ".");
  ScTemplate scTemplate;
  sections_builder::buildDecompositionTupleTemplate(scTemplate, parentSection, section);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(scTemplate, searchResult);

  if (!searchResult.IsEmpty())
  {
    HandleSection(searchResult[0], section);
    HandleParentSection(searchResult[0], parentSection);
    sectionsModule::SetUtils::AddToSets(&m_context, section, {SectionsKeynodes::removed_section});

    return true;
  }
  return false;
}

bool RemoveSectionAgent::RemoveSection(ScAddr const & section)
{
  SC_AGENT_LOG_DEBUG("Section system idtf is " << m_context.GetElementSystemIdentifier(section) << ".");
  ScTemplate scTemplate;
  sections_builder::buildDecompositionTupleTemplate(scTemplate, section);
  ScTemplateSearchResult searchResult;
  m_context.SearchByTemplate(scTemplate, searchResult);

  if (!searchResult.IsEmpty())
  {
    for (size_t i = 0; i < searchResult.Size(); i++)
    {
      ScAddr parentSection = searchResult[i][sections_aliases::PARENT_SECTION];
      SC_AGENT_LOG_DEBUG(
          "Parent section system idtf is " << m_context.GetElementSystemIdentifier(parentSection) << ".");
      HandleSection(searchResult[i], section);
      HandleParentSection(searchResult[i], parentSection);
    }
    sectionsModule::SetUtils::AddToSets(&m_context, section, {SectionsKeynodes::removed_section});

    return true;
  }
  ScAddr edge = m_context.GenerateConnector(ScType::ConstPermPosArc, SectionsKeynodes::removed_section, section);
  return m_context.IsElement(edge);
}

void RemoveSectionAgent::HandleSection(ScTemplateSearchResultItem const & searchResult, ScAddr const & section)
{
  ScAddr tuple = searchResult[sections_aliases::DECOMPOSITION_TUPLE];

  HandleNeighboringSections(tuple, section);
}

void RemoveSectionAgent::HandleParentSection(
    ScTemplateSearchResultItem const & searchResult,
    ScAddr const & parentSection)
{
  ScAddr tuple = searchResult[sections_aliases::DECOMPOSITION_TUPLE];

  if (CommonUtils::getSetPower(&m_context, tuple) == 0)
  {
    m_context.EraseElement(tuple);
    sectionsModule::SetUtils::AddToSets(&m_context, parentSection, {SectionsKeynodes::atomic_section});
    sectionsModule::SetUtils::RemoveFromSets(&m_context, parentSection, {SectionsKeynodes::non_atomic_section});
  }
}

void RemoveSectionAgent::HandleNeighboringSections(ScAddr const & tuple, ScAddr const & section)
{
  ScTemplate previousSectionTemplate;
  sections_builder::buildPreviousSectionTemplate(previousSectionTemplate, tuple, section);
  ScTemplateSearchResult previousSectionSearchResult;
  m_context.SearchByTemplate(previousSectionTemplate, previousSectionSearchResult);

  ScAddr currentSectionEdge;
  ScAddr previousSection;
  ScAddr previousSectionEdge;
  if (!previousSectionSearchResult.IsEmpty())
  {
    previousSection = previousSectionSearchResult[0][sections_aliases::SECTION_NODE];
    previousSectionEdge = previousSectionSearchResult[0][sections_aliases::PREVIOUS_SECTION_ARC];
    currentSectionEdge = previousSectionSearchResult[0][sections_aliases::SECTION_EDGE];
    SC_AGENT_LOG_DEBUG(
        "Previous neighboring section system idtf is " + m_context.GetElementSystemIdentifier(previousSection) + ".");
  }

  ScTemplate nextSectionTemplate;
  sections_builder::buildNextSectionTemplate(nextSectionTemplate, tuple, section);
  ScTemplateSearchResult nextSectionSearchResult;
  m_context.SearchByTemplate(nextSectionTemplate, nextSectionSearchResult);

  ScAddr nextSection;
  ScAddr nextSectionEdge;
  if (!nextSectionSearchResult.IsEmpty())
  {
    nextSection = nextSectionSearchResult[0][sections_aliases::SECTION_NODE];
    nextSectionEdge = nextSectionSearchResult[0][sections_aliases::NEXT_SECTION_ARC];
    currentSectionEdge = nextSectionSearchResult[0][sections_aliases::SECTION_EDGE];
    SC_AGENT_LOG_DEBUG(
        "Next neighboring section system idtf is " << m_context.GetElementSystemIdentifier(nextSection) << ".");
  }

  // If current element is the first and the last
  if (!m_context.IsElement(currentSectionEdge))
  {
    ScIterator3Ptr currentSectionEdgeIterator = m_context.CreateIterator3(tuple, ScType::ConstPermPosArc, section);
    if (currentSectionEdgeIterator->Next())
    {
      currentSectionEdge = currentSectionEdgeIterator->Get(1);
    }
  }

  m_context.EraseElement(currentSectionEdge);
  // If current element has previous and has next
  if (m_context.IsElement(previousSection) && m_context.IsElement(nextSection))
  {
    GenerationUtils::generateRelationBetween(
        &m_context, previousSectionEdge, nextSectionEdge, ScKeynodes::nrel_basic_sequence);
  }

  // If current element is the last
  if (m_context.IsElement(previousSection) && !m_context.IsElement(nextSection))
  {
    m_context.GenerateConnector(ScType::ConstTempPosArc, SectionsKeynodes::rrel_last, previousSectionEdge);
  }

  // If current element is the first
  if (!m_context.IsElement(previousSection) && m_context.IsElement(nextSection))
  {
    m_context.GenerateConnector(ScType::ConstPermPosArc, ScKeynodes::rrel_1, nextSectionEdge);
  }
}

}  // namespace sectionsModule

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "utils/set_utils.hpp"

#include "builder/subject_domain_builder.hpp"
#include "constants/subject_domain_aliases.hpp"
#include "keynodes/subject_domain_keynodes.hpp"

#include "remove_section_agent.hpp"

using namespace scAgentsCommon;
using namespace utils;

namespace subjectDomainModule
{
SC_AGENT_IMPLEMENTATION(RemoveSectionAgent)
{
  ScAddr const & questionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!CheckActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_DEBUG("RemoveSectionAgent started");

  ScAddr sectionAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr parentSectionAddr =
      IteratorUtils::getAnyByOutRelation(&m_memoryCtx, questionNode, scAgentsCommon::CoreKeynodes::rrel_2);

  if (!m_memoryCtx.IsElement(sectionAddr))
  {
    SC_LOG_ERROR("RemoveSectionAgent: section node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  bool result = m_memoryCtx.IsElement(parentSectionAddr) ? RemoveSection(sectionAddr, parentSectionAddr)
                                                         : RemoveSection(sectionAddr);
  if (!result)
  {
    SC_LOG_ERROR("RemoveSectionAgent: error in the section deletion.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, (ScAddrVector){sectionAddr}, true);

  SC_LOG_DEBUG("RemoveSectionAgent finished");
  return SC_RESULT_OK;
}

bool RemoveSectionAgent::CheckActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(
      SubjectDomainKeynodes::action_remove_section, actionNode, ScType::EdgeAccessConstPosPerm);
}

bool RemoveSectionAgent::RemoveSection(ScAddr const & section, ScAddr const & parentSection)
{
  SC_LOG_DEBUG("RemoveSectionAgent: section system idtf is " + m_memoryCtx.HelperGetSystemIdtf(section) + ".");
  SC_LOG_DEBUG(
      "RemoveSectionAgent: parent section system idtf is " + m_memoryCtx.HelperGetSystemIdtf(parentSection) + ".");
  ScTemplate scTemplate;
  subject_domain_builder::buildDecompositionTupleTemplate(scTemplate, parentSection, section);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);

  if (!searchResult.IsEmpty())
  {
    HandleSection(searchResult[0], section);
    HandleParentSection(searchResult[0], parentSection);
    subjectDomainModule::SetUtils::AddToSets(&m_memoryCtx, section, {SubjectDomainKeynodes::removed_section});

    return true;
  }
  return false;
}

bool RemoveSectionAgent::RemoveSection(ScAddr const & section)
{
  SC_LOG_DEBUG("RemoveSectionAgent: section system idtf is " + m_memoryCtx.HelperGetSystemIdtf(section) + ".");
  ScTemplate scTemplate;
  subject_domain_builder::buildDecompositionTupleTemplate(scTemplate, section);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);

  if (!searchResult.IsEmpty())
  {
    for (size_t i = 0; i < searchResult.Size(); i++)
    {
      ScAddr parentSection = searchResult[i][subject_domain_aliases::PARENT_SECTION];
      SC_LOG_DEBUG(
          "RemoveSectionAgent: parent section system idtf is " + m_memoryCtx.HelperGetSystemIdtf(parentSection) + ".");
      HandleSection(searchResult[i], section);
      HandleParentSection(searchResult[i], parentSection);
    }
    subjectDomainModule::SetUtils::AddToSets(&m_memoryCtx, section, {SubjectDomainKeynodes::removed_section});

    return true;
  }
  ScAddr edge =
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, SubjectDomainKeynodes::removed_section, section);
  return m_memoryCtx.IsElement(edge);
}

void RemoveSectionAgent::HandleSection(ScTemplateSearchResultItem const & searchResult, ScAddr const & section)
{
  ScAddr tuple = searchResult[subject_domain_aliases::DECOMPOSITION_TUPLE];

  HandleNeighboringSections(tuple, section);
}

void RemoveSectionAgent::HandleParentSection(
    ScTemplateSearchResultItem const & searchResult,
    ScAddr const & parentSection)
{
  ScAddr tuple = searchResult[subject_domain_aliases::DECOMPOSITION_TUPLE];

  if (CommonUtils::getSetPower(&m_memoryCtx, tuple) == 0)
  {
    m_memoryCtx.EraseElement(tuple);
    subjectDomainModule::SetUtils::AddToSets(&m_memoryCtx, parentSection, {SubjectDomainKeynodes::atomic_section});
    subjectDomainModule::SetUtils::RemoveFromSets(
        &m_memoryCtx, parentSection, {SubjectDomainKeynodes::non_atomic_section});
  }
}

void RemoveSectionAgent::HandleNeighboringSections(ScAddr const & tuple, ScAddr const & section)
{
  ScTemplate previousSectionTemplate;
  subject_domain_builder::buildPreviousSectionTemplate(previousSectionTemplate, tuple, section);
  ScTemplateSearchResult previousSectionSearchResult;
  m_memoryCtx.HelperSearchTemplate(previousSectionTemplate, previousSectionSearchResult);

  ScAddr currentSectionEdge;
  ScAddr previousSection;
  ScAddr previousSectionEdge;
  if (!previousSectionSearchResult.IsEmpty())
  {
    previousSection = previousSectionSearchResult[0][subject_domain_aliases::SECTION_NODE];
    previousSectionEdge = previousSectionSearchResult[0][subject_domain_aliases::PREVIOUS_SECTION_ARC];
    currentSectionEdge = previousSectionSearchResult[0][subject_domain_aliases::SECTION_EDGE];
    SC_LOG_DEBUG(
        "RemoveSectionAgent: previous neighboring section system idtf is "
        + m_memoryCtx.HelperGetSystemIdtf(previousSection) + ".");
  }

  ScTemplate nextSectionTemplate;
  subject_domain_builder::buildNextSectionTemplate(nextSectionTemplate, tuple, section);
  ScTemplateSearchResult nextSectionSearchResult;
  m_memoryCtx.HelperSearchTemplate(nextSectionTemplate, nextSectionSearchResult);

  ScAddr nextSection;
  ScAddr nextSectionEdge;
  if (!nextSectionSearchResult.IsEmpty())
  {
    nextSection = nextSectionSearchResult[0][subject_domain_aliases::SECTION_NODE];
    nextSectionEdge = nextSectionSearchResult[0][subject_domain_aliases::NEXT_SECTION_ARC];
    currentSectionEdge = nextSectionSearchResult[0][subject_domain_aliases::SECTION_EDGE];
    SC_LOG_DEBUG(
        "RemoveSectionAgent: next neighboring section system idtf is " << m_memoryCtx.HelperGetSystemIdtf(nextSection)
                                                                       << ".");
  }

  // If current element is the first and the last
  if (!m_memoryCtx.IsElement(currentSectionEdge))
  {
    ScIterator3Ptr currentSectionEdgeIterator = m_memoryCtx.Iterator3(tuple, ScType::EdgeAccessConstPosPerm, section);
    if (currentSectionEdgeIterator->Next())
    {
      currentSectionEdge = currentSectionEdgeIterator->Get(1);
    }
  }

  m_memoryCtx.EraseElement(currentSectionEdge);
  // If current element has previous and has next
  if (m_memoryCtx.IsElement(previousSection) && m_memoryCtx.IsElement(nextSection))
  {
    GenerationUtils::generateRelationBetween(
        &m_memoryCtx, previousSectionEdge, nextSectionEdge, CoreKeynodes::nrel_basic_sequence);
  }

  // If current element is the last
  if (m_memoryCtx.IsElement(previousSection) && !m_memoryCtx.IsElement(nextSection))
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, SubjectDomainKeynodes::rrel_last, previousSectionEdge);
  }

  // If current element is the first
  if (!m_memoryCtx.IsElement(previousSection) && m_memoryCtx.IsElement(nextSection))
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::rrel_1, nextSectionEdge);
  }
}

}  // namespace subjectDomainModule

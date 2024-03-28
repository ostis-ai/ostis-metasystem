/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec, Maksim Orlov
 */

#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "../../commonModule/utils/SetUtils.hpp"

#include "builder/SubjDomainBuilder.hpp"
#include "constants/SubjDomainAliases.hpp"
#include "keynodes/Keynodes.hpp"

#include "RemoveSectionAgent.hpp"

using namespace scAgentsCommon;
using namespace utils;

namespace subjectDomainModule
{
SC_AGENT_IMPLEMENTATION(RemoveSectionAgent)
{
  ScAddr questionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!checkActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_DEBUG("RemoveSectionAgent started");

  ScAddr sectionAddr = IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, questionNode,scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr parentSectionAddr = IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, questionNode,scAgentsCommon::CoreKeynodes::rrel_2);

  if (!m_memoryCtx.IsElement(sectionAddr))
  {
    SC_LOG_ERROR("RemoveSectionAgent: section node not found.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  bool result = m_memoryCtx.IsElement(parentSectionAddr) ?
        removeSection(sectionAddr, parentSectionAddr) : removeSection(sectionAddr);
  if (!result)
  {
    SC_LOG_ERROR("RemoveSectionAgent: error in the section deletion.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR_INVALID_PARAMS;
  }
  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, (ScAddrVector) { sectionAddr }, true);

  SC_LOG_DEBUG("RemoveSectionAgent finished");
  return SC_RESULT_OK;
}

bool RemoveSectionAgent::checkActionClass(ScAddr const & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(Keynodes::action_remove_section, actionNode, ScType::EdgeAccessConstPosPerm);
}

bool RemoveSectionAgent::removeSection(ScAddr const & section, ScAddr const & parentSection)
{
  SC_LOG_DEBUG("RemoveSectionAgent: section system idtf is " + m_memoryCtx.HelperGetSystemIdtf(section) + ".");
  SC_LOG_DEBUG("RemoveSectionAgent: parent section system idtf is "
               + m_memoryCtx.HelperGetSystemIdtf(parentSection) + ".");
  ScTemplate scTemplate;
  SubjDomainBuilder::buildDecompositionTupleTemplate(scTemplate, parentSection, section);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);

  if (!searchResult.IsEmpty())
  {
    handleSection(searchResult[0], section);
    handleParentSection(searchResult[0], parentSection);
    commonModule::SetUtils::addToSets(&m_memoryCtx, section, { Keynodes::removed_section });

    return true;
  }
  return false;
}

bool RemoveSectionAgent::removeSection(ScAddr const & section)
{
  SC_LOG_DEBUG("RemoveSectionAgent: section system idtf is " + m_memoryCtx.HelperGetSystemIdtf(section) + ".");
  ScTemplate scTemplate;
  SubjDomainBuilder::buildDecompositionTupleTemplate(scTemplate, section);
  ScTemplateSearchResult searchResult;
  m_memoryCtx.HelperSearchTemplate(scTemplate, searchResult);

  if (!searchResult.IsEmpty())
  {
    for (size_t i = 0; i < searchResult.Size(); i++)
    {
      ScAddr parentSection = searchResult[i][SubjDomainAliases::PARENT_SECTION];
      SC_LOG_DEBUG("RemoveSectionAgent: parent section system idtf is "
                   + m_memoryCtx.HelperGetSystemIdtf(parentSection) + ".");
      handleSection(searchResult[i], section);
      handleParentSection(searchResult[i], parentSection);
    }
    commonModule::SetUtils::addToSets(&m_memoryCtx, section, { Keynodes::removed_section });

    return true;
  }
  ScAddr edge = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::removed_section, section);
  return m_memoryCtx.IsElement(edge);
}

void RemoveSectionAgent::handleSection(ScTemplateSearchResultItem const & searchResult, ScAddr const & section)
{
  ScAddr tuple = searchResult[SubjDomainAliases::DECOMPOSITION_TUPLE];

  handleNeighboringSections(tuple, section);
}

void RemoveSectionAgent::handleParentSection(
      ScTemplateSearchResultItem const & searchResult,
      ScAddr const & parentSection)
{
  ScAddr tuple = searchResult[SubjDomainAliases::DECOMPOSITION_TUPLE];

  if (CommonUtils::getSetPower(&m_memoryCtx, tuple) == 0)
  {
    m_memoryCtx.EraseElement(tuple);
    commonModule::SetUtils::addToSets(&m_memoryCtx, parentSection, { Keynodes::atomic_section });
    commonModule::SetUtils::removeFromSets(&m_memoryCtx, parentSection, { Keynodes::non_atomic_section });
  }
}

void RemoveSectionAgent::handleNeighboringSections(ScAddr const & tuple, ScAddr const & section)
{
  ScTemplate previousSectionTemplate;
  SubjDomainBuilder::buildPreviousSectionTemplate(previousSectionTemplate, tuple, section);
  ScTemplateSearchResult previousSectionSearchResult;
  m_memoryCtx.HelperSearchTemplate(previousSectionTemplate, previousSectionSearchResult);

  ScAddr currentSectionEdge;
  ScAddr previousSection;
  ScAddr previousSectionEdge;
  if (!previousSectionSearchResult.IsEmpty())
  {
    previousSection = previousSectionSearchResult[0][SubjDomainAliases::SECTION_NODE];
    previousSectionEdge = previousSectionSearchResult[0][SubjDomainAliases::PREVIOUS_SECTION_ARC];
    currentSectionEdge = previousSectionSearchResult[0][SubjDomainAliases::SECTION_EDGE];
    SC_LOG_DEBUG("RemoveSectionAgent: previous neighboring section system idtf is "
                 + m_memoryCtx.HelperGetSystemIdtf(previousSection) + ".");
  }

  ScTemplate nextSectionTemplate;
  SubjDomainBuilder::buildNextSectionTemplate(nextSectionTemplate, tuple, section);
  ScTemplateSearchResult nextSectionSearchResult;
  m_memoryCtx.HelperSearchTemplate(nextSectionTemplate, nextSectionSearchResult);

  ScAddr nextSection;
  ScAddr nextSectionEdge;
  if (!nextSectionSearchResult.IsEmpty())
  {
    nextSection = nextSectionSearchResult[0][SubjDomainAliases::SECTION_NODE];
    nextSectionEdge = nextSectionSearchResult[0][SubjDomainAliases::NEXT_SECTION_ARC];
    currentSectionEdge = nextSectionSearchResult[0][SubjDomainAliases::SECTION_EDGE];
    SC_LOG_DEBUG("RemoveSectionAgent: next neighboring section system idtf is "
                 + m_memoryCtx.HelperGetSystemIdtf(nextSection) + ".");
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
          &m_memoryCtx,
          previousSectionEdge,
          nextSectionEdge,
          CoreKeynodes::nrel_basic_sequence);
  }

  // If current element is the last
  if (m_memoryCtx.IsElement(previousSection) && !m_memoryCtx.IsElement(nextSection))
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, Keynodes::rrel_last, previousSectionEdge);
  }

  // If current element is the first
  if (!m_memoryCtx.IsElement(previousSection) && m_memoryCtx.IsElement(nextSection))
  {
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::rrel_1, nextSectionEdge);
  }
}

}  // namespace subjDomainModule

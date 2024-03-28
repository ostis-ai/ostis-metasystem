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

#include "constants/SubjDomainAliases.hpp"
#include "generator/SubjDomainGenerator.hpp"
#include "keynodes/Keynodes.hpp"
#include "utils/SubjDomainUtils.hpp"

#include "AddSectionAgent.hpp"


using namespace scAgentsCommon;
using namespace utils;

namespace subjectDomainModule
{
SC_AGENT_IMPLEMENTATION(AddSectionAgent)
{
  ScAddr const & questionNode = m_memoryCtx.GetEdgeTarget(edgeAddr);
  if (!checkActionClass(questionNode))
    return SC_RESULT_OK;

  SC_LOG_DEBUG("AddSectionAgent started");

  ScAddr sectionNameAddr = IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, questionNode,scAgentsCommon::CoreKeynodes::rrel_1);
  ScAddr parentSectionAddr = IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, questionNode,scAgentsCommon::CoreKeynodes::rrel_2);
  ScAddr langAddr = IteratorUtils::getAnyByOutRelation(
        &m_memoryCtx, questionNode,scAgentsCommon::CoreKeynodes::rrel_3);

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
    sectionAddr = generateSection(sectionName, parentSectionAddr, langAddr);
  else
    sectionAddr = generateSection(sectionName, langAddr);

  if (!m_memoryCtx.IsElement(sectionAddr))
  {
    SC_LOG_ERROR("AddSectionAgent: section is not generated.");
    utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, false);
    return SC_RESULT_ERROR;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, questionNode, (ScAddrVector) { sectionAddr }, true);
  SC_LOG_DEBUG("AddSectionAgent finished");
  return SC_RESULT_OK;
}

bool AddSectionAgent::checkActionClass(const ScAddr & actionNode)
{
  return m_memoryCtx.HelperCheckEdge(Keynodes::action_add_section, actionNode, ScType::EdgeAccessConstPosPerm);
}

ScAddr AddSectionAgent::generateSection(
      std::string const & sectionName,
      ScAddr const & parentSection,
      ScAddr const & lang)
{
  ScAddr decompositionTuple = SubjDomainUtils::getSectionDecompositionTuple(&m_memoryCtx, parentSection);
  if (!m_memoryCtx.IsElement(decompositionTuple))
    return {};

  ScAddr newSection = SubjDomainUtils::findSectionByName(&m_memoryCtx, sectionName);
  if (!m_memoryCtx.IsElement(newSection))
    newSection = SubjDomainGenerator::generateSection(&m_memoryCtx, sectionName, lang, true);
  else
  {
    if (m_memoryCtx.HelperCheckEdge(decompositionTuple, newSection, ScType::EdgeAccessConstPosPerm) ||
        parentSection == newSection)
      return {};
  }

  ScAddr lastSection = SubjDomainUtils::getLastSubSection(&m_memoryCtx, decompositionTuple);
  ScAddr newSectionArc = m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, decompositionTuple, newSection);
  if (m_memoryCtx.IsElement(lastSection))
  {
    SC_LOG_DEBUG("AddSectionAgent: last section system idtf is \"" +
                 m_memoryCtx.HelperGetSystemIdtf(lastSection) + "\".");
    ScIterator5Ptr lastSectionIterator = m_memoryCtx.Iterator5(
          decompositionTuple,
          ScType::EdgeAccessConstPosPerm,
          lastSection,
          ScType::EdgeAccessConstPosTemp,
          Keynodes::rrel_last
    );
    if (lastSectionIterator->Next())
    {
      ScAddr previousSectionArc = lastSectionIterator->Get(1);
      ScAddr previousSectionTempArc = lastSectionIterator->Get(3);
      m_memoryCtx.EraseElement(previousSectionTempArc);

      utils::GenerationUtils::generateRelationBetween(
            &m_memoryCtx,
            previousSectionArc,
            newSectionArc,
            CoreKeynodes::nrel_basic_sequence);
      m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, Keynodes::rrel_last, newSectionArc);
    }
  }
  else
  {
    SC_LOG_DEBUG("AddSectionAgent: added section is new.");
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosPerm, CoreKeynodes::rrel_1, newSectionArc);
    m_memoryCtx.CreateEdge(ScType::EdgeAccessConstPosTemp, Keynodes::rrel_last, newSectionArc);
  }

  commonModule::SetUtils::addToSets(&m_memoryCtx, parentSection, { Keynodes::non_atomic_section });
  commonModule::SetUtils::removeFromSets(&m_memoryCtx, parentSection, { Keynodes::atomic_section });
  commonModule::SetUtils::removeFromSets(&m_memoryCtx, newSection, { Keynodes::removed_section });
  return newSection;
}

ScAddr AddSectionAgent::generateSection(std::string const & sectionName, ScAddr const & lang)
{
  ScAddr section = SubjDomainUtils::findSectionByName(&m_memoryCtx, sectionName);
  if (!m_memoryCtx.IsElement(section))
    section = SubjDomainGenerator::generateSection(&m_memoryCtx, sectionName, lang, true);
  else
    commonModule::SetUtils::removeFromSets(&m_memoryCtx, section, { Keynodes::removed_section });
  return section;
}

}  // namespace subjDomainModule

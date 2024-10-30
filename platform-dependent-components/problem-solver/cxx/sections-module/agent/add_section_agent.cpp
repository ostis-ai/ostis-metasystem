/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/GenerationUtils.hpp"

#include "utils/set_utils.hpp"

#include "constants/sections_aliases.hpp"
#include "generator/sections_generator.hpp"
#include "keynodes/sections_keynodes.hpp"
#include "utils/sections_utils.hpp"

#include "add_section_agent.hpp"

using namespace utils;

namespace sectionsModule
{
ScResult AddSectionAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto const [sectionNameAddr, parentSectionAddr, langAddr] = action.GetArguments<3>();
  if (!m_context.IsElement(sectionNameAddr))
  {
    SC_AGENT_LOG_ERROR("Section identifier link not found.");
    return action.FinishUnsuccessfully();
  }
  if (!m_context.IsElement(langAddr))
  {
    SC_AGENT_LOG_ERROR("Lang node not found.");
    return action.FinishUnsuccessfully();
  }

  std::string sectionName;
  m_context.GetLinkContent(sectionNameAddr, sectionName);
  SC_AGENT_LOG_DEBUG(R"(New section name is ")" << sectionName << R"(".)");
  ScAddr sectionAddr;
  if (m_context.IsElement(parentSectionAddr))
    sectionAddr = GenerateSection(sectionName, parentSectionAddr, langAddr);
  else
    sectionAddr = GenerateSection(sectionName, langAddr);

  if (!m_context.IsElement(sectionAddr))
  {
    SC_AGENT_LOG_ERROR("Section is not generated.");
    return action.FinishUnsuccessfully();
  }
  ScStructure result = m_context.GenerateStructure();
  result << sectionAddr;
  action.SetResult(result);
  return action.FinishSuccessfully();
}

ScAddr AddSectionAgent::GetActionClass() const
{
  return SectionsKeynodes::action_add_section;
}

ScAddr AddSectionAgent::GenerateSection(
    std::string const & sectionName,
    ScAddr const & parentSection,
    ScAddr const & lang)
{
  ScAddr decompositionTuple = sections_utils::GetSectionDecompositionTuple(&m_context, parentSection);
  if (!m_context.IsElement(decompositionTuple))
    return {};

  ScAddr newSection = sections_utils::FindSectionByName(&m_context, sectionName);
  if (!m_context.IsElement(newSection))
    newSection = sections_generator::GenerateSection(&m_context, sectionName, lang, true);
  else
  {
    if (m_context.CheckConnector(decompositionTuple, newSection, ScType::EdgeAccessConstPosPerm)
        || parentSection == newSection)
      return {};
  }

  ScAddr lastSection = sections_utils::GetLastSubSection(&m_context, decompositionTuple);
  ScAddr newSectionArc = m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, decompositionTuple, newSection);
  if (m_context.IsElement(lastSection))
  {
    SC_AGENT_LOG_DEBUG("Last section system idtf is \"" + m_context.GetElementSystemIdentifier(lastSection) + "\".");
    ScIterator5Ptr lastSectionIterator = m_context.CreateIterator5(
        decompositionTuple,
        ScType::EdgeAccessConstPosPerm,
        lastSection,
        ScType::EdgeAccessConstPosTemp,
        SectionsKeynodes::rrel_last);
    if (lastSectionIterator->Next())
    {
      ScAddr previousSectionArc = lastSectionIterator->Get(1);
      ScAddr previousSectionTempArc = lastSectionIterator->Get(3);
      m_context.EraseElement(previousSectionTempArc);

      utils::GenerationUtils::generateRelationBetween(
          &m_context, previousSectionArc, newSectionArc, ScKeynodes::nrel_basic_sequence);
      m_context.GenerateConnector(ScType::EdgeAccessConstPosTemp, SectionsKeynodes::rrel_last, newSectionArc);
    }
  }
  else
  {
    SC_AGENT_LOG_DEBUG("Added section is new.");
    m_context.GenerateConnector(ScType::EdgeAccessConstPosPerm, ScKeynodes::rrel_1, newSectionArc);
    m_context.GenerateConnector(ScType::EdgeAccessConstPosTemp, SectionsKeynodes::rrel_last, newSectionArc);
  }

  sectionsModule::SetUtils::AddToSets(&m_context, parentSection, {SectionsKeynodes::non_atomic_section});
  sectionsModule::SetUtils::RemoveFromSets(&m_context, parentSection, {SectionsKeynodes::atomic_section});
  sectionsModule::SetUtils::RemoveFromSets(&m_context, newSection, {SectionsKeynodes::removed_section});
  return newSection;
}

ScAddr AddSectionAgent::GenerateSection(std::string const & sectionName, ScAddr const & lang)
{
  ScAddr section = sections_utils::FindSectionByName(&m_context, sectionName);
  if (!m_context.IsElement(section))
    section = sections_generator::GenerateSection(&m_context, sectionName, lang, true);
  else
    sectionsModule::SetUtils::RemoveFromSets(&m_context, section, {SectionsKeynodes::removed_section});
  return section;
}

}  // namespace sectionsModule

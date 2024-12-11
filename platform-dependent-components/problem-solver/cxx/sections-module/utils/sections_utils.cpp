/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sections_utils.hpp"

#include "constants/sections_aliases.hpp"

namespace sectionsModule
{
ScAddr sections_utils::GetLastSubSection(ScMemoryContext * context, ScAddr const & tuple)
{
  ScAddr lastSection = utils::IteratorUtils::getAnyByOutRelation(context, tuple, SectionsKeynodes::rrel_last);

  return lastSection;
}

ScAddr sections_utils::GetSectionDecompositionTuple(ScMemoryContext * context, ScAddr const & sectionAddr)
{
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      sectionAddr,
      ScType::VarCommonArc,
      ScType::VarNode >> sections_aliases::DECOMPOSITION_TUPLE,
      ScType::VarPermPosArc,
      SectionsKeynodes::nrel_entity_decomposition);
  ScTemplateSearchResult searchResult;
  context->SearchByTemplate(scTemplate, searchResult);
  if (!searchResult.IsEmpty())
    return searchResult[0][sections_aliases::DECOMPOSITION_TUPLE];
  ScTemplateResultItem genResult;
  context->GenerateByTemplate(scTemplate, genResult);

  return genResult[sections_aliases::DECOMPOSITION_TUPLE];
}

ScAddr sections_utils::FindSectionByName(ScMemoryContext * context, std::string const & sectionName)
{
  ScAddr sectionAddr;
  ScAddrSet links = context->SearchLinksByContentSubstring(sectionName);
  if (!links.empty())
  {
    for (ScAddr const & link : links)
    {
      ScAddr section = utils::IteratorUtils::getAnyByInRelation(context, link, ScKeynodes::nrel_main_idtf);
      if (context->IsElement(section))
      {
        sectionAddr = section;
        break;
      }
    }
  }
  return sectionAddr;
}

}  // namespace sectionsModule

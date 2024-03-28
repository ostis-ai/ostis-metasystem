/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "constants/subject_domain_aliases.hpp"

#include "subject_domain_utils.hpp"

namespace subjectDomainModule
{
ScAddr subject_domain_utils::GetLastSubSection(ScMemoryContext * context, ScAddr const & tuple)
{
  ScAddr lastSection = utils::IteratorUtils::getAnyByOutRelation(context, tuple, SubjectDomainKeynodes::rrel_last);

  return lastSection;
}

ScAddr subject_domain_utils::GetSectionDecompositionTuple(ScMemoryContext * context, ScAddr const & sectionAddr)
{
  ScTemplate scTemplate;
  scTemplate.Quintuple(
      ScType::NodeVar >> subject_domain_aliases::DECOMPOSITION_TUPLE,
      ScType::EdgeDCommonVar,
      sectionAddr,
      ScType::EdgeAccessVarPosPerm,
      SubjectDomainKeynodes::nrel_entity_decomposition);
  ScTemplateSearchResult searchResult;
  context->HelperSearchTemplate(scTemplate, searchResult);
  if (!searchResult.IsEmpty())
    return searchResult[0][subject_domain_aliases::DECOMPOSITION_TUPLE];
  ScTemplateGenResult genResult;
  ScTemplate::Result result = context->HelperGenTemplate(scTemplate, genResult);
  if (result)
    return genResult[subject_domain_aliases::DECOMPOSITION_TUPLE];
  return {};
}

ScAddr subject_domain_utils::FindSectionByName(ScMemoryContext * context, std::string const & sectionName)
{
  ScAddr sectionAddr;
  ScAddrVector links = context->FindLinksByContent(sectionName);
  if (!links.empty())
  {
    for (ScAddr const & link : links)
    {
      ScAddr section =
          utils::IteratorUtils::getAnyByInRelation(context, link, scAgentsCommon::CoreKeynodes::nrel_main_idtf);
      if (context->IsElement(section))
      {
        sectionAddr = section;
        break;
      }
    }
  }
  return sectionAddr;
}

}  // namespace subjectDomainModule

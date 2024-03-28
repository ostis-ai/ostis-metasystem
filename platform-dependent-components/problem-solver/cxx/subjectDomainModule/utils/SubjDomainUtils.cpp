/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#include "constants/SubjDomainAliases.hpp"

#include "SubjDomainUtils.hpp"

namespace subjectDomainModule
{
ScAddr SubjDomainUtils::getLastSubSection(ScMemoryContext * context, ScAddr const & tuple)
{
  ScAddr lastSection = utils::IteratorUtils::getAnyByOutRelation(context, tuple, Keynodes::rrel_last);

  return lastSection;
}

ScAddr SubjDomainUtils::getSectionDecompositionTuple(ScMemoryContext * context, ScAddr const & sectionAddr)
{
  ScTemplate scTemplate;
  scTemplate.Quintuple(
        ScType::NodeVar >> SubjDomainAliases::DECOMPOSITION_TUPLE,
        ScType::EdgeDCommonVar,
        sectionAddr,
        ScType::EdgeAccessVarPosPerm,
        Keynodes::nrel_entity_decomposition
  );
  ScTemplateSearchResult searchResult;
  context->HelperSearchTemplate(scTemplate, searchResult);
  if (!searchResult.IsEmpty())
    return searchResult[0][SubjDomainAliases::DECOMPOSITION_TUPLE];
  ScTemplateGenResult genResult;
  ScTemplate::Result result = context->HelperGenTemplate(scTemplate, genResult);
  if (result)
    return genResult[SubjDomainAliases::DECOMPOSITION_TUPLE];
  return {};
}

ScAddr SubjDomainUtils::findSectionByName(ScMemoryContext * context, std::string const & sectionName)
{
  ScAddr sectionAddr;
  ScAddrVector links = context->FindLinksByContent(sectionName);
  if (!links.empty())
  {
    for (auto link: links)
    {
      ScAddr section = utils::IteratorUtils::getAnyByInRelation(
            context,
            link,
            scAgentsCommon::CoreKeynodes::nrel_main_idtf);
      if (context->IsElement(section))
      {
        sectionAddr = section;
        break;
      }
    }
  }
  return sectionAddr;
}

}

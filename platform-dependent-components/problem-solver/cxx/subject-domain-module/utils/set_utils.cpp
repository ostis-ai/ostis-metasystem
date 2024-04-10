/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/GenerationUtils.hpp"
#include "sc-memory/sc_template.hpp"

#include "set_utils.hpp"

namespace subjectDomainModule
{
ScAddrVector SetUtils::GetAllElementsByEdgeType(ScMemoryContext * context, ScAddr const & set, ScType edgeType)
{
  if (!context->IsElement(set))
    return {};
  ScAddrVector elements;
  ScTemplate rightsTemplate;
  rightsTemplate.Triple(set, edgeType, ScType::NodeVar >> "_node");
  ScTemplateSearchResult result;
  context->HelperSearchTemplate(rightsTemplate, result);
  if (!result.IsEmpty())
  {
    for (int i = 0; i < result.Size(); i++)
      elements.push_back(result[i]["_node"]);
  }
  return elements;
}

ScAddr SetUtils::GetEdge(ScMemoryContext * context, ScAddr const & source, ScAddr const & target)
{
  ScTemplate scTemplate;
  scTemplate.Triple(source, ScType::EdgeAccessVarPosPerm >> "_edge", target);
  ScTemplateSearchResult result;
  context->HelperSearchTemplate(scTemplate, result);
  if (!result.IsEmpty())
    return result[0]["_edge"];
  else
    return {};
}

void SetUtils::AddToSets(ScMemoryContext * context, ScAddr const & element, ScAddrVector const & sets)
{
  for (ScAddr const & set : sets)
    utils::GenerationUtils::addToSet(context, set, element);
}

void SetUtils::RemoveFromSets(ScMemoryContext * context, ScAddr const & element, ScAddrVector const & sets)
{
  for (ScAddr const & set : sets)
  {
    ScAddr edge = GetEdge(context, set, element);
    if (context->IsElement(edge))
      context->EraseElement(edge);
  }
}

void SetUtils::RemoveBaseEdgesFromSet(ScMemoryContext * context, ScAddr const & set)
{
  ScIterator3Ptr setElementsIterator = context->Iterator3(set, ScType::EdgeAccessConstPosPerm, ScType::Unknown);

  while (setElementsIterator->Next())
  {
    context->EraseElement(setElementsIterator->Get(1));
  }
}

}  // namespace subjectDomainModule

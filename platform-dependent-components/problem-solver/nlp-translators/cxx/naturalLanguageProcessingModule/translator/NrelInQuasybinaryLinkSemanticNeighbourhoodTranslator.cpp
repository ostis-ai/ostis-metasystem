#include "NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator::NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator(
    ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & tupleIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE
        || isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    if (anyIsInStructure({tupleIterator->Get(2), tupleIterator->Get(4)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string const & nrelMainIdtf = getEnglishMainIdtf(nrelNode);
    if (nrelMainIdtf.empty())
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(2);
    auto const & linkIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
    while (linkIterator->Next() && translations.size() < maxTranslations)
    {
      if (isInStructure(linkIterator->Get(1), structure) == SC_FALSE)
        continue;
      ScAddr const & linkNode = linkIterator->Get(2);
      std::string const & linkContent = getEnglishContent(linkNode);
      if (linkContent.empty())
        continue;

      translations.push_back(nrelMainIdtf + " " + linkContent);
    }
  }
  return translations;
}

std::list<ScAddrVector> NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;
  auto const & tupleIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::NodeConstTuple, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next())
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE
        || isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(2);
    auto const & linkIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
    while (linkIterator->Next())
    {
      if (isInStructure(linkIterator->Get(1), structure) == SC_FALSE)
        continue;

      answer.push_back(
          {tupleIterator->Get(1),
           tupleIterator->Get(2),
           tupleIterator->Get(3),
           tupleIterator->Get(4),
           linkIterator->Get(1),
           linkIterator->Get(2)});
    }
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule

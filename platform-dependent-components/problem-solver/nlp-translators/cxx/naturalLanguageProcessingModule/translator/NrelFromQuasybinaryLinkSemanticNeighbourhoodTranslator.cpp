#include "NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator::NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator(
    ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & tupleIterator = context->Iterator5(
      ScType::NodeConstTuple, ScType::EdgeDCommonConst, node, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE
        || isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    if (anyIsInStructure({tupleIterator->Get(0), tupleIterator->Get(4)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string const & nrelMainIdtf = getEnglishMainIdtf(nrelNode);
    if (nrelMainIdtf.empty())
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(0);
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

std::list<ScAddrVector> NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;
  auto const & tupleIterator = context->Iterator5(
      ScType::NodeConstTuple, ScType::EdgeDCommonConst, node, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (tupleIterator->Next())
  {
    if (isInStructure(tupleIterator->Get(1), structure) == SC_FALSE
        || isInStructure(tupleIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelNode = tupleIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    ScAddr const & tupleNode = tupleIterator->Get(0);
    auto const & linkIterator = context->Iterator3(tupleNode, ScType::EdgeAccessConstPosPerm, ScType::LinkConst);
    while (linkIterator->Next())
    {
      if (isInStructure(linkIterator->Get(1), structure) == SC_FALSE)
        continue;

      answer.push_back(
          {tupleIterator->Get(0),
           tupleIterator->Get(1),
           tupleIterator->Get(3),
           tupleIterator->Get(4),
           linkIterator->Get(1),
           linkIterator->Get(2)});
    }
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule

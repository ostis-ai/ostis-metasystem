#include "NrelInLinkSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelInLinkSemanticNeighbourhoodTranslator::NrelInLinkSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelInLinkSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & linkIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::LinkConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (linkIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(linkIterator->Get(1), structure) == SC_FALSE
        || isInStructure(linkIterator->Get(3), structure) == SC_FALSE)
      continue;
    if (anyIsInStructure({linkIterator->Get(4)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const linkNode = linkIterator->Get(2);
    std::string const & linkContent = getEnglishContent(linkNode);
    if (linkContent.empty())
      continue;
    ScAddr const nrelNode = linkIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string nrelMainIdtf = getEnglishMainIdtf(nrelNode);
    if (nrelMainIdtf.empty())
      continue;

    translations.push_back(nrelMainIdtf.append(" ").append(linkContent));
  }
  return translations;
}

std::list<ScAddrVector> NrelInLinkSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;

  auto const & linkIterator = context->Iterator5(
      node, ScType::EdgeDCommonConst, ScType::LinkConst, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (linkIterator->Next())
  {
    if (isInStructure(linkIterator->Get(1), structure) == SC_FALSE
        || isInStructure(linkIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const nrelNode = linkIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    answer.push_back({linkIterator->Get(1), linkIterator->Get(2), linkIterator->Get(3), linkIterator->Get(4)});
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule

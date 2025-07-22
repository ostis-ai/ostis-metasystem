#include "NrelFromNodeSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
NrelFromNodeSemanticNeighbourhoodTranslator::NrelFromNodeSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> NrelFromNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & nrelIterator = context->Iterator5(
      ScType::NodeConst, ScType::EdgeDCommonConst, node, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (nrelIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(nrelIterator->Get(1), structure) == SC_FALSE
        || isInStructure(nrelIterator->Get(3), structure) == SC_FALSE)
      continue;
    if (anyIsInStructure({nrelIterator->Get(0), nrelIterator->Get(4)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const & nrelSourceNode = nrelIterator->Get(0);
    if (isInIgnoredKeynodes(nrelSourceNode))
      continue;
    if (isInIgnoredKeynodes(nrelSourceNode))
      continue;
    std::string const & nrelSourceMainIdtf = getEnglishMainIdtf(nrelSourceNode);
    if (nrelSourceMainIdtf.empty())
      continue;

    ScAddr const & nrelNode = nrelIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;
    std::string nrelMainIdtf = getEnglishMainIdtf(nrelNode);
    if (nrelMainIdtf.empty())
      continue;

    translations.push_back(nrelMainIdtf.append(" ").append(nrelSourceMainIdtf));
  }
  return translations;
}

std::list<ScAddrVector> NrelFromNodeSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;
  auto const & nrelIterator = context->Iterator5(
      ScType::NodeConst, ScType::EdgeDCommonConst, node, ScType::EdgeAccessConstPosPerm, ScType::NodeConstNoRole);
  while (nrelIterator->Next())
  {
    if (isInStructure(nrelIterator->Get(1), structure) == SC_FALSE
        || isInStructure(nrelIterator->Get(3), structure) == SC_FALSE)
      continue;
    ScAddr const & nrelSourceNode = nrelIterator->Get(0);
    if (isInIgnoredKeynodes(nrelSourceNode))
      continue;
    if (isInIgnoredKeynodes(nrelSourceNode))
      continue;

    ScAddr const & nrelNode = nrelIterator->Get(4);
    if (isInIgnoredKeynodes(nrelNode))
      continue;

    answer.push_back({nrelIterator->Get(0), nrelIterator->Get(1), nrelIterator->Get(3), nrelIterator->Get(4)});
  }
  return answer;
}
}  // namespace naturalLanguageProcessingModule

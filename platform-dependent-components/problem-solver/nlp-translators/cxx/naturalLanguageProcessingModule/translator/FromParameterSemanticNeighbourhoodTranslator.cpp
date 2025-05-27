#include "FromParameterSemanticNeighbourhoodTranslator.hpp"

#include "keynodes/TranslationKeynodes.hpp"

namespace naturalLanguageProcessingModule
{
FromParameterSemanticNeighbourhoodTranslator::FromParameterSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> FromParameterSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslations,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> translations;
  translations.reserve(maxTranslations);
  auto const & classIterator = context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, node);
  while (classIterator->Next() && translations.size() < maxTranslations)
  {
    if (isInStructure(classIterator->Get(1), structure) == SC_FALSE)
      continue;
    ScAddr const & classNode = classIterator->Get(0);
    if (isInIgnoredKeynodes(classNode))
      continue;
    std::string const & classMainIdtf = getEnglishMainIdtf(classNode);
    if (classMainIdtf.empty())
      continue;

    auto const & parameterIterator =
        context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, classNode);
    while (parameterIterator->Next() && translations.size() < maxTranslations)
    {
      if (isInStructure(parameterIterator->Get(1), structure) == SC_FALSE)
        continue;
      if (anyIsInStructure({classIterator->Get(0), parameterIterator->Get(0)}, atLeastOneNodeFromConstruction)
          == SC_FALSE)
        continue;
      ScAddr const & parameterNode = parameterIterator->Get(0);
      if (isInIgnoredKeynodes(parameterNode))
        continue;
      if (context->HelperCheckEdge(TranslationKeynodes::parameter, parameterNode, ScType::EdgeAccessConstPosPerm)
          == SC_FALSE)
        continue;
      std::string parameterMainIdtf = getEnglishMainIdtf(parameterNode);
      if (parameterMainIdtf.empty())
        continue;

      translations.push_back(parameterMainIdtf.append(" is ").append(classMainIdtf));
    }
  }
  return translations;
}

std::list<ScAddrVector> FromParameterSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answer;

  auto const & classIterator = context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, node);
  while (classIterator->Next())
  {
    if (isInStructure(classIterator->Get(1), structure) == SC_FALSE)
      continue;
    ScAddr const & classNode = classIterator->Get(0);
    if (isInIgnoredKeynodes(classNode))
      continue;

    auto const & parameterIterator =
        context->Iterator3(ScType::NodeConstClass, ScType::EdgeAccessConstPosPerm, classNode);
    while (parameterIterator->Next())
    {
      if (isInStructure(parameterIterator->Get(1), structure) == SC_FALSE)
        continue;
      ScAddr const & parameterNode = parameterIterator->Get(0);
      if (isInIgnoredKeynodes(parameterNode))
        continue;
      if (context->HelperCheckEdge(TranslationKeynodes::parameter, parameterNode, ScType::EdgeAccessConstPosPerm)
          == SC_FALSE)
        continue;

      answer.push_back(
          {classIterator->Get(1), classIterator->Get(2), parameterIterator->Get(1), parameterIterator->Get(2)});
    }
  }
  return answer;
}

}  // namespace naturalLanguageProcessingModule

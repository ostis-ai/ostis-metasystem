#include "keynodes/TranslationKeynodes.hpp"

#include "FromConceptSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
FromConceptSemanticNeighbourhoodTranslator::FromConceptSemanticNeighbourhoodTranslator(ScMemoryContext * context)
  : SemanticNeighbourhoodTranslator(context)
{
}

std::vector<std::string> FromConceptSemanticNeighbourhoodTranslator::getSemanticNeighbourhoods(

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
    if (anyIsInStructure({classIterator->Get(0)}, atLeastOneNodeFromConstruction) == SC_FALSE)
      continue;
    ScAddr const & classNode = classIterator->Get(0);
    if (isInIgnoredKeynodes(classNode))
      continue;
    if (isParameter(classNode))
      continue;
    std::string const & classMainIdtf = getEnglishMainIdtf(classNode);
    if (classMainIdtf.empty())
      continue;

    SC_LOG_INFO("adding is " << classMainIdtf);
    translations.push_back("is " + classMainIdtf);
  }
  return translations;
}

bool FromConceptSemanticNeighbourhoodTranslator::isParameter(ScAddr const & node) const
{
  std::string const & NODE_ALIAS = "_node;";
  ScTemplate scTemplate;
  scTemplate.Triple(TranslationKeynodes::parameter, ScType::EdgeAccessVarPosPerm, ScType::NodeVarClass >> NODE_ALIAS);
  scTemplate.Triple(NODE_ALIAS, ScType::EdgeAccessVarPosPerm, node);
  ScTemplateSearchResult searchResult;
  context->HelperSearchTemplate(scTemplate, searchResult);
  return searchResult.IsEmpty() == SC_FALSE;
}

std::list<ScAddrVector> FromConceptSemanticNeighbourhoodTranslator::getSemanticNeighbourhoodsElements(
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
    if (isParameter(classNode))
      continue;
    answer.push_back({classIterator->Get(0), classIterator->Get(1)});
  }

  return answer;
}
}  // namespace naturalLanguageProcessingModule

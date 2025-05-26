#include "SemanticNeighbourhoodTranslatorSet.hpp"

#include "NrelInNodeSemanticNeighbourhoodTranslator.hpp"
#include "NrelInLinkSemanticNeighbourhoodTranslator.hpp"
#include "NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"
#include "NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"
#include "NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator.hpp"
#include "FromParameterSemanticNeighbourhoodTranslator.hpp"
#include "FromConceptSemanticNeighbourhoodTranslator.hpp"
#include "NrelFromNodeSemanticNeighbourhoodTranslator.hpp"
#include "NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
SemanticNeighbourhoodTranslatorSet::SemanticNeighbourhoodTranslatorSet(ScMemoryContext * context)
  : handlers(
      {new NrelInNodeSemanticNeighbourhoodTranslator(context),
       new NrelInLinkSemanticNeighbourhoodTranslator(context),
       new NrelInQuasybinaryLinkSemanticNeighbourhoodTranslator(context),
       new NrelInQuasybinaryNodeSemanticNeighbourhoodTranslator(context),
       new NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(context),
       new FromParameterSemanticNeighbourhoodTranslator(context),
       new FromConceptSemanticNeighbourhoodTranslator(context),
       new NrelFromNodeSemanticNeighbourhoodTranslator(context),
       new NrelFromQuasybinaryLinkSemanticNeighbourhoodTranslator(context)})
{
}

SemanticNeighbourhoodTranslatorSet::~SemanticNeighbourhoodTranslatorSet()
{
  for (auto const & handler : handlers)
    delete handler;
}

std::vector<std::string> SemanticNeighbourhoodTranslatorSet::getSemanticNeighbourhoods(
    ScAddr const & node,
    size_t const & maxTranslationsFromEachHandler,
    ScAddrSet const & structure,
    ScAddrSet const & atLeastOneNodeFromConstruction) const
{
  std::vector<std::string> answer;
  for (auto const & handler : handlers)
  {
    auto translations = handler->getSemanticNeighbourhoods(
        node, maxTranslationsFromEachHandler, structure, atLeastOneNodeFromConstruction);
    answer.insert(answer.cend(), translations.cbegin(), translations.cend());
  }
  return answer;
}

std::list<ScAddrVector> SemanticNeighbourhoodTranslatorSet::getSemanticNeighbourhoodsElements(
    ScAddr const & node,
    ScAddrSet const & structure) const
{
  std::list<ScAddrVector> answerList;
  for (auto const & handler : handlers)
  {
    std::list<ScAddrVector> const & addrs = handler->getSemanticNeighbourhoodsElements(node, structure);
    answerList.insert(answerList.cend(), addrs.cbegin(), addrs.cend());
  }
  return answerList;
}
}  // namespace naturalLanguageProcessingModule

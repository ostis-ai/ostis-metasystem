#pragma once

#include "translator/SemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
class SemanticNeighbourhoodTranslatorSet
{
public:
  explicit SemanticNeighbourhoodTranslatorSet(ScMemoryContext * context);

  std::vector<std::string> getSemanticNeighbourhoods(
      ScAddr const & node,
      size_t const & maxTranslationsFromEachHandler,
      ScAddrSet const & structure = {},
      ScAddrSet const & atLeastOneNodeFromConstruction = {}) const;

  std::list<ScAddrVector> getSemanticNeighbourhoodsElements(ScAddr const & node, ScAddrSet const & structure = {})
      const;

  ~SemanticNeighbourhoodTranslatorSet();

private:
  std::set<SemanticNeighbourhoodTranslator *> const handlers;
};

}  // namespace naturalLanguageProcessingModule

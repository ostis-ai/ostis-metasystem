#pragma once

#include "SemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
/**
 * parameter _-> _parameter_class;;
 * _parameter_class _-> _parameter_value;;
 * _parameter_value _-> node;;
 */
class FromParameterSemanticNeighbourhoodTranslator : public SemanticNeighbourhoodTranslator
{
public:
  explicit FromParameterSemanticNeighbourhoodTranslator(ScMemoryContext * context);

  std::vector<std::string> getSemanticNeighbourhoods(
      ScAddr const & node,
      size_t const & maxTranslations,
      ScAddrSet const & structure,
      ScAddrSet const & atLeastOneNodeFromConstruction) const override;

  std::list<ScAddrVector> getSemanticNeighbourhoodsElements(ScAddr const & node, ScAddrSet const & structure)
      const override;
};

}  // namespace naturalLanguageProcessingModule

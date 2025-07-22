#pragma once

#include "SemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
/**
 * _concept _-> node;;
 *
 * note: translation is generated only if following construction does not exist
 * _parameter_class
 *  _<- parameter;
 *  _-> _concept;;
 */
class FromConceptSemanticNeighbourhoodTranslator : public SemanticNeighbourhoodTranslator
{
public:
  explicit FromConceptSemanticNeighbourhoodTranslator(ScMemoryContext * context);

  std::vector<std::string> getSemanticNeighbourhoods(
      ScAddr const & node,
      size_t const & maxTranslations,
      ScAddrSet const & structure,
      ScAddrSet const & atLeastOneNodeFromConstruction) const override;

  std::list<ScAddrVector> getSemanticNeighbourhoodsElements(ScAddr const & node, ScAddrSet const & structure)
      const override;

private:
  bool isParameter(ScAddr const & node) const;
};

}  // namespace naturalLanguageProcessingModule

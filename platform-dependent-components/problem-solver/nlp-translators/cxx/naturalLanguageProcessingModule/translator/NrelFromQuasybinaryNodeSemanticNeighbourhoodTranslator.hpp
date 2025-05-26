#pragma once

#include "SemanticNeighbourhoodTranslator.hpp"

namespace naturalLanguageProcessingModule
{
/**
 * node _<= _nrel_relation:: {_quasybinary_node};;
 */
class NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator : public SemanticNeighbourhoodTranslator
{
public:
  explicit NrelFromQuasybinaryNodeSemanticNeighbourhoodTranslator(ScMemoryContext * context);

  std::vector<std::string> getSemanticNeighbourhoods(
      ScAddr const & node,
      size_t const & maxTranslations,
      ScAddrSet const & structure,
      ScAddrSet const & atLeastOneNodeFromConstruction) const override;

  std::list<ScAddrVector> getSemanticNeighbourhoodsElements(ScAddr const & node, ScAddrSet const & structure)
      const override;

private:
};

}  // namespace naturalLanguageProcessingModule

#pragma once

#include "sc-memory/sc_memory.hpp"

namespace structuredRepresentationGenerationModule
{
class StructuredRepresentationGenerator
{
public:
  explicit StructuredRepresentationGenerator(ScMemoryContext * context);

  ScAddrVector GenerateStructuredRepresentation(ScAddr const & structure, ScAddr const & rule);

private:
  ScMemoryContext * context;

  ScAddrVector currentGeneratedElements;

  ScAddr GenerateStructuredRepresentation(ScAddr const & structure, ScAddr const & rule, ScAddr const & startElement);

  ScAddrVector ProcessTemplateWithAdditionRule(
      ScAddr const & structure,
      ScAddr const & templ,
      ScAddr const & templateKeyVariable,
      ScAddr const & additionRule,
      ScAddr const & startElement);

  ScAddrVector ProcessTemplate(
      ScAddr const & structure,
      ScAddr const & templ,
      ScAddr const & templateKeyVariable,
      ScAddr const & startElement);

  ScAddr WrapInStructure(ScTemplateResultItem const & resultItem);

  void AddElementToTuple(ScAddr const & element, ScAddr const & tuple);
};
}  // namespace structuredRepresentationGenerationModule

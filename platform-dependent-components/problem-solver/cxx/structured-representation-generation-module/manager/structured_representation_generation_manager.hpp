#pragma once

#include "sc-memory/sc_memory.hpp"
#include "generator/structured_representation_generator.hpp"

namespace structuredRepresentationGenerationModule
{
class StructuredRepresentationGenerationManager
{
public:
  explicit StructuredRepresentationGenerationManager(ScMemoryContext * context);

  ScAddrVector Manage(ScAddrVector const & processParameters);

private:
  std::unique_ptr<StructuredRepresentationGenerator> generator;
};
}  // namespace structuredRepresentationGenerationModule

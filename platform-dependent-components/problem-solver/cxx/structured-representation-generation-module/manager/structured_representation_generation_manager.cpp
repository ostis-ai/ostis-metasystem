#include "structured_representation_generation_manager.hpp"

using namespace structuredRepresentationGenerationModule;

StructuredRepresentationGenerationManager::StructuredRepresentationGenerationManager(ScMemoryContext * context)
  : generator(std::make_unique<StructuredRepresentationGenerator>(context))
{
}

ScAddrVector StructuredRepresentationGenerationManager::Manage(ScAddrVector const & processParameters)
{
  ScAddr const structure = processParameters.at(0);
  ScAddr const rule = processParameters.at(1);

  ScAddrVector answer = generator->GenerateStructuredRepresentation(structure, rule);

  return answer;
}


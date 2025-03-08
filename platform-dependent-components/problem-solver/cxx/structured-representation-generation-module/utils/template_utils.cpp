#include "template_utils.hpp"
#include "sc-memory/sc_memory.hpp"

using namespace structuredRepresentationGenerationModule;

bool TemplateUtils::CheckAllSearchResultItemInStructure(ScMemoryContext * context, ScTemplateResultItem const & resultItem, ScAddr const & structure)
{
  return std::all_of(resultItem.begin(), resultItem.end(), [context, structure](ScAddr const & item){
    return context->HelperCheckEdge(structure, item, ScType::EdgeAccessConstPosPerm);
  });
}


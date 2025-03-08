#pragma once

#include "sc-memory/sc_template.hpp"

namespace structuredRepresentationGenerationModule
{

class TemplateUtils
{
public:
  static bool CheckAllSearchResultItemInStructure(ScMemoryContext * context, ScTemplateResultItem const & resultItem, ScAddr const & structure);
};

}

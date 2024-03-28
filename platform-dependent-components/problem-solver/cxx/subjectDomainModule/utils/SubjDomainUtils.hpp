/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#pragma once

#include "sc-memory/sc_memory.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/Keynodes.hpp"

#include <string>

namespace subjectDomainModule
{

class SubjDomainUtils
{
public:
  static ScAddr findSectionByName(ScMemoryContext *, std::string const &);

  static ScAddr getLastSubSection(ScMemoryContext * context, ScAddr const & tuple);

  // Get decomposition tuple if it exists or generate it
  static ScAddr getSectionDecompositionTuple(ScMemoryContext *, ScAddr const &);
};

}

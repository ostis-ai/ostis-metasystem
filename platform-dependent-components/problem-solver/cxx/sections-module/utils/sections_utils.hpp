/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_memory.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "keynodes/sections_keynodes.hpp"

#include <string>

namespace sectionsModule
{

class sections_utils
{
public:
  static ScAddr FindSectionByName(ScMemoryContext *, std::string const &);

  static ScAddr GetLastSubSection(ScMemoryContext * context, ScAddr const & tuple);

  // Get decomposition tuple if it exists or generate it
  static ScAddr GetSectionDecompositionTuple(ScMemoryContext *, ScAddr const &);
};

}  // namespace subjectDomainModule

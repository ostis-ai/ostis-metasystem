/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include <sc-memory/sc_memory.hpp>

namespace sectionsModule
{
class sections_generator
{
public:
  static ScAddr GenerateSection(ScMemoryContext *, std::string const &, ScAddr const &, bool);
};
}  // namespace sectionsModule

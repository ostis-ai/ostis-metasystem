/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Ruslan Korshunov
 */

#pragma once

#include <string>

#include "sc-memory/sc_memory.hpp"

namespace subjectDomainModule
{
class SubjDomainGenerator
{
public:
  static ScAddr generateSection(ScMemoryContext *, std::string const &, ScAddr const &, bool);

  static void generateSectionBaseOrder(ScMemoryContext *, ScAddr const &, ScAddr const &);
};
}

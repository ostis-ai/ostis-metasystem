/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Ruslan Korshunovc
 */

#pragma once

#include "sc-memory/sc_memory.hpp"

namespace subjectDomainModule
{
class SubjDomainBuilder
{
public:
  static void buildDecompositionTupleTemplate(ScTemplate &, ScAddr const &, ScAddr const &);

  static void buildDecompositionTupleTemplate(ScTemplate &, ScAddr const &);

  static void buildNextSectionTemplate(ScTemplate &, ScAddr const &, ScAddr const &);

  static void buildPreviousSectionTemplate(ScTemplate &, ScAddr const &, ScAddr const &);
};
}

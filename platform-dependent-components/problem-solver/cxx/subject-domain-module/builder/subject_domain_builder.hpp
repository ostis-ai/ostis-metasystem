/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_memory.hpp"

namespace subjectDomainModule
{
class subject_domain_builder
{
public:
  static void buildDecompositionTupleTemplate(ScTemplate &, ScAddr const &, ScAddr const &);

  static void buildDecompositionTupleTemplate(ScTemplate &, ScAddr const &);

  static void buildNextSectionTemplate(ScTemplate &, ScAddr const &, ScAddr const &);

  static void buildPreviousSectionTemplate(ScTemplate &, ScAddr const &, ScAddr const &);
};
}  // namespace subjectDomainModule

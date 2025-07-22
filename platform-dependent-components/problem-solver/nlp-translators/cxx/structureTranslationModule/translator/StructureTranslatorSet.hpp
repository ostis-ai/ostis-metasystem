/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "translator/StructureTranslator.hpp"

namespace structureTranslationModule
{
class StructureTranslatorSet
{
public:
  explicit StructureTranslatorSet(ScMemoryContext * context);

  std::string translate(ScAddr const & structAddr, ScAddr const & lang) const;

  ~StructureTranslatorSet();

private:
  std::unordered_set<StructureTranslator *> const handlers;
};

}  // namespace structureTranslationModule

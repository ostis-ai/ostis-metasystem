/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "StructureTranslator.hpp"

namespace structureTranslationModule
{
/**
 * _class _-> _node;;
 */
class FromConceptTranslator : public StructureTranslator
{
public:
  explicit FromConceptTranslator(ScMemoryContext * context);

  std::stringstream translate(ScAddr const & structAddr, ScAddr const & lang) const override;
};

}  // namespace structureTranslationModule

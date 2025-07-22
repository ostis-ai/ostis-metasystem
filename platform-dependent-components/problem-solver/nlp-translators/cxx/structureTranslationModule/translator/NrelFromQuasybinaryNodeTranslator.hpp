/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "StructureTranslator.hpp"
#include <sstream>

namespace structureTranslationModule
{
/**
 * {_tuple} _=> _nrel:: _node;;
 */
class NrelFromQuasybinaryNodeTranslator : public StructureTranslator
{
public:
  explicit NrelFromQuasybinaryNodeTranslator(ScMemoryContext * context);

  std::stringstream translate(ScAddr const & structAddr, ScAddr const & lang) const override;

private:
};

}  // namespace structureTranslationModule

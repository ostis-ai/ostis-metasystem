/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include <sc-memory/sc_memory.hpp>

namespace verificationModule
{

class SearchUtils
{
public:
  static void getSingularRelations(ScMemoryContext * context, ScAddrList & singularRelations);

  static void getQuasybinaryRelations(ScMemoryContext * context, ScAddrUnorderedSet & quasybinaryRelations);
};

}  // namespace verificationModule

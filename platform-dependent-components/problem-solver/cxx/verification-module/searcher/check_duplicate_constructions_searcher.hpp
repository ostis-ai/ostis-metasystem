/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>

#include "sc-memory/sc_addr.hpp"
#include "sc-memory/sc_memory.hpp"

namespace verificationModule
{
class checkDuplicateConstructionSearcher
{
public:
  explicit checkDuplicateConstructionSearcher(ScMemoryContext * context);

  static std::ofstream openOrCreateFile(ScMemoryContext * context, ScAddr & classAddr, std::filesystem::path const & filename);

private:
  ScMemoryContext * context;
};
}  // namespace verificationModule

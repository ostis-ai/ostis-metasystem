/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <string>
#include "filesystem"

#include "sc-memory/sc_memory.hpp"

namespace verificationModule
{
class verificationGenerator
{
public:
  explicit verificationGenerator(ScMemoryContext * context);

  static std::ofstream generateDuplicationFile(ScMemoryContext * context, ScAddr const &, std::filesystem::path const & filename);

private:
  ScMemoryContext * context;
};
}  // namespace verificationModule

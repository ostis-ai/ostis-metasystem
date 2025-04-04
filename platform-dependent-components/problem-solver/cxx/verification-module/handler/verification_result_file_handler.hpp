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
class VerificationResulFileHandler
{
public:
  explicit VerificationResulFileHandler(ScMemoryContext * context);

  std::ofstream createOutputFile(
      std::filesystem::path const & filePath,
      std::string const & checkedElementIdtf,
      ScAddr const & checkedElementAddr) const;

private:
  ScMemoryContext * context;

  void resolveFileSpecification(ScAddr const & checkedElementAddr, std::filesystem::path const & fileFullNAme) const;

  void generateDuplicationFileSpecification(ScAddr const &, std::filesystem::path const & fileFullNAme) const;
};
}  // namespace verificationModule

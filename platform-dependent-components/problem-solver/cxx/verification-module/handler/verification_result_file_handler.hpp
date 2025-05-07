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
class VerificationResultFileHandler
{
public:
  explicit VerificationResultFileHandler(ScMemoryContext * context);

  std::ofstream CreateOutputFile(
      std::filesystem::path const & filePath,
      std::string const & checkedElementIdtf,
      ScAddr const & checkedElementAddr,
      ScAddrUnorderedSet & resultElements) const;

private:
  ScMemoryContext * context;

  void ResolveFileSpecification(
      ScAddr const & checkedElementAddr,
      std::filesystem::path const & fileFullName,
      ScAddrUnorderedSet & resultElements) const;

  void GenerateDuplicationFileSpecification(
      ScAddr const & checkedElementAddr,
      std::filesystem::path const & fileFullName,
      ScAddrUnorderedSet & resultElements) const;
};
}  // namespace verificationModule

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fstream>
#include <sstream>

#include "data-structures/set_duplications_check_result.hpp"

namespace verificationModule
{
class VerificationResultLogger
{
public:
  void LogSetCheckResult(SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const;

  void LogSetInfo(SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const;

private:
  void AddEnumeration(std::list<std::string> const & enumerationElements, std::stringstream & stringStream) const;

  void LogElementCheckResult(ElementDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const;

  std::string const INDENT = "    ";
};
}  // namespace verificationModule

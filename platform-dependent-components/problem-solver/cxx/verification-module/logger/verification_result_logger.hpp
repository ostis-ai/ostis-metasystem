/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fstream>
#include <sstream>

#include "dataStructures/set_duplications_check_result.hpp"

namespace verificationModule
{
class VerificationResultLogger
{
public:
  void logSetCheckResult(SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const;

  void logSetInfo(SetDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const;

private:
  void addEnumeration(std::list<std::string> const & enumerationElements, std::stringstream & stringStream) const;

  void logElementCheckResult(ElementDuplicationsCheckResult const & checkResult, std::ofstream & logFile) const;

  std::string const INDENT = "    ";
};
}  // namespace verificationModule

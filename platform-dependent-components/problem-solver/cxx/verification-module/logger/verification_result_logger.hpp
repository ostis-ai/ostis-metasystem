/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <fstream>

#include "dataStructures/set_check_result.hpp"

namespace verificationModule
{
class VerificationResultLogger
{
public:
  void logSetCheckResult(SetCheckResult const & checkResult, std::ofstream & logFile);

  void logSetInfo(SetCheckResult const & checkResult, std::ofstream & logFile);

  void logElementCheckResult(ElementCheckResult const & checkResult, std::ofstream & logFile);

private:
  std::string const INDENT = "    ";
};
}  // namespace verificationModule

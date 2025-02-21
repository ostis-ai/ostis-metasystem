/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "sc-memory/sc_memory.hpp"

namespace verificationModule
{
class VerificationConstants
{
public:
  static ScAddr const LANG;
};

class FileConfigs
{
public:
  static std::string const FILE_PATH;
  static std::string const VERIFICATION_ENDPOINT;
  static std::string const NOT_ENOUGH_SPECIFIED_OBJECTS_FILE_NAME;
};

}  // namespace verificationModule

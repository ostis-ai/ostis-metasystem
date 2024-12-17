/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "verification_constants.hpp"
#include "keynodes/verification_keynodes.hpp"

namespace verificationModule
{
ScAddr const VerificationConstants::LANG = VerificationKeynodes::lang_en;

std::string const FileConfigs::FILE_PATH = "file_path";
std::string const FileConfigs::VERIFICATION_ENDPOINT = "verification";
std::string const FileConfigs::KEY_NODE = "key_node";
std::string const FileConfigs::TRANSLATION_NODE = "translation_node";

}  // namespace verificationModule

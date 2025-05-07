/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <filesystem>

#include <sc-memory/sc_agent.hpp>

#include "manager/duplications_check_manager.hpp"
#include "handler/verification_result_file_handler.hpp"

namespace verificationModule
{
class CheckDuplicateConstructionsAgent : public ScActionInitiatedAgent
{
public:
  ScAddr GetActionClass() const override;

  ScResult DoProgram(ScActionInitiatedEvent const & event, ScAction & action) override;

private:
  std::filesystem::path filePath;
  std::unique_ptr<DuplicationsCheckManager> duplicationsCheckManager;
  std::unique_ptr<VerificationResultFileHandler> fileHandler;

  ScAddr FormResultStructure(ScAddrUnorderedSet const & resultElements) const;

  void RunCheck(ScAddr const & classAddr, ScAddrUnorderedSet & resultElements) const;
};

}  // namespace verificationModule

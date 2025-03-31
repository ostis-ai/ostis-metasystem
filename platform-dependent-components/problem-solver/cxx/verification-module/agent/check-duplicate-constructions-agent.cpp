/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include <fstream>
#include "sc-memory/sc_keynodes.hpp"

#include "constants/verification_constants.hpp"
#include "keynodes/verification_keynodes.hpp"
#include "manager/duplications_check_manager.hpp"
#include "handler/verification_result_file_handler.hpp"
#include "logger/verification_result_logger.hpp"
#include "config/config.hpp"

#include "check-duplicate-constructions-agent.hpp"

using namespace utils;

namespace verificationModule
{
ScResult CheckDuplicateConstructionsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  SC_LOG_DEBUG("CheckDuplicateConstructionsAgent started");

  filePath = Config::getInstance()->getValue(
      FileConfigs::VERIFICATION_ENDPOINT, FileConfigs::FILE_PATH);

  auto [classAddr] = action.GetArguments<1>();

  if (m_context.IsElement(classAddr))
  {
    SC_AGENT_LOG_INFO("CheckDuplicateConstructionsAgent: running check for "
                      << m_context.GetElementSystemIdentifier(classAddr));
    runCheck(classAddr);
  }
  else
  {
    SC_AGENT_LOG_INFO("CheckDuplicateConstructionsAgent: argument not found, running check for all classes");
    //todo: replace specialized "class_node" with sc_node_class after corresponding changes to sc-machine
    ScIterator3Ptr classesIterator = m_context.CreateIterator3(
        VerificationKeynodes::class_node, ScType::ConstPermPosArc, ScType::Unknown);
    while (classesIterator->Next())
      runCheck(classesIterator->Get(2));
  }

  SC_LOG_DEBUG("CheckDuplicateConstructionsAgent finished");

  return action.FinishSuccessfully();
}

ScAddr CheckDuplicateConstructionsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_check_duplicate_constructions;
}

void CheckDuplicateConstructionsAgent::runCheck(ScAddr const & classAddr) const
{
  try
  {
    SetCheckResult setCheckResult;
    DuplicationsCheckManager duplicationsCheckManager(&m_context);
    duplicationsCheckManager.checkSetElementsDuplications(classAddr, setCheckResult);

    VerificationResulFileHandler fileHandler(&m_context);
    std::ofstream outputFile = fileHandler.createOutputFile(filePath, setCheckResult.setIdtf, classAddr);

    VerificationResultLogger resultLogger;
    resultLogger.logSetCheckResult(setCheckResult, outputFile);
  }
  catch (utils::ScException const & exception)
  {
    SC_AGENT_LOG_ERROR("CheckDuplicateConstructionsAgent: error during "
                       << m_context.GetElementSystemIdentifier(classAddr) << " processing.");
  }
}

}  // namespace verificationModule

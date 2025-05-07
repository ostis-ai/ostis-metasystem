/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include <fstream>
#include <sc-memory/sc_keynodes.hpp>
#include <sc-memory/sc_agent.hpp>

#include "constants/verification_constants.hpp"
#include "keynodes/verification_keynodes.hpp"
#include "logger/verification_result_logger.hpp"
#include "data-structures/set_duplications_check_result.hpp"
#include "config/config.hpp"

#include "check-duplicate-constructions-agent.hpp"
#include "utils/identifier_utils.hpp"

using namespace utils;

namespace verificationModule
{
ScResult CheckDuplicateConstructionsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  filePath = Config::GetInstance()->GetValue(FileConfigs::VERIFICATION_ENDPOINT, FileConfigs::FILE_PATH);

  auto [classAddr] = action.GetArguments<1>();

  duplicationsCheckManager = std::make_unique<DuplicationsCheckManager>(&m_context);
  fileHandler = std::make_unique<VerificationResultFileHandler>(&m_context);

  ScAddrUnorderedSet resultElements;
  if (m_context.IsElement(classAddr))
    RunCheck(classAddr, resultElements);
  else
  {
    m_logger.Info("Argument not found, running check for all classes");
    // todo (NikiforovSergei):
    //  replace specialized "class_node" with sc_node_class after corresponding changes to sc-machine
    ScIterator3Ptr classesIterator =
        m_context.CreateIterator3(VerificationKeynodes::entity_class, ScType::ConstPermPosArc, ScType::Unknown);
    while (classesIterator->Next())
      RunCheck(classesIterator->Get(2), resultElements);
  }

  ScAddr const & resultStructure = FormResultStructure(resultElements);
  action.SetResult(resultStructure);

  SC_LOG_DEBUG("CheckDuplicateConstructionsAgent finished");
  return action.FinishSuccessfully();
}

ScAddr CheckDuplicateConstructionsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_check_duplicate_constructions;
}

ScAddr CheckDuplicateConstructionsAgent::FormResultStructure(ScAddrUnorderedSet const & resultElements) const
{
  ScAddr const & resultStructure = m_context.GenerateNode(ScType::ConstNodeStructure);
  for (auto const & element : resultElements)
    m_context.GenerateConnector(ScType::ConstPermPosArc, resultStructure, element);

  return resultStructure;
}

void CheckDuplicateConstructionsAgent::RunCheck(ScAddr const & classAddr, ScAddrUnorderedSet & resultElements) const
{
  try
  {
    m_logger.Info("Running check for ", IdentifierUtils::GetIdentifiersString(&m_context, classAddr));

    SetDuplicationsCheckResult setCheckResult;
    if (!duplicationsCheckManager->CheckSetElementsDuplications(classAddr, setCheckResult))
      return;

    std::ofstream outputFile =
        fileHandler->CreateOutputFile(filePath, setCheckResult.setIdtf, classAddr, resultElements);

    VerificationResultLogger resultLogger;
    resultLogger.LogSetCheckResult(setCheckResult, outputFile);
  }
  catch (utils::ScException const & exception)
  {
    m_logger.Error(
        "Error during ", IdentifierUtils::GetIdentifiersString(&m_context, classAddr),
        " processing: ", exception.Description());
  }
}

}  // namespace verificationModule

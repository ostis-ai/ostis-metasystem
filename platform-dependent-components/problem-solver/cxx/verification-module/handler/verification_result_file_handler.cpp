/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/verification_keynodes.hpp"
#include "constants/verification_constants.hpp"

#include "verification_result_file_handler.hpp"

namespace verificationModule
{

VerificationResulFileHandler::VerificationResulFileHandler(ScMemoryContext * context) : context(context)
{
}

std::ofstream VerificationResulFileHandler::createOutputFile(
    std::filesystem::path const & filePath, std::string const & checkedElementIdtf, ScAddr const & checkedElementAddr) const
{
  std::filesystem::create_directories(filePath);

  std::string fileName ="duplications_check_result_file_for_" + checkedElementIdtf;
  std::filesystem::path fileFullNAme = filePath / fileName;

  SC_LOG_INFO("Duplications file for " << checkedElementIdtf << " is " << fileFullNAme.string());

  resolveFileSpecification(checkedElementAddr, fileFullNAme);

  return std::ofstream(fileFullNAme, std::ios::app);
}

void VerificationResulFileHandler::resolveFileSpecification(
    ScAddr const & checkedElementAddr,
    std::filesystem::path const & fileFullNAme) const
{
  ScAddr fileAddr;
  ScIterator5Ptr const & fileIterator = context->CreateIterator5(
      checkedElementAddr,
      ScType::ConstPermPosArc,
      ScType::ConstNodeLink,
      ScType::ConstCommonArc,
      VerificationKeynodes::nrel_duplicate_construction_file);
  if (fileIterator->Next())
    context->SetLinkContent(fileIterator->Get(2), fileFullNAme);
  else
    generateDuplicationFileSpecification(checkedElementAddr, fileFullNAme);
}

void VerificationResulFileHandler::generateDuplicationFileSpecification(
    ScAddr const & checkedElementAddr,
    std::filesystem::path const & fileFullNAme) const
{
  ScAddr const & fileAddr = context->GenerateLink(ScType::ConstNodeLink);
  context->SetLinkContent(fileAddr, fileFullNAme);
  ScAddr const & edge = context->GenerateConnector(ScType::ConstCommonArc, checkedElementAddr, fileAddr);
  ScAddr const & relationEdge =
      context->GenerateConnector(ScType::ConstPermPosArc, VerificationKeynodes::nrel_duplicate_construction_file, edge);
}

}  // namespace verificationModule

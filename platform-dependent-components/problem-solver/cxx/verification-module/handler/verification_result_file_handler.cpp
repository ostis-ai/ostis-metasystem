/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/verification_keynodes.hpp"
#include "constants/verification_constants.hpp"
#include "utils/container_utils.hpp"

#include "verification_result_file_handler.hpp"

namespace verificationModule
{

VerificationResultFileHandler::VerificationResultFileHandler(ScMemoryContext * context)
  : context(context)
{
}

std::ofstream VerificationResultFileHandler::createOutputFile(
    std::filesystem::path const & filePath,
    std::string const & checkedElementIdtf,
    ScAddr const & checkedElementAddr,
    ScAddrUnorderedSet & resultElements) const
{
  std::filesystem::create_directories(filePath);

  std::string fileName = "duplications_check_result_file_for_" + checkedElementIdtf;
  std::filesystem::path fileFullName = filePath / fileName;

  SC_LOG_INFO("Duplications file for " << checkedElementIdtf << " is " << fileFullName.string());

  resolveFileSpecification(checkedElementAddr, fileFullName, resultElements);

  return {fileFullName, std::ios::app};
}

void VerificationResultFileHandler::resolveFileSpecification(
    ScAddr const & checkedElementAddr,
    std::filesystem::path const & fileFullName,
    ScAddrUnorderedSet & resultElements) const
{
  ScAddr fileAddr;
  ScIterator5Ptr const & fileIterator = context->CreateIterator5(
      checkedElementAddr,
      ScType::ConstCommonArc,
      ScType::ConstNodeLink,
      ScType::ConstPermPosArc,
      VerificationKeynodes::nrel_duplicate_construction_file);
  if (fileIterator->Next())
  {
    ScAddr const & fileAddrLink = fileIterator->Get(2);
    context->SetLinkContent(fileAddrLink, fileFullName);

    ContainerUtils::insertSeveral(
        resultElements,
        {checkedElementAddr,
        fileIterator->Get(1),
        fileAddrLink,
        fileIterator->Get(3),
        VerificationKeynodes::nrel_duplicate_construction_file});
  }
  else
    generateDuplicationFileSpecification(checkedElementAddr, fileFullName, resultElements);
}

void VerificationResultFileHandler::generateDuplicationFileSpecification(
    ScAddr const & checkedElementAddr,
    std::filesystem::path const & fileFullName,
    ScAddrUnorderedSet & resultElements) const
{
  ScAddr const & fileAddrLink = context->GenerateLink(ScType::ConstNodeLink);
  context->SetLinkContent(fileAddrLink, fileFullName);
  ScAddr const & relationPair = context->GenerateConnector(ScType::ConstCommonArc, checkedElementAddr, fileAddrLink);
  ScAddr const & relationAccessArc =
      context->GenerateConnector(
          ScType::ConstPermPosArc, VerificationKeynodes::nrel_duplicate_construction_file, relationPair);

  ContainerUtils::insertSeveral(
      resultElements,
      {checkedElementAddr,
      relationPair,
      fileAddrLink,
      relationAccessArc,
      VerificationKeynodes::nrel_duplicate_construction_file});
}
}  // namespace verificationModule

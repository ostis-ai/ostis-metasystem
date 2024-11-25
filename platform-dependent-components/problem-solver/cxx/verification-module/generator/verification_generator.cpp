/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/CommonUtils.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "verification_generator.hpp"

namespace verificationModule
{
std::ofstream verificationGenerator::generateDuplicationFile(
    ScMemoryContext * context,
    ScAddr const & classAddr,
    std::filesystem::path const & filename)
{
  std::ofstream outputFile = std::ofstream(filename, std::ios::app);

  ScAddr const & fileAddr = context->GenerateLink(ScType::ConstNodeLink);
  context->SetLinkContent(fileAddr, filename);
  ScAddr const & edge = context->GenerateConnector(ScType::ConstCommonArc, classAddr, fileAddr);
  ScAddr const & relationEdge =
      context->GenerateConnector(ScType::ConstPermPosArc, VerificationKeynodes::nrel_duplicate_construction_file, edge);

  return outputFile;
}

}  // namespace verificationModule

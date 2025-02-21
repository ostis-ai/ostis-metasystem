/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "filesystem"
#include "optional"

#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "constants/verification_constants.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "generator/verification_generator.hpp"

#include "check_duplicate_constructions_searcher.hpp"

namespace verificationModule
{
checkDuplicateConstructionSearcher::checkDuplicateConstructionSearcher(ScMemoryContext * context)
  : context(context)
{
}

std::ofstream checkDuplicateConstructionSearcher::openOrCreateFile(
    ScMemoryContext * context,
    ScAddr & classAddr,
    std::filesystem::path const & filename)
{
  ScAddr fileAddr;
  ScIterator5Ptr const & fileIterator = context->CreateIterator5(
      classAddr,
      ScType::ConstPermPosArc,
      ScType::Node,
      ScType::ConstCommonArc,
      VerificationKeynodes::nrel_duplicate_construction_file);
  if (fileIterator->Next())
  {
    fileAddr = fileIterator->Get(2);
    std::string fileIdtf =
        utils::CommonUtils::getIdtf(context, fileAddr, ScKeynodes::nrel_main_idtf, {VerificationConstants::LANG});
    std::filesystem::path outputFile = filename.c_str() + fileIdtf;
    return std::ofstream(outputFile, std::ios::app);
  }
  else
  {
    std::ofstream outputFile = verificationGenerator::generateDuplicationFile(context, classAddr, filename);
    return outputFile;
  }
}

}  // namespace verificationModule

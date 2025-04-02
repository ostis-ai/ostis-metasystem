/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include <fstream>
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "config/config.hpp"
#include "constants/verification_constants.hpp"
#include "keynodes/verification_keynodes.hpp"

#include "check_homonyms_agent.hpp"

namespace verificationModule
{

ScResult CheckHomonymsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  try
  {
    auto const & [identifierRelation] = action.GetArguments<1>();
    if (!m_context.IsElement(identifierRelation))
      SC_THROW_EXCEPTION(utils::ExceptionItemNotFound, "Identifier relation is not found");
    std::string static const & outputFileLocationStr = Config::getInstance()->getValue(FileConfigs::VERIFICATION_ENDPOINT, FileConfigs::FILE_PATH);
    if (outputFileLocationStr.empty())
      SC_THROW_EXCEPTION(utils::ExceptionInvalidState, "Output directory not found, use group " << FileConfigs::VERIFICATION_ENDPOINT << " and field " << FileConfigs::FILE_PATH << " in config.ini file.");
    std::unordered_map<std::string, ScAddrUnorderedSet> identifiers;
    searchIdentifiers(identifierRelation, identifiers);
    SC_LOG_INFO("found " << identifiers.size() << " distinct identifiers");
    std::stringstream synonymsInfo;
    fillSynonymsInfo(synonymsInfo, identifiers);

    ScAddr const & resultLink = m_context.GenerateLink(ScType::ConstNodeLink);
    m_context.SetLinkContent(resultLink, synonymsInfo.str());
    SC_LOG_INFO("homonyms for identifier " << m_context.GetElementSystemIdentifier(identifierRelation) << " have content\n" << synonymsInfo.str());

  SC_LOG_INFO("output file path: " << outputFileLocationStr);
    std::filesystem::create_directories(outputFileLocationStr);

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", localTime);

    std::string const & outputFileName = outputFileLocationStr + "/" + buffer + "_homonyms_" + m_context.GetElementSystemIdentifier(identifierRelation) + ".txt";
    auto outputFile = generateSynonymFile(identifierRelation, outputFileName);
    if (!outputFile.is_open())
      SC_THROW_EXCEPTION(utils::ExceptionInvalidState, "Failed to open file " << outputFileName);
    outputFile << synonymsInfo.str();
    outputFile.close();
    action.SetResult(resultLink);

    return action.FinishSuccessfully();
  } catch (utils::ScException const & exception)
  {
    SC_LOG_ERROR(exception.Description());
    return action.FinishWithError();
  }
}

void CheckHomonymsAgent::searchIdentifiers(
  ScAddr const & identifierRelation,
  std::unordered_map<std::string, ScAddrUnorderedSet> & identifiers) const
{
  auto const & identifierIterator = m_context.CreateIterator5(
    ScType::Node,
    ScType::ConstCommonArc,
    ScType::NodeLink,
    ScType::ConstPermPosArc,
    identifierRelation
    );
  while (identifierIterator->Next())
  {
    std::string identifierValue;
    m_context.GetLinkContent(identifierIterator->Get(2), identifierValue);
    identifiers[identifierValue].insert(identifierIterator->Get(0));
  }
}

void CheckHomonymsAgent::fillSynonymsInfo(
  std::stringstream & synonymsInfo,
  std::unordered_map<std::string, ScAddrUnorderedSet> const & identifiers) const
{
  for (auto const & [identifier, nodes] : identifiers)
  {
    if (nodes.size() > 1)
    {
      synonymsInfo << "Found " << nodes.size() << " repeated elements with identifier " << identifier << ".\n";
      for (ScAddr const & node : nodes)
      {
        synonymsInfo << "element";
        fillNodeInfo(synonymsInfo, node);
        fillSubjectDomainInfo(synonymsInfo, node);
        fillClassesInfo(synonymsInfo, node);
      }
      synonymsInfo << "\n";
    }
  }
}

void CheckHomonymsAgent::fillSubjectDomainInfo(std::stringstream & synonymsInfo, ScAddr const & node) const
{
  auto const & structuresIterator = m_context.CreateIterator3(
    ScType::ConstNodeStructure,
    ScType::ConstPermPosArc,
    node
  );
  while (structuresIterator->Next())
  {
    ScAddr const & subjectDomain = structuresIterator->Get(0);
    if (!m_context.CheckConnector(VerificationKeynodes::subject_domain, subjectDomain, ScType::ConstPermPosArc))
      continue;
    synonymsInfo << " is in subject domain";
    fillNodeInfo(synonymsInfo, subjectDomain);
  }
}

void CheckHomonymsAgent::fillClassesInfo(std::stringstream & synonymsInfo, ScAddr const & node) const
{
  auto const & classesIterator = m_context.CreateIterator3(
    ScType::NodeClass,
    ScType::ConstPermPosArc,
    node
  );
  if (classesIterator->Next())
  {
    synonymsInfo << " is in class";
    do
    {
      ScAddr const & classAddr = classesIterator->Get(0);
      fillNodeInfo(synonymsInfo, classAddr);
    } while (classesIterator->Next());
  }
  else
    synonymsInfo << " is not in any class.";
  synonymsInfo << "\n";
}

void CheckHomonymsAgent::fillNodeInfo(std::stringstream & synonymsInfo, ScAddr const & node) const
{
  std::string const systemIdentifier = m_context.GetElementSystemIdentifier(node);
  if (systemIdentifier.empty())
    synonymsInfo << " with hash `" << node.Hash() << "`";
  else
    synonymsInfo << " with system identifier " << systemIdentifier;
}

std::ofstream CheckHomonymsAgent::generateSynonymFile(
    ScAddr const & classAddr,
    std::string const & filename)
{
  std::ofstream outputFile = std::ofstream(filename, std::ios::app);

  ScAddr const & fileAddr = m_context.GenerateLink(ScType::ConstNodeLink);
  m_context.SetLinkContent(fileAddr, filename);
  ScAddr const & arc = m_context.GenerateConnector(ScType::ConstCommonArc, classAddr, fileAddr);
  m_context.GenerateConnector(ScType::ConstPermPosArc, VerificationKeynodes::nrel_synonym_objects_file, arc);

  return outputFile;
}

ScAddr CheckHomonymsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_check_homonyms;
}

}  // namespace verificationModule

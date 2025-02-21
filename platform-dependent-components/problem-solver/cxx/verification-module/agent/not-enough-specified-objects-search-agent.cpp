/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <filesystem>

#include <sc-memory/sc_keynodes.hpp>
#include "sc-agents-common/utils/IteratorUtils.hpp"

#include "config/config.hpp"

#include "constants/verification_constants.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "not-enough-specified-objects-search-agent.hpp"

using namespace utils;

namespace verificationModule
{

std::filesystem::path SearchNotEnoughSpecifiedObjectsAgent::filePath;

ScResult SearchNotEnoughSpecifiedObjectsAgent::DoProgram(ScAction & action)
{
  // list of  <templAddr, elements>
  std::vector<std::pair<ScAddr, ScAddrVector>> templAddrAndElementsList = GetSpecificationTemplateAndElements();

  std::stringstream infoAboutBasicSpec;
  std::string nodeName;
  std::string subjectDomainName;

  filePath = Config::getInstance()->getValue(FileConfigs::VERIFICATION_ENDPOINT, FileConfigs::FILE_PATH);

  std::for_each(
      std::begin(templAddrAndElementsList),
      std::end(templAddrAndElementsList),
      [&](std::pair<ScAddr, ScAddrVector> & templAddrAndElements)
      {
        ScAddr const & templAddr = templAddrAndElements.first;
        ScAddrVector const & elementsAddrs = templAddrAndElements.second;
        std::for_each(
            std::begin(elementsAddrs),
            std::end(elementsAddrs),
            [&](ScAddr const & elementAddr)
            {
              try
              {
                SC_LOG_INFO(m_context.GetElementSystemIdentifier(elementAddr));
                CheckTemplate(m_context, elementAddr, templAddr);
              }
              catch (utils::ScException const & exception)  // No template or no specification
              {
                SC_LOG_ERROR(exception.Description());
                getNodeSystemId(infoAboutBasicSpec, elementAddr);
                getNodeSubjectDomainsId(infoAboutBasicSpec, elementAddr);
              }
            });
      });

  bool const & resultOfWrite = WriteInFile(infoAboutBasicSpec.str());
  if (!resultOfWrite)
  {
    SC_AGENT_LOG_ERROR("File hasn't been created");
    return action.FinishUnsuccessfully();
  }
  SC_AGENT_LOG_DEBUG("File has been created");
  return action.FinishSuccessfully();
}

ScAddr SearchNotEnoughSpecifiedObjectsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_search_not_enough_specified_objects;
}

std::vector<std::pair<ScAddr, ScAddrVector>> SearchNotEnoughSpecifiedObjectsAgent::GetSpecificationTemplateAndElements()
    const
{
  std::vector<std::pair<ScAddr, ScAddrVector>> templAddrAndElementsList;
  ScIterator5Ptr const & itTemplate = m_context.CreateIterator5(
      ScType::ConstNodeStructure, //specificationAddr
      ScType::ConstCommonArc,
      ScType::ConstNodeStructure, // templAddr
      ScType::ConstMembershipArc,
      VerificationKeynodes::nrel_template);
  std::pair<ScAddr, ScAddrVector> templAddrAndElements;
  while (itTemplate->Next())
  {
    templAddrAndElements.first = itTemplate->Get(2);
    ScAddr decompositionAddr = utils::IteratorUtils::getAnyByOutRelation(
        &m_context, itTemplate->Get(0), VerificationKeynodes::nrel_decomposition);
    if (!decompositionAddr.IsValid()) // template without elements classes
      continue;
    ScIterator3Ptr const & itElements =
        m_context.CreateIterator3(decompositionAddr, ScType::ConstMembershipArc, ScType::Unknown);
    ScAddrVector elements;
    while (itElements->Next())
    {
      elements = GetElements(m_context, itElements->Get(2)); // elements of classes
      templAddrAndElements.second.insert(templAddrAndElements.second.end(), elements.begin(), elements.end());
    }
    if (templAddrAndElements.second.empty()) // template without classes elements
      continue;
    templAddrAndElementsList.push_back(templAddrAndElements);
  }
  return templAddrAndElementsList;
}

void SearchNotEnoughSpecifiedObjectsAgent::getNodeSystemId(
    std::stringstream & infoAboutBasicSpec,
    ScAddr const & nodeAddr) const
{
  if (!nodeAddr.IsValid())
    SC_LOG_ERROR("Invalid given node.");
  infoAboutBasicSpec << "system id: \n";
  infoAboutBasicSpec << "\t" << m_context.GetElementSystemIdentifier(nodeAddr) << "\n";
}

void SearchNotEnoughSpecifiedObjectsAgent::getNodeSubjectDomainsId(
    std::stringstream & infoAboutBasicSpec,
    ScAddr const & nodeAddr) const
{
  ScAddrVector subjectDomainAddrs = GetSubjectDomainAddr(nodeAddr);
  if (subjectDomainAddrs.empty())
    return;
  infoAboutBasicSpec << "subject domains: \n";
  std::for_each(
      std::begin(subjectDomainAddrs),
      std::end(subjectDomainAddrs),
      [&](ScAddr const & subjectDomainAddr)
      {
        infoAboutBasicSpec << "\t" << m_context.GetElementSystemIdentifier(subjectDomainAddr) << "\n";
      });
  infoAboutBasicSpec << "\n";
}

ScAddrVector SearchNotEnoughSpecifiedObjectsAgent::GetSubjectDomainAddr(ScAddr const & nodeAddr) const
{
  ScAddrVector subjectDomainAddrs;
  ScIterator3Ptr it = m_context.CreateIterator3(ScType::ConstNodeStructure, ScType::ConstMembershipArc, nodeAddr);
  while (it->Next())
  {
    if (m_context.CheckConnector(VerificationKeynodes::subject_domain, it->Get(0), ScType::ConstMembershipArc))
      subjectDomainAddrs.push_back(it->Get(0));
  }
  return subjectDomainAddrs;
}

ScAddrVector SearchNotEnoughSpecifiedObjectsAgent::GetElements(ScMemoryContext & m_context, ScAddr const & class_node)
{
  ScAddrVector arguments;
  ScIterator3Ptr const & it = m_context.CreateIterator3(class_node, ScType::ConstPermPosArc, ScType::Unknown);
  while (it->Next())
    arguments.push_back(it->Get(2));

  return arguments;
}

bool SearchNotEnoughSpecifiedObjectsAgent::CheckTemplate(
    ScMemoryContext & m_context,
    ScAddr const & argument,
    ScAddr const & templAddr)
{
  if (!templAddr.IsValid() || m_context.GetElementType(templAddr) != ScType::ConstNodeStructure)
  {
    SC_THROW_EXCEPTION(utils::ScException, "Class of the element doesn't have the template of the basic specification.");
  }
  ScTemplateSearchResult result;
  ScTemplate templ;
  ScTemplateParams templParams;
  ScAddr const & keyScElementOfTempl =
      utils::IteratorUtils::getAnyByOutRelation(&m_context, templAddr, ScKeynodes::rrel_key_sc_element);
  if (!keyScElementOfTempl.IsValid())
    return false;
  templParams.Add(keyScElementOfTempl, argument);
  m_context.BuildTemplate(templ, templAddr, templParams);
  m_context.SearchByTemplate(templ, result);
  if (result.IsEmpty())
  {
    SC_THROW_EXCEPTION(utils::ScException, "Element doesn't have the full basic specification.");
  }
  return true;
}

bool SearchNotEnoughSpecifiedObjectsAgent::WriteInFile(std::string const & strIdentifiers)
{
  std::filesystem::create_directories(filePath);
  std::filesystem::path directoryPath = filePath / FileConfigs::NOT_ENOUGH_SPECIFIED_OBJECTS_FILE_NAME;
  SC_LOG_DEBUG(directoryPath);
  try
  {
    std::fstream file(directoryPath.c_str(), std::fstream::out);
    if (file.is_open())
    {
      file << strIdentifiers;
    }
    file.close();
  }
  catch (std::exception const & exception)
  {
    SC_LOG_ERROR(exception.what());
    return false;
  }
  return true;
}

}  // namespace verificationModule
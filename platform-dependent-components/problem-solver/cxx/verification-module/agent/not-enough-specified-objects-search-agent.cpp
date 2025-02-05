/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <fstream>
#include <filesystem>
#include "config/config.hpp"
#include "constants/verification_constants.hpp"

#include <sc-memory/sc_keynodes.hpp>

#include "keynodes/verification_keynodes.hpp"

#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"

#include "not-enough-specified-objects-search-agent.hpp"

using namespace utils;

namespace verificationModule
{

std::filesystem::path SearchNotEnoughSpecifiedObjectsAgent::filePath;

ScResult SearchNotEnoughSpecifiedObjectsAgent::DoProgram(ScAction & action)
{
  auto const classNode = action.GetArgument(1);
  if (!classNode.IsValid())
  {
    SC_LOG_INFO("Invalid argument");
    return action.FinishUnsuccessfully();
  }

  ScType classNodeType = m_context.GetElementType(classNode);
  if (!scTypeAndActionClass.at(classNodeType).IsValid())
  {
    SC_LOG_INFO("Invalid type of the argument");
  }

  ScAddrVector const & arguments = GetElements(m_context, classNode);
  std::stringstream infoAboutBasicSpec;
  std::string nodeName;
  std::string subjectDomainName;
  filePath = Config::getInstance()->getValue(FileConfigs::VERIFICATION_ENDPOINT, FileConfigs::FILE_PATH);


  std::for_each(
      std::begin(arguments),
      std::end(arguments),
      [&](ScAddr const & argument)
      {
        if (CheckTemplate(m_context, argument, classNode))
          return;
        getInfoAboutNode(infoAboutBasicSpec, argument);
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

void SearchNotEnoughSpecifiedObjectsAgent::getInfoAboutNode(
    std::stringstream & infoAboutBasicSpec,
    ScAddr const & nodeAddr) const
{
  infoAboutBasicSpec << m_context.GetElementSystemIdentifier(nodeAddr) << "\n";
  ScAddrVector subjectDomainAddrs = GetSubjectDomainAddr(nodeAddr);
  if (subjectDomainAddrs.empty())
    return;
  std::for_each(
      std::begin(subjectDomainAddrs),
      std::end(subjectDomainAddrs),
      [&](ScAddr const & subjectDomainAddr)
      {
        infoAboutBasicSpec << m_context.GetElementSystemIdentifier(subjectDomainAddr) << "\n";
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

ScAddr SearchNotEnoughSpecifiedObjectsAgent::GetTemplateAddr(ScMemoryContext & m_context, ScAddr const & classNode)
{
  ScAddr argTemplate;
  ScIterator3Ptr const & it = m_context.CreateIterator3(ScType::ConstNodeTuple, ScType::ConstMembershipArc, classNode);
  while (it->Next())
  {
    ScAddr const & specificationAddr =
        utils::IteratorUtils::getAnyByInRelation(&m_context, it->Get(0), VerificationKeynodes::nrel_decomposition);
    if (!specificationAddr.IsValid())
      return argTemplate;
    argTemplate =
        utils::IteratorUtils::getAnyByOutRelation(&m_context, specificationAddr, VerificationKeynodes::nrel_template);
    if (!argTemplate.IsValid())
      SC_LOG_DEBUG("There's no template of the basic specification.");
    return argTemplate;
  }
  return argTemplate;
}

bool SearchNotEnoughSpecifiedObjectsAgent::CheckTemplate(
    ScMemoryContext & m_context,
    ScAddr const & argument,
    ScAddr const & classNode)
{
  ScAddr const & templAddr = GetTemplateAddr(m_context, classNode);
  if (!templAddr.IsValid())
  {
    SC_LOG_ERROR("Class of the element doesn't have the template of a basic specification.");
    return false;
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
    SC_LOG_ERROR("Element doesn't have the full basic specification.");
    return false;
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
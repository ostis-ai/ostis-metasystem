/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "keynodes/identifiers_keynodes.hpp"
#include "translate_main_system_idtfs_from_sc_to_file_agent.hpp"

using namespace identifiersModule;

ScResult TranslateMainSystemIdtfsFromScToFileAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  // TODO: replace by ScKeynodes::nrel_system_identifier after release
  ScAddr const & nrelSystemIdtf = m_context.SearchElementBySystemIdentifier("nrel_system_identifier");

  std::stringstream streamIdtfs;

  ScIterator3Ptr const & iterator3PtrEdgeBelongsToNrelSystemIdtf =
      m_context.CreateIterator3(nrelSystemIdtf, ScType::ConstPermPosArc, ScType::ConstCommonArc);

  ScAddr edgeBelongsToNrelSystemIdtf;
  ScAddr sourceOfEdgeBelongsToNrelSystemIdtf;

  std::string systemIdentifier;
  std::string mainIdentifier;
  std::string stringType;

  while (iterator3PtrEdgeBelongsToNrelSystemIdtf->Next())
  {
    edgeBelongsToNrelSystemIdtf = iterator3PtrEdgeBelongsToNrelSystemIdtf->Get(2);
    sourceOfEdgeBelongsToNrelSystemIdtf = m_context.GetArcSourceElement(edgeBelongsToNrelSystemIdtf);
    try
    {
      systemIdentifier = GetSystemIdtfAndVerifyNode(m_context, sourceOfEdgeBelongsToNrelSystemIdtf);
      mainIdentifier = GetMainIdtfAndVerifyNode(m_context, sourceOfEdgeBelongsToNrelSystemIdtf);
      stringType = GetStrScType(sourceOfEdgeBelongsToNrelSystemIdtf);

      if (!systemIdentifier.empty() && !mainIdentifier.empty() && !stringType.empty())
      {
        streamIdtfs << R"({")" << mainIdentifier << R"(", )"
                    << R"({")" << systemIdentifier << R"(", ")" << stringType << R"("}},)" << "\n";
      }
    }
    catch (utils::ScException const & exception)
    {
      SC_LOG_ERROR(exception.Description());

      ScStructure result = m_context.GenerateStructure();
      result << action;
      action.SetResult(result);
      return action.FinishUnsuccessfully();
    }
  }

  std::string strIdtfs(streamIdtfs.str());
  // Remove last symbols "," and "\n"
  if (!strIdtfs.empty())
  {
    strIdtfs.pop_back();
    if (!strIdtfs.empty())
    {
      strIdtfs.pop_back();
    }
  }

  bool const & resultOfWrite = WriteInFile(strIdtfs);

  if (resultOfWrite)
  {
    SC_AGENT_LOG_DEBUG("File has been created");
    return action.FinishSuccessfully();
  }
  else
  {
    SC_AGENT_LOG_ERROR("File hasn't been created");
    return action.FinishUnsuccessfully();
  }
}

ScAddr TranslateMainSystemIdtfsFromScToFileAgent::GetActionClass() const
{
  return IdentifiersKeynodes::action_find_identifiers;
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::GetSystemIdtfAndVerifyNode(
    ScMemoryContext & m_context,
    ScAddr const & node)
{
  std::string identifier;
  ScAddr identifierLink;
  ScIterator5Ptr const & iterator5PtrCheckOnlyOneIdtf = m_context.CreateIterator5(
      node,
      ScType::ConstCommonArc,
      ScType::ConstNodeLink,
      ScType::ConstPermPosArc,
      m_context.SearchElementBySystemIdentifier("nrel_system_identifier"));

  if (iterator5PtrCheckOnlyOneIdtf->Next())
  {
    identifierLink = iterator5PtrCheckOnlyOneIdtf->Get(2);
    if (iterator5PtrCheckOnlyOneIdtf->Next())
      SC_THROW_EXCEPTION(utils::ScException, "You have more than one system identifier for " + identifier);
    m_context.GetLinkContent(identifierLink, identifier);
  }
  return identifier;
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::GetMainIdtfAndVerifyNode(
    ScMemoryContext & m_context,
    ScAddr const & node)
{
  std::string identifier;
  ScAddr mainIdentifierLink;
  ScAddr mainAnotherIdentifierLink;
  ScIterator5Ptr const & iterator5PtrCheckOnlyOneIdtf = m_context.CreateIterator5(
      node, ScType::ConstCommonArc, ScType::ConstNodeLink, ScType::ConstPermPosArc, ScKeynodes::nrel_main_idtf);

  bool isLangRu;
  while (iterator5PtrCheckOnlyOneIdtf->Next())
  {
    mainIdentifierLink = iterator5PtrCheckOnlyOneIdtf->Get(2);
    isLangRu = m_context.CheckConnector(ScKeynodes::lang_ru, mainIdentifierLink, ScType::ConstPermPosArc);

    if (isLangRu)
    {
      while (iterator5PtrCheckOnlyOneIdtf->Next())
      {
        mainAnotherIdentifierLink = iterator5PtrCheckOnlyOneIdtf->Get(2);
        isLangRu =
            m_context.CheckConnector(ScKeynodes::lang_ru, mainAnotherIdentifierLink, ScType::ConstPermPosArc);

        if (isLangRu)
          return identifier;
      }
      m_context.GetLinkContent(mainIdentifierLink, identifier);
      break;
    }
  }
  return identifier;
}

std::string TranslateMainSystemIdtfsFromScToFileAgent::GetStrScType(ScAddr const & node)
{
  std::string strType;
  ScType const & type = m_context.GetElementType(node);
  if (ScTypesOfNodesWithSCsClasses.count(type))
    strType = ScTypesOfNodesWithSCsClasses[type];
  else if (ScTypesOfEdgesWithSCsClasses.count(type))
    strType = ScTypesOfEdgesWithSCsClasses[type];
  return strType;
}

bool TranslateMainSystemIdtfsFromScToFileAgent::WriteInFile(std::string const & strIdentifiers)
{
  try
  {
    std::ofstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
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

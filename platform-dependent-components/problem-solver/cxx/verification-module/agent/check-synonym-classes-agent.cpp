/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include <fstream>
#include "tuple"
#include "filesystem"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-memory/sc_keynodes.hpp"

#include "constants/verification_constants.hpp"
#include "keynodes/verification_keynodes.hpp"

#include "check-synonym-classes-agent.hpp"

using namespace utils;

namespace verificationModule
{

ScResult CheckSynonymClassesAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [classAddr] = action.GetArguments<1>();
  if (!m_context.IsElement(classAddr))
  {
    SC_AGENT_LOG_ERROR("CheckSynonymClassesAgent: class not found.");
    return action.FinishUnsuccessfully();
  }

  SC_LOG_DEBUG("CheckSynonymClassesAgent started");

  try
  {
    std::string stringMainIdtf = m_context.GetElementSystemIdentifier(classAddr);
    std::vector<ScAddr> classObjects = createClassObjectsVector(classAddr);
    std::ofstream outputFile = createOutputFile(stringMainIdtf, classAddr);

    checkSynonymFives(classObjects, outputFile);

    outputFile.close();

    return action.FinishSuccessfully();
  }
  catch (utils::ScException const & exception)
  {
    return action.FinishUnsuccessfully();
  }
}

void CheckSynonymClassesAgent::checkSynonymFives(
    std::vector<ScAddr> & classObjects,
    std::ofstream & outputFile)
{
  for (auto class : classObjects)
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fives = fillFivesVector(class);

    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> otherFives;
    ScAddr duplicateIdentifier;
    for (size_t index = 0; index < fives.size() - 1; ++index)
    {
      otherFives = std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>>(fives.begin() + 1 + index, fives.end());
      size_t otherIndex = find(otherFives.begin(), otherFives.end(), fives[index]);
      if (find(otherFives.begin(), otherFives.end(), fives[index]) != otherFives.end())
      {
        duplicateIdentifier = std::get<2>(fives[index]);
        createSynonymClassesInfo(duplicateIdentifier, std::get<0>(fives[index]), std::get<0>(otherFives[otherIndex]), outputFile);
      }
    }
  }
}

void CheckSynonymClassesAgent::createSynonymClassesInfo(ScAddr & identifier, ScAddr & firstSynonym,ScAddr & secondSynonym, std::ofstream & outputFile)
{
  std::string domainSection = findDomainSectionIdtf(firstSynonym);
  std::string classIdtf = m_context.GetElementSystemIdentifier(identifier);

  outputFile << "Class: " << classIdtf;
  outputFile << ", domain section: " << domainSectionIdtf;

  outputFile << "\nFirst class: ";
  createClassInfo(firstSynonym, outputFile);

  outputFile << "\nSecond class: ";
  createClassInfo(secondSynonym, outputFile);
  
  SC_LOG_ERROR("CheckSynonymRelationsAgent: synonym relations are found");
}

void CheckSynonymClassesAgent::createClassInfo(ScAddr & class, std::ofstream & outputFile)
{
  std::string relationIdtf = m_context.GetElementSystemIdentifier(class);
  std::string firstDomainIdtf = m_context.GetElementSystemIdentifier(firsDomain);
  std::string secondDomainIdtf = m_context.GetElementSystemIdentifier(secondDomain);

  std::string definition getDefinition(class);
  if (!definition.empty())
    outputFile << "\ndefinition: " << definition;

  std::vector<ScAddr> classObjects = createClassObjectsVector(class);
  if (!classObjects.empty())
  {
    outputFile << "\nclass objects: " << definition;
    for (size_t index = 0; index < classObjects.size() - 1; ++index)
      outputFile << "\n" << m_context.GetElementSystemIdentifier(classObjects[index]);
  }
}

std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> CheckSynonymClassesAgent::fillFivesVector(
    ScAddr & classObject)
{
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fives;

  ScIterator5Ptr const & fivesWithObjects = m_context.CreateIterator5(
      classObject, ScType::ConstPermPosArc, ScType::Node, ScKeynodes::nrel_main_idtf, ScType::ConstPermPosArc);

  while (fivesWithObjects->Next())
  {
    fives.push_back(std::make_tuple(
        fivesWithObjects->Get(0),
        fivesWithObjects->Get(1),
        fivesWithObjects->Get(2),
        fivesWithObjects->Get(3),
        fivesWithObjects->Get(
            4)));
  }

  return fives;
}

std::string CheckSynonymClassesAgent::findDomainSectionIdtf(ScAddr & object)
{
  std::string stringDomainSection;
  std::string stringMainIdtf = m_context.GetElementSystemIdentifier(object);

  ScIterator5Ptr const & subjectDomainClassIterator = m_context.CreateIterator5(
      ScType::Node,
      ScType::ConstPermPosArc,
      object,
      ScType::ConstPermPosArc,
      VerificationKeynodes::rrel_maximum_studied_object_class);
  if (subjectDomainClassIterator->Next())
  {
    ScAddr domainSection = subjectDomainClassIterator->Get(0);
    stringDomainSection =
        m_context.GetElementSystemIdentifier(domainSection);
  }

  if (stringDomainSection == "")
  {
    ScIterator5Ptr const & subjectDomainClassIterator = m_context.CreateIterator5(
        ScType::Node,
        ScType::ConstPermPosArc,
        object,
        ScType::ConstPermPosArc,
        VerificationKeynodes::rrel_not_maximum_studied_object_class);

    if (subjectDomainClassIterator->Next())
    {
      ScAddr domainSection = subjectDomainClassIterator->Get(0);
      stringDomainSection = m_context.GetElementSystemIdentifier(
          domainSection);
    }
  }
  return stringDomainSection;
}

std::ofstream CheckSynonymClassesAgent::createOutputFile(std::string & stringMainIdtf, ScAddr & classAddr)
{
  std::string fileName = "synonym_file_for_" + stringMainIdtf;

  std::ofstream outputFile = openOrCreateFile(classAddr, fileName);

  return outputFile;
}

std::ofstream CheckSynonymClassesAgent::openOrCreateFile(
    ScAddr & classAddr,
    std::string const & filename)
{
  ScAddr fileAddr;
  ScIterator5Ptr const & fileIterator = m_context->CreateIterator5(
      classAddr,
      ScType::ConstPermPosArc,
      ScType::Node,
      ScType::ConstCommonArc,
      VerificationKeynodes::nrel_synonym_objects_file);
  if (fileIterator->Next())
  {
    fileAddr = fileIterator->Get(2);
    std::string fileIdtf =
        utils::CommonUtils::getIdtf(m_context, fileAddr, ScKeynodes::nrel_main_idtf, {VerificationConstants::LANG});
    std::string outputFile = filename + fileIdtf;
    return std::ofstream(outputFile, std::ios::app);
  }
  else
  {
    std::ofstream outputFile = generateSynonymFile(classAddr, filename);
    return outputFile;
  }
}

std::ofstream CheckSynonymClassesAgent::generateSynonymFile(
    ScAddr const & classAddr,
    std::string const & filename)
{
  std::ofstream outputFile = std::ofstream(filename, std::ios::app);

  ScAddr const & fileAddr = m_context->GenerateLink(ScType::ConstNodeLink);
  m_context->SetLinkContent(fileAddr, filename);
  ScAddr const & edge = m_context->GenerateConnector(ScType::ConstCommonArc, classAddr, fileAddr);
  ScAddr const & relationEdge =
      m_context->GenerateConnector(ScType::ConstPermPosArc, VerificationKeynodes::nrel_synonym_objects_file, edge);

  return outputFile;
}

std::vector<ScAddr> CheckSynonymClassesAgent::createClassObjectsVector(ScAddr & classAddr)
{
  std::vector<ScAddr> classObjects;

  ScIterator3Ptr const & classRelationIter = m_context.CreateIterator3(classAddr, ScType::ConstPermPosArc, ScType::Node);
  while (classRelationIter->Next())
    classObjects.push_back(classRelationIter->Get(2));

  return classObjects;
}

std::string CheckSynonymClassesAgent::getDefinition(ScAddr & class)
{
  std::string definition;
  ScTemplate definitionTemplate;
  definitionTemplate.Quintuple(
      ScType::NodeVar >> VerificationConstants::KEY_NODE,
      ScType::VarPermPosArc,
      class,
      ScType::VarPermPosArc,
      ScKeynodes::rrel_key_sc_element);
  definitionTemplate.Quintuple(
      ScType::NodeVar >> VerificationConstants::TRANSLATION_NODE,
      ScType::VarCommonArc,
      VerificationConstants::KEY_NODE,
      ScType::VarPermPosArc,
      ScKeynodes::nrel_sc_text_translation);
  definitionTemplate.Quintuple(
      VerificationConstants::TRANSLATION_NODE,
      ScType::VarPermPosArc,
      VerificationConstants::DEFINITION_LINK,
      ScType::VarPermPosArc,
      ScKeynodes::rrel_example);
  ScTemplateSearchResult result;
  m_context.HelperSearchTemplate(definitionTemplate, result);

  if (!result.IsEmpty())
  {
    ScAddr definitionLink = result[0][VerificationConstants::DEFINITION_LINK];
    if (definitionLink.IsValid())
    m_context.GetLinkContent(definitionLink, definition);
  }

  return definition;
}

ScAddr CheckSynonymClassesAgent::GetActionClass() const
{
  return VerificationKeynodes::action_check_synonym_classes;
}

}  // namespace verificationModule

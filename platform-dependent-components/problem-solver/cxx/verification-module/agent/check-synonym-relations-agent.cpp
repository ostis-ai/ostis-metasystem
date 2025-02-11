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

#include "check-synonym-relations-agent.hpp"

using namespace utils;

namespace verificationModule
{

ScResult CheckSynonymRelationsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [classAddr] = action.GetArguments<1>();
  if (!m_context.IsElement(classAddr))
  {
    SC_AGENT_LOG_ERROR("CheckSynonymRelationsAgent: class not found.");
    return action.FinishUnsuccessfully();
  }

  SC_LOG_DEBUG("CheckSynonymRelationsAgent started");

  try
  {
    std::string stringMainIdtf = m_context.GetElementSystemIdentifier(classAddr);
    std::vector<ScAddr> classObjects = createRelationObjectsVector(classAddr);
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

void CheckSynonymRelationsAgent::checkSynonymFives(
    std::vector<ScAddr> & classObjects,
    std::ofstream & outputFile)
{
  for (auto relation : classObjects)
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fives = fillFivesVector(relation);

    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> otherFives;
    ScAddr duplicateIdentifier;
    for (size_t index = 0; index < fives.size() - 1; ++index)
    {
      otherFives = std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>>(fives.begin() + 1 + index, fives.end());
      size_t otherIndex = find(otherFives.begin(), otherFives.end(), fives[index]);
      if (find(otherFives.begin(), otherFives.end(), fives[index]) != otherFives.end())
      {
        duplicateIdentifier = std::get<2>(fives[index]);
        createSynonymRelationsInfo(duplicateIdentifier, std::get<0>(fives[index]), std::get<0>(otherFives[otherIndex]), outputFile);
      }
    }
  }
}

void CheckSynonymRelationsAgent::createSynonymRelationsInfo(ScAddr & identifier, ScAddr & firstSynonym,ScAddr & secondSynonym, std::ofstream & outputFile)
{
  std::string domainSection = findDomainSectionIdtf(firstSynonym);
  std::string relationIdtf = m_context.GetElementSystemIdentifier(identifier);

  outputFile << "Relation: " << relationIdtf;
  outputFile << ", domain section: " << domainSectionIdtf;

  outputFile << "\nFirst relation: ";
  createRelationInfo(firstSynonym, outputFile);

  outputFile << "\nSecond relation: ";
  createRelationInfo(secondSynonym, outputFile);
  
  SC_LOG_ERROR("CheckSynonymRelationsAgent: synonym relations are found");
}

void CheckSynonymRelationsAgent::createRelationInfo(ScAddr & relation, std::ofstream & outputFile)
{
  ScAddr firsDomain = m_context.getAnyByOutRelation(relation, VerificationKeynodes::nrel_first_domain);
  ScAddr secondDomain = m_context.getAnyByOutRelation(relation, VerificationKeynodes::nrel_first_domain);

  std::string relationIdtf = m_context.GetElementSystemIdentifier(relation);
  std::string firstDomainIdtf = m_context.GetElementSystemIdentifier(firsDomain);
  std::string secondDomainIdtf = m_context.GetElementSystemIdentifier(secondDomain);

  outputFile << "first domain: " << firstDomainIdtf;
  outputFile << ", second domain: " << secondDomainIdtf;

  std::string definition getDefinition(relation);
  if (!definition.empty())
    outputFile << "\ndefinition: " << definition;

  std::string relationClasses = getRelationClassesString(relation)
  if (!relationClasses.empty())
    outputFile << "\nrelation classes:\n" << relationClasses;
}

std::string CheckSynonymRelationsAgent::getRelationClassesString(ScAddr & relation)
{
  std::string classes;
  ScIterator3Ptr const & classesIterator = m_context.CreateIterator3(
      ScType::Node, ScType::ConstPermPosArc, relation);
  while (classesIterator->Next())
  {
    classes += m_context.GetElementSystemIdentifier(classesIterator->Get(0)) + "\n"
  }
  return classes
}

std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> CheckSynonymRelationsAgent::fillFivesVector(
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

std::string CheckSynonymRelationsAgent::findDomainSectionIdtf(ScAddr & object)
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

std::ofstream CheckSynonymRelationsAgent::createOutputFile(std::string & stringMainIdtf, ScAddr & classAddr)
{
  std::string fileName = "synonym_file_for_" + stringMainIdtf;

  std::ofstream outputFile = openOrCreateFile(classAddr, fileName);

  return outputFile;
}

std::ofstream CheckSynonymRelationsAgent::openOrCreateFile(
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

std::ofstream CheckSynonymRelationsAgent::generateSynonymFile(
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

std::vector<ScAddr> CheckSynonymRelationsAgent::createRelationObjectsVector(ScAddr & classAddr)
{
  std::vector<ScAddr> relationObjects;

  ScIterator3Ptr const & objectRelationIter = m_context.CreateIterator3(classAddr, ScType::ConstPermPosArc, ScType::Node);
  while (objectRelationIter->Next())
    relationObjects.push_back(objectRelationIter->Get(2));

  return relationObjects;
}

std::string CheckSynonymClassesAgent::getDefinition(ScAddr & relation)
{
  std::string definition;
  ScTemplate definitionTemplate;
  definitionTemplate.Quintuple(
      ScType::NodeVar >> VerificationConstants::KEY_NODE,
      ScType::VarPermPosArc,
      relation,
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

ScAddr CheckSynonymRelationsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_check_synonym_relations;
}

}  // namespace verificationModule

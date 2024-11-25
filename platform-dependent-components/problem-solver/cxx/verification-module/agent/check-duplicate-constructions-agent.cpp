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
#include "searcher/check_duplicate_constructions_searcher.hpp"

#include "check-duplicate-constructions-agent.hpp"

using namespace utils;

namespace verificationModule
{

ScResult CheckDuplicateConstructionsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  auto [classAddr] = action.GetArguments<1>();
  if (!m_context.IsElement(classAddr))
  {
    SC_AGENT_LOG_ERROR("CheckDuplicateConstructionsAgent: class not found.");
    return action.FinishUnsuccessfully();
  }

  SC_LOG_DEBUG("CheckDuplicateConstructionsAgent started");

  try
  {
    std::string stringMainIdtf = m_context.GetElementSystemIdentifier(classAddr);
    std::string stringDomainSection;
    // находим идентификатор класса
    stringDomainSection = findDomainSectionIdtf(classAddr, stringDomainSection);

    std::ofstream outputFile = createOutputFile(stringMainIdtf, classAddr);

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    // Форматируем дату и время
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);

    outputFile << "Current date and time: " << buffer << std::endl;

    std::vector<ScAddr> classObjects = createClassObjectsVector(classAddr);

    checkDuplicateTriplets(classObjects, outputFile, stringDomainSection);

    checkDuplicateFives(classObjects, outputFile, stringDomainSection);

    checkDuplicateQuasyRelations(classObjects, outputFile);

    outputFile.close();

    return action.FinishSuccessfully();
  }
  catch (utils::ScException const & exception)
  {
    return action.FinishUnsuccessfully();
  }
}

std::string CheckDuplicateConstructionsAgent::findDomainSectionIdtf(ScAddr & classAddr, std::string & stringDomainSection)
{
  std::string stringMainIdtf = m_context.GetElementSystemIdentifier(classAddr);

  ScIterator5Ptr const & subjectDomainClassIterator = m_context.CreateIterator5(
      ScType::Node,
      ScType::ConstPermPosArc,
      classAddr,
      ScType::ConstPermPosArc,
      VerificationKeynodes::rrel_maximum_studied_object_class);
  if (subjectDomainClassIterator->Next())
  {
    ScAddr domainSection = subjectDomainClassIterator->Get(0);
    stringDomainSection =
        m_context.GetElementSystemIdentifier(domainSection);  // нашли идентификатор предметной области объекта класса
  }

  if (stringDomainSection == "")
  {
    ScIterator5Ptr const & subjectDomainClassIterator = m_context.CreateIterator5(
        ScType::Node,
        ScType::ConstPermPosArc,
        classAddr,
        ScType::ConstPermPosArc,
        VerificationKeynodes::rrel_not_maximum_studied_object_class);

    if (subjectDomainClassIterator->Next())
    {
      ScAddr domainSection = subjectDomainClassIterator->Get(0);
      stringDomainSection = m_context.GetElementSystemIdentifier(
          domainSection);  // нашли идентификатор предметной области объекта класса
    }
  }
  return stringDomainSection;
}

std::ofstream CheckDuplicateConstructionsAgent::createOutputFile(std::string & stringMainIdtf, ScAddr & classAddr)
{
  std::string fileName = "duplicate_file_for_" + stringMainIdtf;

  std::ofstream outputFile = checkDuplicateConstructionSearcher::openOrCreateFile(&m_context, classAddr, fileName);

  return outputFile;
}

std::vector<ScAddr> CheckDuplicateConstructionsAgent::createClassObjectsVector(ScAddr & classAddr)
{
  std::vector<ScAddr> classObjects;

  ScIterator3Ptr const & objectClassIter = m_context.CreateIterator3(classAddr, ScType::ConstPermPosArc, ScType::Node);
  while (objectClassIter->Next())
    classObjects.push_back(objectClassIter->Get(2));

  return classObjects;
}

void CheckDuplicateConstructionsAgent::checkDuplicateTriplets(
    std::vector<ScAddr> & classObjects,
    std::ofstream & outputFile,
    std::string & stringDomainSection)
{
  for (auto classObject : classObjects)
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr>> triplets =
        fillTripleVector(classObject);  // заполняем вектор тройками

    for (auto triplet : triplets)  // проходим по вектору кортежей
    {
      ScAddr relationObject =
          std::get<2>(triplet);  // проверяем связь с другим объектом, объектом, который не принадлежит исходному классу

      int numberOfDuplicateTriples = checkDuplicateRelationTriplets(relationObject, classObject);

      SC_LOG_DEBUG(numberOfDuplicateTriples);

      if (numberOfDuplicateTriples > 1)
        createDuplicateTripletsInfo(classObject, relationObject, outputFile, stringDomainSection);
    }
  }
}

void CheckDuplicateConstructionsAgent::checkDuplicateFives(
    std::vector<ScAddr> & classObjects,
    std::ofstream & outputFile,
    std::string & stringDomainSection)
{
  for (auto objectClass : classObjects)  // проходим по всем объектам класса
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fives = fillFivesVector(objectClass);

    for (auto five : fives)  // проходим по вектору кортежей
    {
      ScAddr relationObject = std::get<2>(five);
      ScAddr relation = std::get<4>(five);

      int numberOfDuplicateFifths = checkDuplicateRelationFives(
          relationObject,
          objectClass,
          relation);  // проверяем связь с другим объектом, объектом, который не принадлежит исходному классу

      if (numberOfDuplicateFifths > 1)
        createDuplicateFivesInfo(objectClass, relationObject, relation, outputFile, stringDomainSection);
    }
  }
}

void CheckDuplicateConstructionsAgent::checkDuplicateQuasyRelations(
    std::vector<ScAddr> & classObjects,
    std::ofstream & outputFile)
{
  for (auto objectClass : classObjects)
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> tuples = fillQuasyRelationVector(objectClass);

    for (auto tuple : tuples)
    {
      ScAddr tupleObject =
          std::get<2>(tuple);  // проверяем связь с другим объектом, объектом, который не принадлежит исходному классу
      ScAddr relation = std::get<4>(tuple);

      int numberOfDuplicateTuples =
          checkDuplicateRelationQuasyRelations(tupleObject, objectClass, relation);  // создаем счетчик повторений

      if (numberOfDuplicateTuples > 1)  // если счётчик повторений больше 1, то записываем в файл все данные
        createDuplicateQuasyRelationsInfo(objectClass, tupleObject, relation, outputFile);
    }
  }
}

std::vector<std::tuple<ScAddr, ScAddr, ScAddr>> CheckDuplicateConstructionsAgent::fillTripleVector(ScAddr & classObject)
{
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr>> triplets;

  ScIterator3Ptr const & triplesWithObjects =
      m_context.CreateIterator3(classObject, ScType::ConstPermPosArc, ScType::Node);

  while (triplesWithObjects->Next())
  {
    triplets.push_back(
        std::make_tuple(triplesWithObjects->Get(0), triplesWithObjects->Get(1), triplesWithObjects->Get(2)));
  }

  return triplets;
}

std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> CheckDuplicateConstructionsAgent::fillFivesVector(
    ScAddr & classObject)
{
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fives;

  ScIterator5Ptr const & fivesWithObjects = m_context.CreateIterator5(
      classObject, ScType::ConstPermPosArc, ScType::Node, ScType::ConstCommonArc, ScType::ConstPermPosArc);

  while (fivesWithObjects->Next())
  {
    fives.push_back(std::make_tuple(
        fivesWithObjects->Get(0),
        fivesWithObjects->Get(1),
        fivesWithObjects->Get(2),
        fivesWithObjects->Get(3),
        fivesWithObjects->Get(
            4)));  // записываем в отдельный вектор кортежи из пятерок, чтобы потом проверить повторения
  }

  return fives;
}

std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> CheckDuplicateConstructionsAgent::
    fillQuasyRelationVector(ScAddr & classObject)
{
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> tuples;

  ScIterator5Ptr const & tupleIter = m_context.CreateIterator5(
      classObject, ScType::ConstPermPosArc, ScType::NodeTuple, ScType::ConstCommonArc, ScType::ConstPermPosArc);

  while (tupleIter->Next())
  {
    tuples.push_back(
        std::make_tuple(tupleIter->Get(0), tupleIter->Get(1), tupleIter->Get(2), tupleIter->Get(3), tupleIter->Get(4)));
  }

  return tuples;
}

int CheckDuplicateConstructionsAgent::checkDuplicateRelationTriplets(ScAddr & relationObject, ScAddr & classObject)
{
  int numberOfDuplicateTriples = 0;

  ScIterator3Ptr const & tupleIter = m_context.CreateIterator3(classObject, ScType::ConstPermPosArc, relationObject);

  while (tupleIter->Next())
    numberOfDuplicateTriples++;

  return numberOfDuplicateTriples;
}

int CheckDuplicateConstructionsAgent::checkDuplicateRelationFives(
    ScAddr & relationObject,
    ScAddr & classObject,
    ScAddr & relation)
{
  int numberOfDuplicateFifths = 0;  // создаем счетчик повторений

  ScIterator5Ptr const & tupleIter =
      m_context.CreateIterator5(classObject, ScType::ConstPermPosArc, relationObject, ScType::ConstCommonArc, relation);

  while (tupleIter->Next())
    numberOfDuplicateFifths++;

  return numberOfDuplicateFifths;
}

int CheckDuplicateConstructionsAgent::checkDuplicateRelationQuasyRelations(
    ScAddr & tupleObject,
    ScAddr & objectClass,
    ScAddr & relation)
{
  int numberOfDuplicateTuples = 0;

  ScIterator5Ptr const & tupleIter = m_context.CreateIterator5(
      objectClass, ScType::ConstPermPosArc, tupleObject, ScType::ConstCommonArc, ScType::ConstPermPosArc);

  if (tupleIter->Next())
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr>> tupleElements;
    // создаем итератор на 3 по tuples
    ScIterator3Ptr const & tupleElementsIter =
        m_context.CreateIterator3(tupleObject, ScType::ConstPermPosArc, ScType::Node);

    if (tupleElementsIter->Next())
    {
      // Создаем кортеж и добавляем его в вектор
      tupleElements.push_back(
          std::make_tuple(tupleElementsIter->Get(0), tupleElementsIter->Get(1), tupleElementsIter->Get(2)));
    }

    int numberDuplicateTupleElements = 0;
    for (auto tupleElement : tupleElements)
    {
      ScAddr relationObject = std::get<2>(tupleElement);
      ScIterator3Ptr const & tupleIter =
          m_context.CreateIterator3(relationObject, ScType::ConstPermPosArc, objectClass);
      // создаем счетчик повторений

      if (tupleIter->Next())
        numberDuplicateTupleElements++;
    }

    if (numberDuplicateTupleElements > 1)
      numberOfDuplicateTuples++;
  }

  return numberOfDuplicateTuples;
}

void CheckDuplicateConstructionsAgent::createDuplicateTripletsInfo(
    ScAddr & classObject,
    ScAddr & relationObject,
    std::ofstream & outputFile,
    std::string & stringDomainSection)
{
  SC_LOG_ERROR("CheckDuplicateConstructionsAgent: duplication construction is found 1111111");

  std::string stringObjectIdtf;
  stringObjectIdtf =
      m_context.GetElementSystemIdentifier(classObject);  // нашли идентификатор предметной области объекта класса

  std::string stringRelationObject;

  stringRelationObject = m_context.GetElementSystemIdentifier(relationObject);

  outputFile << "Object: " << stringObjectIdtf << std::endl;
  outputFile << ", domain section: " << stringDomainSection << std::endl;
  outputFile << ", relation type: inclusion " << std::endl;
  outputFile << "Inclusion object: " << stringRelationObject << std::endl;

  SC_LOG_ERROR("CheckDuplicateConstructionsAgent: duplication construction is found");
}

void CheckDuplicateConstructionsAgent::createDuplicateFivesInfo(
    ScAddr & classObject,
    ScAddr & relationObject,
    ScAddr & relation,
    std::ofstream & outputFile,
    std::string & stringDomainSection)
{
  std::string stringObjectIdtf;
  std::string stringRelationObject;
  std::string stringRelation;

  stringObjectIdtf =
      m_context.GetElementSystemIdentifier(classObject);  // нашли идентификатор предметной области объекта класса
  stringRelation =
        m_context.GetElementSystemIdentifier(relation);
  stringRelationObject =
      m_context.GetElementSystemIdentifier(relationObject);

  outputFile << "Object: " << stringObjectIdtf;
  outputFile << ", domain section: " << stringDomainSection;
  outputFile << ", relation type: " << stringRelation;
  outputFile << ", second domain: " << stringRelationObject;
  outputFile.close();
  SC_LOG_ERROR("CheckDuplicateConstructionsAgent: duplication construction is found");
}

void CheckDuplicateConstructionsAgent::createDuplicateQuasyRelationsInfo(
    ScAddr & classObject,
    ScAddr & relationObject,
    ScAddr & relation,
    std::ofstream & outputFile)
{
  std::string stringObjectIdtf;
  stringObjectIdtf = CommonUtils::getIdtf(
      &m_context,
      classObject,
      ScKeynodes::nrel_main_idtf,
      {VerificationConstants::LANG});  // нашли идентификатор предметной области объекта класса

  std::string stringDomainSection;
  std::string stringRelationObject;
  std::string stringRelation;

  ScIterator5Ptr const & subjectDomainIterator = m_context.CreateIterator5(
      classObject,
      ScType::ConstPermPosArc,
      ScType::Node,
      ScType::ConstCommonArc,
      VerificationKeynodes::rrel_not_maximum_studied_object_class);
  if (subjectDomainIterator->Next())
  {
    stringRelation =
        CommonUtils::getIdtf(&m_context, relation, ScKeynodes::nrel_main_idtf, {VerificationConstants::LANG});
    ScAddr domainSection = subjectDomainIterator->Get(2);
    stringDomainSection = CommonUtils::getIdtf(
        &m_context,
        domainSection,
        ScKeynodes::nrel_main_idtf,
        {VerificationConstants::LANG});  // нашли идентификатор предметной области объекта класса
  }

  outputFile << "Object: " << stringObjectIdtf;
  outputFile << ", domain section: " << stringDomainSection;
  outputFile << ", relation type: " << stringRelation;
  outputFile << ", second domain: tuple";
  outputFile.close();
  SC_LOG_ERROR("CheckDuplicateConstructionsAgent: duplicate construction is found");
}

ScAddr CheckDuplicateConstructionsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_check_duplicate_constructions;
}

}  // namespace verificationModule

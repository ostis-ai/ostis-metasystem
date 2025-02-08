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
#include "config/config.hpp"

#include "check-duplicate-constructions-agent.hpp"

using namespace utils;

namespace verificationModule
{

std::filesystem::path CheckDuplicateConstructionsAgent::filePath;

ScResult CheckDuplicateConstructionsAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  if(filePath.empty())
    filePath = Config::getInstance()->getValue(FileConfigs::VERIFICATION_ENDPOINT, FileConfigs::FILE_PATH);

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

    stringDomainSection = findDomainSectionIdtf(classAddr, stringDomainSection);

    std::ofstream outputFile = createOutputFile(stringMainIdtf, classAddr);

    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

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
  std::filesystem::create_directories(filePath);

  // Имя файла
  std::string fileName ="duplicate_file_for_" + stringMainIdtf;

  // Полный путь к файлу
  std::filesystem::path directoryPath = filePath / fileName;

  SC_LOG_DEBUG(directoryPath);

  std::ofstream outputFile = checkDuplicateConstructionSearcher::openOrCreateFile(&m_context, classAddr, directoryPath);

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
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> tupleInFile;

  for (auto classObject : classObjects)  // проходим по всем объектам класса
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> fives = fillFivesVector(classObject);

    for (auto five : fives)  // проходим по вектору кортежей
    {
      int numberOfDuplicateRelClassObject, numberOfDuplicateClassRelObject;
      ScAddr relationObject, relation;

      relationObject = std::get<2>(five);
      relation = std::get<4>(five);

      numberOfDuplicateRelClassObject = checkDuplicateRelationFives(
          relationObject,
          classObject,
          relation);  // проверяем связь с другим объектом, объектом, который не принадлежит исходному классу

      if(numberOfDuplicateRelClassObject > 1)
      {
        auto addTupleIfNotExist = [&tupleInFile](auto& tup){
          if (std::find(tupleInFile.begin(), tupleInFile.end(), tup) == tupleInFile.end())
          {
            tupleInFile.push_back(tup);
            SC_LOG_DEBUG(tupleInFile.size());
            return true;
          }
        };
        if (addTupleIfNotExist(five))
          createDuplicateFivesInfo(classObject, relationObject, relation, outputFile, stringDomainSection);
      }

      numberOfDuplicateClassRelObject = checkDuplicateRelationFives(
          classObject,
          relationObject,
          relation);

      if(numberOfDuplicateClassRelObject > 1)
      {
        auto addTupleIfNotExist = [&tupleInFile](auto& tup){
          if (std::find(tupleInFile.begin(), tupleInFile.end(), tup) == tupleInFile.end())
          {
            tupleInFile.push_back(tup);
            SC_LOG_DEBUG(tupleInFile.size());
            return true;
          }
        };

        if (addTupleIfNotExist(five))
          createDuplicateFivesInfo(classObject, relationObject, relation, outputFile, stringDomainSection);
      }
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
      ScAddr tupleObject = std::get<2>(tuple);  // проверяем связь с другим объектом, объектом, который не принадлежит исходному классу
      ScAddr relation = std::get<4>(tuple);

      int numberOfDuplicateTupleObject =
          checkDuplicateRelationQuasyRelations(tupleObject, objectClass, relation);  // создаем счетчик повторений

      int numberOfDuplicateObjectTuple =
          checkDuplicateRelationQuasyRelations(objectClass, tupleObject, relation);

      if (numberOfDuplicateTupleObject > 1)  // если счётчик повторений больше 1, то записываем в файл все данные
        createDuplicateQuasyRelationsInfo(objectClass, tupleObject, relation, outputFile);

      if (numberOfDuplicateObjectTuple > 1)  // если счётчик повторений больше 1, то записываем в файл все данные
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
      classObject, ScType::ConstConnector, ScType::Node, ScType::ConstPermPosArc, ScType::ConstNode);

  while (fivesWithObjects->Next())
  {
    auto newTuple = std::make_tuple(
        fivesWithObjects->Get(0),
        fivesWithObjects->Get(1),
        fivesWithObjects->Get(2),
        fivesWithObjects->Get(3),
        fivesWithObjects->Get(4)
    );

    // Проверяем, не добавлен ли кортеж ранее в tupleInFile
    if (std::find(fives.begin(), fives.end(), newTuple) == fives.end())
    {
      SC_LOG_DEBUG(fives.size());
      fives.push_back(newTuple);  // Добавляем кортеж в fives
    }
  }

  ScIterator5Ptr const & fivesWithRelationObjects = m_context.CreateIterator5(
      ScType::Node, ScType::ConstConnector, classObject, ScType::ConstPermPosArc, ScType::ConstNode);

  while (fivesWithRelationObjects->Next())
  {
    std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr> newFivesTuple =
        std::make_tuple(
        fivesWithRelationObjects->Get(2),
        fivesWithRelationObjects->Get(1),
        fivesWithRelationObjects->Get(0),
        fivesWithRelationObjects->Get(3),
        fivesWithRelationObjects->Get(4));

    // Проверяем, не добавлен ли кортеж ранее в tupleInFile
    if (std::find(fives.begin(), fives.end(), newFivesTuple) == fives.end())
    {
      SC_LOG_DEBUG(fives.size());
      fives.push_back(newFivesTuple);  // Добавляем кортеж в fives
    }
  }

  return fives;
}

std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> CheckDuplicateConstructionsAgent::fillQuasyRelationVector(ScAddr & classObject)
{
  std::vector<std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>> tuples;

  ScIterator5Ptr const & tupleIter = m_context.CreateIterator5(
      classObject, ScType::ConstConnector, ScType::NodeTuple, ScType::ConstPermPosArc, ScType::ConstNode);

  while (tupleIter->Next())
  {
    tuples.push_back(
        std::make_tuple(tupleIter->Get(0),
                        tupleIter->Get(1),
                        tupleIter->Get(2),
                        tupleIter->Get(3),
                        tupleIter->Get(4)));
  }

  ScIterator5Ptr const & classIter = m_context.CreateIterator5(
      ScType::NodeTuple, ScType::ConstConnector, classObject, ScType::ConstPermPosArc, ScType::ConstNode);

  while (classIter->Next())
  {
    std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr> newQuasyTuple =
            std::make_tuple(classIter->Get(2),
                            classIter->Get(1),
                            classIter->Get(0),
                            classIter->Get(3),
                            classIter->Get(4));

    auto addTupleIfNotExists = [&tuples](const std::tuple<ScAddr, ScAddr, ScAddr, ScAddr, ScAddr>& tuple) {
      if (std::find(tuples.begin(), tuples.end(), tuple) == tuples.end())
        tuples.push_back(tuple);
    };

    addTupleIfNotExists(newQuasyTuple);
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
  int numberOfDuplicateFives = 0;  // создаем счетчик повторений

  ScIterator5Ptr const & tupleIter =
      m_context.CreateIterator5(classObject, ScType::ConstConnector, relationObject, ScType::ConstPermPosArc, relation);

  while (tupleIter->Next())
    numberOfDuplicateFives++;

  return numberOfDuplicateFives;
}

int CheckDuplicateConstructionsAgent::checkDuplicateRelationQuasyRelations(
    ScAddr & tupleObject,
    ScAddr & objectClass,
    ScAddr & relation)
{
  int numberOfDuplicateTuples = 0; // создаем счетчик тьюплов

  ScIterator5Ptr const & tupleMainIter = m_context.CreateIterator5(
      objectClass, ScType::ConstConnector, tupleObject, ScType::ConstCommonArc, relation);

  while (tupleMainIter->Next()) // считаем в этом цикле тьюплы
  {
    std::vector<std::tuple<ScAddr, ScAddr, ScAddr>> tupleElements; // создаем вектор для повторяющихся элементов внутри тьюпла

    ScIterator3Ptr const & tupleElementsIter = // создаем итератор на три, чтобы посмотреть элементы тьюпла
        m_context.CreateIterator3(tupleObject, ScType::ConstPermPosArc, ScType::Node);

    while (tupleElementsIter->Next()) // ходим по элементам тьюпла
    {
      // Создаем кортеж и добавляем его в вектор
      tupleElements.push_back(
          std::make_tuple(tupleElementsIter->Get(0),
                          tupleElementsIter->Get(1),
                          tupleElementsIter->Get(2)));
    }

    int numberDuplicateTupleElements = 0; // создаем счетчик для повторяющихся элементов внутри тьюпла
    for (auto tupleElement : tupleElements) // пройдем по вектору элементов тьюпла
    {
      ScAddr relationObject = std::get<2>(tupleElement);
      ScIterator3Ptr const & tupleIter =
          m_context.CreateIterator3(relationObject, ScType::ConstPermPosArc, tupleObject);
      // создаем счетчик повторений

      while (tupleIter->Next()) // если нашли повторяющийся элемент, то добавляем 
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
  std::string stringObjectIdtf;
  stringObjectIdtf =
      m_context.GetElementSystemIdentifier(classObject);  // нашли идентификатор предметной области объекта класса

  std::string stringRelationObject;

  stringRelationObject = m_context.GetElementSystemIdentifier(relationObject);

  outputFile << "Object: " << stringObjectIdtf << std::endl;
  outputFile << "Domain section: " << stringDomainSection << std::endl;
  outputFile << "Relation type: inclusion " << std::endl;
  outputFile << "Inclusion object: " << stringRelationObject << std::endl;
  outputFile << "------------------------------------" << std::endl;
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
      m_context.GetElementSystemIdentifier(classObject);
  stringRelation =
        m_context.GetElementSystemIdentifier(relation);
  stringRelationObject =
      m_context.GetElementSystemIdentifier(relationObject);

  outputFile << "Object: " << stringObjectIdtf << std::endl;
  outputFile << "Domain section: " << stringDomainSection << std::endl;
  outputFile << "Relation type: " << stringRelation << std::endl;
  outputFile << "Second domain: " << stringRelationObject << std::endl;
  outputFile << "------------------------------------" << std::endl;

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

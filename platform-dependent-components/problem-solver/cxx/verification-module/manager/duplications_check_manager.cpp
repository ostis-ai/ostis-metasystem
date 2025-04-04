/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <optional>
#include <ctime>
#include <iomanip>

#include "sc-agents-common/utils/SetOperationsUtils.hpp"

#include "utils/search_utils.hpp"
#include "utils/identifier_utils.hpp"
#include "keynodes/verification_keynodes.hpp"

#include "duplications_check_manager.hpp"

namespace verificationModule
{

DuplicationsCheckManager::DuplicationsCheckManager(ScMemoryContext * context)
  : m_context(context)
{
  SearchUtils::getSingularRelations(m_context, singularRelations);
  SearchUtils::getQuasybinaryRelations(m_context, quasybinaryRelations);
}

void DuplicationsCheckManager::checkSetElementsDuplications(ScAddr const & checkedSet, SetCheckResult & setCheckResult)
{
  fillCheckedSetInfo(checkedSet, setCheckResult);

  ScIterator3Ptr elementsIterator = m_context->CreateIterator3(checkedSet, ScType::ConstPermPosArc, ScType::Unknown);
  while (elementsIterator->Next())
  {
    ElementCheckResult elementCheckResult;
    if (checkElementDuplications(elementsIterator->Get(2), elementCheckResult))
      setCheckResult.elemtnsCheckResults.emplace_back(elementCheckResult);
  }
}

bool DuplicationsCheckManager::checkElementDuplications(ScAddr const & checkedElement, ElementCheckResult & checkResult)
{
  /*
   * Each method does its own pass.
   * While it's possible to collect all necessary info in one pass, it's deemed suboptimal because:
   *   - resulting datastructures containing infor about relation pairs with relations are not that simple
   *       and on big KBs can take alot of space;
   *   - introduction of new check (for a new kind of constructions) with high probability leads to changes
   *       in these structures thus leading to problems with future modifications.
   */
  checkConnectorsDuplications(checkedElement, checkResult);
  checkSingularRelations(checkedElement, checkResult);
  checkQuasybinaryRelations(checkedElement, checkResult);

  if (!checkResult.errorsDescriptions.empty() || !checkResult.warningDescriptions.empty())
  {
    checkResult.elementIdtf = IdentifierUtils::getIdentifier(m_context, checkedElement);

    return true;
  }

  return false;
}

void DuplicationsCheckManager::checkConnectorsDuplications(
    ScAddr const & checkedElement,
    ElementCheckResult & checkResult)
{
  ScAddrToValueUnorderedMap<ScAddrList> targetElementsWithConnectors;

  ScIterator3Ptr connectionsIterator =
      m_context->CreateIterator3(checkedElement, ScType::ConstPermPosArc, ScType::Unknown);

  while (connectionsIterator->Next())
    targetElementsWithConnectors[connectionsIterator->Get(2)].emplace_back(connectionsIterator->Get(1));

  processMultipleAccessArcs(checkedElement, targetElementsWithConnectors, checkResult);

  targetElementsWithConnectors.clear();

  connectionsIterator = m_context->CreateIterator3(checkedElement, ScType::ConstCommonArc, ScType::Unknown);

  while (connectionsIterator->Next())
    targetElementsWithConnectors[connectionsIterator->Get(2)].emplace_back(connectionsIterator->Get(1));

  processMultipleCommonArcs(checkedElement, targetElementsWithConnectors, checkResult);
}

void DuplicationsCheckManager::processMultipleAccessArcs(
    ScAddr const & checkedElement,
    ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
    ElementCheckResult & checkResult)
{
  for (auto const & targetElementWithConnectors : targetElementsWithConnectors)
  {
    ScAddr const & targetElement = targetElementWithConnectors.first;
    ScAddrList const & connectors = targetElementWithConnectors.second;

    if (connectors.size() == 1)
      continue;

    checkResult.warningDescriptions.emplace_back(
        "Found multiple access arcs to " + IdentifierUtils::getIdentifier(m_context,targetElement)
        + ". Possible duplication if it was not meant as multiset.");

    RelationsInfo relationsInfo;
    getRelationsInfo(connectors, ScType::ConstNodeRole, relationsInfo);

    for (auto const & relation : relationsInfo.duplicatingRelations)
      checkResult.errorsDescriptions.emplace_back(
          "Found duplication of relation " + IdentifierUtils::getIdentifier(m_context,relation) + " to "
          + IdentifierUtils::getIdentifier(m_context,targetElement));

    if (relationsInfo.containsArcsWithoutIntersectingRelations)
      checkResult.warningDescriptions.emplace_back(
          "Found multiple access arcs of different relations to " + IdentifierUtils::getIdentifier(m_context,targetElement)
          + ". Possible it's better to use one arc belonging to several relations.");
  }
}

// todo: подумать как переработать чтобы убрать дублирование перд тем как делать ПР
void DuplicationsCheckManager::processMultipleCommonArcs(
    ScAddr const & checkedElement,
    ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
    ElementCheckResult & checkResult)
{
  for (auto const & targetElementWithConnectors : targetElementsWithConnectors)
  {
    ScAddr const & targetElement = targetElementWithConnectors.first;
    ScAddrList const & connectors = targetElementWithConnectors.second;

    if (connectors.size() == 1)
      continue;

    RelationsInfo relationsInfo;
    getRelationsInfo(connectors, ScType::ConstNodeNonRole, relationsInfo);

    for (auto const & relation : relationsInfo.duplicatingRelations)
      checkResult.errorsDescriptions.emplace_back(
          "Found duplication of relation " + IdentifierUtils::getIdentifier(m_context,relation) + " to "
          + IdentifierUtils::getIdentifier(m_context,targetElement));

    // Check logic a single common arc without relation, but this is not a kind of error we search here
    // but rather an accompanying one
    if (relationsInfo.containsArcsWithoutRelations)
      checkResult.warningDescriptions.emplace_back(
          "Found common arcs to " + IdentifierUtils::getIdentifier(m_context,targetElement)
          + " not belonging to any relations. Possible incorrect construction.");

    if (relationsInfo.containsArcsWithoutIntersectingRelations)
      checkResult.warningDescriptions.emplace_back(
          "Found multiple common arcs of different relations to " + IdentifierUtils::getIdentifier(m_context,targetElement)
          + ". Possible it's better to use one arc belonging to several relations.");
  }
}

void DuplicationsCheckManager::getRelationsInfo(
    ScAddrList const & connectors,
    ScType const & relationType,
    RelationsInfo & relationsInfo)
{
  ScAddrSet foundRelations;
  relationsInfo.containsArcsWithoutRelations = false;
  relationsInfo.containsArcsWithoutIntersectingRelations = false;
  bool insertedOnCurrentStep = false;

  for (auto const & connector : connectors)
  {
    bool insertedOnPreviousStep = insertedOnCurrentStep;
    ScIterator3Ptr relationIterator = m_context->CreateIterator3(relationType, ScType::ConstPermPosArc, connector);

    bool arcContainsRelations = false;
    while (relationIterator->Next())
    {
      arcContainsRelations = true;
      auto const & insertionResult = foundRelations.insert(relationIterator->Get(0));
      if (!insertionResult.second)
        relationsInfo.duplicatingRelations.insert(relationIterator->Get(0));

      else
      {
        relationsInfo.containsArcsWithoutIntersectingRelations = insertedOnPreviousStep;
        insertedOnCurrentStep = true;
      }
    }

    relationsInfo.containsArcsWithoutRelations |= !arcContainsRelations;
  }
}

void DuplicationsCheckManager::checkSingularRelations(ScAddr const & checkedElement, ElementCheckResult & checkResult)
{
  // presumably there are less singular relations than connectors, outgoing from the checked element
  for (auto const & relation : singularRelations)
  {
    ScType const & relationType = m_context->GetElementType(relation);
    ScType arcType = relationType == ScType::ConstNodeRole ? ScType::ConstPermPosArc : ScType::ConstCommonArc;

    ScIterator5Ptr relationPairsIterator =
        m_context->CreateIterator5(checkedElement, arcType, ScType::Unknown, ScType::ConstPermPosArc, relation);

    unsigned relationPairsNum = 0;
    while (relationPairsIterator->Next())
    {
      relationPairsNum++;
      if (relationPairsNum > 1)
      {
        checkResult.errorsDescriptions.emplace_back(
            "Duplicating relation " + IdentifierUtils::getIdentifier(m_context,relation) + " from "
            + IdentifierUtils::getIdentifier(m_context,checkedElement)
            + ". Expected only one outgoing relation pair per element.");

        break;
      }
    }
  }
}

void DuplicationsCheckManager::checkQuasybinaryRelations(
    ScAddr const & checkedElement,
    ElementCheckResult & checkResult)
{
  auto processRelationPair = [this](
                                 ScAddr const & relation,
                                 ScAddr const & tuple,
                                 ScAddrToValueUnorderedMap<ScAddrList> & foundQuasybinaryRelationsTuples) -> void
  {
    if (!quasybinaryRelations.count(relation))
      return;

    foundQuasybinaryRelationsTuples[relation].push_back(tuple);
  };

  auto checkRelationsSets = [this](
                                ScAddrToValueUnorderedMap<ScAddrList> const & foundQuasybinaryRelationsTuples,
                                ElementCheckResult & checkResult) -> void
  {
    for (auto const & relationWithTuple : foundQuasybinaryRelationsTuples)
      checkDuplicationInQuasybinaryRelationSets(relationWithTuple.first, relationWithTuple.second, checkResult);
  };

  ScAddrToValueUnorderedMap<ScAddrList> foundQuasybinaryRelationsTuples;

  // presumably there are less norole relations pairs in/from the checked element than quasybinary relations
  // also we cannot determine quasybinary relations orientation without implementation of additional checks

  ScIterator5Ptr relationPairsIterator = m_context->CreateIterator5(
      checkedElement, ScType::ConstCommonArc, ScType::Unknown, ScType::ConstPermPosArc, ScType::ConstNodeNonRole);
  while (relationPairsIterator->Next())
    processRelationPair(relationPairsIterator->Get(4), relationPairsIterator->Get(2), foundQuasybinaryRelationsTuples);

  checkRelationsSets(foundQuasybinaryRelationsTuples, checkResult);
  foundQuasybinaryRelationsTuples.clear();

  relationPairsIterator = m_context->CreateIterator5(
      ScType::Unknown, ScType::ConstCommonArc, checkedElement, ScType::ConstPermPosArc, ScType::ConstNodeNonRole);
  while (relationPairsIterator->Next())
    processRelationPair(relationPairsIterator->Get(4), relationPairsIterator->Get(0), foundQuasybinaryRelationsTuples);

  checkRelationsSets(foundQuasybinaryRelationsTuples, checkResult);
}

void DuplicationsCheckManager::checkDuplicationInQuasybinaryRelationSets(
    ScAddr const & relation,
    ScAddrList const & checkedTuples,
    ElementCheckResult & checkResult)
{
  ScAddrUnorderedSet tupleElements;
  std::map<ScAddr::HashType, ScAddrVector> tuplesHashes;

  for (auto const & tuple : checkedTuples)
  {
    ScAddr::HashType tupleHash = 0;
    ScIterator3Ptr elementsIterator = m_context->CreateIterator3(tuple, ScType::ConstPermPosArc, ScType::Unknown);
    while (elementsIterator->Next())
    {
      ScAddr const & element = elementsIterator->Get(2);
      tupleHash ^= element.Hash();
      auto const & insertionResult = tupleElements.insert(element);
      if (!insertionResult.second)
        checkResult.warningDescriptions.emplace_back(
            "Found duplication of " + IdentifierUtils::getIdentifier(m_context,relation) + "'s tuple elements.");
    }

    tuplesHashes[tupleHash].push_back(tuple);
    tupleElements.clear();
  }

  for (auto const & hashWithCorrespondingTuples : tuplesHashes)
  {
    if (hashWithCorrespondingTuples.second.size() == 1)
      continue;

    if (atleastTwoSetsAreEqual(hashWithCorrespondingTuples.second))
      checkResult.errorsDescriptions.emplace_back(
          "Two or more " + IdentifierUtils::getIdentifier(m_context,relation)
          + "'s tuples are equal. Likely duplication.");
  }
}

bool DuplicationsCheckManager::atleastTwoSetsAreEqual(ScAddrVector const & setsToCompare)
{
  for (size_t first = 0; first < setsToCompare.size() - 1; ++first)
  {
    for (size_t second = first + 1; second < setsToCompare.size(); ++second)
    {
      if (utils::SetOperationsUtils::compareSets(m_context, setsToCompare[first], setsToCompare[second]))
        return true;
    }
  }
  return false;
}

std::string DuplicationsCheckManager::getCurrentDatetimeString()
{
  std::time_t now = std::time(nullptr);
  std::tm * localTime = std::localtime(&now);

  std::ostringstream oss;
  oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");

  return oss.str();
}

bool DuplicationsCheckManager::findMaxObjectClassSubjectDomain(
    ScAddr const & checkedSet,
    std::string & subjectDomainContainingAsMaximumClass)
{
  ScIterator5Ptr const & subjectDomainClassIterator = m_context->CreateIterator5(
      ScType::Node,
      ScType::ConstPermPosArc,
      checkedSet,
      ScType::ConstPermPosArc,
      VerificationKeynodes::rrel_maximum_studied_object_class);
  if (subjectDomainClassIterator->Next())
  {
    ScAddr domainSection = subjectDomainClassIterator->Get(0);
    subjectDomainContainingAsMaximumClass = IdentifierUtils::getIdentifier(m_context,domainSection);
    return true;
  }

  return false;
}

void DuplicationsCheckManager::findNonMaxObjectClassSubjectDomains(
    ScAddr const & checkedSet,
    std::list<std::string> & subjectDomainsContainingAsNotMaximumClass)
{
  ScIterator5Ptr const & subjectDomainClassIterator = m_context->CreateIterator5(
      ScType::Node,
      ScType::ConstPermPosArc,
      checkedSet,
      ScType::ConstPermPosArc,
      VerificationKeynodes::rrel_not_maximum_studied_object_class);

  while (subjectDomainClassIterator->Next())
  {
    ScAddr domainSection = subjectDomainClassIterator->Get(0);
    subjectDomainsContainingAsNotMaximumClass.emplace_back(IdentifierUtils::getIdentifier(m_context,domainSection));
  }
}

void DuplicationsCheckManager::fillCheckedSetInfo(ScAddr const & checkedSet, SetCheckResult & setCheckResult)
{
  setCheckResult.setIdtf = IdentifierUtils::getIdentifier(m_context,checkedSet);
  setCheckResult.checkTime = getCurrentDatetimeString();
  std::string subjectDomainContainingAsMaximumClass;
  if (findMaxObjectClassSubjectDomain(checkedSet, subjectDomainContainingAsMaximumClass))
    setCheckResult.subjectDomainContainingAsMaximumClass = subjectDomainContainingAsMaximumClass;
  findNonMaxObjectClassSubjectDomains(checkedSet, setCheckResult.subjectDomainsContainingAsNotMaximumClass);
}

}  // namespace verificationModule

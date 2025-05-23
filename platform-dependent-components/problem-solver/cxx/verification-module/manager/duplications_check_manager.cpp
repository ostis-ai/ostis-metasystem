/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <optional>
#include <ctime>
#include <iomanip>

#include "sc-agents-common/utils/SetOperationsUtils.hpp"

#include "constants/verification_constants.hpp"
#include "utils/search_utils.hpp"
#include "utils/identifier_utils.hpp"
#include "keynodes/verification_keynodes.hpp"

#include "duplications_check_manager.hpp"

namespace verificationModule
{

DuplicationsCheckManager::DuplicationsCheckManager(ScMemoryContext * context)
  : m_context(context)
{
  SearchUtils::GetSingularRelations(m_context, singularRelations);
  SearchUtils::GetQuasybinaryRelations(m_context, quasybinaryRelations);
}

bool DuplicationsCheckManager::CheckSetElementsDuplications(
    ScAddr const & checkedSet,
    SetDuplicationsCheckResult & checkResult) const
{
  bool foundDuplicationsAtLeastInOneElement = false;

  ScIterator3Ptr elementsIterator = m_context->CreateIterator3(checkedSet, ScType::ConstPermPosArc, ScType::Unknown);
  while (elementsIterator->Next())
  {
    ElementDuplicationsCheckResult elementCheckResult;
    if (CheckElementDuplications(elementsIterator->Get(2), elementCheckResult))
    {
      foundDuplicationsAtLeastInOneElement = true;
      checkResult.elemtnsCheckResults.emplace_back(elementCheckResult);
    }
  }

  if (foundDuplicationsAtLeastInOneElement)
    FillCheckedSetInfo(checkedSet, checkResult);

  return foundDuplicationsAtLeastInOneElement;
}

bool DuplicationsCheckManager::CheckElementDuplications(
    ScAddr const & checkedElement,
    ElementDuplicationsCheckResult & checkResult) const
{
  /*
   * Each method does its own pass.
   * While it's possible to collect all necessary info in one pass, it's deemed suboptimal because:
   *   - resulting datastructures containing info about relation pairs with relations are not that simple
   *       and on big KBs can take alot of space;
   *   - introduction of new check (for a new kind of constructions) with high probability leads to changes
   *       in these structures thus leading to problems with future modifications.
   */
  // todo(NikiforovSergei): add the ability to configure which checks to use
  CheckConnectorsDuplications(checkedElement, checkResult);
  CheckSingularRelations(checkedElement, checkResult);
  CheckQuasybinaryRelations(checkedElement, checkResult);

  if (!checkResult.errorsDescriptions.empty() || !checkResult.warningDescriptions.empty())
  {
    checkResult.elementIdtf = IdentifierUtils::GetIdentifiersString(m_context, checkedElement);

    return true;
  }

  return false;
}

void DuplicationsCheckManager::CheckConnectorsDuplications(
    ScAddr const & checkedElement,
    ElementDuplicationsCheckResult & checkResult) const
{
  ScAddrToValueUnorderedMap<ScAddrList> targetElementsWithConnectors;

  ScIterator3Ptr connectionsIterator =
      m_context->CreateIterator3(checkedElement, ScType::ConstPermPosArc, ScType::Unknown);

  while (connectionsIterator->Next())
    targetElementsWithConnectors[connectionsIterator->Get(2)].emplace_back(connectionsIterator->Get(1));

  ProcessMultipleMembershipArcs(targetElementsWithConnectors, checkResult);

  targetElementsWithConnectors.clear();

  connectionsIterator = m_context->CreateIterator3(checkedElement, ScType::ConstCommonArc, ScType::Unknown);

  while (connectionsIterator->Next())
    targetElementsWithConnectors[connectionsIterator->Get(2)].emplace_back(connectionsIterator->Get(1));

  ProcessMultipleCommonArcs(targetElementsWithConnectors, checkResult);
}

void DuplicationsCheckManager::ProcessMultipleMembershipArcs(
    ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
    ElementDuplicationsCheckResult & checkResult) const
{
  for (auto const & targetElementWithConnectors : targetElementsWithConnectors)
  {
    ScAddr const & targetElement = targetElementWithConnectors.first;
    ScAddrList const & connectors = targetElementWithConnectors.second;

    if (connectors.size() == 1)
      continue;

    checkResult.warningDescriptions.emplace_back(
        "Found multiple membership arcs to " + IdentifierUtils::GetIdentifiersString(m_context, targetElement)
        + ". Duplication is possible if it was not intended as a multiset.");

    RelationsDuplicationInfo relationsInfo;
    GetRelationsInfo(connectors, ScType::ConstNodeRole, relationsInfo);

    for (auto const & relation : relationsInfo.duplicatingRelations)
      checkResult.errorsDescriptions.emplace_back(
          "Found duplicate relation " + IdentifierUtils::GetIdentifiersString(m_context, relation) + " to "
          + IdentifierUtils::GetIdentifiersString(m_context, targetElement));

    if (relationsInfo.containsConnectorsWithoutIntersectingRelations)
      checkResult.warningDescriptions.emplace_back(
          "Found multiple membership arcs of different relations to "
          + IdentifierUtils::GetIdentifiersString(m_context, targetElement)
          + ". It might be better to use a single arc belonging to multiple relations.");
  }
}

void DuplicationsCheckManager::ProcessMultipleCommonArcs(
    ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
    ElementDuplicationsCheckResult & checkResult) const
{
  for (auto const & targetElementWithConnectors : targetElementsWithConnectors)
  {
    ScAddr const & targetElement = targetElementWithConnectors.first;
    ScAddrList const & connectors = targetElementWithConnectors.second;

    if (connectors.size() == 1)
      continue;

    RelationsDuplicationInfo relationsInfo;
    GetRelationsInfo(connectors, ScType::ConstNodeNonRole, relationsInfo);

    for (auto const & relation : relationsInfo.duplicatingRelations)
      checkResult.errorsDescriptions.emplace_back(
          "Found duplication of relation " + IdentifierUtils::GetIdentifiersString(m_context, relation) + " to "
          + IdentifierUtils::GetIdentifiersString(m_context, targetElement));

    // Check logic a single common arc without relation, but this is not a kind of error we search here
    // but rather an accompanying one
    if (relationsInfo.containsConnectorsWithoutRelations)
      checkResult.warningDescriptions.emplace_back(
          "Found common arcs to " + IdentifierUtils::GetIdentifiersString(m_context, targetElement)
          + " not belonging to any relations. Possible incorrect construction.");

    if (relationsInfo.containsConnectorsWithoutIntersectingRelations)
      checkResult.warningDescriptions.emplace_back(
          "Found multiple common arcs directed to"
          + IdentifierUtils::GetIdentifiersString(m_context, targetElement)
          + ", which belong to different relations. It might be better to use a single arc belonging to multiple relations.");
  }
}

void DuplicationsCheckManager::GetRelationsInfo(
    ScAddrList const & connectors,
    ScType const & relationType,
    RelationsDuplicationInfo & relationsInfo) const
{
  ScAddrSet foundRelations;
  relationsInfo.containsConnectorsWithoutRelations = false;
  relationsInfo.containsConnectorsWithoutIntersectingRelations = false;
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
        relationsInfo.containsConnectorsWithoutIntersectingRelations = insertedOnPreviousStep;
        insertedOnCurrentStep = true;
      }
    }

    relationsInfo.containsConnectorsWithoutRelations |= !arcContainsRelations;
  }
}

void DuplicationsCheckManager::CheckSingularRelations(
    ScAddr const & checkedElement,
    ElementDuplicationsCheckResult & checkResult) const
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
            "Duplicate relation " + IdentifierUtils::GetIdentifiersString(m_context, relation) + " from "
            + IdentifierUtils::GetIdentifiersString(m_context, checkedElement)
            + ". Expected only one outgoing relation pair per element.");

        break;
      }
    }
  }
}

void DuplicationsCheckManager::CheckQuasybinaryRelations(
    ScAddr const & checkedElement,
    ElementDuplicationsCheckResult & checkResult) const
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
                                ElementDuplicationsCheckResult & checkResult) -> void
  {
    for (auto const & relationWithTuple : foundQuasybinaryRelationsTuples)
      CheckDuplicationInQuasybinaryRelationSets(relationWithTuple.first, relationWithTuple.second, checkResult);
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

void DuplicationsCheckManager::CheckDuplicationInQuasybinaryRelationSets(
    ScAddr const & relation,
    ScAddrList const & checkedTuples,
    ElementDuplicationsCheckResult & checkResult) const
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
            "Found duplication of " + IdentifierUtils::GetIdentifiersString(m_context, relation)
            + "'s tuple elements.");
    }

    tuplesHashes[tupleHash].push_back(tuple);
    tupleElements.clear();
  }

  for (auto const & hashWithCorrespondingTuples : tuplesHashes)
  {
    if (hashWithCorrespondingTuples.second.size() == 1)
      continue;

    if (AtLeastTwoSetsAreEqual(hashWithCorrespondingTuples.second))
      checkResult.errorsDescriptions.emplace_back(
          "Two or more " + IdentifierUtils::GetIdentifiersString(m_context, relation)
          + "'s tuples are equal. Likely duplication.");
  }
}

bool DuplicationsCheckManager::AtLeastTwoSetsAreEqual(ScAddrVector const & setsToCompare) const
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

std::string DuplicationsCheckManager::GetCurrentDatetimeString() const
{
  std::time_t now = std::time(nullptr);
  std::tm * localTime = std::localtime(&now);

  std::ostringstream oss;
  oss << std::put_time(localTime, Formats::TIME);

  return oss.str();
}

void DuplicationsCheckManager::FindStructuresContainingElementByRoleRelationIdentifiers(
    ScAddr const & element,
    ScAddr const & relation,
    std::list<std::string> & structuresIdentifiers) const
{
  ScIterator5Ptr const & structuresIterator =
      m_context->CreateIterator5(ScType::Node, ScType::ConstPermPosArc, element, ScType::ConstPermPosArc, relation);

  while (structuresIterator->Next())
  {
    ScAddr const & structure = structuresIterator->Get(0);
    structuresIdentifiers.emplace_back(IdentifierUtils::GetIdentifiersString(m_context, structure));
  }
}

void DuplicationsCheckManager::FindMaxObjectClassSubjectDomain(
    ScAddr const & checkedSet,
    std::list<std::string> & subjectDomainContainingAsMaximumClass) const
{
  FindStructuresContainingElementByRoleRelationIdentifiers(
      checkedSet, VerificationKeynodes::rrel_maximum_studied_object_class, subjectDomainContainingAsMaximumClass);
}

void DuplicationsCheckManager::FindNonMaxObjectClassSubjectDomains(
    ScAddr const & checkedSet,
    std::list<std::string> & subjectDomainsContainingAsNotMaximumClass) const
{
  FindStructuresContainingElementByRoleRelationIdentifiers(
      checkedSet,
      VerificationKeynodes::rrel_not_maximum_studied_object_class,
      subjectDomainsContainingAsNotMaximumClass);
}

void DuplicationsCheckManager::FillCheckedSetInfo(ScAddr const & checkedSet, SetDuplicationsCheckResult & checkResult)
    const
{
  checkResult.setIdtf = IdentifierUtils::GetIdentifiersString(m_context, checkedSet);
  checkResult.checkTime = GetCurrentDatetimeString();
  FindMaxObjectClassSubjectDomain(checkedSet, checkResult.subjectDomainsContainingAsMaximumClass);
  FindNonMaxObjectClassSubjectDomains(checkedSet, checkResult.subjectDomainsContainingAsNotMaximumClass);
}

}  // namespace verificationModule

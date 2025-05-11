/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "data-structures/set_duplications_check_result.hpp"
#include "data-structures/element_duplications_check_result.hpp"
#include "data-structures/relations_duplications_info.hpp"

#include "sc-memory/sc_memory.hpp"

namespace verificationModule
{
class DuplicationsCheckManager
{
public:
  explicit DuplicationsCheckManager(ScMemoryContext * context);

  bool CheckSetElementsDuplications(ScAddr const & checkedSet, SetDuplicationsCheckResult & checkResult) const;

private:
  ScMemoryContext * m_context;
  ScAddrList singularRelations;
  ScAddrUnorderedSet quasybinaryRelations;

  bool CheckElementDuplications(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void CheckConnectorsDuplications(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void ProcessMultipleMembershipArcs(
      ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
      ElementDuplicationsCheckResult & checkResult) const;

  void ProcessMultipleCommonArcs(
      ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
      ElementDuplicationsCheckResult & checkResult) const;

  void GetRelationsInfo(
      ScAddrList const & connectors,
      ScType const & relationType,
      RelationsDuplicationInfo & relationsInfo) const;

  void CheckSingularRelations(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void CheckQuasybinaryRelations(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void CheckDuplicationInQuasybinaryRelationSets(
      ScAddr const & relation,
      ScAddrList const & checkedTuples,
      ElementDuplicationsCheckResult & checkResult) const;

  bool AtLeastTwoSetsAreEqual(ScAddrVector const & setsToCompare) const;

  std::string GetCurrentDatetimeString() const;

  void FillCheckedSetInfo(ScAddr const & checkedSet, SetDuplicationsCheckResult & checkResult) const;

  void FindMaxObjectClassSubjectDomain(
      ScAddr const & checkedSet,
      std::list<std::string> & subjectDomainContainingAsMaximumClass) const;

  void FindNonMaxObjectClassSubjectDomains(
      ScAddr const & checkedSet,
      std::list<std::string> & subjectDomainsContainingAsNotMaximumClass) const;

  void FindStructuresContainingElementByRoleRelationIdentifiers(
      ScAddr const & element,
      ScAddr const & relation,
      std::list<std::string> & structuresIdentifiers) const;
};
}  // namespace verificationModule

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "dataStructures/set_duplications_check_result.hpp"
#include "dataStructures/element_duplications_check_result.hpp"
#include "dataStructures/relations_duplications_info.hpp"

#include "sc-memory/sc_memory.hpp"

namespace verificationModule
{
class DuplicationsCheckManager
{
public:
  explicit DuplicationsCheckManager(ScMemoryContext * context);

  bool checkSetElementsDuplications(ScAddr const & checkedSet, SetDuplicationsCheckResult & checkResult) const;

private:
  ScMemoryContext * m_context;
  ScAddrList singularRelations;
  ScAddrUnorderedSet quasybinaryRelations;

  bool checkElementDuplications(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void checkConnectorsDuplications(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void processMultipleAccessArcs(
      ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
      ElementDuplicationsCheckResult & checkResult) const;

  void processMultipleCommonArcs(
      ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
      ElementDuplicationsCheckResult & checkResult) const;

  void getRelationsInfo(
      ScAddrList const & connectors,ScType const & relationType,
      RelationsDuplicationInfo & relationsInfo) const;

  void checkSingularRelations(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void checkQuasybinaryRelations(ScAddr const & checkedElement, ElementDuplicationsCheckResult & checkResult) const;

  void checkDuplicationInQuasybinaryRelationSets(
      ScAddr const & relation,
      ScAddrList const & checkedTuples,
      ElementDuplicationsCheckResult & checkResult) const;

  bool atleastTwoSetsAreEqual(ScAddrVector const & setsToCompare) const;

  std::string getCurrentDatetimeString() const;

  void fillCheckedSetInfo(ScAddr const & checkedSet, SetDuplicationsCheckResult & checkResult) const;

  void findMaxObjectClassSubjectDomain(
      ScAddr const & checkedSet,
      std::list<std::string> & subjectDomainContainingAsMaximumClass) const;

  void findNonMaxObjectClassSubjectDomains(
      ScAddr const & checkedSet,
      std::list<std::string> & subjectDomainsContainingAsNotMaximumClass) const;

  void findStructuresContainingElementByRoleRelationIdentifiers(
      ScAddr const & element,
      ScAddr const & relation,
      std::list<std::string> & structuresIdentifiers) const;
};
}  // namespace verificationModule

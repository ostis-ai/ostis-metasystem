/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include "dataStructures/set_check_result.hpp"
#include "dataStructures/element_check_result.hpp"
#include "dataStructures/relations_info.hpp"

#include "sc-memory/sc_memory.hpp"

namespace verificationModule
{
class DuplicationsCheckManager
{
public:
  explicit DuplicationsCheckManager(ScMemoryContext * context);

  void checkSetElementsDuplications(ScAddr const & checkedSet, SetCheckResult & checkResult);

private:
  ScMemoryContext * m_context;
  ScAddrList singularRelations;
  ScAddrUnorderedSet quasybinaryRelations;

  bool checkElementDuplications(ScAddr const & checkedElement, ElementCheckResult & checkResult);

  void checkConnectorsDuplications(ScAddr const & checkedElement, ElementCheckResult & checkResult);

  void processMultipleAccessArcs(
      ScAddr const & checkedElement,
      ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
      ElementCheckResult & checkResult);

  void processMultipleCommonArcs(
      ScAddr const & checkedElement,
      ScAddrToValueUnorderedMap<ScAddrList> const & targetElementsWithConnectors,
      ElementCheckResult & checkResult);

  void getRelationsInfo(ScAddrList const & connectors, ScType const & relationType, RelationsInfo & relationsInfo);

  void checkSingularRelations(ScAddr const & checkedElement, ElementCheckResult & checkResult);

  void checkQuasybinaryRelations(ScAddr const & checkedElement, ElementCheckResult & checkResult);

  void checkDuplicationInQuasybinaryRelationSets(
      ScAddr const & relation,
      ScAddrList const & checkedTuples,
      ElementCheckResult & checkResult);

  bool atleastTwoSetsAreEqual(ScAddrVector const & setsToCompare);

  void fillCheckedSetInfo(ScAddr const & checkedSet, SetCheckResult & checkResult);

  std::string getCurrentDatetimeString();

  bool findMaxObjectClassSubjectDomain(ScAddr const & checkedSet, std::string & subjectDomainContainingAsMaximumClass);

  void findNonMaxObjectClassSubjectDomains(
      ScAddr const & checkedSet,
      std::list<std::string> & subjectDomainsContainingAsNotMaximumClass);
};
}  // namespace verificationModule

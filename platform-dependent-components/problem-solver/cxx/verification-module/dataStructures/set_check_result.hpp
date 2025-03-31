/*
* This source file is part of an OSTIS project. For the latest info, see
* http://ostis.net Distributed under the MIT License (See accompanying file
* COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <list>
#include <string>

#include "dataStructures/element_check_result.hpp"

struct SetCheckResult
{
  std::string setIdtf;
  std::string subjectDomainContainingAsMaximumClass;
  std::list<std::string> subjectDomainsContainingAsNotMaximumClass;
  std::string checkTime;
  std::list<ElementCheckResult> elemtnsCheckResults;
};

/*
* This source file is part of an OSTIS project. For the latest info, see
* http://ostis.net Distributed under the MIT License (See accompanying file
* COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <list>
#include <string>

struct ElementCheckResult
{
  std::string elementIdtf;
  std::list<std::string> warningDescriptions;
  std::list<std::string> errorsDescriptions;
};

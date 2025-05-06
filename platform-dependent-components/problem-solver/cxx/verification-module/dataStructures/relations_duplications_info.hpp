/*
* This source file is part of an OSTIS project. For the latest info, see
* http://ostis.net Distributed under the MIT License (See accompanying file
* COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/sc_addr.hpp>

#include "dataStructures/element_duplications_check_result.hpp"

struct RelationsDuplicationInfo
{
  ScAddrSet duplicatingRelations;
  bool containsConnectorsWithoutRelations;
  bool containsConnectorsWithoutIntersectingRelations;
};

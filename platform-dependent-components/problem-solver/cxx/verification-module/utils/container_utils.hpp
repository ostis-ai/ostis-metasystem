/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <sc-memory/sc_addr.hpp>

namespace verificationModule
{

class ContainerUtils
{
public:
  static void InsertSeveral(ScAddrUnorderedSet & set, std::initializer_list<ScAddr> const & elements);
};

}  // namespace verificationModule

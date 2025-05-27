/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#pragma once

#include <map>
#include <string>

#include "sc-memory/sc_memory.hpp"

namespace searchAnswerModule
{
class SearchAnswerConstants
{
  public:
    static ScAddrToValueUnorderedMap<std::string> none;
    static std::string const EDGE;
    static std::string const SET_NODE;
    static std::string const RREL_EDGE;
    static std::string const COMMON_EDGE;
    static std::string const VAR_EDGE;
};

}  // namespace SearchAnswerModule

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <map>
#include <vector>
#include <variant>
#include "sc-memory/sc_keynodes.hpp"

#include "keynodes/search_answer_keynodes.hpp"
#include "search_answer_constants.hpp"

namespace searchAnswerModule
{
std::map<ScAddr, std::string> SearchAnswerConstants::none = 
{
    {ScKeynodes::lang_ru, "нет"},
    {SearchAnswerKeynodes::lang_en, "none"}
};

std::string const SearchAnswerConstants::EDGE = "edge";
std::string const SearchAnswerConstants::SET_NODE = "set_node";
std::string const SearchAnswerConstants::RREL_EDGE = "rrel_edge";
std::string const SearchAnswerConstants::COMMON_EDGE = "common_edge";
std::string const SearchAnswerConstants::VAR_EDGE = "var_edge";

}  // namespace SearchAnswerModule
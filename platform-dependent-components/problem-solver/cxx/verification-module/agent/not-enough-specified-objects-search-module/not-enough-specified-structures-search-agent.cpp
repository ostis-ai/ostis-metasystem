/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */
#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-memory/sc_keynodes.hpp>

#include "constants/verification_constants.hpp"

#include "keynodes/verification_keynodes.hpp"

#include "not-enough-specified-structures-search-agent.hpp"

using namespace utils;

namespace verificationModule
{

ScResult SearchNotEnoughSpecifiedStructuresAgent::DoProgram(ScAction & action)
{
  return action.FinishSuccessfully();
}

ScAddr SearchNotEnoughSpecifiedStructuresAgent::GetActionClass() const
{
  return VerificationKeynodes::action_search_not_enough_specified_structures;
}
}  // namespace verificationModule
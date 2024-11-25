/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-memory/sc_keynodes.hpp>

#include "keynodes/verification_keynodes.hpp"

#include "not-enough-specified-objects-search-agent.hpp"


using namespace utils;

namespace verificationModule
{

ScResult SearchNotEnoughSpecifiedObjectsAgent::DoProgram(ScAction & action)
{
  auto const argument = action.GetArgument(1);
  if(!argument.IsValid()){
    return action.FinishUnsuccessfully();
  }

  ScType type = m_context.GetElementType(argument);
  if(scTypeAndActionClass.at(type).IsValid()){
    SC_LOG_INFO("ok");
  }
  else{
    SC_LOG_INFO("No");
  }
  return action.FinishSuccessfully();
}

ScAddr SearchNotEnoughSpecifiedObjectsAgent::GetActionClass() const
{
  return VerificationKeynodes::action_search_not_enough_specified_objects;
}
}  // namespace verificationModule
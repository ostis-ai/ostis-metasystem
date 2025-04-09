/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include <sc-agents-common/utils/CommonUtils.hpp>
#include <sc-agents-common/utils/IteratorUtils.hpp>
#include "sc-memory/sc_keynodes.hpp"

#include "keynodes/message_reply_keynodes.hpp"

#include "NotGenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

ScResult NotGenerateReplyMessageAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  if(!m_context.HelperCheckEdge(
        messageReplyModule::MessageReplyKeynodes::action_interpret_non_atomic_action, action, ScType::EdgeAccessConstPosPerm))
  {
    return action.FinishSuccessfully();
  }

  return action.FinishSuccessfully();
}

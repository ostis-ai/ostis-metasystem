/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MessageReplyModule.hpp"

#include "keynodes/MessageReplyKeynodes.hpp"
#include "agent/MessageReplyAgent.hpp"

using namespace messageReplyModule;

SC_IMPLEMENT_MODULE(MessageReplyModule)

sc_result MessageReplyModule::InitializeImpl()
{
  if (!MessageReplyModule::InitGlobal())
  {
    SC_LOG_ERROR("MessageReplyModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!MessageReplyKeynodes::InitGlobal())
  {
    SC_LOG_ERROR("MessageReplyKeynodes is deactivated");
    return SC_RESULT_ERROR;
  }
  else
  {
    SC_AGENT_REGISTER(MessageReplyAgent);
  }

  return SC_RESULT_OK;
}

sc_result MessageReplyModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(MessageReplyAgent);

  return SC_RESULT_OK;
}

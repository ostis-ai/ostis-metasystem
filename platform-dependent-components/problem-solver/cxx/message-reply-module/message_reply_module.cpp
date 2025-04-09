/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "message_reply_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/message-reply-agent.hpp"

using namespace messageReplyModule;

SC_MODULE_REGISTER(MessageReplyModule)->Agent<MessageReplyAgent>();

void MessageReplyModule::Initialize(ScMemoryContext *m_context) 
{
    
}

void MessageReplyModule::Shutdown(ScMemoryContext * m_context)
{

}
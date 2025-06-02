/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "message_classification_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/message-topic-classification-agent.hpp"

using namespace messageClassificationModule;

SC_MODULE_REGISTER(MessageClassificationModule)->Agent<MessageTopicClassificationAgent>();

void MessageClassificationModule::Initialize(ScMemoryContext *m_context) 
{
    
}

void MessageClassificationModule::Shutdown(ScMemoryContext * m_context)
{

}



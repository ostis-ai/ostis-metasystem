/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "search_answer_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/search-properties-agent.hpp"

using namespace searchAnswerModule;

SC_MODULE_REGISTER(SearchAnswerModule)->Agent<SearchPropertiesAgent>();

void SearchAnswerModule::Initialize(ScMemoryContext *m_context) 
{
    
}

void SearchAnswerModule::Shutdown(ScMemoryContext * m_context)
{

}
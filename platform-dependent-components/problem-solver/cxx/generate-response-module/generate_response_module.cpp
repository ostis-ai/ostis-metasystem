/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "generate_response_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/generate-response-agent.hpp"

using namespace generateResponseModule;

SC_MODULE_REGISTER(GenerateResponseModule)->Agent<GenerateResponseAgent>();

void GenerateResponseModule::Initialize(ScMemoryContext *m_context) 
{
    
}

void GenerateResponseModule::Shutdown(ScMemoryContext * m_context)
{

}


/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "structure_translation_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "keynodes/translation_keynodes.hpp"

#include "agent/structure-translation-agent.hpp"

using namespace structureTranslationModule;

SC_MODULE_REGISTER(StructureTranslationModule)->Agent<StructureTranslationAgent>();

void StructureTranslationModule::Initialize(ScMemoryContext *m_context) 
{
    
}

void StructureTranslationModule::Shutdown(ScMemoryContext * m_context)
{

}

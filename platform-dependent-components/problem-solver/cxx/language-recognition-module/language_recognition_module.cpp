/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "language_recognition_module.hpp"

#include "sc-memory/sc_memory.hpp"

#include "agent/language-recognition-agent.hpp"

using namespace languageRecognitionModule;

SC_MODULE_REGISTER(LanguageRecognitionModule)->Agent<LanguageRecognitionAgent>();

void LanguageRecognitionModule::Initialize(ScMemoryContext *m_context) 
{
    
}

void LanguageRecognitionModule::Shutdown(ScMemoryContext * m_context)
{

}
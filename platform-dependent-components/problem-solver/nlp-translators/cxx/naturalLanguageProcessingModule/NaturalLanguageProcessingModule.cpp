

#include "keynodes/TranslationKeynodes.hpp"

#include "agent/FormSemanticNeighbourhoodTranslationAgent.hpp"

#include "NaturalLanguageProcessingModule.hpp"

namespace naturalLanguageProcessingModule
{
SC_IMPLEMENT_MODULE(NaturalLanguageProcessingModule)

sc_result NaturalLanguageProcessingModule::InitializeImpl()
{
  if (TranslationKeynodes::InitGlobal() == SC_FALSE)
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(FormSemanticNeighbourhoodTranslationAgent)

  return SC_RESULT_OK;
}

sc_result NaturalLanguageProcessingModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(FormSemanticNeighbourhoodTranslationAgent)

  return SC_RESULT_OK;
}
}  // namespace naturalLanguageProcessingModule

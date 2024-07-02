/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "structured_representation_generation_module.hpp"
#include "agent/structured_representation_generation_agent.hpp"
#include "keynodes/structured_representation_generation_keynodes.hpp"

using namespace structuredRepresentationGenerationModule;

SC_IMPLEMENT_MODULE(StructuredRepresentationGenerationModule)

sc_result StructuredRepresentationGenerationModule::InitializeImpl()
{
  if (!StructuredRepresentationGenerationModule::InitGlobal())
  {
    SC_LOG_ERROR("StructuredRepresentationGenerationModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!StructuredRepresentationGenerationKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(StructuredRepresentationGenerationAgent);
  return SC_RESULT_OK;
}

sc_result StructuredRepresentationGenerationModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(StructuredRepresentationGenerationAgent);
  return SC_RESULT_OK;
}

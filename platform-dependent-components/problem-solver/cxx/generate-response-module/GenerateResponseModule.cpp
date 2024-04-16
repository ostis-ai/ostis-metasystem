/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "GenerateResponseModule.hpp"

#include "keynodes/Keynodes.hpp"
#include "agent/GenerateResponseAgent.hpp"

using namespace generateResponseModule;

SC_IMPLEMENT_MODULE(GenerateResponseModule)

sc_result GenerateResponseModule::InitializeImpl()
{
  if (!GenerateResponseModule::InitGlobal())
  {
    SC_LOG_ERROR("GenerateResponseModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!Keynodes::InitGlobal())
  {
    SC_LOG_ERROR("GenerateResponseKeynodes is deactivated");
    return SC_RESULT_ERROR;
  }
  else
  {
    SC_AGENT_REGISTER(GenerateResponseAgent);
  }

  return SC_RESULT_OK;
}

sc_result GenerateResponseModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(GenerateResponseAgent);

  return SC_RESULT_OK;
}

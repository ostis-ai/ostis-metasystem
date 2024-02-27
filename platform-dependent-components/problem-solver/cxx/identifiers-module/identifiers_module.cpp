/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "identifiers_module.hpp"

#include "keynodes/identifiers_keynodes.hpp"
#include "agent/translate_main_system_idtfs_from_sc_to_file_agent.hpp"

using namespace identifiersModule;

SC_IMPLEMENT_MODULE(IdentifiersModule)

sc_result IdentifiersModule::InitializeImpl()
{
  if (!IdentifiersModule::InitGlobal())
  {
    SC_LOG_ERROR("IdentifiersModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!IdentifiersKeynodes::InitGlobal())
  {
    SC_LOG_ERROR("IdentifiersKeynodes is deactivated");
    return SC_RESULT_ERROR;
  }
  else
  {
    SC_AGENT_REGISTER(TranslateMainSystemIdtfsFromScToFileAgent);
  }

  return SC_RESULT_OK;
}
sc_result IdentifiersModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(TranslateMainSystemIdtfsFromScToFileAgent);

  return SC_RESULT_OK;
}

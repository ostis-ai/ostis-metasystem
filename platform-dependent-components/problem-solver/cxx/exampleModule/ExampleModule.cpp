#include "ExampleModule.hpp"

#include "keynodes/ExampleKeynodes.hpp"
#include "agent/ExampleAgent.hpp"

using namespace exampleModule;

SC_IMPLEMENT_MODULE(ExampleModule)

sc_result ExampleModule::InitializeImpl()
{
  if (!ExampleModule::InitGlobal())
  {
    SC_LOG_ERROR("ExampleModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!ExampleKeynodes::InitGlobal())
  {
    SC_LOG_ERROR("ExampleKeynodes is deactivated");
    return SC_RESULT_ERROR;
  }
  else
  {
    SC_AGENT_REGISTER(ExampleAgent);
  }

  return SC_RESULT_OK;
}

sc_result ExampleModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(ExampleAgent);

  return SC_RESULT_OK;
}

#include "LocalCommonModule.hpp"

#include "sc-memory/sc_memory.hpp"

#include "keynodes/LocalKeynodes.hpp"

using namespace commonModule;

SC_IMPLEMENT_MODULE(LocalCommonModule)

sc_result LocalCommonModule::InitializeImpl()
{
  if (!LocalKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  return SC_RESULT_OK;
}

sc_result LocalCommonModule::ShutdownImpl()
{
  return SC_RESULT_OK;
}

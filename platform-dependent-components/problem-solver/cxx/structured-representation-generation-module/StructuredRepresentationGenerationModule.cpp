/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "StructuredRepresentationGenerationModule.hpp"

using namespace structuredRepresentationGenerationModule;

SC_IMPLEMENT_MODULE(StructuredRepresentationGenerationModule)

sc_result StructuredRepresentationGenerationModule::InitializeImpl()
{
  if (!StructuredRepresentationGenerationModule::InitGlobal())
  {
    SC_LOG_ERROR("StructuredRepresentationGenerationModule is deactivated");
    return SC_RESULT_ERROR;
  }

  if (!StructuredRepresentationGenerationModule::InitGlobal())
  {
    SC_LOG_ERROR("MessageReplyKeynodes is deactivated");
    return SC_RESULT_ERROR;
  }

  return SC_RESULT_OK;
}

sc_result StructuredRepresentationGenerationModule::ShutdownImpl()
{
  return SC_RESULT_OK;
}

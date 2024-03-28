/*
 * Copyright (c) 2022 Intelligent Semantic Systems LLC, All rights reserved.
 * Author Viktor Markovec
 */

#include "agent/GetDecompositionAgent.hpp"
#include "agent/AddSectionAgent.hpp"
#include "agent/RemoveSectionAgent.hpp"
#include "keynodes/Keynodes.hpp"
#include "SubjectDomainModule.hpp"

using namespace subjectDomainModule;

SC_IMPLEMENT_MODULE(SubjectDomainModule)

sc_result SubjectDomainModule::InitializeImpl()
{
  if (!subjectDomainModule::Keynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(GetDecompositionAgent)
  SC_AGENT_REGISTER(AddSectionAgent)
  SC_AGENT_REGISTER(RemoveSectionAgent)

  return SC_RESULT_OK;
}

sc_result SubjectDomainModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(GetDecompositionAgent)
  SC_AGENT_UNREGISTER(AddSectionAgent)
  SC_AGENT_UNREGISTER(RemoveSectionAgent)

  return SC_RESULT_OK;
}

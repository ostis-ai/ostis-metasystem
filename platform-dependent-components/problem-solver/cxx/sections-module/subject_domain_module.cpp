/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "agent/get_decomposition_agent.hpp"
#include "agent/add_section_agent.hpp"
#include "agent/remove_section_agent.hpp"

#include "keynodes/sections_keynodes.hpp"

#include "subject_domain_module.hpp"

using namespace sectionsModule;

SC_IMPLEMENT_MODULE(SectionsModule)

sc_result SectionsModule::InitializeImpl()
{
  if (!sectionsModule::SectionsKeynodes::InitGlobal())
    return SC_RESULT_ERROR;

  SC_AGENT_REGISTER(GetDecompositionAgent)
  SC_AGENT_REGISTER(AddSectionAgent)
  SC_AGENT_REGISTER(RemoveSectionAgent)

  return SC_RESULT_OK;
}

sc_result SectionsModule::ShutdownImpl()
{
  SC_AGENT_UNREGISTER(GetDecompositionAgent)
  SC_AGENT_UNREGISTER(AddSectionAgent)
  SC_AGENT_UNREGISTER(RemoveSectionAgent)

  return SC_RESULT_OK;
}

/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "sc-agents-common/utils/AgentUtils.hpp"
#include "sc-agents-common/utils/CommonUtils.hpp"
#include "sc-agents-common/keynodes/coreKeynodes.hpp"
#include "sc-memory/kpm/sc_agent.hpp"
#include "keynodes/IdentifiersKeynodes.hpp"
#include "FindIdentifiersAgent.hpp"

#include "../../sc-machine/sc-kpm/sc-ui/uiKeynodes.h"

using namespace identifiersModule;
using namespace scAgentsCommon;

SC_AGENT_IMPLEMENTATION(FindIdentifiersAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr))
  {
    return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("FindIdentifiersAgent started");

  ScAddr node_nrel_system_idtf = ms_context->HelperFindBySystemIdtf("nrel_system_identifier");
  std::stringstream stream_idtfs;

  ScIterator3Ptr it =
      ms_context->Iterator3(node_nrel_system_idtf, ScType::EdgeAccessConstPosPerm, ScType::EdgeDCommonConst);
  while (it->Next())
  {
    ScAddr edge_nrel_system_idtf = it->Get(2);
    ScAddr node = ms_context->GetEdgeSource(edge_nrel_system_idtf);
    ScAddr node_idtf = ms_context->GetEdgeTarget(edge_nrel_system_idtf);

    std::string system_idtf;
    ms_context->GetLinkContent(node_idtf, system_idtf);

    std::string main_idtf =
        utils::CommonUtils::getMainIdtf(&m_memoryCtx, node, {scAgentsCommon::CoreKeynodes::lang_ru});

    std::string str_type = get_str_ScType(node);

    if (!main_idtf.empty())
    {
      stream_idtfs << "{\"" << main_idtf << "\", "
           << "{\"" << system_idtf << "\", \"" << str_type << "\"} },\n";
    }
  }
  string str_idtfs(stream_idtfs.str());
  str_idtfs.pop_back();
  str_idtfs.pop_back();

  if (write_in_file(str_idtfs)){
    SC_LOG_DEBUG("File has been created");
  }

  SC_LOG_DEBUG("FindIdentifiersAgent finished");
  return SC_RESULT_OK;
}

bool FindIdentifiersAgent::checkAction(ScAddr const & actionAddr)
{
  return m_memoryCtx.HelperCheckEdge(IdentifiersKeynodes::action_find_identifiers, actionAddr, ScType::EdgeAccessConstPosPerm);
}

std::string FindIdentifiersAgent::get_str_ScType(ScAddr const & node)
{
  std::string str_type = "";
  ScType type = ms_context->GetElementType(node);
  if (nodes.count(type))
  {
    str_type = nodes[type];
  }
  else if (edges.count(type))
  {
    str_type = edges[type];
  }
  return str_type;
}

bool FindIdentifiersAgent::write_in_file(std::string& str_idtfs)
{
  try
  {
    std::ofstream file(IDENTIFIERS_MODULE_PATH "identifiers.txt");
    if (file.is_open())
    {
      file << str_idtfs;
    }
    file.close();
  }
  catch (const std::exception & err)
  {
    SC_LOG_ERROR(err.what());
    return false;
  }
  return true;
}

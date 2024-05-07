#include <sc-agents-common/utils/AgentUtils.hpp>

#include "sc-agents-common/keynodes/coreKeynodes.hpp"

#include "NotGenerateReplyMessageAgent.hpp"

using namespace messageReplyModuleTest;

SC_AGENT_IMPLEMENTATION(NotGenerateReplyMessageAgent)
{
  ScAddr const & actionAddr = otherAddr;
  if(!m_memoryCtx.HelperCheckEdge(
        messageReplyModule::MessageReplyKeynodes::action_interpret_non_atomic_action, actionAddr, ScType::EdgeAccessConstPosPerm))
  {
    return SC_RESULT_OK;
  }

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, true);
  return SC_RESULT_OK;
}

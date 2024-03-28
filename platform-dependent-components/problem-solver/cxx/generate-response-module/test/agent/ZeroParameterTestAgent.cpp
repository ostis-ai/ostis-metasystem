#include "sc-agents-common/utils/IteratorUtils.hpp"
#include "sc-agents-common/utils/AgentUtils.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "ZeroParameterTestAgent.hpp"

using namespace generateResponseModuleTest;

SC_AGENT_IMPLEMENTATION(ZeroParameterTestAgent)
{
  ScAddr const & actionAddr = otherAddr;

  if (!checkAction(actionAddr)) {
      return SC_RESULT_OK;
  }

  SC_LOG_DEBUG("ZeroParameterTestAgent started");

  ScAddr successNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.HelperSetSystemIdtf("test_success", successNode);

  SC_LOG_DEBUG("ZeroParameterTestAgent ended");

  utils::AgentUtils::finishAgentWork(&m_memoryCtx, actionAddr, {successNode});
  return SC_RESULT_OK;
}

bool ZeroParameterTestAgent::checkAction(ScAddr const & actionAddr) 
{
    return m_memoryCtx.HelperCheckEdge(
            TestKeynodes::test_action,
            actionAddr,
            ScType::EdgeAccessConstPosPerm
            );
}

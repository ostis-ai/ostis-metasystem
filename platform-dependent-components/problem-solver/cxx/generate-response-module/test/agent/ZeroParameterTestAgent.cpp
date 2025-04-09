#include <sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-agents-common/utils/AgentUtils.hpp>

#include "sc-memory/sc_addr.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "ZeroParameterTestAgent.hpp"

using namespace generateResponseModuleTest;


ScResult OneParameterTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  SC_LOG_DEBUG("ZeroParameterTestAgent started");

  ScAddr successNode = m_memoryCtx.CreateNode(ScType::NodeConst);
  m_memoryCtx.HelperSetSystemIdtf("test_success", successNode);

  SC_LOG_DEBUG("ZeroParameterTestAgent ended");

  return action.FinishSuccessfully();
}

bool ZeroParameterTestAgent::checkAction(ScAddr const & action) 
{
    return m_memoryCtx.HelperCheckEdge(
            TestKeynodes::action_zero_param,
            action,
            ScType::EdgeAccessConstPosPerm
            );
}

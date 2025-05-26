#include "sc-memory/sc_addr.hpp"

#include "test/keynodes/TestKeynodes.hpp"

#include "ZeroParameterTestAgent.hpp"

using namespace generateResponseModuleTest;

ScResult ZeroParameterTestAgent::DoProgram(ScActionInitiatedEvent const & event, ScAction & action)
{
  SC_LOG_DEBUG("ZeroParameterTestAgent started");

  ScAddr successNode = m_context.CreateNode(ScType::NodeConst);
  m_context.HelperSetSystemIdtf("test_success", successNode);

  SC_LOG_DEBUG("ZeroParameterTestAgent ended");

  return action.FinishSuccessfully();
}

bool ZeroParameterTestAgent::checkAction(ScAddr const & action) 
{
    return m_context.HelperCheckEdge(
            TestKeynodes::action_zero_param,
            action,
            ScType::EdgeAccessConstPosPerm
            );
}
